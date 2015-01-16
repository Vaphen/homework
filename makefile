NAME = homework
CC = g++
CFLAGS = -std=c++11 -lsqlite3 `pkg-config --libs --cflags gtkmm-2.4`

default: $(NAME)

FILES = main SQLiteConnect GUI LessonPage SettingsLessonTable SettingsPage LessonTableRow

$(NAME):
	@echo "Building..."
	$(CC) $(CFLAGS) -o $(NAME) $(foreach var,$(FILES),$(var).o)

LessonPage.o: ./gui/LessonPage.cpp ./gui/LessonPage.h
	$(CC) $(CFLAGS) -c ./gui/LessonPage.cpp ./gui/LessonPage.h

SettingsLessonTable.o: ./gui/SettingsLessonTable.cpp ./gui/SettingsLessonTable.h
	$(CC) $(CFLAGS) -c ./gui/SettingsLessonTable.cpp ./gui/SettingsLessonTable.h

SettingsPage.o: ./gui/SettingsPage.cpp ./gui/SettingsPage.h
	$(CC) $(CFLAGS) -c ./gui/SettingsPage.cpp ./gui/SettingsPage.h

LessonTableRow.o: ./gui/LessonTableRow.cpp ./gui/LessonTableRow.h
	$(CC) $(CFLAGS) -c ./gui/LessonTableRow.cpp ./gui/LessonTableRow.h

SQLiteConnect.o: ./sql/SQLiteConnect.cpp ./sql/SQLiteConnect.h
	$(CC) $(CFLAGS) -c ./sql/SQLiteConnect.cpp ./sql/SQLiteConnect.h

constants.o: ./constants/constants.h
	$(CC) $(CFLAGS) -c ./constants/constants.h

main.o: ./main.cpp
	$(CC) $(CFLAGS) -c ./main.cpp

gui.o: ./gui/*.cpp ./gui/*.h
	$(CC) $(CFLAGS) -c ./gui/*.cpp ./gui/*.h

sql.o: ./sql/*.cpp ./sql/*.h
	$(CC) $(CFLAGS) -c ./sql/*.cpp ./sql/*.h

clean:
	rm $(NAME) *.o *.~
