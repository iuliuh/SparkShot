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

public slots:
	void shoot();
	void onOverlayColorChanged(QColor color);
	void onRubberBandColorChanged(QColor color);
	void onRubberBandWidthChanged(int width);
	void onPenWidthChanged(int width);
	void onDotsRadiusChanged(int radius);
	void onFontSizeChanged(int size);

private slots:
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

private:
	ToolBar *m_pToolBar;

	QPixmap m_originalCapture;
	QPixmap m_paintBoard;
	QPixmap m_helperBoard;

	QPoint m_selectionTopLeftPoint;
	QPoint m_selectionBottomRightPoint;

	QPoint m_topLeftPointBeforeSelectionMove;
	QPoint m_bottomRightPointBeforeSelectionMove;
	QPoint m_pressPointBeforeSelectionMove;
	QRect m_screenShotArea;
	QString m_currentText;
	QPoint m_textPoint;

	QString m_clientId;
	QNetworkAccessManager m_networkAccessManager;
	QNetworkReply *m_pNetworkReply;
	UploadDialog *m_pUploadDialog;

	bool m_leftButtonPressed;
	bool m_selectionStarted;
	bool m_moveSelectionArea;
	bool m_textPositioned;
	bool m_startedDrawing;

private:
	QColor m_darkOverlayColor;
	QColor m_rubberBandColor;

	QPoint m_brushInitialPoint;
	QPoint m_brushFinalPoint;

	int m_rubberBandWidth;
	int m_penWidth;
	int m_rubberBandPointRadius;
	int m_fontSize;

	float m_arrowHeight;
	float m_arrowBaseWidth;
};

#endif // SCREENSHOTAREA_H
