#include "settingsdialog.h"
#include "colorpicker.h"

#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QPushButton>
#include <QSpinBox>
#include <QKeyEvent>

SettingsDialog::SettingsDialog(QWidget *parent) :
    Dialog(parent),
    m_dotsRadius(Preferences::instance().rubberBandWidth() * 2) // Fix this uglyness...
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
	m_pPenWidthSpinBox->setValue(Preferences::instance().penWidth());

	m_pRubberBandWidthLabel = new QLabel(tr("Band width:"));
	m_pRubberBandWidthSpinBox = new QSpinBox;
	m_pRubberBandWidthSpinBox->setRange(0, 100);
	m_pRubberBandWidthSpinBox->setValue(Preferences::instance().rubberBandWidth());

	m_pFontSizeLabel = new QLabel(tr("Font size:"));
	m_pFontSizeSpinBox = new QSpinBox;
	m_pFontSizeSpinBox->setRange(0, 100);
	m_pFontSizeSpinBox->setValue(Preferences::instance().fontSize());

	m_pResetDefaultLayout = new QHBoxLayout;
	m_pResetDefaultsButton = new QPushButton(tr("Reset Defaults"));
	m_pResetDefaultsButton->setObjectName("ResetDefaultsButton");
	m_pResetDefaultsSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding);

	m_pVerticalSpacer = new QSpacerItem(20, 10,  QSizePolicy::Minimum, QSizePolicy::Fixed);
	m_pResetDefaultLayout->addItem(m_pResetDefaultsSpacer);
	m_pResetDefaultLayout->addWidget(m_pResetDefaultsButton);

	const QColor defaultOverlayColor = Preferences::instance().overlayColor();
	m_pOverlayColorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)")
	                                     .arg(defaultOverlayColor.red())
	                                     .arg(defaultOverlayColor.green())
	                                     .arg(defaultOverlayColor.blue())
	                                     .arg(defaultOverlayColor.alpha()));

	const QColor defaultRubberBandColor = Preferences::instance().rubberBandColor();
	m_pRubberBandColorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)")
	                                        .arg(defaultRubberBandColor.red())
	                                        .arg(defaultRubberBandColor.green())
	                                        .arg(defaultRubberBandColor.blue())
	                                        .arg(defaultRubberBandColor.alpha()));

	m_pOverlayColorPicker = new ColorPicker(this);
	m_pOverlayColorPicker->setColor(defaultOverlayColor);
	m_pOverlayColorPicker->setArrowLocation(ColorPicker::Left);

	m_pRubberBandColorPicker = new ColorPicker(this);
	m_pRubberBandColorPicker->setColor(defaultRubberBandColor);
	m_pRubberBandColorPicker->setArrowLocation(ColorPicker::Left);

	connect(m_pOverlayColorButton, &QPushButton::clicked,
	        this, &SettingsDialog::onOverlayColorButtonClicked);

	connect(m_pRubberBandColorButton, &QPushButton::clicked,
	        this, &SettingsDialog::onRubberBandColorButtonClicked);

	connect(m_pOverlayColorPicker, &ColorPicker::colorChanged,
	        this, &SettingsDialog::onOverlayColorChanged);

	connect(m_pRubberBandColorPicker, &ColorPicker::colorChanged,
	        this, &SettingsDialog::onRubberBandColorChanged);

	connect(m_pRubberBandWidthSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	        this, &SettingsDialog::onRubberBandWidthChanged);

	connect(m_pPenWidthSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	        this, &SettingsDialog::onPenWidthChanged);

	connect(m_pFontSizeSpinBox,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
	        this, &SettingsDialog::onFontSizeChanged);

	connect(m_pResetDefaultsButton, &QPushButton::clicked,
	        this, &SettingsDialog::onResetButtonClicked);

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

SettingsDialog::~SettingsDialog()
{
}

QColor SettingsDialog::overlayColor() const
{
	return Preferences::instance().overlayColor();
}

void SettingsDialog::setOverlayColor(const QColor &overlayColor)
{
	Preferences::instance().setOverlayColor(overlayColor);

	Q_EMIT overlayColorChanged(overlayColor);
}

QColor SettingsDialog::rubberBandColor() const
{
	return Preferences::instance().rubberBandColor();
}

