# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# OpenGL and FreeGLUT libraries
LIBS = -lGL -lGLU -lglut -ljpeg

# Target executable
TARGET = Miroir

# Source files
SRCS = miroir.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)


