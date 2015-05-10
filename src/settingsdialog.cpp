#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "preferences.h"

#include <QKeyEvent>
#include <QApplication>

SettingsDialog::SettingsDialog(QWidget *pParent) :
    QDialog(pParent),
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
	        this, SLOT(onShowTrayIconChanged(bool)));
	connect(ui->languageComboBox, SIGNAL(currentIndexChanged(QString)),
	        this, SLOT(onLanguageChanged(const QString&)));
	connect(ui->okButton, SIGNAL(clicked()),
	        this, SLOT(accept()));
	connect(ui->resetToDefaultsButton, SIGNAL(clicked()),
	        this, SLOT(onResetButtonClicked()));

	ui->keySequenceEdit->setKeySequence(Preferences::instance().hotKey());
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

void SettingsDialog::setHotKey(const QString& hotKey)
{
	ui->keySequenceEdit->setKeySequence(QKeySequence::fromString(hotKey));
}

void SettingsDialog::onKeySequenceChanged(const QKeySequence &keySequence)
{
	const QString keySequenceString = keySequence.toString();

	if(keySequenceString.isEmpty())
	{
		return;
	}

	//! \note Do NOT write the HotKey preferences here. The HotKey must be
	//!       validated by the HotKeyBinder and the preferences are set by
	//!       the HotKeyBinder

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

void SettingsDialog::onResetButtonClicked()
{
	Preferences::instance().reset();

	const QString keySequenceString = Preferences::instance().hotKey();
	ui->keySequenceEdit->setKeySequence(QKeySequence::fromString(keySequenceString));
	Q_EMIT keySequenceChanged(keySequenceString);

	const int trayIconIndex = Preferences::instance().trayIconType();
	ui->trayIconComboBox->setCurrentIndex(trayIconIndex);
	Q_EMIT trayIconTypeChanged(trayIconIndex);

	const bool mouseCursorCaptureState = Preferences::instance().isMouseCursorCaptured();
	ui->captureMouseCursorCheckBox->setChecked(mouseCursorCaptureState);
	Q_EMIT captureMouseCursorChanged(mouseCursorCaptureState);

	const bool launchAtStartupState = Preferences::instance().isLaunchingOnSystemStartup();
	ui->launchOnSystemStartupCheckBox->setChecked(launchAtStartupState);
	Q_EMIT launchAtSystemStartupChanged(launchAtStartupState);

	const bool showTrayIconState = Preferences::instance().isTrayIconShown();
	ui->showTrayIconCheckBox->setChecked(showTrayIconState);
	Q_EMIT showTrayIconChanged(showTrayIconState);

	const QString languageString = Preferences::instance().language();
	ui->languageComboBox->setCurrentText(languageString);
	Q_EMIT languageChanged(languageString);
}


