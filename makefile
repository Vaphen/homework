NAME := homework
CC := g++
CFLAGS := -ggdb -std=c++11 -lsqlite3 `pkg-config --libs --cflags gtkmm-2.4`

C_SRCS := $(shell find -name "*.cpp")
C_OBJS := $(subst ./,obj/,$(addsuffix .o,$(basename $(C_SRCS)))) 
C_PREH := $(shell find -name "*.h.gch")

default: $(NAME)

all: $(C_OBJS)

$(NAME): $(C_OBJS)
	@echo "Building..."
	@echo $(C_SRCS)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $?

#LessonPage.o: ./gui/LessonPage.cpp ./gui/LessonPage.h
#	$(CC) $(CFLAGS) -c ./gui/LessonPage.cpp ./gui/LessonPage.h

#SettingsLessonTable.o: ./gui/SettingsLessonTable.cpp ./gui/SettingsLessonTable.h
#	$(CC) $(CFLAGS) -c ./gui/SettingsLessonTable.cpp ./gui/SettingsLessonTable.h

#SettingsPage.o: ./gui/SettingsPage.cpp ./gui/SettingsPage.h
#	$(CC) $(CFLAGS) -c ./gui/SettingsPage.cpp ./gui/SettingsPage.h

#LessonTableRow.o: ./gui/LessonTableRow.cpp ./gui/LessonTableRow.h
#	$(CC) $(CFLAGS) -c ./gui/LessonTableRow.cpp ./gui/LessonTableRow.h

#SQLiteConnect.o: ./sql/SQLiteConnect.cpp ./sql/SQLiteConnect.h
#	$(CC) $(CFLAGS) -c ./sql/SQLiteConnect.cpp ./sql/SQLiteConnect.h

#constants.o: HelpDialogs.o Labels.o ./constants/constants.h 
#	$(CC) $(CFLAGS) -c ./constants/constants.h ./

#HelpDialogs.o: ./constants/HelpDialogs.h
#	$(CC) $(CFLAGS) -c ./constants/HelpDialogs.h

#Labels.o: ./constants/Labels.h
#	$(CC) $(CFLAGS) -c ./constants/Labels.h

#main.o: ./main.cpp
#	$(CC) $(CFLAGS) -c ./main.cpp

#gui.o: ./gui/*.cpp ./gui/*.h
#	$(CC) $(CFLAGS) -c ./gui/*.cpp ./gui/*.h

#sql.o: ./sql/*.cpp ./sql/*.h
#	$(CC) $(CFLAGS) -c ./sql/*.cpp ./sql/*.h

#BasicFileOps.o: ./folderOperations/BasicFileOps.cpp ./folderOperations/BasicFileOps.h
#	$(CC) $(CFLAGS) -c ./folderOperations/BasicFileOps.cpp ./folderOperations/BasicFileOps.h

clean:
	rm $(NAME) $(C_OBJS) $(C_PREH)
