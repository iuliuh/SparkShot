#include "uploaddialog.h"
#include "ui_uploaddialog.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>

UploadDialog::UploadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadDialog)
{
	ui->setupUi(this);

	connect(ui->copyButton, &QPushButton::clicked, this, &UploadDialog::onCopyButtonClicked);
	connect(ui->openButton, &QPushButton::clicked, this, &UploadDialog::onOpenButtonClicked);
}

UploadDialog::~UploadDialog()
{
	delete ui;
}

void UploadDialog::setLink(const QString& text)
{
	ui->linkLineEdit->setText(text);
}

void UploadDialog::onCopyButtonClicked()
{
	QGuiApplication::clipboard()->setText(ui->linkLineEdit->text());

	close();
}

void UploadDialog::onOpenButtonClicked()
{
	QDesktopServices::openUrl(QUrl(ui->linkLineEdit->text()));

	close();
}