void SettingsDialog::setRubberBandColor(const QColor &rubberBandColor)
{
	Preferences::instance().setRubberBandColor(rubberBandColor);

	Q_EMIT rubberBandColorChanged(rubberBandColor);
}

int SettingsDialog::rubberBandWidth() const
{
	return Preferences::instance().rubberBandWidth();
}

void SettingsDialog::setRubberBandWidth(int rubberBandWidth)
{
	Preferences::instance().setRubberBandWidth(rubberBandWidth);

	Q_EMIT rubberBandWidthChanged(rubberBandWidth);
}

int SettingsDialog::penWidth() const
{
	return Preferences::instance().penWidth();
}

void SettingsDialog::setPenWidth(int penWidth)
{
	Preferences::instance().setPenWidth(penWidth);

	Q_EMIT penWidthChanged(penWidth);
}

int SettingsDialog::dotsRadius() const
{
	return m_dotsRadius;
}

void SettingsDialog::setDotsRadius(int dotsRadius)
{
	m_dotsRadius = dotsRadius;

	Q_EMIT dotsRadiusChanged(dotsRadius);
}

int SettingsDialog::fontSize() const
{
	return Preferences::instance().fontSize();
}

void SettingsDialog::setFontSize(int fontSize)
{
	Preferences::instance().setFontSize(fontSize);

	Q_EMIT fontSizeChanged(fontSize);
}

void SettingsDialog::moveEvent(QMoveEvent *pEvent)
{
	Q_UNUSED(pEvent)

	m_pRubberBandColorPicker->hide();
	m_pOverlayColorPicker->hide();
}

void SettingsDialog::keyPressEvent(QKeyEvent *pEvent)
{
	if(pEvent->key() == Qt::Key_Escape)
	{

		m_pRubberBandColorPicker->hide();
		m_pOverlayColorPicker->hide();
		hide();
	}
}

void SettingsDialog::onOverlayColorButtonClicked()
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

void SettingsDialog::onOverlayColorChanged(QColor newColor)
{
	newColor.setAlpha(127);

	m_pOverlayColorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
	                                     .arg(newColor.red())
	                                     .arg(newColor.green())
	                                     .arg(newColor.blue())
	                                     .arg(newColor.alpha()));
	setOverlayColor(newColor);
}

void SettingsDialog::onRubberBandColorButtonClicked()
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

void SettingsDialog::onRubberBandColorChanged(QColor newColor)
{
	m_pRubberBandColorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
	                                        .arg(newColor.red())
	                                        .arg(newColor.green())
	                                        .arg(newColor.blue()));
	setRubberBandColor(newColor);
}

void SettingsDialog::onRubberBandWidthChanged(int width)
{
	setRubberBandWidth(width);
}

void SettingsDialog::onPenWidthChanged(int width)
{
	setPenWidth(width);
}

void SettingsDialog::onFontSizeChanged(int size)
{
	setFontSize(size);
}

void SettingsDialog::onResetButtonClicked()
{
	Preferences::instance().reset();

	// Overlay color
	const QColor overlayColor = Preferences::instance().overlayColor();
	m_pOverlayColorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4);")
	                                     .arg(overlayColor.red())
	                                     .arg(overlayColor.green())
	                                     .arg(overlayColor.blue())
	                                     .arg(overlayColor.alpha()));
	m_pOverlayColorPicker->setColor(overlayColor);
	setOverlayColor(overlayColor);

	// Rubber band color
	const QColor rubberBandColor = Preferences::instance().rubberBandColor();
	m_pRubberBandColorPicker->setColor(rubberBandColor);
	setRubberBandColor(rubberBandColor);

	// Rubber band width
	const int rubberBandWidth = Preferences::instance().rubberBandWidth();
	setRubberBandWidth(rubberBandWidth);
	m_pRubberBandWidthSpinBox->setValue(rubberBandWidth);

	// Pen width
	const int penWidth = Preferences::instance().penWidth();
	setPenWidth(penWidth);
	m_pPenWidthSpinBox->setValue(penWidth);

	// Font size
	const int fontSize =Preferences::instance().fontSize();
	setFontSize(fontSize);
	m_pFontSizeSpinBox->setValue(fontSize);
}

