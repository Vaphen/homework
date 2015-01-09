NAME = homework

all: $(NAME)

FILES = constants.h LessonPage.cpp SettingsLessonTable.h SettingsPage.cpp SQLiteConnect.h GUI.cpp LessonPage.h main.cpp SettingsPage.h GUI.h SettingsLessonTable.cpp SQLiteConnect.cpp
$(NAME):
	@echo "Building..."
	g++ -std=c++11 -lsqlite3 `pkg-config --libs --cflags gtkmm-2.4` -o "homework" $(FILES)

clean:
	rm -r $(NAME)
