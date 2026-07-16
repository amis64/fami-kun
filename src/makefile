CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = fami-kun
SRCS = famicom.c bus.c cartridge.c CPU.c mappers.c
# Automatically derive object file names from source files
OBJS = $(SRCS:.c=.o)
# The default rule to build the program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
