#ifndef COLORPICKERDIALOG_H
#define COLORPICKERDIALOG_H

#include "dialog.h"
#include "colorpicker.h"

#include <QGridLayout>

class ColorPickerDialog : public Dialog
{
	Q_OBJECT

public:
	explicit ColorPickerDialog(QWidget *parent = 0);
	~ColorPickerDialog();

signals:
	void colorChanged(QColor);

private:
	QGridLayout *m_pLayout;
	ColorPicker m_colorPicker;
};

#endif // COLORPICKERDIALOG_H
