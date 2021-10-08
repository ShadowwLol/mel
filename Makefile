# Flags
MCC :=
CFLAGS = -std=c99 -O3 -Wextra -fstack-protector -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -fstack-clash-protection -Wshadow -Wformat=2 -Wformat-truncation -Wformat-overflow -fno-common -fstack-usage
MLIBS :=
LIBS   = -ldl -lglfw -lpthread -lGL -lm
WLIBS  = -lglfw3dll -lopengl32 -lmingw32 -lgdi32 -luser32 -lkernel32 -lpthread -lm
SRCF = src/*.c
INCF = include/*.h
OUT = Game

ifeq ($(OS),Windows_NT)
    MCC += i686-w64-mingw32-gcc
	#CCFLAGS += -D WIN32
	MLIBS += -lglfw3dll -lopengl32 -lmingw32 -lgdi32 -luser32 -lkernel32 -lpthread -lm
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        MCC += gcc
		#CCFLAGS += -D LINUX
		MLIBS += -ldl -lglfw -lpthread -lGL -lm
    endif
endif

amp:
	$(MCC) $(CFLAGS) -o $(OUT) $(SRCF) -I$(INCF) $(MLIBS)

debug:
	$(MCC) $(CFLAGS) -g -o $(OUT) $(SRCF) -I$(INCF) $(MLIBS)

all: clean linux windows

clean:
	@rm -rf bin
	@rm -rf *.su

linux:
	gcc $(CFLAGS) -o $(OUT) $(SRCF) -I$(INCF) $(LIBS)

windows:
	i686-w64-mingw32-gcc $(CFLAGS) -o $(OUT) $(SRCF) -I$(INCF) $(WLIBS)
