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
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::onKeySequenceChanged(const QKeySequence &keySequence)
{
	qDebug() << keySequence.toString();
}


