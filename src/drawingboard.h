#ifndef SCREENSHOTAREA_H
#define SCREENSHOTAREA_H

#include "toolbar.h"
#include "uploaddialog.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QPoint>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DrawingBoard : public QWidget
{
	Q_OBJECT

public:
	explicit DrawingBoard(QWidget *parent = 0);
	~DrawingBoard();

protected:
	void keyPressEvent(QKeyEvent *pEvent);
	void mouseMoveEvent(QMouseEvent *pEvent);
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);
	void paintEvent(QPaintEvent *pEvent);

public Q_SLOTS:
	void shoot();
	void onOverlayColorChanged(QColor color);
	void onRubberBandColorChanged(QColor color);
	void onRubberBandWidthChanged(int width);
	void onPenWidthChanged(int width);
	void onDotsRadiusChanged(int radius);
	void onFontSizeChanged(int size);

private Q_SLOTS:
	void onToolbarToolChanged(ToolBar::Tool tool);
	void onUploadButtonPressed();
	void onSaveButtonPressed();

	void replyFinished();
	void onError(QNetworkReply::NetworkError error);
	void onSslErrors(QList<QSslError> errorList);

private:
	void drawRubberBand(QPainter* pPainter);
	void drawCroppedArea(QPainter* pPainter);
	void drawDarkOverlay(QPainter* pPainter);
	void drawArrow(QPainter* pPainter);
	void drawLine(QPainter* pPainter);
	void drawSquare(QPainter* pPainter);
	void drawBrush(QPainter* pPainter);
	void drawEllipse(QPainter *pPainter);
	void drawText(QPainter* pPainter);
	void commitSketches();

private:
	ToolBar *m_pToolBar;

	QPixmap m_originalCapture;
	QPixmap m_screenshotBoard;
	QPixmap m_currentSketchesBoard;
	QPixmap m_temporarySketchesBoard;

	QString m_currentText;
	QPoint m_textPoint;

	QPoint m_initialMousePosition;
	QPoint m_finalMousePosition;
	QPoint m_brushInitialPosition;
	QPoint m_brushFinalPosition;
	QPoint m_initialCropRectMovePoint;
	QPoint m_currentMousePosition;
	QRect m_cropRect;
	QRect m_cropRectBeforeMove;

	bool m_leftButtonPressed;
	bool m_moveCroppedArea;

	QString m_clientId;
	QNetworkAccessManager m_networkAccessManager;
	QNetworkReply *m_pNetworkReply;
	UploadDialog *m_pUploadDialog;

private:
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
