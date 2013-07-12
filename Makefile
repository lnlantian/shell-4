FLAGS = -g # used in compilation
SRC = $(wildcard *.cpp) # mask for source files
OBJ = $(SRC:.cpp=.o) # object files filenames made from SRC
NAME = shell # name of executive

all: \
	shell \
	clean

# main program
shell: $(OBJ)
	g++ -o $(NAME) $(OBJ)

# creating object (OBJ) files from source (SRC)
# (inference rules)
# % - matching zero or more characters
# $< - first dependency name
# $@ - output filename
%.o: %.cpp
	g++ -o $@ -c $< $(FLAGS)

# after compilation cleaning
clean:
	rm $(OBJ)

# recipes that don't create files
.PHONY: clean
