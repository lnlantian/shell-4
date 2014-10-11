COMP := g++
CFLAGS := -g --std=c++11 # used in compilation
LFLAGS := -lreadline
SRC := $(wildcard *.cpp) # mask for source files
OBJ := $(SRC:.cpp=.o) # object files filenames made from SRC
NAME := shell # name of executive

all: \
	shell \
	clean

# main program
shell: $(OBJ)
	$(COMP) -o $(NAME) $(OBJ) $(LFLAGS)

# creating object (OBJ) files from source (SRC)
# (inference rules)
# % - matching zero or more characters
# $< - first dependency name
# $@ - output filename
%.o: %.cpp
	$(COMP) -o $@ -c $< $(CFLAGS)

# after compilation cleaning
clean:
	rm $(OBJ)

# recipes that don't create files
.PHONY: clean
