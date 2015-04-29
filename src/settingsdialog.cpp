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

	emit overlayColorChanged(overlayColor);
}

QColor SettingsDialog::rubberBandColor() const
{
	return Preferences::instance().rubberBandColor();
}

void SettingsDialog::setRubberBandColor(const QColor &rubberBandColor)
{
	Preferences::instance().setRubberBandColor(rubberBandColor);

	emit rubberBandColorChanged(rubberBandColor);
}

int SettingsDialog::rubberBandWidth() const
{
	return Preferences::instance().rubberBandWidth();
}

void SettingsDialog::setRubberBandWidth(int rubberBandWidth)
{
	Preferences::instance().setRubberBandWidth(rubberBandWidth);

	emit rubberBandWidthChanged(rubberBandWidth);
}

int SettingsDialog::penWidth() const
{
	return Preferences::instance().penWidth();
}

void SettingsDialog::setPenWidth(int penWidth)
{
	Preferences::instance().setPenWidth(penWidth);

	emit penWidthChanged(penWidth);
}

int SettingsDialog::dotsRadius() const
{
	return m_dotsRadius;
}

void SettingsDialog::setDotsRadius(int dotsRadius)
{
	m_dotsRadius = dotsRadius;

	emit dotsRadiusChanged(dotsRadius);
}

int SettingsDialog::fontSize() const
{
	return Preferences::instance().fontSize();
}

void SettingsDialog::setFontSize(int fontSize)
{
	Preferences::instance().setFontSize(fontSize);

	emit fontSizeChanged(fontSize);
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

