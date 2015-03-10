NAME := homework
CC := g++
CFLAGS := -lboost_filesystem -ggdb -std=c++11 -lsqlite3 `pkg-config --libs --cflags gtkmm-2.4` 
ADDITIONAL_LIBRARIES := /usr/local/lib/librudeconfig.a -lboost_system

C_SRCS := $(shell find -name "*.cpp")
C_OBJS := $(subst ./,obj/,$(addsuffix .o,$(basename $(C_SRCS)))) 
C_PREH := $(shell find -name "*.h.gch")

default: $(NAME)

all: $(C_OBJS)

$(NAME): $(C_OBJS)
	@echo "Building..."
	@echo $(C_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(ADDITIONAL_LIBRARIES)

obj/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $?

clean:
	rm $(NAME) $(C_OBJS) $(C_PREH)
