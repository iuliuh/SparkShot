#ifndef UPLOADDIALOG_H
#define UPLOADDIALOG_H

// Qt includes
#include <QDialog>

namespace Ui {
	// Forward declaration
	class UploadDialog;
}

//! \brief Dialog which appears when the user wants
//!        to upload his screenshot.
class UploadDialog : public QDialog
{
	Q_OBJECT

public:
	//! \brief Constructs a UploadDialog object.
	//! \brief pParent Parent object.
	explicit UploadDialog(QWidget *pParent = 0);

	//! \brief Destroys a UploadDialog object.
	~UploadDialog();

	//! \brief Sets the link to show after the image was uploaded.
	//! \param link The link to show.
	void setLink(const QString& link);

	//! \brief Sets the progress bar view for the dialog. This
	//!        view is set after the upload request has been made
	//!        and till the uploading has finished.
	void setProgressBarView();

	//! \brief Sets the link view for the dialog. This
	//!        view is set after the upload reply has been received
	//!        and the upload was successful. It shows the url to the
	//!        uploaded image.
	void setLinkView();

private Q_SLOTS:
	//! \brief Slot called when the copy button has been clicked.
	void onCopyButtonClicked();

	//! \brief Slot called when the open button has been clicked.
	void onOpenButtonClicked();

private:
	// UI
	Ui::UploadDialog *ui;
};

#endif // UPLOADDIALOG_H
