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

const QString Preferences::LANGUAGE_KEY = "Language";
const QString Preferences::LANGUAGE_DEFAULT_VALUE = "English";

const QString Preferences::HOTKEY_KEY = "HotKey";
const QString Preferences::HOTKEY_DEFAULT_VALUE = "Print";

const QString Preferences::TRAY_ICON_TYPE_KEY = "TrayIconType";
const int Preferences::TRAY_ICON_TYPE_DEFAULT_VALUE = 0;

const QString Preferences::CAPTURE_MOUSE_CURSLOR_KEY = "CaptureCursor";
const bool Preferences::CAPTURE_MOUSE_CURSLOR_DEFAULT_VALUE = false;

const QString Preferences::LAUNCH_ON_SYSTEM_STARTUP_KEY = "LaunchOnStartup";
const bool Preferences::LAUNCH_ON_SYSTEM_STARTUP_DEFAULT_VALUE = true;

const QString Preferences::SHOW_TRAY_ICON_KEY = "ShowTrayIcon";
const bool Preferences::SHOW_TRAY_ICON_DEFAULT_VALUE = true;

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

void Preferences::setHotKey(const QString& hotKey)
{
	m_pSettings->setValue(HOTKEY_KEY, hotKey);
}

QString Preferences::hotKey() const
{
	return m_pSettings->value(HOTKEY_KEY, HOTKEY_DEFAULT_VALUE).toString();
}

void Preferences::setTrayIconType(int iconType)
{
	m_pSettings->setValue(TRAY_ICON_TYPE_KEY, iconType);
}

int Preferences::trayIconType() const
{
	return m_pSettings->value(TRAY_ICON_TYPE_KEY, TRAY_ICON_TYPE_DEFAULT_VALUE).toInt();
}

void Preferences::setCaptureMouseCursor(bool capture)
{
	m_pSettings->setValue(CAPTURE_MOUSE_CURSLOR_KEY, capture);
}

bool Preferences::isMouseCursorCaptured() const
{
	return m_pSettings->value(CAPTURE_MOUSE_CURSLOR_KEY, CAPTURE_MOUSE_CURSLOR_DEFAULT_VALUE).toBool();
}

void Preferences::setLaunchOnSystemStartup(bool launch)
{
	m_pSettings->setValue(LAUNCH_ON_SYSTEM_STARTUP_KEY, launch);
}

bool Preferences::isLaunchingOnSystemStartup() const
{
	return m_pSettings->value(LAUNCH_ON_SYSTEM_STARTUP_KEY, LAUNCH_ON_SYSTEM_STARTUP_DEFAULT_VALUE).toBool();
}

void Preferences::setShowTrayIcon(bool show)
{
	m_pSettings->setValue(SHOW_TRAY_ICON_KEY, show);
}

bool Preferences::isTrayIconShown() const
{
	return m_pSettings->value(SHOW_TRAY_ICON_KEY, SHOW_TRAY_ICON_DEFAULT_VALUE).toBool();
}

void Preferences::setLanguage(const QString &language)
{
	m_pSettings->setValue(LANGUAGE_KEY, language);
}

QString Preferences::language() const
{
	return m_pSettings->value(LANGUAGE_KEY, LANGUAGE_DEFAULT_VALUE).toString();
}

void Preferences::reset()
{
	setOverlayColor(OVERLAY_COLOR_DEFAULT_VALUE);
	setRubberBandColor(RUBBER_BAND_COLOR_DEFAULT_VALUE);
	setRubberBandWidth(RUBBER_BAND_WIDTH_DEFAULT_VALUE);
	setPenWidth(PEN_WIDTH_DEFAULT_VALUE);
	setFontSize(FONT_SIZE_DEFAULT_VALUE);
	setHotKey(HOTKEY_DEFAULT_VALUE);
	setTrayIconType(TRAY_ICON_TYPE_DEFAULT_VALUE);
	setCaptureMouseCursor(CAPTURE_MOUSE_CURSLOR_DEFAULT_VALUE);
	setLaunchOnSystemStartup(LAUNCH_ON_SYSTEM_STARTUP_DEFAULT_VALUE);
	setShowTrayIcon(SHOW_TRAY_ICON_DEFAULT_VALUE);
	setLanguage(LANGUAGE_DEFAULT_VALUE);
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
