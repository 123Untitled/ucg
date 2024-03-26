
# -- M A K E F I L E ----------------------------------------------------------
#                                                                             #
#                 ░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░                     #
#                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░                    #
#                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░                           #
#                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒▒▓███▓▒░                    #
#                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░                    #
#                 ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░                    #
#                  ░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░                     #
#                                                                             #
###############################################################################



# -- S E T T I N G S ----------------------------------------------------------

# set default target
.DEFAULT_GOAL := all

# use one shell for all commands
.ONESHELL:

# delete intermediate files on error
.DELETE_ON_ERROR:

# silent mode
.SILENT:

# set shell program
override SHELL := $(shell which zsh)

# set shell flags
.SHELLFLAGS := --no-rcs --no-globalrcs --errexit --no-unset -c -o pipefail

# set make flags
override MAKEFLAGS += --warn-undefined-variables --no-builtin-rules



# -- O P E R A T I N G  S Y S T E M -------------------------------------------

# Detect operating system
override OS := $(shell uname -s)

# check for macOSX
ifeq ($(OS), Darwin)
override PLATFORM := macosx
override THREAD   := $(shell sysctl -n hw.ncpu)
# check for Linux
else ifeq ($(OS), Linux)
override PLATFORM := linux
override THREAD   := $(shell nproc)
# unsupported OS
else
$(error $(shell echo "Unsupported OS: \x1b[31m$(OS)\x1b[0m"))
endif


# -- T A R G E T S ------------------------------------------------------------

# project name
override PROJECT = ucg

# main executable
override EXEC = $(PROJECT)

# compile commands for clangd
override COMPILE_COMMANDS = compile_commands.json



# -- D I R E C T O R I E S ----------------------------------------------------

# source directory
override SRC_DIR := sources

# include directory
override INC_DIR := includes

# tools directory
override TOOL_DIR := tools

# external directory
override EXT_DIR := external


# -- F L A G S ----------------------------------------------------------------

# includes
override INCLUDES ?=

# libraries
override LIBRARIES ?=



# -- E X T E R N A L  D E P E N D E N C I E S ---------------------------------

# toml++ directory
override TOML++_DIR := $(EXT_DIR)/toml++

# append include flags
override INCLUDES += -I$(TOML++_DIR)/include


# yaml-cpp directory
override YAMLCPP_DIR := $(EXT_DIR)/yaml-cpp

# append include flags
override INCLUDES += -I$(YAMLCPP_DIR)/include

# append library flags
override LIBRARIES += -L$(YAMLCPP_DIR)/lib -lyaml-cpp




# -- S O U R C E S ------------------------------------------------------------

# get all source files
override SRCS := $(wildcard $(SRC_DIR)/**/*.cpp) \
				 $(wildcard $(SRC_DIR)/*.cpp)

# append include directory
override INCLUDES += -I$(INC_DIR)

# object file rules
override OBJS := $(SRCS:%.cpp=%.o)

# dependency file rules
override DEPS := $(SRCS:%.cpp=%.d)

# json file rules
override JSNS := $(SRCS:%.cpp=%.json)


# -- T O O L S ----------------------------------------------------------------

# make directory if not exists
override MKDIR := mkdir -p

# remove recursively force
override RM := rm -rf


# -- C O M P I L E R  S E T T I N G S -----------------------------------------

# compiler
override CXX := $(shell which clang++)

# compiler standard
override STD := -std=c++2a

# compiler optimization
override OPT := -O0 -g3

# warning scope
override CXXFLAGS := -Wall -Wextra

# warning impact
override CXXFLAGS += -Werror

# standard respect
override CXXFLAGS += -Wpedantic -Weffc++

# unused suppression
override CXXFLAGS += -Wno-unused -Wno-unused-variable -Wno-unused-parameter

# optimization
override CXXFLAGS += -Winline

# type conversion
override CXXFLAGS += -Wconversion -Wsign-conversion -Wfloat-conversion -Wnarrowing

# shadowing
#override CXXFLAGS += -Wshadow

# linker flags
override LDFLAGS := $(LIBRARIES)

# dependency flags
override DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# compile commands flags
override CMPFLAGS = -MJ $*.json

# command line defines
DEF ?=

# defines flags
override DEFINES := $(addprefix -D,$(DEF))



# -- C O L O R S --------------------------------------------------------------

define COLOR
	@printf "\e[7;32m %s \e[0m\n" $(1)
endef

define LINES
	@printf "\e[90m%s\e[0m\n" '-----------------------------------------------'
endef



# -- P H O N Y  T A R G E T S -------------------------------------------------

# phony targets
.PHONY: all clean fclean re ascii obj exec dependencies


define install
if [ -n "$${MYBIN+x}" ]; then
	cp $(1) "$$MYBIN"
	echo 'copy' $(1) 'to' "$$MYBIN"
else
	echo '$$MYBIN is not defined.'
fi
endef



# -- M A I N  T A R G E T S ---------------------------------------------------

all: ascii dependencies obj $(EXEC) $(COMPILE_COMMANDS)
	$(call LINES)
	$(call COLOR,"done")
	echo
	$(call install, $(EXEC))


dependencies: $(TOML++_DIR) $(YAMLCPP_DIR)


# -- E X E C U T A B L E ------------------------------------------------------

$(EXEC): $(OBJS)
	$(call LINES)
	$(call COLOR,"linking project")
	$(CXX) $^ -o $@ $(LDFLAGS)
	ls -l $@


# -- C O M P I L A T I O N ----------------------------------------------------

# self call with threads
obj:
	$(call LINES)
	$(call COLOR,"compiling project")
	$(MAKE) --silent -j$(THREAD) $(OBJS)

-include $(DEPS)
%.o: %.cpp Makefile
	$(CXX) $(STD) $(OPT) $(CXXFLAGS) $(DEFINES) $(CMPFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@
	ls -l $@


# -- C O M P I L E  C O M M A N D S -------------------------------------------

$(COMPILE_COMMANDS): $(JSNS)
	$(call LINES)
	$(call COLOR,"generating compile commands")
	echo "[\n"$$(cat $(JSNS) | sed '$$s/,\s*$$//')"\n]" | jq > $@
	ls -l $@


# -- C L E A N I N G ----------------------------------------------------------

clean:
	$(call COLOR,"cleaning project")
	$(RM) $(OBJS) $(DEPS) $(JSNS) $(COMPILE_COMMANDS) .cache

fclean: clean
	$(call COLOR,"fcleaning project")
	$(RM) $(EXEC) $(EXT_DIR)


# -- R E C O M P I L E --------------------------------------------------------

re: fclean all

$(TOML++_DIR):
	$(TOOLDIR)/install_toml++.sh

$(YAMLCPP_DIR):
	$(TOOLDIR)/install_yaml-cpp.sh


# -- A S C I I  A R T ---------------------------------------------------------

ascii:
	echo \
		'\n\x1b[1;34m' \
		'░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░ ░▒▓██████▓▒░ \n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░       \n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒▒▓███▓▒░\n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░\n' \
		'░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n' \
		'░▒▓██████▓▒░ ░▒▓██████▓▒░ ░▒▓██████▓▒░  \n' \
		'\t\t\tGNU Make: '$(MAKE_VERSION)'\x1b[0m'
