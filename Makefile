CC=gcc
LIBS=-ldl -lglfw -lpthread -lGL -lm
CFLAGS=$(LIBS) -O3

WCC=i686-w64-mingw32-gcc
WLIBS=-lglfw3dll -lopengl32 -lmingw32 -lgdi32 -luser32 -lkernel32 -lpthread -lm
WCFLAGS=$(WLIBS) -O3

SRCF=src/*.c
INCF=include/*.h
OUT=Game

all:
	$(CC) $(SRCF) -I$(INCF) $(CFLAGS) -o $(OUT)
	$(WCC) $(SRCF) -I$(INCF) $(WCFLAGS) -o $(OUT)

debug:
	$(CC) -g $(SRCF) -I$(INCF) $(CFLAGS) -o $(OUT)

test:
	$(CC) -g tests/*.c -I$(INCF) $(CFLAGS) -o TEST$(OUT)