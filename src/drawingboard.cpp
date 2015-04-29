#include "drawingboard.h"

#include <QDesktopWidget>
#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QtMath>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QByteArray>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>

#include "defines.h"

DrawingBoard::DrawingBoard(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::WindowStaysOnTopHint |
	               Qt::BypassWindowManagerHint |
	               Qt::FramelessWindowHint |
	               Qt::NoDropShadowWindowHint);

	setAttribute(Qt::WA_DeleteOnClose);

	m_pToolBar = new ToolBar(ToolBar::Horizontal, this);
	m_pToolBar->hide();

	m_darkOverlayColor = QColor(0, 0, 0, 155);
	m_rubberBandColor = Qt::cyan;
	m_rubberBandWidth = 2;
	m_penWidth = 2;
	m_rubberBandPointRadius = 3;
	m_arrowBaseWidth = 8;
	m_arrowHeight = 11.0 * qSqrt(3.0);

	m_leftButtonPressed = false;
	m_selectionStarted = false;
	m_textPositioned = false;
	m_startedDrawing = false;
	m_moveSelectionArea = false;

	connect(m_pToolBar, &ToolBar::discardButtonPressed,
	        this, &DrawingBoard::close);
	connect(m_pToolBar, &ToolBar::uploadButtonPressed,
	        this, &DrawingBoard::onUploadButtonPressed);
	connect(m_pToolBar, &ToolBar::saveButtonPressed,
	        this, &DrawingBoard::onSaveButtonPressed);

	connect(m_pToolBar, &ToolBar::overlayColorChanged,
	        this, &DrawingBoard::onOverlayColorChanged);
	connect(m_pToolBar, &ToolBar::rubberBandColorChanged,
	        this, &DrawingBoard::onRubberBandColorChanged);
	connect(m_pToolBar, &ToolBar::rubberBandWidthChanged,
	        this, &DrawingBoard::onRubberBandWidthChanged);
	connect(m_pToolBar, &ToolBar::penWidthChanged,
	        this, &DrawingBoard::onPenWidthChanged);
	connect(m_pToolBar, &ToolBar::dotsRadiusChanged,
	        this, &DrawingBoard::onDotsRadiusChanged);
	connect(m_pToolBar, &ToolBar::fontSizeChanged,
	        this, &DrawingBoard::onFontSizeChanged);

	setGeometry(QGuiApplication::primaryScreen()->geometry());

	m_pUploadDialog = new UploadDialog;

	setMouseTracking(true);
}

DrawingBoard::~DrawingBoard()
{
	delete m_pUploadDialog;
}

void DrawingBoard::keyPressEvent(QKeyEvent *pEvent)
{
	bool pressedAltF4 = pEvent->key() == Qt::Key_F4 &&
	                    pEvent->modifiers() == Qt::AltModifier;

	bool pressedEsc = pEvent->key() == Qt::Key_Escape;

	// If user hits Esc or Alt+F4 the application closes
	if(pressedAltF4 || pressedEsc)
	{
		close();
	}

	if(m_textPositioned)
	{
		if(pEvent->key() == Qt::Key_Backspace)
		{
			m_currentText.remove(m_currentText.length() - 1, 1);
		}
		else
		{
			m_currentText.append(pEvent->text());
		}
		if(pEvent->key() == Qt::Key_Enter)
		{
			m_currentText.append("\n");
		}
	}

	update();

	QWidget::keyPressEvent(pEvent);
}

void DrawingBoard::shoot()
{
	m_originalCapture = QPixmap();

	QScreen *pScreen = QGuiApplication::primaryScreen();
	if (pScreen)
	{
		m_originalCapture = pScreen->grabWindow(0);
		m_paintBoard = m_originalCapture;
		m_helperBoard = m_originalCapture;
	}

	show();
	activateWindow();
}

void DrawingBoard::onOverlayColorChanged(QColor color)
{
	m_darkOverlayColor = color;
	update();
}

