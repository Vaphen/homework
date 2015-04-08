//main.cpp
#include "gui/GUI.h"
#include "fileOperations/BasicFileOps.h"
#include "sql/SQLiteConnect.h"
#include "helpers/HelpDialogs.h"
#include <gtkmm.h>
#include <iostream>
#if defined(_WIN32) || defined(WIN32)
	#define OS "WindowsCompiled"
#else
	#define OS "LinuxCompiled"
#endif

using namespace Gtk;

void initializeEnvironment() {
	BasicFileOps fileManager;
	if(!fileManager.isFileExistant(CONFIG_FILE)) {
		fileManager.callConfigParser().createDefaultConfigFile();
	}

	if(!fileManager.isFileExistant(FOLDER_PATH)) {
		fileManager.createFolder(FOLDER_PATH);
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
	initializeEnvironment();
	std::cout << OS << std::endl;
	Gtk::Main kit(argc, argv);
	GUI rootGUI;
	Gtk::Main::run(rootGUI);
	return 0;
}
