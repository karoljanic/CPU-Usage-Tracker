# PROJECT STRUCTURE
SRC_DIR := ./src
INC_DIR := ./inc
APP_DIR := ./app
TESTS_DIR := ./tests

# PROJECT FILES
MODE := app
SRC := $(wildcard $(SRC_DIR)/*.c) # list of all the C source files in the SRC_DIR directory

APP_SRC := $(SRC) $(wildcard $(APP_DIR)/*.c)
APP_TARGET := main.out

TEST_SRC := $(SRC) $(wildcard $(TESTS_DIR)/*.c)
TEST_TARGET := $(TESTS_DIR)/test.out

APP_OBJ := $(APP_SRC:%.c=%.o) # replace .c with .o for each element in APP_SRC
TEST_OBJ := $(TEST_SRC:%.c=%.o) # replace .c with .o for each element in APP_SRC

APP_DEPS := $(APP_OBJ:%.o=%.d) # replace .o with .d for each element in OBJ
TEST_DEPS := $(TEST_OBJ:%.o=%.d) # replace .o with .d for each element in OBJ

# LIBRARIES
LIBS := pthread

# COMPILATION
CC ?= gcc # default compiler
C_FLAGS := -Wall -Wextra -Werror
DEP_FLAGS := -MMD -MP # automatically generate .d dependenciees

INCS_INC := $(foreach i, $(INC_DIR), -I$i) # foreach var in INC_DIR expand with -I$var
LIBS_INC := $(foreach l, $(LIBS), -l$l)

# If CC is set to clang, add -Weverything flag
ifeq ($(CC), clang)
	C_FLAGS += -Weverything -Wno-vla -Wno-disabled-macro-expansion -Wno-deprecated-pragma
endif

# Set -O flag if O= argument is present, default=-O3
ifeq ("$(origin O)", "command line")
	OPT := -O$(O)
else
	OPT := -O3
endif

# Set -G flag if G= argument is present, default=none
ifeq ("$(origin G)", "command line")
	GGDB := -ggdb$(G)
else
	GGDB :=
endif

C_FLAGS += $(OPT) $(GGDB) $(DEP_FLAGS)

# TARGETS
print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true

all: $(APP_TARGET)

test: $(TEST_TARGET)

$(APP_TARGET): $(APP_OBJ)
	$(CC) $(C_FLAGS) $(INCS_INC) $(APP_OBJ) -o $@ $(LIBS_INC)

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(C_FLAGS) $(INCS_INC) $(TEST_OBJ) -o $@ $(LIBS_INC)

%.o:%.c %.d
	$(CC) $(C_FLAGS) $(INCS_INC) -c $< -o $@

clean:
	rm -rf $(APP_TARGET)
	rm -rf $(TEST_TARGET)
	rm -rf $(APP_OBJ)
	rm -rf $(TEST_OBJ)
	rm -rf $(APP_DEPS)
	rm -rf $(TEST_DEPS)

$(APP_DEPS):
include $(wildcard $(APP_DEPS))

$(TEST_DEPS):
include $(wildcard $(TEST_DEPS))
