CC ?= $(CROSS_COMPILE)g++
#CFLAGS ?=-Wextra -Wall -fstack-protector-all -fno-stack-protector -pedantic -fwrapv --param ssp-buffer-size=4 -fvisibility=hidden -Wcast-align -Wmissing-field-initializers -Wshadow -Wswitch-enum -O0
#CFLAGS += -DDEBUG

C_SRCS := $(sort $(wildcard *.cpp))
C_OBJS := ${C_SRCS:.cpp=.o}
C_SRCS += plugins/adapter.cpp
C_OBJS += adapter.o
OBJS := $(C_OBJS)

INCLUDE_DIRS := #/home/narman/Qt/5.15.2/gcc_64/include/QtGui /home/narman/Qt/5.15.2/gcc_64/include

CFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

SHARED_LIB := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

all: lib

lib:
	$(CC) -c -fPIC $(CFLAGS) $(C_SRCS)
	$(CC) $(C_OBJS) -lstdc++ $(SHARED_LIB) -lm -o do

clean:
	rm *.o
	rm *.log