void DrawingBoard::onRubberBandColorChanged(QColor color)
{
	m_rubberBandColor = color;

	update();
}

void DrawingBoard::onRubberBandWidthChanged(int width)
{
	m_rubberBandWidth = width;
	update();
}

void DrawingBoard::onPenWidthChanged(int width)
{
	m_penWidth = width;
	update();
}

void DrawingBoard::onDotsRadiusChanged(int radius)
{
	m_rubberBandPointRadius = radius;
	update();
}

void DrawingBoard::onFontSizeChanged(int size)
{
	m_fontSize = size;
	update();
}

void DrawingBoard::onUploadButtonPressed()
{
	QNetworkRequest networkRequest;

	QUrl requestUrl("https://api.imgur.com/3/image.json");
	if(requestUrl.isValid())
	{
		networkRequest.setUrl(requestUrl);
	}

	networkRequest.setRawHeader("Authorization", "Client-ID " CLIENT_ID);

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QHttpPart imagePart;
	imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image"));
	imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\""));

	QPixmap newPixmap = m_paintBoard.copy(m_screenShotArea);

	// Preparation of our QPixmap
	QByteArray pixmapByteArray;
	QBuffer buffer(&pixmapByteArray);
	buffer.open(QIODevice::WriteOnly);
	newPixmap.save(&buffer, "PNG");

	imagePart.setBody(pixmapByteArray);

	multiPart->append(imagePart);
	m_pNetworkReply = m_networkAccessManager.post(networkRequest, multiPart);

	connect(m_pNetworkReply, &QNetworkReply::finished,
	        this, &DrawingBoard::replyFinished);
	connect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	        this, &DrawingBoard::onError);
	connect(m_pNetworkReply, &QNetworkReply::sslErrors,
	        this, &DrawingBoard::onSslErrors);

	m_pUploadDialog->setProgressBarView();
	m_pUploadDialog->show();

	m_pToolBar->hide();
	hide();
}

void DrawingBoard::onSaveButtonPressed()
{
	QPixmap newPixmap = m_paintBoard.copy(m_screenShotArea);

	QString defaultFilter("*.png");

	QString fileName = QFileDialog::getSaveFileName(this,
	                                                tr("Save File"),
	                                                QDir::homePath(),
	                                                tr("Images (*.png *.bmp *.jpg *jpeg)"),
	                                                &defaultFilter);

	if(!fileName.isEmpty())
	{
		newPixmap.save(fileName);

		close();
	}
}

void DrawingBoard::replyFinished()
{
	if(m_pNetworkReply->error())
	{
		qDebug() << m_pNetworkReply->errorString();
	}
	else
	{
		//! \todo: Unsafe, make it safe and more elegant!!!
		QJsonDocument jsonDocument = QJsonDocument::fromJson(m_pNetworkReply->readAll());
		QJsonObject jsonObject = jsonDocument.object();
		QJsonObject jsonData = jsonObject.take("data").toObject();

		m_pUploadDialog->setLink(jsonData.take("link").toString());
		m_pUploadDialog->setLinkView();
	}

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &DrawingBoard::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &DrawingBoard::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &DrawingBoard::onSslErrors);

	m_pNetworkReply->deleteLater();

	close();
}

void DrawingBoard::onError(QNetworkReply::NetworkError error)
{
	Q_UNUSED(error)

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &DrawingBoard::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &DrawingBoard::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &DrawingBoard::onSslErrors);
}

void DrawingBoard::onSslErrors(QList<QSslError> errorList)
{
	Q_UNUSED(errorList)

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &DrawingBoard::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &DrawingBoard::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &DrawingBoard::onSslErrors);
}

