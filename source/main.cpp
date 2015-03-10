//main.cpp
#include "gui/GUI.h"
#include <gtkmm.h>
#include <iostream>
#if defined(_WIN32) || defined(WIN32)
	#define OS "WindowsCompiled"
#else
	#define OS "LinuxCompiled"
#endif

using namespace Gtk;


int main(int argc, char *argv[]) {
	std::cout << OS << std::endl;
	Gtk::Main kit(argc, argv);
	GUI rootGUI;
	Gtk::Main::run(rootGUI);
	return 0;
}

