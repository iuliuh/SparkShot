#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "preferences.h"

#include <QKeyEvent>
#include <QApplication>
#include <QKeySequence>
#include <QDebug>
#include <QtGlobal>

SettingsDialog::SettingsDialog(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);

	setupTrayIconComboBox();

	setAttribute(Qt::WA_DeleteOnClose);

	connect(ui->keySequenceEdit, &QKeySequenceEdit::editingFinished,
	        this, &SettingsDialog::onKeySequenceEditingFinished);
	connect(ui->trayIconComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
	        this, QOverload<const QString &>::of(&SettingsDialog::onTrayIconTypeChanged));
	connect(ui->launchOnSystemStartupCheckBox, &QCheckBox::clicked,
	        this, &SettingsDialog::onLaunchAtSystemStartupChanged);
	connect(ui->showTrayIconCheckBox, &QCheckBox::clicked,
	        this, &SettingsDialog::onShowTrayIconChanged);
	connect(ui->languageComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
	        this, QOverload<const QString &>::of(&SettingsDialog::onLanguageChanged));
	connect(ui->okButton, &QPushButton::clicked,
	        this, &SettingsDialog::accept);
	connect(ui->resetToDefaultsButton, &QPushButton::clicked,
	        this, &SettingsDialog::onResetButtonClicked);

	ui->keySequenceEdit->setKeySequence(QKeySequence::fromString(Preferences::instance().hotKey(), QKeySequence::NativeText));
	ui->trayIconComboBox->setCurrentText(m_trayIcons.key(Preferences::instance().trayIconType()));
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

void SettingsDialog::onKeySequenceEditingFinished()
{
	Q_EMIT keySequenceChanged(ui->keySequenceEdit->keySequence());
}

void SettingsDialog::onTrayIconTypeChanged(const QString &text)
{
	Preferences::instance().setTrayIconType(m_trayIcons.value(text));

	Q_EMIT trayIconTypeChanged(m_trayIcons.value(text));
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
	ui->keySequenceEdit->setKeySequence(QKeySequence::fromString(keySequenceString, QKeySequence::NativeText));
	Q_EMIT keySequenceChanged(keySequenceString);

	const QString trayIcon = Preferences::instance().trayIconType();
	ui->trayIconComboBox->setCurrentText(m_trayIcons.key(trayIcon));
	Q_EMIT trayIconTypeChanged(trayIcon);

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

void SettingsDialog::setupTrayIconComboBox()
{
	m_trayIcons.insert(QStringLiteral("Normal"), QStringLiteral(":/images/trayIconNormal"));
	m_trayIcons.insert(QStringLiteral("Light"), QStringLiteral(":/images/trayIconLight"));
	m_trayIcons.insert(QStringLiteral("Dark"), QStringLiteral(":/images/trayIconDark"));

    QMapIterator<QString, QString> it(m_trayIcons);
    while (it.hasNext()) {
        it.next();
        const QString name = it.key();
        const QString icon = it.value();

        ui->trayIconComboBox->addItem(QIcon(icon), name);
    }
}


