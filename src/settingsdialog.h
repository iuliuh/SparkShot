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
	void onTrayIconTypeChanged(const QString& icon);
	void onCaptureMouseCursorChanged(bool state);
	void onLaunchAtSystemStartupChanged(bool state);
	void onShowTrayIcon(bool state);
	void onLanguageChanged(const QString &language);

Q_SIGNALS:
	void keySequenceChanged(const QString& keySequence);

private:
	Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
