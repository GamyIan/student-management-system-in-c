# Compiler and Flags
CC = gcc
CFLAGS = -Iinclude -I"C:/raylib/include" -Wall -std=c99 -g
LDFLAGS = -L"C:/raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm -static

# Source Files and Executable
SRC = main.c src/student_op.c src/csv_op.c src/ui_op.c
OBJ = $(SRC:.c=.o)
EXEC = student_management.exe

# Default Target
all: $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)
	rm -f $(OBJ)

# Object File Compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Up
clean:
	rm -f $(OBJ)
