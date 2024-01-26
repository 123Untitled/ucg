#!/usr/bin/env -S zsh --no-rcs --no-globalrcs


# This script is used to install toml++

# -- V A R I A B L E S --------------------------------------------------------

# required packages
REQUIRED_PKG=('git' 'openssl')

# url of repository
REPO_URL='https://github.com/marzer/tomlplusplus.git'

# path of repository
REPO_PATH=$(openssl sm3 <<< $REPO_URL)

# install path
INSTALL_PATH='external/toml++/include'


# -- C H E C K  P A C K A G E S -----------------------------------------------

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

echo '\x1b[1;34m'
# clone repo
if ! git clone -v $REPO_URL $REPO_PATH; then
	echo 'error: failed to clone repo :('
	echo '\x1b[0m'
	exit 1
fi
echo '\x1b[0m'


# -- I N S T A L L ------------------------------------------------------------

mkdir -p $INSTALL_PATH

cp -r $REPO_PATH'/include/toml++' $INSTALL_PATH

rm -rf $REPO_PATH

echo 'toml++ installed successfully :)'
