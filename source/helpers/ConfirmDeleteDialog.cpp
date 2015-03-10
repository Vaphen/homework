/*
 * ConfirmDeleteDialog.cpp
 *
 *  Created on: 01.02.2015
 *      Author: john
 */

#include "ConfirmDeleteDialog.h"
#include "../constants/constants.h"

ConfirmDeleteDialog::ConfirmDeleteDialog(std::string const &title, std::string const &message) {
	set_title("Löschen bestätigen");

	Gtk::Label *mainText = Gtk::manage(new Gtk::Label);
	deleteEverythingButton = Gtk::manage(new Gtk::CheckButton);
	deleteEverythingButton->set_label(message);
	mainText->set_markup(title);
	get_vbox()->pack_start(*mainText);
	get_vbox()->pack_start(*deleteEverythingButton);
	add_button("Löschen", int(ANSWERS::DELETE));
	add_button("Abbrechen", int(ANSWERS::CANCEL));
	set_default_response(int(ANSWERS::CANCEL));
	show_all_children();

}

ConfirmDeleteDialog::~ConfirmDeleteDialog() {

}

unsigned int ConfirmDeleteDialog::run() {
	unsigned int response = Gtk::Dialog::run();
	if(response == int(ANSWERS::DELETE) && deleteEverythingButton->get_active())
		return int(ANSWERS::DELETE_ALL);
	return response;
}

