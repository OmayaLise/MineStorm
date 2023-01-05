PROGRAM=minestorm

# Add your objs to generate in OBJS var
OBJS=src/main.o src/game.o src/mathLibrary.o src/collider.o src/ennemies.o src/menu.o src/player.o src/projectile.o



CC?=gcc
TARGET?=$(shell $(CC) -dumpmachine)

CFLAGS=-O0 -g -Wall -Wextra -Wno-unused-parameter 
CPPFLAGS=-Iinclude -Ithird_party/include -MMD
LDFLAGS=-Lthird_party/libs-$(TARGET)
LDLIBS=-lraylib

ifeq ($(TARGET),x86_64-linux-gnu)
LDLIBS+=-ldl -lpthread -lm
else ifeq ($(TARGET),x86_64-pc-cygwin)
LDLIBS+=-lgdi32
endif

DEPS=$(OBJS:.o=.d)$(DEPTEST:.o=.d)

.PHONY: all clean

all: $(PROGRAM)

-include $(DEPS)

%.o: %.c 
	$(CC) -c  $(CFLAGS) $(CPPFLAGS) $< -o $@

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

build.tar.gz: $(PROGRAM) $(wildcard assets/*)
	tar czf build.tar.gz $(PROGRAM) assets

clean:
	rm -f $(OBJS) $(DEPS) build.tar.gz $(PROGRAM)

