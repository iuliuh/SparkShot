#ifndef TOOLBAR_H
#define TOOLBAR_H

// Qt includes
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

// Local includes
#include "colorpicker.h"
#include "quicksettingsdialog.h"

//! \brief This widget represents the application toolbar. This is where all
//!        the tools are located and from which the user can select the items
//!        he wants.
class ToolBar : public QWidget
{
	Q_OBJECT

public:
	//! \brief Type of the toolbar
	enum Type {
		Horizontal, //!< Horizontal toolbar
		Vertical    //!< Vertical toolbar
	};

	//! \brief Tool type
	enum Tool {
		Crop,       //!< Crop tool.
		Arrow,      //!< Arrow too.
		Line,       //!< Line tool.
		Square,     //!< Square tool.
		Ellipse,    //!< Ellipse tool.
		Brush,      //!< Brush tool.
		Text        //!< Text tool.
	};

	//! \brief Constructs a GNULinuxHotKeyBinder object.
	//! \param type The toolbar type.
	//! \param pParent Parent object.
	//! \see ToolBar::Tool
	explicit ToolBar(Type type = Vertical, QWidget *pParent = 0);

	//! \brief Destroys a GNULinuxHotKeyBinder object.
	~ToolBar();

	//! \brief Provides the current used tool.
	//! \return Current tool.
	Tool currentTool() const;

	//! \brief Provides the current drawing color.
	//! \return The color items are drawn.
	QColor currentColor() const;

public Q_SLOTS:
	//! \brief Slot called when the toolbar is hidden.
	void hide();

	//! \brief Slot called when the toolbar is shown.
	void show();

Q_SIGNALS:
	//! \brief Signal emitted when the discard button is pressed.
	void discardButtonPressed();

	//! \brief Signal emitted when the settings button is pressed.
	void settingsButtonPressed();

	//! \brief Signal emitted when the upload button is pressed.
	void uploadButtonPressed();

	//! \brief Signal emitted when the save button is pressed.
	void saveButtonPressed();

	//! \brief Signal emitted when the overlay color changes.
	//! \param color The new overlay color.
	void overlayColorChanged(QColor color);

	//! \brief Signal emitted when the rubber band color changes.
	//! \param color The new rubber band color.
	void rubberBandColorChanged(QColor color);

	//! \brief Signal emitted when the rubber band width changes.
	//! \param width The new rubber band width.
	void rubberBandWidthChanged(int width);

	//! \brief Signal emitted when the pen width changes.
	//! \param width The new pen width.
	void penWidthChanged(int width);

	//! \brief Signal emitted when the rubber band dots radius changes.
	//! \param radius The new rubber band duts radius.
	void dotsRadiusChanged(int radius);

	//! \brief Signal emitted when the font size changes.
	//! \param size The new font size.
	void fontSizeChanged(int size);

protected:
	//! \see QWidget::mousePressEvent
	void mousePressEvent(QMouseEvent *pEvent);

	//! \see QWidget::mouseReleaseEvent
	void mouseReleaseEvent(QMouseEvent *pEvent);

	//! \see QWidget::mouseMoveEvent
	void mouseMoveEvent(QMouseEvent *pEvent);

	//! \see QWidget::paintEvent
	void paintEvent(QPaintEvent *pEvent);

private Q_SLOTS:
	//! \brief Slot called when the color button was clicked.
	void onColorButtonClicked();

	//! \brief Slot called when the text button was clicked.
	void onTextToolButtonPressed();

	//! \brief Slot called when the brush button was clicked.
	void onBrushToolButtonPressed();

	//! \brief Slot called when the tool button was clicked.
	void onSquareToolButtonPressed();

	//! \brief Slot called when the ellipse button was clicked.
	void onEllipseToolButtonPressed();

	//! \brief Slot called when the line button was clicked.
	void onLineToolButtonPressed();

	//! \brief Slot called when the arrow button was clicked.
	void onArrowToolButtonPressed();

	//! \brief Slot called when the crop button was clicked.
	void onCropToolButtonPressed();

	//! \brief Slot called when the settings button was clicked.
	void onSettingsButtonPressed();

	//! \brief Slot called when the drawing color changed.
	void onDrawingColorChanged(QColor newColor);

private:
	//! \brief Positions the color poicker to the right position.
	void autoPositionColorPicker();

	//! \brief Positions the settings dialog to the right position.
	void autoPositionSettingsDialog();

	//! \brief Sets the toolbar styling.
	void setTheme();

private:
	// Type of the toolbar
	Type m_type;

	Tool m_currentTool;
	ColorPicker m_colorPickerDialog;
	QuickSettingsDialog m_settingsDialog;

	// Layout
	QLayout *m_pLayout;

	// Button group
	QButtonGroup *m_pButtonGroup;

	// Buttons
	QPushButton *m_pSettingsButton;
	QPushButton *m_pColorButton;
	QPushButton *m_pCropButton;
	QPushButton *m_pArrowButton;
	QPushButton *m_pLineButton;
	QPushButton *m_pSquareButton;
	QPushButton *m_pEllipseButton;
	QPushButton *m_pSaveButton;
	QPushButton *m_pBrushButton;
	QPushButton *m_pTextButton;
	QPushButton *m_pUploadButton;
	QPushButton *m_pCloseButton;

	// Separators
	QLabel *m_pSeparator1;
	QLabel *m_pSeparator2;

	// Handle
	QLabel *m_pHandler;

	// true if left mouse button is pressed, false otherwise
	bool m_leftButtonPressed;

	// The position of the mouse on the toolbar
	QPoint m_mousePosOnBar;

	// Drawing color
	QColor m_drawColor;

	// Screen rectangle
	QRect m_screenRect;
};

#endif // TOOLBAR_H
