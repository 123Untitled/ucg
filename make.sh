#!/usr/bin/env -S zsh --no-rcs --no-globalrcs

# compile with latest clang
export CXX='clang++'

local EXEC='ucg'
local SCRIPT=$0
local BUILD_DIR='.build'
local CMAKELISTS='CMakeLists.txt'
local DATABASE='compile_commands.json'
local CLANGD_CACHE='.cache'

function handle_clean {
	local COUNT=${$(rm -rvf $BUILD_DIR $DATABASE $CLANGD_CACHE './exec' | wc -l)// /}
	echo $COUNT 'files removed.'
}

function maybe_reconfig {
	# check if we need to re-config the build
	if [[ ! -d $BUILD_DIR || $SCRIPT -nt $BUILD_DIR || $CMAKELISTS -nt $BUILD_DIR ]]; then
		# make build directory
		mkdir -p $BUILD_DIR
		# re-config the build
		cmake -S '.' -G 'Ninja' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE='Debug' -B $BUILD_DIR
		# create a symlink to compile_commands.json
		ln -sf $BUILD_DIR/compile_commands.json .
	fi
}

function handle_build {
	# check if we need to re-config the build
	maybe_reconfig
	# build the project
	if ninja -C $BUILD_DIR; then
		ln -sf $BUILD_DIR/$EXEC ./exec
	fi
}

function handle_install {
	# make build directory
	mkdir -p $BUILD_DIR
	# re-config the build
	cmake -S '.' -G 'Ninja' -DCMAKE_BUILD_TYPE='Release' -B $BUILD_DIR
	# install the project
	ninja -C $BUILD_DIR install
}



function handle_arguments {

	echo \
		'\n\x1b[1;34m' \
		'░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░ \n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░       \n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒▒▓███▓▒░\n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░\n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n' \
		'░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░  \n' \
		'       universal color generator\x1b[0m\n'

	# check if no arguments are passed
	if [[ $# -eq 0 ]]; then
		handle_build
		exit $?
	fi

	# switch case for first argument
	case $1 in
		--clean|-c)
			shift
			handle_clean $@
			;;
		--build|-b)
			shift
			handle_build $@
			;;
		--install|-i)
			shift
			handle_install $@
			;;
		*)
			echo $SCRIPT 'unknown argument' "'"$1"'"
			exit 1
			;;
	esac
}

handle_arguments $@
