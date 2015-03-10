/*
 * ConfirmDeleteDialog.h
 *
 *  Created on: 01.02.2015
 *      Author: john
 */

#ifndef HELPERS_CONFIRMDELETEDIALOG_H_
#define HELPERS_CONFIRMDELETEDIALOG_H_

#include <gtkmm.h>

class ConfirmDeleteDialog  : Gtk::Dialog {
public:
	ConfirmDeleteDialog(std::string const&, std::string const&);
	virtual ~ConfirmDeleteDialog();
	unsigned int run();
private:
	Gtk::CheckButton *deleteEverythingButton;
};

#endif /* HELPERS_CONFIRMDELETEDIALOG_H_ */
