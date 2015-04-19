#include "screenshotarea.h"
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

ScreenshotArea::ScreenshotArea(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::WindowStaysOnTopHint |
	               Qt::BypassWindowManagerHint |
	               Qt::FramelessWindowHint |
	               Qt::NoDropShadowWindowHint);

	m_pToolBar = new ToolBar(ToolBar::Horizontal, this);
	m_pToolBar->move(100, 100);
	m_pToolBar->show();
	m_darkOverlayColor = QColor(0, 0, 0, 155);
	m_rubberBandColor = Qt::cyan;
	m_rubberBandWidth = 2;
	m_penWidth = 2;
	m_rubberBandPointRadius = 3;
	m_arrowBaseWidth = 8;
	m_arrowHeight = 11.0 * qSqrt(3.0);

	m_leftButtonPressed = false;
	m_selectionStarted = false;

	connect(m_pToolBar, &ToolBar::discardButtonPressed,
	        this, &QWidget::close);
	connect(m_pToolBar, &ToolBar::settingsButtonPressed,
	        this, &ScreenshotArea::onSettingsButtonPressed);
	connect(m_pToolBar, &ToolBar::uploadButtonPressed,
	        this, &ScreenshotArea::onUploadButtonPressed);
	connect(m_pToolBar, &ToolBar::saveButtonPressed,
	        this, &ScreenshotArea::onSaveButtonPressed);

	setGeometry(QGuiApplication::primaryScreen()->geometry());

	m_pUploadDialog = new UploadDialog;

	setMouseTracking(true);
}

ScreenshotArea::~ScreenshotArea()
{

}

void ScreenshotArea::keyPressEvent(QKeyEvent *pEvent)
{
	bool pressedAltF4 = pEvent->key() == Qt::Key_F4 &&
	                    pEvent->modifiers() == Qt::AltModifier;

	bool pressedEsc = pEvent->key() == Qt::Key_Escape;

	// If user hits Esc or Alt+F4 the application closes
	if(pressedAltF4 || pressedEsc)
	{
		close();
	}

	QWidget::keyPressEvent(pEvent);
}

void ScreenshotArea::shoot()
{
	m_originalCapture = QPixmap();

	QScreen *pScreen = QGuiApplication::primaryScreen();
	if (pScreen)
	{
		m_originalCapture = pScreen->grabWindow(0);
		m_paintBoard = m_originalCapture;
	}

	show();
	activateWindow();
}

void ScreenshotArea::onSettingsButtonPressed()
{
	qDebug() << Q_FUNC_INFO;
}

void ScreenshotArea::onUploadButtonPressed()
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
	        this, &ScreenshotArea::replyFinished);
	connect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	        this, &ScreenshotArea::onError);
	connect(m_pNetworkReply, &QNetworkReply::sslErrors,
	        this, &ScreenshotArea::onSslErrors);

	m_pUploadDialog->show();
	hide();
}

void ScreenshotArea::onSaveButtonPressed()
{
	QPixmap newPixmap = m_paintBoard.copy(m_screenShotArea);

	QString defaultFilter = tr("*.png");
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

Qt::CursorShape ScreenshotArea::currentCursorShape(const QPoint &point)
{

}

void ScreenshotArea::replyFinished()
{
	qDebug() << "replyFinished";
	if(m_pNetworkReply->error())
	{
		qDebug() << "ERROR!";
		qDebug() << m_pNetworkReply->errorString();
	}
	else
	{
		// TODO: Make it nice, man!!!
		QJsonDocument jsonDocument = QJsonDocument::fromJson(m_pNetworkReply->readAll());
		QJsonObject jsonObject = jsonDocument.object();
		QJsonObject jsonData = jsonObject.take("data").toObject();

		m_pUploadDialog->setLink(jsonData.take("link").toString());
	}

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &ScreenshotArea::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &ScreenshotArea::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &ScreenshotArea::onSslErrors);

	m_pNetworkReply->deleteLater();

	close();
}

void ScreenshotArea::onError(QNetworkReply::NetworkError)
{
	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &ScreenshotArea::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	this, &ScreenshotArea::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &ScreenshotArea::onSslErrors);
}

void ScreenshotArea::onSslErrors(QList<QSslError>)
{
	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &ScreenshotArea::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &ScreenshotArea::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &ScreenshotArea::onSslErrors);
}

