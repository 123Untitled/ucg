#!/usr/bin/env -S zsh --no-rcs --no-globalrcs


# -- U C G --------------------------------------------------------------------

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


# -- O P E R A T I N G  S Y S T E M -------------------------------------------

# get operating system
declare -rg os=$(uname -s)


# -- T H I S  S C R I P T -----------------------------------------------------

# get script absolute directory path
declare -rg cwd_dir=${${0%/*}:a}

# get script absolute path
declare -rg script=${0:a}


# compile with latest clang
#export CXX='clang++'

local EXEC='ucg'
local SCRIPT=$0
local build_dir='.build'
local CMAKELISTS='CMakeLists.txt'
local DATABASE='compile_commands.json'
local CLANGD_CACHE='.cache'


function handle_clean {
	local COUNT=${$(rm -rvf $build_dir $DATABASE $CLANGD_CACHE './exec' | wc -l)// /}
	echo $COUNT 'files removed.'
}

function maybe_reconfig {
	# check if we need to re-config the build
	if [[ ! -d $build_dir || $SCRIPT -nt $build_dir || $CMAKELISTS -nt $build_dir ]]; then
		# make build directory
		mkdir -p $build_dir
		# re-config the build
		cmake -S '.' -G 'Ninja' -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE='Debug' -B $build_dir
		# create a symlink to compile_commands.json
		ln -sf $build_dir/compile_commands.json .
	fi
}

function handle_build {
	# check if we need to re-config the build
	maybe_reconfig
	# build the project
	if ninja -C $build_dir; then
		ln -sf $build_dir'/'$EXEC ./exec
	fi
}

function handle_install {
	# make build directory
	mkdir -p $build_dir
	# re-config the build
	cmake -S '.' -G 'Ninja' -DCMAKE_BUILD_TYPE='Release' -B $build_dir
	# install the project
	ninja -C $build_dir install
}



function _run {

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

_run $@
