#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

private Q_SLOTS:
	void onKeySequenceChanged(const QKeySequence& keySequence);
	void onTrayIconTypeChanged(int index);
	void onCaptureMouseCursorChanged(bool state);
	void onLaunchAtSystemStartupChanged(bool state);
	void onShowTrayIconChanged(bool state);
	void onLanguageChanged(const QString &language);

Q_SIGNALS:
	void keySequenceChanged(const QString& keySequence);
	void trayIconTypeChanged(int index);
	void captureMouseCursorChanged(bool state);
	void launchAtSystemStartupChanged(bool state);
	void showTrayIconChanged(bool state);
	void languageChanged(const QString &language);

private:
	Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
