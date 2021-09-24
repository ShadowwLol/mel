CC=gcc
LIBS=-ldl -lglfw -lpthread -lGL -lm
CFLAGS=$(LIBS) -O3

WCC=i686-w64-mingw32-gcc
WLIBS=-lglfw3dll -lopengl32 -lmingw32 -lgdi32 -luser32 -lkernel32 -lpthread -lm
WCFLAGS=$(WLIBS) -O3

FLAGS=-std=c99

SRCF=src/*.c
INCF=include/*.h
OUT=Game

all:
	$(CC) $(SRCF) -I$(INCF) $(CFLAGS) $(FLAGS) -o $(OUT)
	$(WCC) $(SRCF) -I$(INCF) $(WCFLAGS) $(FLAGS) -o $(OUT)

linux:
	$(CC) $(SRCF) -I$(INCF) $(CFLAGS) $(FLAGS) -o $(OUT)

windows:
	$(WCC) $(SRCF) -I$(INCF) $(WCFLAGS) $(FLAGS) -o $(OUT)

debug:
	$(CC) -g $(SRCF) -I$(INCF) $(CFLAGS) $(FLAGS) -o $(OUT)

test:
	$(CC) -g tests/*.c -I$(INCF) $(CFLAGS) $(FLAGS) -o TEST$(OUT)
