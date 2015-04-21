#include "settings.h"
#include "color-picker.h"

#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>
#include <QSpinBox>

Settings::Settings(QWidget *parent) :
    Dialog(parent),
    m_overlayColor(0, 0, 0, 155),
    m_rubberBandColor(Qt::cyan),
    m_rubberBandWidth(2),
    m_penWidth(2),
    m_dotsRadius(m_rubberBandWidth * 2),
    m_fontSize(12)
{
	m_pLayout = new QGridLayout;

	m_pOverlayColorLabel = new QLabel(tr("Overlay color:"));
	m_pOverlayColorButton = new QPushButton;
	m_pOverlayColorButton->setFlat(true);

	m_pRubberBandColorLabel = new QLabel(tr("Band color:"));
	m_pRubberBandColorButton = new QPushButton;
	m_pRubberBandColorButton->setFlat(true);

	m_pPenWidthLabel = new QLabel(tr("Pen width:"));
	m_pPenWidthSpinBox = new QSpinBox;
	m_pPenWidthSpinBox->setRange(0, 100);

	m_pRubberBandWidthLabel = new QLabel(tr("Band width:"));
	m_pRubberBandWidthSpinBox = new QSpinBox;
	m_pRubberBandWidthSpinBox->setRange(0, 100);

	m_pFontSizeLabel = new QLabel(tr("Font size:"));
	m_pFontSizeSpinBox = new QSpinBox;
	m_pFontSizeSpinBox->setRange(0, 100);

	m_pResetDefaultLayout = new QHBoxLayout;
	m_pResetDefaultsButton = new QPushButton(tr("Reset Defaults"));
	m_pResetDefaultsButton->setObjectName("ResetDefaultsButton");
	m_pResetDefaultsSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding);

	m_pVerticalSpacer = new QSpacerItem(20, 40,  QSizePolicy::Minimum, QSizePolicy::Expanding);
	m_pResetDefaultLayout->addItem(m_pResetDefaultsSpacer);
	m_pResetDefaultLayout->addWidget(m_pResetDefaultsButton);

	m_pOverlayColorButton->setStyleSheet("background-color: black");
	m_pRubberBandColorButton->setStyleSheet("background-color: cyan");

	m_pOverlayColorPicker = new ColorPicker(this);
	m_pOverlayColorPicker->setArrowLocation(ColorPicker::Left);

	m_pRubberBandColorPicker = new ColorPicker(this);
	m_pRubberBandColorPicker->setArrowLocation(ColorPicker::Left);

	connect(m_pOverlayColorButton, &QPushButton::clicked,
	        this, &Settings::onOverlayColorButtonClicked);

	connect(m_pRubberBandColorButton, &QPushButton::clicked,
	        this, &Settings::onRubberBandColorButtonClicked);

	connect(m_pOverlayColorPicker, &ColorPicker::colorChanged,
	        this, &Settings::onOverlayColorChanged);

	connect(m_pRubberBandColorPicker, &ColorPicker::colorChanged,
	        this, &Settings::onRubberBandColorChanged);

	m_pLayout->addWidget(m_pOverlayColorLabel, 0, 0);
	m_pLayout->addWidget(m_pOverlayColorButton, 0, 1);
	m_pLayout->addWidget(m_pRubberBandColorLabel, 1, 0);
	m_pLayout->addWidget(m_pRubberBandColorButton, 1, 1);
	m_pLayout->addWidget(m_pPenWidthLabel, 2, 0);
	m_pLayout->addWidget(m_pPenWidthSpinBox, 2, 1);
	m_pLayout->addWidget(m_pRubberBandWidthLabel, 3, 0);
	m_pLayout->addWidget(m_pRubberBandWidthSpinBox, 3, 1);
	m_pLayout->addWidget(m_pFontSizeLabel, 4, 0);
	m_pLayout->addWidget(m_pFontSizeSpinBox, 4, 1);
	m_pLayout->addItem(m_pVerticalSpacer, 5, 0, 1, 0);
	m_pLayout->addLayout(m_pResetDefaultLayout, 6, 0, 1, 0);

	m_pLayout->setMargin(25);

	setLayout(m_pLayout);

	resize(200, 270);
}

Settings::~Settings()
{

}

