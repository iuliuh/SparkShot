#ifndef UPLOADDIALOG_H
#define UPLOADDIALOG_H

#include <QDialog>

namespace Ui {
	class UploadDialog;
}

class UploadDialog : public QDialog
{
	Q_OBJECT

public:
	explicit UploadDialog(QWidget *parent = 0);
	~UploadDialog();

	void setLink(const QString& text);
	void setProgressBarView();
	void setLinkView();

private Q_SLOTS:
	void onCopyButtonClicked();
	void onOpenButtonClicked();

private:
	Ui::UploadDialog *ui;
};

#endif // UPLOADDIALOG_H
