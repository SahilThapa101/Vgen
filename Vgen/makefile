CC=gcc
CFLAGS=-Wall -Wextra 
SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))
#EXECUTABLE=bin/Vengeance.exe
EXECUTABLE=Vengeance.exe

#all:	build $(EXECUTABLE)
all:	$(EXECUTABLE)

$(EXECUTABLE):	$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

$(OBJECTS):	src/%.o	:	src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p bin

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) bin
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;
