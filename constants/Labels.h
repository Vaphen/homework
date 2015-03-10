#ifndef CONSTANTS_LABELS_H_
#define CONSTANTS_LABELS_H_

namespace LessonPageLabels {
	static const std::string FRAME_HEADER = "Fach: ";

	static const std::string SAVE_CHANGINGS_BUTTON_TOOLTIP = "Änderungen speichern";
	static const std::string RESET_BUTTON_TOOLTIP = "Änderungen zurücksetzen";
	static const std::string STATISTICS_BUTTON_TOOLTIP = "Statistik anzeigen";

	static const std::string UNTIL = "<b>Aufgabe zu erledigen bis</b>";
	static const std::string REACHED_POINTS = "<b>Erreichte Punkte</b>";
	static const std::string TOTAL_POINTS = "<b>Maximale Punktzahl</b>";
	static const std::string OPEN_DIR = "<b>Verzeichnis öffnen</b>";
	static const std::string OPEN_PDF = "<b>Aufgabenblatt öffnen</b>";
	static const std::string EXERCISE_FINISHED = "<b>Erledigt</b>";
	static const std::string EXERCISE_COMMENT = "<b>Kommentar</b>";
	static const std::string DELETE_EXERCISE = "<b>Löschen</b>";

	static const std::string NO_EXERCISE_ADDED_LABEL = "<b><u>Dieses Fach enthält noch keine Aufgaben.</u></b>";
	static const std::string MULTIPLE_DELETE_TITLE = "Wollen Sie die Aufgabe löschen?";
	static const std::string MULTIPLE_DELETE_MESSAGE = "Die Aufgabenblätter, Punkte etc. ebenfalls löschen";
	static const std::string NEW_EXERCISE_ERROR_TITLE = "Fehlendes Aufgabendatum";
	static const std::string NEW_EXERCISE_ERROR_MESSAGE = "Das Feld 'bis' darf nicht leer sein.";

	static const std::string DELETE_TOOLTIP = "Aufgabe löschen";
}

namespace LessonTableRowLabels {
	static const std::string OPEN_PDF_TOOLTIP = "PDF öffnen";
	static const std::string OPEN_FOLDER_TOOLTIP = "Aufgabenordner öffnen";
}

namespace SettingsPageLabels {
	static const std::string NEW_LESSON_LABEL = "Neues Fach: ";
	static const std::string NEW_LESSON_BUTTON = "Fach anlegen";
	static const std::string DELETE_BUTTON = "Löschen";
	static const std::string ENVIRONMENT_SETTINGS_HEADING = "Umgebungseinstellungen";
	static const std::string PATH_TO_DIR_LABEL = "Aufgabenordner: ";
	static const std::string PDF_EXECUTABLE_LABEL = "Pfad zum PDF-Betrachter: ";
	static const std::string FILE_MANAGER_LABEL = "Pfad zum Dateimanager: ";
	static const std::string SAVE_ENVIRONMENT_SETTINGS = "Speichern";

	static const std::string NEW_LESSON_ERROR_TITLE = "Fehlendes Fach";
	static const std::string NEW_LESSON_ERROR_MESSAGE = "Das Feld 'Fach' darf nicht leer sein.";

	static const std::string SAVING_SUCCESS_TITLE = "Speichern erfolgreich";
	static const std::string SAVING_SUCCESS_MESSAGE = "Das neue Fach wurde der Liste hinzugefügt.";

	static const std::string MULTIPLE_DELETE_TITLE = "Wollen Sie die Aufgabe löschen?";
	static const std::string MULTIPLE_DELETE_MESSAGE = "Alle zugehörigen Aufgaben inklusive der Blätter ebenfalls löschen";
}

namespace StatisticsLessonTableLabels {
	static const std::string UNTIL_HEADING_LABEL = "<b>zu erledigebn bis:</b>";
	static const std::string REACHED_HEADING_LABEL = "<b>erreichte Punkte</b>";
	static const std::string TOTAL_HEADING_LABEL = "<b>gesamte Punkte</b>";
}

namespace GuiLabels {
	static const std::string SETTINGS = "Einstellungen";
	static const std::string STATISTICS = "Statistik";
}

namespace SettingsLessonTableLabels {
	static const std::string TABLE_HEADING = "Fächer";
}

#endif /* CONSTANTS_LABELS_H_ */
