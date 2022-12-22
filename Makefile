CC = cc
CFLAGS = -std=c11 -pedantic -Wall -Wextra -Wno-unused-parameter -I. -I./src
LDFLAGS = -lm

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	AR = gcc-ar
	CFLAGS += -DLINUX -D_XOPEN_SOURCE=700 -D_GNU_SOURCE
	LDFLAGS += -Wl,-rpath=./
endif
ifeq ($(UNAME_S),Darwin)
	AR = ar
	CFLAGS += -DDARWIN
endif

CFLAGS += -fPIC
LDFLAGS += -ldl

DEBUG ?= 1
ifeq ($(DEBUG),0)
	CFLAGS += -O2 -flto -DNDEBUG
else
	CFLAGS += -g -DDEBUG
endif

ifeq ($USE_MALLOC),1)
	CFLAGS += -DUSE_MALLOC
endif

SRCS  = src/psyche.c
SRCS += src/arena.c

OBJS = $(addprefix obj/,$(notdir $(SRCS:.c=.o)))

STATIC ?= 0
ifeq ($(STATIC),0)
	LIB = libpsyche.so
	LIBFLAGS = -L. -lpsyche
else
	LIB = libpsyche.a
	CFLAGS += -DSTATICLIB
endif

VPATH  = ./src:./lib

INCS := $(wildcard src/*.h)

TESTS = $(wildcard test/*_test.psy)

.PHONY: mkdir test

all: mkdir psyche

mkdir: obj

obj:
	@mkdir obj

psyche: obj/main.o $(LIB) Makefile
	$(CC) $(CFLAGS) obj/main.o $(LIB) $(LIBFLAGS) $(LDFLAGS) -o $@
	@echo CC psyche

libpsyche.a: $(OBJS) $(INCS) Makefile
	@$(AR) rcu $(OBJS)
	@echo CC libpsyche.a

libpsyche.so: $(OBJS) $(INCS) Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -shared $(OBJS) -o $@
	@echo CC libpsyche.so

obj/%.o: %.c $(INCS) Makefile
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo CC $<

clean:
	@rm -f psyche $(OBJS) libpsyche.a libpsyche.so obj/main.o
	@rmdir obj
	@echo clean psyche $(OBJS)