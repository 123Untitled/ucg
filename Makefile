
# -- M A K E F I L E ----------------------------------------------------------

###############################################################################
#        ▁▁▁▁▁▁▁▁  ▁▁▁▁▁▁▁▁  ▁▁▁▁ ▁▁▁  ▁▁▁▁▁▁▁▁                               #
#       ╱        ╲╱        ╲╱    ╱   ╲╱        ╲    language: makefile        #
#      ╱         ╱         ╱         ╱         ╱    author:   @tutur          #
#     ╱         ╱         ╱        ▁╱       ▁▁╱     created:  2020-05-01      #
#     ╲▁▁╱▁▁╱▁▁╱╲▁▁▁╱▁▁▁▁╱╲▁▁▁▁╱▁▁▁╱╲▁▁▁▁▁▁▁▁╱      updated:  2020-05-01      #
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
    override PLATFORM   := macosx
    override THREAD     := $(shell sysctl -n hw.ncpu)
# check for Linux
else ifeq ($(OS), Linux)
    override PLATFORM   := linux
    override THREAD     := $(shell nproc)
# unsupported OS
else
    $(error $(shell echo "Unsupported OS: \x1b[31m$(OS)\x1b[0m"))
endif


# -- R E P O S I T O R Y ------------------------------------------------------


#override REPO_URL := git@github.com:123Untitled/xns.git
#
## check we are in the right repository
#ifneq ($(shell git config --get remote.origin.url), $(REPO_URL))
#    $(error $(shell echo "Please use \x1b[32m$(REPO_URL)\x1b[0m"))
#endif



# -- T A R G E T S ------------------------------------------------------------

# project name
override PROJECT = ucg

# main executable
override EXEC = $(PROJECT)

# compile commands for clangd
override COMPILE_COMMANDS = compile_commands.json



# -- D I R E C T O R I E S ----------------------------------------------------

# source directory
override SRCDIR := sources

# include directory
override INCDIR := includes

# library directory
override LIBDIR := libraries

# build directory
override BLDDIR := build

# tools directory
override TOOLDIR := tools

# object directory
override OBJDIR := $(BLDDIR)/object

# dependency directory
override DEPDIR := $(BLDDIR)/dependency

# json directory
override JSNDIR := $(BLDDIR)/json

# external directory
override EXTDIR := external



# -- E X T E R N A L  L I B R A R I E S ---------------------------------------

# toml++ directory
override TOML++_DIR := $(EXTDIR)/toml++

# toml++ include directory
override TOML++_INC := $(TOML++_DIR)/include

# yaml-cpp directory
override YAML-CPP_DIR := $(EXTDIR)/yaml-cpp

# yaml-cpp include directory
override YAML-CPP_INC := $(YAML-CPP_DIR)/include

# yaml-cpp library directory
override YAML-CPP_LIB := $(YAML-CPP_DIR)/lib




# -- S O U R C E S ------------------------------------------------------------

# get all source files
override SRC := $(shell find $(SRCDIR) -type f -name '*.cpp')

# get all header files
override HDR := $(shell find $(INCDIR) -type f -name '*.hpp')

# get all header directories
override HDRDIR := $(sort $(dir $(HDR)))

# pattern substitution for object files
override OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o,    $(SRC))

# pattern substitution for dependency files
override DEP := $(patsubst $(OBJDIR)/%.o,   $(DEPDIR)/%.d,    $(OBJ))

# pattern substitution for compile commands
override JSN := $(patsubst $(SRCDIR)/%.cpp, $(JSNDIR)/%.json, $(SRC))


override HIR := $(sort $(dir $(SRC)))
override OBJHIR := $(HIR:$(SRCDIR)/%=$(OBJDIR)/%)
override DEPHIR := $(HIR:$(SRCDIR)/%=$(DEPDIR)/%)
override JSNHIR := $(HIR:$(SRCDIR)/%=$(JSNDIR)/%)





# -- C O M P I L E R  S E T T I N G S -----------------------------------------

# make directory if not exists
override MKDIR := mkdir -p

# remove recursively force
override RM := rm -rf

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
override LDFLAGS := -L$(YAML-CPP_LIB) -lyaml-cpp

# dependency flags
override DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# compile commands flags
override CMPFLAGS = -MJ $(JSNDIR)/$*.json

# include flags
override INCLUDES := $(addprefix -I,$(HDRDIR) $(TOML++_INC) $(YAML-CPP_INC))

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
.PHONY: all clean fclean re ascii obj exec



# -- M A I N  T A R G E T S ---------------------------------------------------

all: ascii $(TOML++_DIR) $(YAML-CPP_DIR) obj $(EXEC) $(COMPILE_COMMANDS)
	$(call LINES)
	$(call COLOR,"done ◝(ᵔᵕᵔ)◜")
	echo
	cp $(EXEC) '/Users/untitled/Desktop/code/bin/'


# -- E X E C U T A B L E ------------------------------------------------------

$(EXEC): $(OBJ)
	$(call LINES)
	$(call COLOR,"linking project")
	$(CXX) $^ -o $@ $(LDFLAGS)
	ls -l $@


# -- C O M P I L A T I O N ----------------------------------------------------

# self call with threads
obj:
	$(call LINES)
	$(call COLOR,"compiling project")
	$(MAKE) --silent -j$(THREAD) $(OBJ)

-include $(DEP)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp Makefile | $(OBJHIR) $(DEPHIR) $(JSNHIR)
	$(CXX) $(STD) $(OPT) $(CXXFLAGS) $(DEFINES) $(CMPFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@
	ls -l $@


# -- C O M P I L E  C O M M A N D S -------------------------------------------

$(COMPILE_COMMANDS): $(JSN)
	$(call LINES)
	$(call COLOR,"generating compile commands")
	echo "[\n"$$(cat $(JSN) | sed '$$s/,\s*$$//')"\n]" | jq > $@
	ls -l $@


# -- D I R E C T O R I E S  C R E A T I O N -----------------------------------

$(OBJHIR) $(DEPHIR) $(JSNHIR):
	$(MKDIR) $@


# -- C L E A N I N G ----------------------------------------------------------

clean:
	$(call COLOR,"cleaning project")
	$(RM) $(BLDDIR) $(COMPILE_COMMANDS) .cache

fclean: clean
	$(call COLOR,"fcleaning project")
	$(RM) $(EXEC) $(TOML++_DIR) $(YAML-CPP_DIR)


# -- R E C O M P I L E --------------------------------------------------------

re: fclean all

$(TOML++_DIR):
	$(TOOLDIR)/install_toml++.sh

$(YAML-CPP_DIR):
	$(TOOLDIR)/install_yaml-cpp.sh


# -- A S C I I  A R T ---------------------------------------------------------


ascii:
	echo \
		'\x1b[1;34m' \
		'   ▁▁▁▁▁▁▁▁  ▁▁▁▁▁▁▁▁  ▁▁▁▁ ▁▁▁  ▁▁▁▁▁▁▁▁	\n' \
		'  ╱        ╲╱        ╲╱    ╱   ╲╱        ╲	\n' \
		' ╱         ╱         ╱         ╱         ╱	\n' \
		'╱         ╱         ╱        ▁╱       ▁▁╱	\n' \
		'╲▁▁╱▁▁╱▁▁╱╲▁▁▁╱▁▁▁▁╱╲▁▁▁▁╱▁▁▁╱╲▁▁▁▁▁▁▁▁╱	\n' \
		'\t\t\tGNU Make: '$(MAKE_VERSION)'\x1b[0m'


