# This is intended to be a template for makefile (default to C project)
# Header to .c file dependency still needs to be fixed.
# Auto dependency generation is needed.

CC = gcc
CFLAGS = -g -Wall -I./$(HDRDIR) 
VPATH = $(SRCDIR):$(BUILDDIR):$(HDRDIR):$(TESTDIR):$(PROJDIR)

SRCDIR = src
TESTDIR = test
OBJDIR= build
HDRDIR  = include
PROJDIR = bin

PROGRAM := $(PROJDIR)/output
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

vpath %.h $(HDRDIR)
vpath %.c $(SRCDIR)

all : $(PROGRAM)

$(PROGRAM) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS) : $(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

foo.c : foo.h
hello.c : foo.h

# List file.o : header1.h header2.h ... below

.PHONY : clean

clean :
	rm $(OBJS) $(PROGRAM)

