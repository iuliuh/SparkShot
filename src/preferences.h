#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>

class QSettings;

class Preferences : public QObject
{
	Q_OBJECT

public:
	static Preferences& instance();

	void setOverlayColor(QColor color);
	QColor overlayColor() const;

	void setRubberBandColor(QColor color);
	QColor rubberBandColor() const;

	void setRubberBandWidth(int width);
	int rubberBandWidth() const;

	void setPenWidth(int width);
	int penWidth() const;

	void setFontSize(int size);
	int fontSize() const;

	void setHotKey(const QString& hotKey);
	QString hotKey() const;

	void setTrayIconType(int iconType);
	int trayIconType() const;

	void setCaptureMouseCursor(bool capture);
	bool isMouseCursorCaptured() const;

	void setLaunchOnSystemStartup(bool launch);
	bool isLaunchingOnSystemStartup() const;

	void setShowTrayIcon(bool show);
	bool isTrayIconShown() const;

	void setLanguage(const QString& language);
	QString language() const;

	void reset();

private:
	Preferences(QObject* pParent = 0);
	~Preferences();

	Q_DISABLE_COPY(Preferences)

	static void cleanUp();

private:
	static Preferences* m_pInstance;

	QSettings* m_pSettings;

	static const QString OVERLAY_COLOR_KEY;
	static const QColor OVERLAY_COLOR_DEFAULT_VALUE;

	static const QString RUBBER_BAND_COLOR_KEY;
	static const QColor RUBBER_BAND_COLOR_DEFAULT_VALUE;

	static const QString RUBBER_BAND_WIDTH_KEY;
	static const int RUBBER_BAND_WIDTH_DEFAULT_VALUE;

	static const QString PEN_WIDTH_KEY;
	static const int PEN_WIDTH_DEFAULT_VALUE;

	static const QString FONT_SIZE_KEY;
	static const int FONT_SIZE_DEFAULT_VALUE;

	static const QString HOTKEY_KEY;
	static const QString HOTKEY_DEFAULT_VALUE;

	static const QString TRAY_ICON_TYPE_KEY;
	static const int TRAY_ICON_TYPE_DEFAULT_VALUE;

	static const QString CAPTURE_MOUSE_CURSLOR_KEY;
	static const bool CAPTURE_MOUSE_CURSLOR_DEFAULT_VALUE;

	static const QString LAUNCH_ON_SYSTEM_STARTUP_KEY;
	static const bool LAUNCH_ON_SYSTEM_STARTUP_DEFAULT_VALUE;

	static const QString SHOW_TRAY_ICON_KEY;
	static const bool SHOW_TRAY_ICON_DEFAULT_VALUE;

	static const QString LANGUAGE_KEY;
	static const QString LANGUAGE_DEFAULT_VALUE;
};

#endif // PREFERENCES_H
