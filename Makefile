# Project Structure
SRC_DIR := ./src
INC_DIR := ./inc
APP_DIR := ./app
TESTS_DIR := ./tests

# Files
SRC := $(wildcard $(SRC_DIR)/*.c)
ASRC := $(SRC) $(wildcard $(APP_DIR)/*.c)
AOBJ := $(ASRC:%.c=%.o)
OBJ := $(AOBJ)
DEPS := $(OBJ:%.o=%.d)

# Libraries
LIBS := pthread

# Exec
EXEC := main.out

# Default Compiler = GCC
CC ?= gcc 

DEP_FLAGS := -MMD -MP

H_INC := $(foreach d, $(INC_DIR), -I$d)
L_INC := $(foreach l, $(LIBS), -l$l)

# Compiler Flags
ifeq ($(CC),clang++)
	C_FLAGS += -Weverything -Wno-vla -Wno-disabled-macro-expansion -std=c99
else
	C_FLAGS += -Wall -Wextra -Werror -std=c99
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

# make V=1 --> verbose, otherwise silence
ifeq ("$(origin V)", "command line")
	Q :=
else
	Q ?= @
endif

C_FLAGS += $(OPT) $(GGDB) $(DEP_FLAGS)

all: $(EXEC)

$(EXEC): $(AOBJ)
	$(Q)$(CC) $(C_FLAGS) $(H_INC) $(AOBJ) -o $@ $(L_INC)

%.o:%.cpp %.d
	$(Q)$(CC) $(C_FLAGS) $(H_INC) -c $< -o $@

clean:
	$(Q)$(RM) $(EXEC)
	$(Q)$(RM) $(OBJ)
	$(Q)$(RM) $(DEPS)

$(DEPS):

include $(wildcard $(DEPS))
