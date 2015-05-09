#include "uploaddialog.h"
#include "ui_uploaddialog.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>

UploadDialog::UploadDialog(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::UploadDialog)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);

	connect(ui->copyButton, &QPushButton::clicked, this, &UploadDialog::onCopyButtonClicked);
	connect(ui->openButton, &QPushButton::clicked, this, &UploadDialog::onOpenButtonClicked);
}

UploadDialog::~UploadDialog()
{
	delete ui;
}

void UploadDialog::setLink(const QString& link)
{
	ui->linkLineEdit->setText(link);
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

void UploadDialog::setProgressBarView()
{
	ui->stackedWidget->setCurrentIndex(1);
}

void UploadDialog::setLinkView()
{
	ui->stackedWidget->setCurrentIndex(0);
}
