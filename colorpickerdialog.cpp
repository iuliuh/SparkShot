#include "colorpickerdialog.h"

#include <QPainter>

ColorPickerDialog::ColorPickerDialog(QWidget *parent) :
    Dialog(parent)
{
	connect(&m_colorPicker, &ColorPicker::colorChanged,
	        this, &ColorPickerDialog::colorChanged);

	resize(100, 100);

	m_pLayout = new QGridLayout;

	m_pLayout->addWidget(&m_colorPicker);

	setLayout(m_pLayout);
}

ColorPickerDialog::~ColorPickerDialog()
{
}