void ScreenshotArea::drawRubberBand(QPainter* painter)
{
	QRect rubberBandRect(m_selectionTopLeftPoint.x(),
	                     m_selectionTopLeftPoint.y(),
	                     m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
	                     m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

	m_screenShotArea = rubberBandRect;

	QPen pen(m_rubberBandColor);
	pen.setWidth(m_rubberBandWidth);

	painter->setPen(pen);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(rubberBandRect.normalized());

	painter->setBrush(m_rubberBandColor);

	QPoint topMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width() / 2, rubberBandRect.y());
	QPoint rightMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width(), rubberBandRect.y() + rubberBandRect.height() / 2);
	QPoint bottomMiddle(rubberBandRect.topLeft().x() + rubberBandRect.width() / 2, rubberBandRect.y() + rubberBandRect.height());
	QPoint leftMiddle(rubberBandRect.topLeft().x(), rubberBandRect.y() + rubberBandRect.height() / 2);

	painter->drawEllipse(rubberBandRect.topLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(rubberBandRect.topRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(rubberBandRect.bottomLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(rubberBandRect.bottomRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(topMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(rightMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(bottomMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	painter->drawEllipse(leftMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
}

void ScreenshotArea::drawCroppedArea(QPainter* painter)
{
	if(m_selectionStarted)
	{
		QRect pixmapRect(m_selectionTopLeftPoint.x(),
		                 m_selectionTopLeftPoint.y(),
		                 m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
		                 m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

		painter->drawPixmap(pixmapRect.normalized(), m_originalCapture, pixmapRect.normalized());
	}
}

void ScreenshotArea::drawDarkOverlay(QPainter* painter)
{
	QPixmap drawnCapture = m_originalCapture;

	painter->drawPixmap(0, 0, drawnCapture.width(), drawnCapture.height(), drawnCapture);

	QBrush darkOverlayBrush(m_darkOverlayColor);

	painter->setBrush(darkOverlayBrush);
	painter->drawRect(drawnCapture.rect());
}

void ScreenshotArea::drawArrow(QPainter* painter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	float halfWidth = m_arrowBaseWidth / 2;;

	// Backup pen and brush
	QPen backupPen = painter->pen();
	QBrush backupBrush = painter->brush();

	// Draw the line
	painter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

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

	painter->setPen(pen);
	painter->drawLine(m_selectionTopLeftPoint, currentShortenedLinePressPoint);

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
	painter->fillPath(path, QBrush(m_pToolBar->currentColor()));

	// Set the pen and brush back to normal
	painter->setPen(backupPen);
	painter->setBrush(backupBrush);
}

void ScreenshotArea::drawLine(QPainter* painter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	painter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	painter->setPen(pen);
	painter->drawLine(m_selectionTopLeftPoint, m_selectionBottomRightPoint);
}

void ScreenshotArea::drawSquare(QPainter* painter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	painter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	painter->setPen(pen);
	painter->drawRect(m_selectionTopLeftPoint.x(),
	                  m_selectionTopLeftPoint.y(),
	                  m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
	                  m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

}

void ScreenshotArea::drawBrush(QPainter* painter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	QPen backupPen = painter->pen();
	QBrush backupBrush = painter->brush();


	QBrush brush(m_pToolBar->currentColor());

	painter->setBrush(brush);
	painter->setPen(QPen(brush, m_penWidth));

	painter->drawEllipse(m_selectionBottomRightPoint.x(),
	                     m_selectionBottomRightPoint.y(),
	                     m_penWidth,
	                     m_penWidth);

	painter->setPen(backupPen);
	painter->setBrush(backupBrush);
}

void ScreenshotArea::drawEllipse(QPainter* painter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	painter->drawPixmap(m_helperBoard.rect(), m_helperBoard, m_helperBoard.rect());

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	painter->setPen(pen);

	QRect ellipseRectangle(m_selectionTopLeftPoint.x(),
	                       m_selectionTopLeftPoint.y(),
	                       m_selectionBottomRightPoint.x() - m_selectionTopLeftPoint.x(),
	                       m_selectionBottomRightPoint.y() - m_selectionTopLeftPoint.y());

	painter->drawEllipse(ellipseRectangle);
}

void ScreenshotArea::drawText(QPainter* painter)
{
	Q_UNUSED(painter)
	// TODO: Draw text here
}

void ScreenshotArea::mousePressEvent(QMouseEvent *e)
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

		if(!m_screenShotArea.contains(e->pos()) || m_pToolBar->currentTool() != ToolBar::NoTool)
		{
			m_selectionTopLeftPoint = e->pos();
			m_selectionBottomRightPoint = e->pos();
		}
		else
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
	}
}

void ScreenshotArea::mouseMoveEvent(QMouseEvent *e)
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
		update();
	}
}

void ScreenshotArea::mouseReleaseEvent(QMouseEvent *e)
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

void ScreenshotArea::paintEvent(QPaintEvent *pEvent)
{
	Q_UNUSED(pEvent)

	QPainter paintBoardPainter(&m_paintBoard);
	paintBoardPainter.setRenderHint(QPainter::Antialiasing);

	if (m_pToolBar->currentTool() == ToolBar::NoTool)
	{
		drawDarkOverlay(&paintBoardPainter);
		drawCroppedArea(&paintBoardPainter);
		drawRubberBand(&paintBoardPainter);
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
