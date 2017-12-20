# This is intended to be a template for makefile (default to C project)
# Change any directory/suffices/flags/compiler as needed.
# Auto-dependencies still cannot handle file-renaming issue

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

.dependencies : $(SRCS)
	$(CC) $(CFLAGS) -MM $(SRCS) > .dependencies

-include .dependencies

.PHONY : clean

clean :
	rm $(OBJDIR)/*.o $(PROGRAM) .dependencies

