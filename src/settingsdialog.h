#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

// Qt includes
#include <QDialog>
#include <QMap>

namespace Ui {
	// Forward declaration
	class SettingsDialog;
}

//! \brief Class representing the application's settings dialog
//!        and the point where the main preferences get changed.
class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	//! \brief Constructs a SettingsDialog object.
	//! \param pParent Parent object.
	explicit SettingsDialog(QWidget *pParent = 0);

	//! \brief Destroys a SettingsDialog object.
	~SettingsDialog();

	//! \brief Sets the hot key for the dialog's QKeySequenceEdit.
	//! \param hotKey The hot key to be set.
	void setHotKey(const QString& hotKey);

private Q_SLOTS:
	//! \brief Slot called when the key sequence editing finishes.
	void onKeySequenceEditingFinished();

	//! \brief Slot called when the tray icon type changes.
	//! \param text The text of the new tray icon type.
	void onTrayIconTypeChanged(const QString &text);

	//! \brief Slot called when the application startup state changes.
	//! \param state State of the application startup setting. true if
	//!              the application should start on system startup, false
	//!              otherwise.
	void onLaunchAtSystemStartupChanged(bool state);

	//! \brief Slot called when the system tray show state changes.
	//! \param state State of the system tray show setting. true if
	//!              the application tray icon should appear in the
	//!              tray area and false otherwise.
	void onShowTrayIconChanged(bool state);

	//! \brief Slot called when the application language changes.
	//! \param language The new application language.
	void onLanguageChanged(const QString &language);

	//! \brief Slot called when the reset button gets pushed.
	void onResetButtonClicked();

Q_SIGNALS:
	//! \brief Signal emitted when the key sequence changes.
	void keySequenceChanged(const QKeySequence& keySequence);

	//! \brief Signal emitted when the tray icon type changes.
	//! \param text The new tray icon type text.
	void trayIconTypeChanged(const QString &text);

	//! \brief Signal emitted when the capture mouse cursor state
	//!        setting changes.
	//! \param state true if the application mouse cursor should be
	//!              captured on screenshot, false otherwise.
	void captureMouseCursorChanged(bool state);

	//! \brief Signal emitted when the launch on system startup
	//!        state setting changes.
	//! \param state true if the application should start at system
	//!              startup, false otherwise.
	void launchAtSystemStartupChanged(bool state);

	//! \brief Signal emitted when the show tray icon state
	//!        setting changes.
	//! \param state true if the application tray icon should be shown,
	//!              false otherwise.
	void showTrayIconChanged(bool state);

	//! \brief Signal emitted when the application language changes.
	//! \param language The new application language.
	void languageChanged(const QString &language);

private:
	void setupTrayIconComboBox();

private:
	// UI
	Ui::SettingsDialog *ui;

	QMap<QString, QString> m_trayIcons;
};

#endif // SETTINGSDIALOG_H
