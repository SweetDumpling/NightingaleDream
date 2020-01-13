SRCS     := $(wildcard src/*.c)
OBJS     := $(subst .c,.o,$(SRCS))

OPT      ?= $(patsubst test/test_%.c,%,$(firstword $(wildcard test/test_*.c)))
TESTSRCS := $(filter-out src/main.c,$(SRCS)) test/test_$(OPT).c test/unity.c
TESTOBJS := $(subst .c,.o,$(TESTSRCS))

TARGET   := bin/nd.exe
TTARGET  := bin/test_$(OPT).exe

CC       := @gcc
LIBS     := -L./lib
LDFLAGS  := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
DEFINES  := -Dmain=SDL_main
INCLUDE  := -I./include
CFLAGS   := -g -Wall -O3 --std=c99 $(DEFINES) $(INCLUDE)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LIBS)

$(TTARGET) : $(TESTOBJS)
	$(CC) $(CFLAGS) -o $(TTARGET) $(TESTOBJS) $(LDFLAGS) $(LIBS)

.PHONY : test rebuild clean cleanall

test : $(TTARGET)
	@$(TTARGET)

rebuild : cleanall $(TARGET)

clean :
	@rm -fr src/*.o test/*.o

cleanall : clean
	@rm -fr bin/*.exe