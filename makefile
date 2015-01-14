NAME = homework

all: $(NAME)

FILES = main.cpp ./gui/*.cpp ./gui/*.h ./constants/*.h ./sql/*.cpp ./sql/*.h
$(NAME):
	@echo "Building..."
	g++ -std=c++11 -lsqlite3 `pkg-config --libs --cflags gtkmm-2.4` -o "homework" $(FILES)

clean:
	rm -r $(NAME)