QColor Settings::overlayColor() const
{
	return m_overlayColor;
}

void Settings::setOverlayColor(const QColor &overlayColor)
{
	m_overlayColor = overlayColor;

	emit overlayColorChanged(overlayColor);
}

QColor Settings::rubberBandColor() const
{
	return m_rubberBandColor;
}

void Settings::setRubberBandColor(const QColor &rubberBandColor)
{
	m_rubberBandColor = rubberBandColor;

	emit rubberBandColorChanged(rubberBandColor);
}

int Settings::rubberBandWidth() const
{
	return m_rubberBandWidth;
}

void Settings::setRubberBandWidth(int rubberBandWidth)
{
	m_rubberBandWidth = rubberBandWidth;

	emit rubberBandWidthChanged(rubberBandWidth);
}

int Settings::penWidth() const
{
	return m_penWidth;
}

void Settings::setPenWidth(int penWidth)
{
	m_penWidth = penWidth;

	emit penWidthChanged(penWidth);
}

int Settings::dotsRadius() const
{
	return m_dotsRadius;
}

void Settings::setDotsRadius(int dotsRadius)
{
	m_dotsRadius = dotsRadius;

	emit dotsRadiusChanged(dotsRadius);
}

int Settings::fontSize() const
{
	return m_fontSize;
}

void Settings::setFontSize(int fontSize)
{
	m_fontSize = fontSize;

	emit fontSizeChanged(fontSize);
}

void Settings::hide()
{
	m_pOverlayColorPicker->hide();
	m_pRubberBandColorPicker->hide();

	QWidget::hide();
}

void Settings::moveEvent(QMoveEvent *pEvent)
{
	Q_UNUSED(pEvent)

	m_pRubberBandColorPicker->hide();
	m_pOverlayColorPicker->hide();
}

void Settings::onOverlayColorButtonClicked()
{
	const int threshold = 15;
	int colorPickerXPosition = -m_pOverlayColorButton->mapFromGlobal(QPoint(0, 0)).x();
	int colorPickerYPosition = -m_pOverlayColorButton->mapFromGlobal(QPoint(0, 0)).y();

	m_pOverlayColorPicker->setGeometry(colorPickerXPosition + m_pOverlayColorButton->width() + threshold,
	                                   colorPickerYPosition - m_pOverlayColorPicker->height() / 2,
	                                   m_pOverlayColorPicker->width(),
	                                   m_pOverlayColorPicker->height());

	m_pRubberBandColorPicker->hide();

	m_pOverlayColorPicker->isVisible() ? m_pOverlayColorPicker->hide() : m_pOverlayColorPicker->show();
}

void Settings::onOverlayColorChanged(QColor newColor)
{
	m_overlayColor = newColor;
	m_overlayColor.setAlpha(127);

	m_pOverlayColorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
	                                     .arg(m_overlayColor.red())
	                                     .arg(m_overlayColor.green())
	                                     .arg(m_overlayColor.blue())
	                                     .arg(m_overlayColor.alpha()));
	setOverlayColor(m_overlayColor);
}

void Settings::onRubberBandColorButtonClicked()
{
	const int threshold = 15;
	int colorPickerXPosition = -m_pRubberBandColorButton->mapFromGlobal(QPoint(0, 0)).x();
	int colorPickerYPosition = -m_pRubberBandColorButton->mapFromGlobal(QPoint(0, 0)).y();

	m_pRubberBandColorPicker->setGeometry(colorPickerXPosition + m_pRubberBandColorButton->width() + threshold,
	                                      colorPickerYPosition - m_pRubberBandColorPicker->height() / 2,
	                                      m_pRubberBandColorPicker->width(),
	                                      m_pRubberBandColorPicker->height());

	m_pOverlayColorPicker->hide();

	m_pRubberBandColorPicker->isVisible() ? m_pRubberBandColorPicker->hide() : m_pRubberBandColorPicker->show();
}

void Settings::onRubberBandColorChanged(QColor newColor)
{
	m_rubberBandColor = newColor;
	m_pRubberBandColorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
	                                        .arg(newColor.red())
	                                        .arg(newColor.green())
	                                        .arg(newColor.blue()));
	setRubberBandColor(m_rubberBandColor);
}

