SRCS     := $(wildcard src/*.c)
OBJS     := $(patsubst src/%.c,%.o,$(SRCS))

TARGET   := bin/nd.exe

CC       := gcc
LIBS     := -L./lib
LDFLAGS  := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
DEFINES  := -Dmain=SDL_main
INCLUDE  := -I./include
CFLAGS   := -g -Wall -O3 --std=c99 $(DEFINES) $(INCLUDE)


.PHONY : all objs clean veryclean rebuild

all : $(TARGET)

objs : $(OBJS)

rebuild: veryclean all

clean :
	rm -fr *.o

veryclean : clean
	rm -fr $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJS) : %.o : src/%.c
	$(CC) -c $(CFLAGS) $< -o $@