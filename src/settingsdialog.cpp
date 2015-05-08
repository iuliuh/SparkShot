#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "preferences.h"

#include <QKeyEvent>
#include <QDebug>
#include <QApplication>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);

	connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)),
	        this, SLOT(onKeySequenceChanged(QKeySequence)));
	connect(ui->trayIconComboBox, SIGNAL(currentIndexChanged(int)),
	        this, SLOT(onTrayIconTypeChanged(int)));
	connect(ui->captureMouseCursorCheckBox, SIGNAL(clicked(bool)),
	        this, SLOT(onCaptureMouseCursorChanged(bool)));
	connect(ui->launchOnSystemStartupCheckBox, SIGNAL(clicked(bool)),
	        this, SLOT(onLaunchAtSystemStartupChanged(bool)));
	connect(ui->showTrayIconCheckBox, SIGNAL(clicked(bool)),
	        this, SLOT(onShowTrayIcon(bool)));
	connect(ui->languageComboBox, SIGNAL(currentIndexChanged(QString)),
	        this, SLOT(onLanguageChanged(const QString&)));

	ui->trayIconComboBox->setCurrentIndex(Preferences::instance().trayIconType());
	ui->captureMouseCursorCheckBox->setChecked(Preferences::instance().isMouseCursorCaptured());
	ui->launchOnSystemStartupCheckBox->setChecked(Preferences::instance().isLaunchingOnSystemStartup());
	ui->showTrayIconCheckBox->setChecked(Preferences::instance().isTrayIconShown());
	ui->languageComboBox->setCurrentText(Preferences::instance().language());
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::onKeySequenceChanged(const QKeySequence &keySequence)
{
	const QString keySequenceString = keySequence.toString();

	if(keySequenceString.isEmpty())
	{
		return;
	}

	Q_EMIT keySequenceChanged(keySequenceString);
}

void SettingsDialog::onTrayIconTypeChanged(int index)
{
	Preferences::instance().setTrayIconType(index);

	Q_EMIT trayIconTypeChanged(index);
}

void SettingsDialog::onCaptureMouseCursorChanged(bool state)
{
	Preferences::instance().setCaptureMouseCursor(state);

	Q_EMIT captureMouseCursorChanged(state);
}

void SettingsDialog::onLaunchAtSystemStartupChanged(bool state)
{
	Preferences::instance().setLaunchOnSystemStartup(state);

	Q_EMIT launchAtSystemStartupChanged(state);
}

void SettingsDialog::onShowTrayIconChanged(bool state)
{
	Preferences::instance().setShowTrayIcon(state);

	Q_EMIT showTrayIconChanged(state);
}

void SettingsDialog::onLanguageChanged(const QString& language)
{
	Preferences::instance().setLanguage(language);

	Q_EMIT languageChanged(language);
}


