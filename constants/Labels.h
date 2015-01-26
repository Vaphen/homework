#ifndef CONSTANTS_LABELS_H_
#define CONSTANTS_LABELS_H_

namespace LessonPageLabels {
	static const std::string FRAME_HEADER = "Fach: ";

	static const std::string SAVE_CHANGINGS_BUTTON = "Änderungen speichern";
	static const std::string RESET_BUTTON = "Änderungen zurücksetzen";

	static const std::string UNTIL = "<b>Aufgabe zu erledigen bis</b>";
	static const std::string REACHED_POINTS = "<b>Erreichte Punkte</b>";
	static const std::string TOTAL_POINTS = "<b>Maximale Punktzahl</b>";
	static const std::string OPEN_DIR = "<b>Verzeichnis öffnen</b>";
	static const std::string EXERCISE_FINISHED = "<b>Erledigt</b>";
	static const std::string EXERCISE_COMMENT = "<b>Kommentar</b>";
	static const std::string DELETE_EXERCISE = "<b>Löschen</b>";

	static const std::string NEW_EXERCISE_ERROR_TITLE = "Fehlendes Aufgabendatum";
	static const std::string NEW_EXERCISE_ERROR_MESSAGE = "Das Feld 'bis' darf nicht leer sein.";
}

namespace SettingsPageLabels {
	static const std::string NEW_LESSON_LABEL = "Neues Fach: ";
	static const std::string NEW_LESSON_BUTTON = "Fach anlegen: ";
	static const std::string DELETE_BUTTON = "Löschen";
	static const std::string PATH_TO_DIR_LABEL = "Pfad zum Aufgabenordner: ";
	static const std::string SAVE_PATH_TO_DIR_BUTTON = "Pfad speichern";

	static const std::string NEW_LESSON_ERROR_TITLE = "Fehlendes Fach";
	static const std::string NEW_LESSON_ERROR_MESSAGE = "Das Feld 'Fach' darf nicht leer sein.";

	static const std::string SAVING_SUCCESS_TITLE = "Speichern erfolgreich";
	static const std::string SAVING_SUCCESS_MESSAGE = "Das neue Fach wurde der Liste hinzugefügt.";
}

namespace GuiLabels {
	static const std::string SETTINGS = "Einstellungen";
}

namespace SettingsLessonTableLabels {
	static const std::string TABLE_HEADING = "Fächer";
}

#endif /* CONSTANTS_LABELS_H_ */
