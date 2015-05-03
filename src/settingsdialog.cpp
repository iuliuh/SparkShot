#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QKeyEvent>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);

	connect(ui->keySequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)),
	        this, SLOT(onKeySequenceChanged(QKeySequence)));
	connect(ui->trayIconComboBox, SIGNAL(currentIndexChanged(QString)),
	        this, SLOT(onTrayIconTypeChanged(QString)));
	connect(ui->captureMouseCursorCheckBox, SIGNAL(clicked(bool)),
	        this, SLOT(onCaptureMouseCursorChanged(bool)));
	connect(ui->launchOnSystemStartupCheckBox, SIGNAL(clicked(bool)),
	        this, SLOT(onLaunchAtSystemStartupChanged(bool)));
	connect(ui->showTrayIconCheckBox, SIGNAL(clicked(bool)),
	        this, SLOT(onShowTrayIcon(bool)));
	connect(ui->languageComboBox, SIGNAL(currentIndexChanged(QString)),
	        this, SLOT(onLanguageChanged(const QString&)));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::onKeySequenceChanged(const QKeySequence &keySequence)
{
	qDebug() << keySequence.toString();
}

void SettingsDialog::onTrayIconTypeChanged(const QString &icon)
{
	Q_UNUSED(icon)
}

void SettingsDialog::onCaptureMouseCursorChanged(bool state)
{
	Q_UNUSED(state)
}

void SettingsDialog::onLaunchAtSystemStartupChanged(bool state)
{
	Q_UNUSED(state)
}

void SettingsDialog::onShowTrayIcon(bool state)
{
	Q_UNUSED(state)
}

void SettingsDialog::onLanguageChanged(const QString& language)
{
	Q_UNUSED(language)
}


