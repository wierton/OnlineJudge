.PHONY: clean all

.DEFAULT_GOAL=all

SRCS=$(shell find . -name "*.cpp")
OBJ_DIR=output
GOALS=$(basename $(notdir $(SRCS)))

.PHONY: $(GOALS)

all: $(addprefix $(OBJ_DIR)/,$(addsuffix .bin,$(GOALS)))

define each_target
$1: $(OBJ_DIR)/$1.bin
endef

$(OBJ_DIR)/%.bin: %.cpp
	mkdir -p $(@D)
	g++ -std=c++14 $^ -o $@
	$@
