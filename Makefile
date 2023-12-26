CC=gcc
CFLAGS=-Iinclude
LDFLAGS=-Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

SRCDIR=src

SOURCES=$(SRCDIR)/snake.c
EXECUTABLE=snake.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	del /Q $(subst /,\,$(EXECUTABLE))