void DrawingBoard::drawRubberBand(QPainter* pPainter)
{
	QRect rubberBandRect(m_selectionTopLeftPoint.x(),
	                     m_selectionTopLeftPoint.y(),
	                     m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
	                     m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

	m_screenShotArea = rubberBandRect;

	QPen pen(m_rubberBandColor);
	pen.setWidth(m_rubberBandWidth);

	pPainter->setPen(pen);
	pPainter->setBrush(Qt::NoBrush);
	pPainter->drawRect(rubberBandRect.normalized());

	pPainter->setBrush(m_rubberBandColor);

	QPoint topMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width() / 2, rubberBandRect.y());
	QPoint rightMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width(), rubberBandRect.y() + rubberBandRect.height() / 2);
	QPoint bottomMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width() / 2, rubberBandRect.y() + rubberBandRect.height());
	QPoint leftMiddle(rubberBandRect.topLeft().x(), rubberBandRect.y() + rubberBandRect.height() / 2);

	pPainter->drawEllipse(rubberBandRect.topLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(rubberBandRect.topRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(rubberBandRect.bottomLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(rubberBandRect.bottomRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(topMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(rightMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(bottomMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(leftMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
}

void DrawingBoard::drawCroppedArea(QPainter* pPainter)
{
	if(m_selectionStarted)
	{
		QRect pixmapRect(m_selectionTopLeftPoint.x(),
		                 m_selectionTopLeftPoint.y(),
		                 m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
		                 m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

		pPainter->drawPixmap(pixmapRect.normalized(), m_originalCapture, pixmapRect.normalized());
	}
}

void DrawingBoard::drawDarkOverlay(QPainter* pPainter)
{
	QPixmap drawnCapture = m_originalCapture;

	pPainter->drawPixmap(0, 0, drawnCapture.width(), drawnCapture.height(), drawnCapture);

	QBrush darkOverlayBrush(m_darkOverlayColor);

	pPainter->setBrush(darkOverlayBrush);
	pPainter->drawRect(drawnCapture.rect());
}

void DrawingBoard::drawArrow(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	float halfWidth = m_arrowBaseWidth / 2;

	// Backup pen and brush
	QPen backupPen = pPainter->pen();
	QBrush backupBrush = pPainter->brush();

	// Draw the line
	pPainter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	QPoint currentShortenedLinePressPoint = m_selectionBottomRightPoint;

	if (m_selectionTopLeftPoint == currentShortenedLinePressPoint)
	{
		// Not a line
		return;
	}

	double dx = currentShortenedLinePressPoint.x() - m_selectionTopLeftPoint.x();
	double dy = currentShortenedLinePressPoint.y() - m_selectionTopLeftPoint.y();
	double shorteningFactor = -m_arrowHeight;
	if (dx == 0)
	{
		// Vertical line
		if (currentShortenedLinePressPoint.y() < m_selectionTopLeftPoint.y())
		{
			currentShortenedLinePressPoint.setY(currentShortenedLinePressPoint.y() - shorteningFactor);
		}
		else
		{
			currentShortenedLinePressPoint.setY(currentShortenedLinePressPoint.y() + shorteningFactor);
		}
	}
	else if (dy == 0)
	{
		// Horizontal line
		if (currentShortenedLinePressPoint.x() < m_selectionTopLeftPoint.x())
		{
			currentShortenedLinePressPoint.setX(currentShortenedLinePressPoint.x() - shorteningFactor);
		}
		else
		{
			currentShortenedLinePressPoint.setX(currentShortenedLinePressPoint.x() + shorteningFactor);
		}
	}
	else
	{
		// Non-horizontal and non-vertical line
		double length = qSqrt(dx * dx + dy * dy);
		double scale = (length + shorteningFactor) / length;
		dx *= scale;
		dy *= scale;
		currentShortenedLinePressPoint.setX(m_selectionTopLeftPoint.x() + dx);
		currentShortenedLinePressPoint.setY(m_selectionTopLeftPoint.y() + dy);
	}

	pPainter->setPen(pen);
	pPainter->drawLine(m_selectionTopLeftPoint, currentShortenedLinePressPoint);

	// Draw the arrow
	QVector2D initPosVec(m_selectionTopLeftPoint);
	QVector2D currPosVec(m_selectionBottomRightPoint);

	QVector2D auxVec(currPosVec - initPosVec);
	double length = qSqrt(qPow(auxVec.x(), 2.0) + qPow(auxVec.y(), 2.0));

	QVector2D currPostToInitPosVec = (currPosVec - initPosVec) / length;
	QVector2D perpendVec(-currPostToInitPosVec.y(), currPostToInitPosVec.x());
	QVector2D arrowEdgeVec1 = currPosVec - m_arrowHeight*currPostToInitPosVec + halfWidth*perpendVec;
	QVector2D arrowEdgeVec2 = currPosVec - m_arrowHeight*currPostToInitPosVec - halfWidth*perpendVec;

	QPoint p1(arrowEdgeVec1.x(), arrowEdgeVec1.y());
	QPoint p2(arrowEdgeVec2.x(), arrowEdgeVec2.y());

	QPolygon p;
	p.append(m_selectionBottomRightPoint);
	p.append(p1);
	p.append(p2);

	QPainterPath path;
	path.addPolygon(p);
	pPainter->fillPath(path, QBrush(m_pToolBar->currentColor()));

	// Set the pen and brush back to normal
	pPainter->setPen(backupPen);
	pPainter->setBrush(backupBrush);
}

void DrawingBoard::drawLine(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	pPainter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	pPainter->setPen(pen);
	pPainter->drawLine(m_selectionTopLeftPoint, m_selectionBottomRightPoint);
}

void DrawingBoard::drawSquare(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	pPainter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	pPainter->setPen(pen);
	pPainter->drawRect(m_selectionTopLeftPoint.x(),
	                  m_selectionTopLeftPoint.y(),
	                  m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
	                  m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

}

void DrawingBoard::drawBrush(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	QPen backupPen = pPainter->pen();
	QBrush backupBrush = pPainter->brush();

	QBrush brush(m_pToolBar->currentColor());

	pPainter->setBrush(brush);
	pPainter->setPen(QPen(brush, m_penWidth));

	pPainter->drawLine(m_brushInitialPoint, m_brushFinalPoint);

	pPainter->setPen(backupPen);
	pPainter->setBrush(backupBrush);
}

void DrawingBoard::drawEllipse(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	pPainter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	pPainter->setPen(pen);

	QRect ellipseRectangle(m_selectionTopLeftPoint.x(),
	                       m_selectionTopLeftPoint.y(),
	                       m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
	                       m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

	pPainter->drawEllipse(ellipseRectangle);
}

void DrawingBoard::drawText(QPainter* pPainter)
{
	if(!m_textPositioned)
	{
		return;
	}

	pPainter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen backupPen = pPainter->pen();
	QBrush backupBrush = pPainter->brush();

	QPen p;
	p.setStyle(Qt::DotLine);
	p.setColor(Qt::black);
	p.setWidth(1);

	pPainter->setPen(p);

	QFont font = pPainter->font();
	QFontMetrics fontMetrics(font);

	QRect textRect(m_textPoint.x(),
	               m_textPoint.y(),
	               fontMetrics.width(m_currentText),
	               fontMetrics.height());

	pPainter->drawText(textRect,
	                  Qt::AlignLeft,
	                  m_currentText);

	textRect.adjust(-2, -2, 2, 2);

	pPainter->drawRect(textRect);

	pPainter->setPen(backupPen);
	pPainter->setBrush(backupBrush);
}

void DrawingBoard::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;
		m_selectionStarted = true;

		m_helperBoard = m_paintBoard;

		if(m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			m_pToolBar->hide();
		}

		if(m_pToolBar->currentTool() == ToolBar::Text)
		{
			m_textPoint = e->pos();
			m_textPositioned = true;
			m_currentText.clear();
		}

		if(!m_screenShotArea.contains(e->pos()) || m_pToolBar->currentTool() != ToolBar::NoTool)
		{
			m_selectionTopLeftPoint = e->pos();
			m_selectionBottomRightPoint = e->pos();
		}
		else if(m_startedDrawing)
		{
			m_moveSelectionArea = true;
			m_topLeftPointBeforeSelectionMove = m_selectionTopLeftPoint;
			m_bottomRightPointBeforeSelectionMove = m_selectionBottomRightPoint;
			m_pressPointBeforeSelectionMove = e->pos();
		}

		if(m_screenShotArea.contains(e->pos()))
		{
			setCursor(Qt::ClosedHandCursor);
		}

		m_startedDrawing = true;
	}
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *e)
{
	if (m_screenShotArea.contains(e->pos(), true) && m_pToolBar->currentTool() == ToolBar::NoTool)
	{
		setCursor(Qt::OpenHandCursor);
	}
	else
	{
		setCursor(Qt::ArrowCursor);
	}

	if (m_leftButtonPressed)
	{
		if(!m_moveSelectionArea)
		{
			m_selectionBottomRightPoint = e->pos();
		}
		else
		{
			setCursor(Qt::ClosedHandCursor);

			QRect translationRectangle(m_topLeftPointBeforeSelectionMove, m_bottomRightPointBeforeSelectionMove);
			translationRectangle.translate(e->pos() - m_pressPointBeforeSelectionMove);
			m_selectionTopLeftPoint = translationRectangle.topLeft();
			m_selectionBottomRightPoint = translationRectangle.bottomRight();
		}

		if (m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			const int threshold = 20;

			QPoint p(qMin(m_screenShotArea.normalized().bottomRight().x() + threshold,
			              m_originalCapture.rect().bottomRight().x() - m_pToolBar->width()),
			         qMin(m_screenShotArea.normalized().bottomRight().y() + threshold,
			              m_originalCapture.rect().bottomRight().y() - m_pToolBar->height()));

			m_pToolBar->move(p);
		}
	}

	static bool pointGuard = true;
	m_brushInitialPoint = pointGuard ? e->pos() : m_brushInitialPoint;
	m_brushFinalPoint = pointGuard ? m_brushFinalPoint : e->pos();
	pointGuard = !pointGuard;

	update();
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
		m_moveSelectionArea = false;

		if (m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			m_pToolBar->show();
		}

		if(m_screenShotArea.contains(e->pos()) && m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			setCursor(Qt::OpenHandCursor);
		}
	}
}

void DrawingBoard::paintEvent(QPaintEvent *pEvent)
{
	Q_UNUSED(pEvent)

	QPainter paintBoardPainter(&m_paintBoard);
	paintBoardPainter.setRenderHint(QPainter::Antialiasing);

	if (m_pToolBar->currentTool() == ToolBar::NoTool)
	{
		drawDarkOverlay(&paintBoardPainter);
		drawCroppedArea(&paintBoardPainter);

		// Draw rubber band only if selection has started
		if(m_selectionStarted)
		{
			drawRubberBand(&paintBoardPainter);
		}
	}

	switch (m_pToolBar->currentTool())
	{
	case ToolBar::NoTool:
		break;
	case ToolBar::Arrow:
		drawArrow(&paintBoardPainter);
		break;
	case ToolBar::Line:
		drawLine(&paintBoardPainter);
		break;
	case ToolBar::Square:
		drawSquare(&paintBoardPainter);
		break;
	case ToolBar::Brush:
		drawBrush(&paintBoardPainter);
		break;
	case ToolBar::Ellipse:
		drawEllipse(&paintBoardPainter);
		break;
	case ToolBar::Text:
		drawText(&paintBoardPainter);
		break;
	default:
		break;
	}

	QPainter formPainter(this);
	formPainter.drawPixmap(m_paintBoard.rect(), m_paintBoard, m_paintBoard.rect());

	update();
}
