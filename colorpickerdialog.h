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
	enum ArrowLocation
	{
		Top,
		Right,
		Bottom,
		Left
	};

	explicit ColorPickerDialog(QWidget *parent = 0);
	~ColorPickerDialog();

	void setArrowLocation(ArrowLocation arrowLocation);
	ArrowLocation arrowLocation() const;

signals:
	void colorChanged(QColor);

protected:
	void paintEvent(QPaintEvent *e);

private:
	Ui::ColorPickerDialog *ui;
	ArrowLocation m_arrowLocation;
};

#endif // COLORPICKERDIALOG_H
