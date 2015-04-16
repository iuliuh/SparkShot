#ifndef COLORPICKERDIALOG_H
#define COLORPICKERDIALOG_H

#include <QDialog>

namespace Ui {
class ColorPickerDialog;
}

class ColorPickerDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ColorPickerDialog(QWidget *parent = 0);
	~ColorPickerDialog();

protected:
	void paintEvent(QPaintEvent *e);

private:
	Ui::ColorPickerDialog *ui;
};

#endif // COLORPICKERDIALOG_H
