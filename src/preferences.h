#ifndef PREFERENCES_H
#define PREFERENCES_H

// Qt includes
#include <QObject>

// Forward declarations
class QSettings;

//! \brief Preferences class which stores all
//!        the application settings.
class Preferences : public QObject
{
	Q_OBJECT

public:
	//! \brief Provides a Preferences instance. If instance is not created
	//!        it automatically creates one.
	//! \return Preferences reference.
	static Preferences& instance();

	//! \brief Sets the overlay color settings.
	//! \param color The new overlay color.
	void setOverlayColor(QColor color);

	//! \brief Provides the current overlay color.
	//! \return The overlay color.
	QColor overlayColor() const;

	//! \brief Sets the rubber band color settings.
	//! \param color The new rubber band color.
	void setRubberBandColor(QColor color);

	//! \brief Provides the current rubber band color.
	//! \return The rubber band color.
	QColor rubberBandColor() const;

	//! \brief Sets the rubber band width settings.
	//! \param width The new rubber band width.
	void setRubberBandWidth(int width);

	//! \brief Provides the current rubber band width.
	//! \return The rubber band width.
	int rubberBandWidth() const;

	//! \brief Sets the pen width settings.
	//! \param width The new pen width.
	void setPenWidth(int width);

	//! \brief Provides the current pen width.
	//! \return The pen width.
	int penWidth() const;

	//! \brief Sets the font size settings.
	//! \param size The new font size.
	void setFontSize(int size);

	//! \brief Provides the current font size.
	//! \return The font size.
	int fontSize() const;

	//! \brief Sets the hot key settings.
	//! \param hotKey The new hot key.
	void setHotKey(const QString& hotKey);

	//! \brief Provides the current hot key.
	//! \return The hot key.
	QString hotKey() const;

	//! \brief Sets the current tray icon type.
	//! \param iconType The new icon type.
	void setTrayIconType(int iconType);

	//! \brief Provides the current tray icon type.
	//! \return The tray icon type.
	int trayIconType() const;

	//! \brief Defines wether or not the mouse cursor
	//!        is captured on screenshot or not.
	//! \param capture true if the mouse cursor should
	//!                be captured, false otherwise.
	void setCaptureMouseCursor(bool capture);

	//! \brief Checks wether the mouse cursor is captured
	//!        on screenshot or not.
	//! \return true if the mouse cursor should be captured,
	//!              false otherwise.
	bool isMouseCursorCaptured() const;

	//! \brief Defines the application should be launched
	//!        on system startup or not.
	//! \param launch true if the application should be launched
	//!        on system startup, false otherwise.
	void setLaunchOnSystemStartup(bool launch);

	//! \brief Checks wether the application should be launched
	//!        on system startup or not.
	//! \return true if the application should be launched
	//!         on system startup, false otherwise.
	bool isLaunchingOnSystemStartup() const;

	//! \brief Defines the system tray icon should be shown or not.
	//! \param show true if the tray icon should be shown, false otherwise.
	void setShowTrayIcon(bool show);

	//! \brief Checks wether the system tray icon should be shown or not.
	//! \return true if the tray icon should be shown, false otherwise.
	bool isTrayIconShown() const;

	//! \brief Defines the current application language.
	//! \param language The new language to be used.
	void setLanguage(const QString& language);

	//! \brief Provides the current application language.
	//! \return The current application language.
	QString language() const;

	//! \brief Resets all settings to their defauld values.
	void reset();

private:
	//! \brief Constructs a Preferences object.
	//! \param pParent Parent object.
	Preferences(QObject* pParent = 0);

	//! \brief Destroys a Preferences object.
	~Preferences();

	//! \brief Disable copying.
	Q_DISABLE_COPY(Preferences)

	//! \brief Frees the allocated resources.
	static void cleanUp();

private:
	// Instance
	static Preferences* m_pInstance;

	// Settings
	QSettings* m_pSettings;

	// Overlay color key and default value
	static const QString OVERLAY_COLOR_KEY;
	static const QColor OVERLAY_COLOR_DEFAULT_VALUE;

	// Rubber band color key and default value
	static const QString RUBBER_BAND_COLOR_KEY;
	static const QColor RUBBER_BAND_COLOR_DEFAULT_VALUE;

	// Rubber band width key and default value
	static const QString RUBBER_BAND_WIDTH_KEY;
	static const int RUBBER_BAND_WIDTH_DEFAULT_VALUE;

	// Pen width key and default value
	static const QString PEN_WIDTH_KEY;
	static const int PEN_WIDTH_DEFAULT_VALUE;

	// Font size key and default value
	static const QString FONT_SIZE_KEY;
	static const int FONT_SIZE_DEFAULT_VALUE;

	// Hotkey key and default value
	static const QString HOTKEY_KEY;
	static const QString HOTKEY_DEFAULT_VALUE;

	// Tray icon type key and default value
	static const QString TRAY_ICON_TYPE_KEY;
	static const int TRAY_ICON_TYPE_DEFAULT_VALUE;

	// Capture mouse cursor state key and default value
	static const QString CAPTURE_MOUSE_CURSLOR_KEY;
	static const bool CAPTURE_MOUSE_CURSLOR_DEFAULT_VALUE;

	// Launch on system startup state key and default value
	static const QString LAUNCH_ON_SYSTEM_STARTUP_KEY;
	static const bool LAUNCH_ON_SYSTEM_STARTUP_DEFAULT_VALUE;

	// Show tray icon state key and default value
	static const QString SHOW_TRAY_ICON_KEY;
	static const bool SHOW_TRAY_ICON_DEFAULT_VALUE;

	// Language key and default value
	static const QString LANGUAGE_KEY;
	static const QString LANGUAGE_DEFAULT_VALUE;
};

#endif // PREFERENCES_H
