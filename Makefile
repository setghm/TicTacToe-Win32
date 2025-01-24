APPNAME=TicTacToe
CC=gcc
RC=windres
LINK=gcc
LIBRARIES=-luser32 -lgdi32 -lkernel32 -lcomctl32

$(APPNAME).exe: main.c procedure.c game.c styles.c
	$(RC) -i resource.rc -o resource.o
	$(CC) -c $^
	$(LINK) $(^:.c=.o) resource.o -o $@ $(LIBRARIES)

.PHONY: clean
clean:
	del *.o *.exe