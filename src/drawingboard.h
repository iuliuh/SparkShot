#ifndef SCREENSHOTAREA_H
#define SCREENSHOTAREA_H

// Qt includes
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QPoint>
#include <QNetworkAccessManager>
#include <QNetworkReply>

// Local includes
#include "toolbar.h"
#include "uploaddialog.h"

//! \brief Class represented the widget on which the screenshot will be put and
//!        processed. This is where all the drawings take place.
class DrawingBoard : public QWidget
{
	Q_OBJECT

public:
	//! \brief Constructs a DrawingBoard object.
	//! \param pParent Parent object.
	explicit DrawingBoard(QWidget *pParent = 0);

	//! \brief Destroys a DrawingBoard object.
	~DrawingBoard();

protected:
	//! \see QWidget::keyPressEvent
	void keyPressEvent(QKeyEvent *pEvent);

	//! \see QWidget::mouseMoveEvent
	void mouseMoveEvent(QMouseEvent *pEvent);

	//! \see QWidget::mousePressEvent
	void mousePressEvent(QMouseEvent *pEvent);

	//! \see QWidget::mouseReleaseEvent
	void mouseReleaseEvent(QMouseEvent *pEvent);

	//! \see QWidget::paintEvent
	void paintEvent(QPaintEvent *pEvent);

public Q_SLOTS:
	//! \brief Creates a screenshot, draws it on this widget and shows it.
	void shoot();

	//! \brief Slot called when the overlay color settings have changed.
	//! \param color New overlay color.
	void onOverlayColorChanged(QColor color);

	//! \brief Slot called when the rubber band color settings have changed.
	//! \param color New rubber band color.
	void onRubberBandColorChanged(QColor color);

	//! \brief Slot called when the rubber band width settings have changed.
	//! \param width New rubber band width value.
	void onRubberBandWidthChanged(int width);

	//! \brief Slot called when the pen width settings have changed.
	//! \param width New pen width value.
	void onPenWidthChanged(int width);

	//! \brief Slot called when the dots radius settings have changed.
	//! \param radius New dots radius value.
	void onDotsRadiusChanged(int radius);

	//! \brief Slot called when the font size settings have changed.
	//! \param size New font size value.
	void onFontSizeChanged(int size);

private Q_SLOTS:
	//! \brief Slot called when the current toolbar tool changes.
	//! \param tool The new selected tool.
	void onToolbarToolChanged(ToolBar::Tool tool);

	//! \brief Slot called when the upload toolbar button is pressed.
	void onUploadButtonPressed();

	//! \brief Slot called when the save toolbar button is pressed.
	void onSaveButtonPressed();

	//! \brief Slot called when HTTP server replys.
	void replyFinished();

	//! \brief Slot called on HTTP server request error.
	//! \param error Network error.
	void onError(QNetworkReply::NetworkError error);

	//! \brief Slot called on SSL errors.
	//! \param errorList List of errors.
	void onSslErrors(QList<QSslError> errorList);

private:
	//! \brief Draws the rubber band.
	//! \param pPainter Painter to draw with.
	void drawRubberBand(QPainter* pPainter);

	//! \brief Draws the cropped area i.e. the area to be saved or uploaded.
	//! \param pPainter Painter to draw with.
	void drawCroppedArea(QPainter* pPainter);

	//! \brief Draws the dark overlay.
	//! \param pPainter Painter to draw with.
	void drawDarkOverlay(QPainter* pPainter);

	//! \brief Draws an arrow.
	//! \param pPainter Painter to draw with.
	void drawArrow(QPainter* pPainter);

	//! \brief Draws a line.
	//! \param pPainter Painter to draw with.
	void drawLine(QPainter* pPainter);

	//! \brief Draws a sqare.
	//! \param pPainter Painter to draw with.
	void drawSquare(QPainter* pPainter);

	//! \brief Draws a brush trace.
	//! \param pPainter Painter to draw with.
	void drawBrush(QPainter* pPainter);

	//! \brief Draws an ellipse.
	//! \param pPainter Painter to draw with.
	void drawEllipse(QPainter *pPainter);

	//! \brief Draws text.
	//! \param pPainter Painter to draw with.
	void drawText(QPainter* pPainter);

	//! \brief Draws hint tutorial.
	//! \param pPainter Painter to draw with.
	void drawHint(QPainter* pPainter);

	//! \brief After the temporary drawing finishes i.e. when the user
	//!        releases the left click, everything gets painted to the
	//!        final drawing board for saving or uploading.
	void commitSketches();

private:
	// The tool bar
	ToolBar *m_pToolBar;

	// Original screenshot pixmap
	QPixmap m_originalCapture;

	// Pixmap on top of which the cropped area, rubber band and dark
	// overlay gets painted
	QPixmap m_screenshotBoard;

	// Pixmap on top of which the sketches get painted
	QPixmap m_currentSketchesBoard;

	// Pixmap on top of which temporary sketches get painted e.g. from
	// the moment in which the left click is pressed to the moment in
	// which the left click is released and a sqare gets painted
	QPixmap m_temporarySketchesBoard;

	// Text tool current text
	QString m_currentText;

	// The point in which the text gets painted
	QPoint m_textPoint;

	// Initial mouse position i.e. left mouse press position
	QPoint m_initialMousePosition;

	// Final mouse position i.e. current mouse position while
	// left mouse button is still pressed
	QPoint m_finalMousePosition;

	// Brush initial position i.e. mouse press position with
	// the brush tool selected
	QPoint m_brushInitialPosition;

	// Brush final position i.e. current mouse position while left
	// mouse button is still pressed and the brush tool selected
	QPoint m_brushFinalPosition;

	// The position of the cropped area before it gets moved
	QPoint m_initialCropRectMovePoint;

	// The current mouse position
	QPoint m_currentMousePosition;

	// The cropped rect i.e. the rubber band rect
	QRect m_cropRect;

	// The crop rect before it gets moved
	QRect m_cropRectBeforeMove;

	// Left button state - true if the left button is pressed,
	// false otherwise
	bool m_leftButtonPressed;

	// Cropped area i.e. rubber band area state - true if the
	// cropped area is in a moving state i.e. the user started
	// moving it, false otherwise
	bool m_moveCroppedArea;

	// Imgur client id
	QString m_clientId;

	// Imgur network access manager
	QNetworkAccessManager m_networkAccessManager;

	// Imgur network reply
	QNetworkReply *m_pNetworkReply;

	// Upload image to Imgur dialog
	UploadDialog *m_pUploadDialog;

	// true if drawing has started, false otherwise
	bool m_drawingStarted;

private:
	// Drawing settings
	QColor m_darkOverlayColor;
	QColor m_rubberBandColor;
	int m_rubberBandWidth;
	int m_penWidth;
	int m_rubberBandPointRadius;
	int m_fontSize;
	float m_arrowHeight;
	float m_arrowBaseWidth;
};

#endif // SCREENSHOTAREA_H
