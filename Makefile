# Compilation
TARGET := my_sudo
CC := clang
FLAGS := -Wall -Wextra -Wno-implicit-function-declaration -g3

# Sources
SRC := $(shell find . -name '*.c')
HDR := $(shell find . -name '*.h')
OBJ := $(SRC:.c=.o)

all: $(TARGET) clean

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) $(SRC) -o $(TARGET) -lcrypt

clean:
	rm -f $(shell find . -name '*.o')
	rm -f $(shell find . -name '*.c~')
	rm -f $(shell find . -name '*.h~')
	rm -f $(shell find . -name '*#*')
	rm -f $(shell find . -name '*.pch')
	rm -f $(shell find . -name '*.out')
	rm -f $(shell find . -name '*~')

fclean: clean
	rm -f $(shell find . -name '$(TARGET)')

re: fclean $(TARGET)

print_src:
	$(info $(shell find . -name '*.c'))

epiclang_all:
	epiclang $(SRC) $(HDR) -lcrypt