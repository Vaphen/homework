//main.cpp
//#include "constants/constants.h"
#include "gui/GUI.h"
#include "fileOperations/BasicFileOps.h"
#include "sql/SQLiteConnect.h"
#include "helpers/HelpDialogs.h"
#include <gtkmm.h>

void initializeEnvironment() {
	BasicFileOps fileManager;

	try {
		if(!fileManager.isFileExistant(CONFIG_FILE)) {
			fileManager.callConfigParser().createDefaultConfigFile();
		}

		if(!fileManager.isFileExistant(FOLDER_PATH)) {
			fileManager.createFolder(FOLDER_PATH);
		}
	} catch (FILE_ERRORS &error) {
		// need this check because if folder alreadz exists, everything is allright
		if(error != FILE_ERRORS::FOLDER_ALREADY_EXISTANT) {
			HelpDialogs::showErrorDialog(error);
			exit(0);
		}
	}


	SQLiteConnect connection;
	try {
		connection.createMarkTable();
		connection.createAllLessonTable();
	} catch(ERRORS& error) {
		HelpDialogs::showErrorDialog(error);
		exit(0);
	}
}

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);
	initializeEnvironment();
	GUI rootGUI;
	Gtk::Main::run(rootGUI);

	return 0;
}
