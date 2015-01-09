//main.cpp
#include "GUI.h"
#include <gtkmm.h>
#include <iostream>

using namespace Gtk;

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);
	GUI rootGUI;
	Gtk::Main::run(rootGUI);
	return 0;
}

