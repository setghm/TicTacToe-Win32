APPNAME=TicTacToe

CC=gcc
RC=windres
LINK=gcc

SRCDIR=src
RESDIR=res
BUILDDIR=build
OBJECTDIR=$(BUILDDIR)\\obj

CFLAGS=-Wall -Wextra
LDFLAGS=-mwindows
INC=-I$(RESDIR)
LIB=-luser32 -lgdi32 -lkernel32 -lcomctl32

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%.c,$(OBJECTDIR)/%.o,$(SOURCES))
RESOURCE=$(OBJECTDIR)/resource.o
OUT=$(BUILDDIR)/$(APPNAME).exe

$(OUT): $(OBJECTS) $(RESOURCE)
	$(LINK) $(LDFLAGS) $^ -o $@ $(LIB)

$(OBJECTDIR)/%.o: $(SRCDIR)/%.c | $(OBJECTDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(RESOURCE): $(RESDIR)/resource.rc | $(OBJECTDIR)
	$(RC) -i $< -o $@

$(BUILDDIR):
	if not exist $(BUILDDIR) mkdir $(BUILDDIR)

$(OBJECTDIR): $(BUILDDIR)
	if not exist $(OBJECTDIR) mkdir $(OBJECTDIR)

.PHONY: clean
clean:
	rmdir /s /q $(BUILDDIR)
