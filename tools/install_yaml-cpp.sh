#!/usr/bin/env -S zsh --no-rcs --no-globalrcs


# This script is used to install yaml-cpp

# -- V A R I A B L E S --------------------------------------------------------

# required packages
REQUIRED_PKG=('git' 'openssl' 'cmake')

# get absolute root directory without external commands
ROOT=${0:a:h:h}

# url of repository
REPO_URL='https://github.com/jbeder/yaml-cpp.git'

# path of repository
REPO_PATH=$ROOT'/'$(openssl sm3 <<< $REPO_URL)

# install path
INSTALL_PATH=$ROOT'/external/yaml-cpp'

# build system
BUILD_SYSTEM=''

# build options
BUILD_OPTIONS=()


# -- C H E C K  P A C K A G E S -----------------------------------------------

# check build system
if command -v 'ninja' &> /dev/null; then
	BUILD_OPTIONS=('-G' 'Ninja')
	BUILD_SYSTEM='ninja'
elif command -v 'make' &> /dev/null; then
	BUILD_OPTIONS=('-G' 'Unix Makefiles')
	BUILD_SYSTEM='make'
else
	echo 'error: ninja or make could not be found :('
	exit 1
fi

# check required packages
for PKG in $REQUIRED_PKG; do
	if ! command -v $PKG &> /dev/null; then
		echo 'error:' $PKG 'could not be found :('
		exit 1
	fi
done


# -- C L O N E  R E P O -------------------------------------------------------

# remove old repo
if [ -d $REPO_PATH ]; then
	rm -rf $REPO_PATH
fi

# clone repo
if ! git clone -v $REPO_URL $REPO_PATH; then
	echo 'error: failed to clone repo :('
	echo '\x1b[0m'
	exit 1
fi


# -- I N S T A L L ------------------------------------------------------------


mkdir -p $INSTALL_PATH $REPO_PATH'/build'
cd $REPO_PATH'/build'


if ! cmake .. $BUILD_OPTIONS -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH -DYAML_BUILD_SHARED_LIBS=OFF; then
	echo 'error: could not configure yaml-cpp :('
	cd ../..
	rm -rf $REPO_PATH
	exit 1
fi

if ! $BUILD_SYSTEM; then
	echo 'error: could not build yaml-cpp :('
	cd '../..'
	rm -rf $REPO_PATH
	exit 1
fi

if ! $BUILD_SYSTEM install; then
	echo 'error: could not install yaml-cpp :('
	cd '../..'
	rm -rf $REPO_PATH
	exit 1
fi

cd '../..'
rm -rf $REPO_PATH

exit 0
