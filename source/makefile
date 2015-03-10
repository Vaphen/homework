NAME := homework
CC := g++

ifeq ($(OS),Windows_NT)
ADDITIONAL_LIBRARIES_WIN := rude/librudeconfig.a sqlite3/libsqlite3.a boost/boost_regex.a boost/boost_system.a boost/boost_filesystem.a boost/boost_thread.a
else
ADDITIONAL_LIBRARIES_LINUX := -lsqlite3 -lrudeconfig -lboost_system -lboost_filesystem -lboost_thread -lboost_regex
endif

C_SRCS := $(shell find -name "*.cpp")
C_OBJS := $(subst ./,../obj/,$(addsuffix .o,$(basename $(C_SRCS)))) 
C_PREH := $(shell find -name "*.h.gch")

default: $(addprefix ../,$(NAME))

all: $(C_OBJS)

$(addprefix ../,$(NAME)): $(C_OBJS)
	@echo "Building..."
	$(CC) -o $@ $^ $(addprefix ../staticLibraries/,$(ADDITIONAL_LIBRARIES_WIN)) $(ADDITIONAL_LIBRARIES_LINUX) `pkg-config --libs gtkmm-2.4`
	
../obj/%.o: %.cpp 
	$(shell mkdir -p $(addprefix ../obj/,$(dir $?)))
	$(CC) -std=c++0x -c -I/home/john/boost_1_55_0 -o $@ $? `pkg-config --cflags gtkmm-2.4`

clean:
	rm -f $(NAME) $(C_OBJS) $(C_PREH)
	@echo "Everything clean."
