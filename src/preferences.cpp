#include "preferences.h"

#include <QSettings>
#include <QColor>

Preferences* Preferences::m_pInstance = 0;

const QString Preferences::OVERLAY_COLOR_KEY = "OverlayColor";
const QColor Preferences::OVERLAY_COLOR_DEFAULT_VALUE(0, 0, 0, 127);

const QString Preferences::RUBBER_BAND_COLOR_KEY = "RubberBandColor";
const QColor Preferences::RUBBER_BAND_COLOR_DEFAULT_VALUE = Qt::cyan;

const QString Preferences::RUBBER_BAND_WIDTH_KEY = "RubberBandWidth";
const int Preferences::RUBBER_BAND_WIDTH_DEFAULT_VALUE = 2;

const QString Preferences::PEN_WIDTH_KEY = "PenWidth";
const int Preferences::PEN_WIDTH_DEFAULT_VALUE = 2;

const QString Preferences::FONT_SIZE_KEY = "FontSize";
const int Preferences::FONT_SIZE_DEFAULT_VALUE = 12;

Preferences& Preferences::instance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new Preferences();
	}

	return *m_pInstance;
}

void Preferences::setOverlayColor(QColor color)
{
	m_pSettings->setValue(OVERLAY_COLOR_KEY, color);
}

QColor Preferences::overlayColor() const
{
	return m_pSettings->value(OVERLAY_COLOR_KEY, OVERLAY_COLOR_DEFAULT_VALUE).value<QColor>();
}

void Preferences::setRubberBandColor(QColor color)
{
	m_pSettings->setValue(RUBBER_BAND_COLOR_KEY, color);
}

QColor Preferences::rubberBandColor() const
{
	return m_pSettings->value(RUBBER_BAND_COLOR_KEY, RUBBER_BAND_COLOR_DEFAULT_VALUE).value<QColor>();
}

void Preferences::setRubberBandWidth(int width)
{
	m_pSettings->setValue(RUBBER_BAND_WIDTH_KEY, width);
}

int Preferences::rubberBandWidth() const
{
	return m_pSettings->value(RUBBER_BAND_WIDTH_KEY, RUBBER_BAND_WIDTH_DEFAULT_VALUE).toInt();
}

void Preferences::setPenWidth(int width)
{
	m_pSettings->setValue(PEN_WIDTH_KEY, width);
}

int Preferences::penWidth() const
{
	return m_pSettings->value(PEN_WIDTH_KEY, PEN_WIDTH_DEFAULT_VALUE).toInt();
}

void Preferences::setFontSize(int size)
{
	m_pSettings->setValue(FONT_SIZE_KEY, size);
}

int Preferences::fontSize() const
{
	return m_pSettings->value(FONT_SIZE_KEY, FONT_SIZE_DEFAULT_VALUE).toInt();
}

Preferences::Preferences(QObject *pParent) : QObject(pParent)
{
	m_pSettings = new QSettings();

	atexit(&cleanUp);
}

Preferences::~Preferences()
{
	delete m_pSettings;

	m_pSettings = 0;
}

void Preferences::cleanUp()
{
	delete m_pInstance;

	m_pInstance = 0;
}
