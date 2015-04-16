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
	m_arrowBaseWidth = 7;
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
	//Uncomment when doing the resize mouse area
	//setMouseTracking(true);
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
//	connect(m_pNetworkReply, &QNetworkReply::error,
//	        this, &ScreenshotArea::onError);
	connect(m_pNetworkReply, &QNetworkReply::sslErrors,
	        this, &ScreenshotArea::onSslErrors);
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
		m_pUploadDialog->show();
		hide();
	}

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &ScreenshotArea::replyFinished);
//	disconnect(m_pNetworkReply, &QNetworkReply::error,
//	           this, &ScreenshotArea::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &ScreenshotArea::onSslErrors);

	m_pNetworkReply->deleteLater();

	close();
}

void ScreenshotArea::onError(QNetworkReply::NetworkError)
{
	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &ScreenshotArea::replyFinished);
//	disconnect(m_pNetworkReply, &QNetworkReply::error,
//	           this, &ScreenshotArea::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &ScreenshotArea::onSslErrors);
}

void ScreenshotArea::onSslErrors(QList<QSslError>)
{
	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &ScreenshotArea::replyFinished);
//	disconnect(m_pNetworkReply, &QNetworkReply::error,
//	           this, &ScreenshotArea::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &ScreenshotArea::onSslErrors);
}

void ScreenshotArea::drawRubberBand(QPainter* painter)
{
	QRect rubberBandRect(m_initialPressPoint.x(),
	                     m_initialPressPoint.y(),
	                     m_currentPressPoint.x() - m_initialPressPoint.x(),
	                     m_currentPressPoint.y() - m_initialPressPoint.y());

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
		QRect pixmapRect(m_initialPressPoint.x(),
		                 m_initialPressPoint.y(),
		                 m_currentPressPoint.x() - m_initialPressPoint.x(),
		                 m_currentPressPoint.y() - m_initialPressPoint.y());

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

	QPoint currentShortenedLinePressPoint = m_currentPressPoint;

	if (m_initialPressPoint == currentShortenedLinePressPoint)
	{
		// Not a line
		return;
	}

	double dx = currentShortenedLinePressPoint.x() - m_initialPressPoint.x();
	double dy = currentShortenedLinePressPoint.y() - m_initialPressPoint.y();
	double shorteningFactor = -m_arrowHeight;
	if (dx == 0)
	{
		// Vertical line
		if (currentShortenedLinePressPoint.y() < m_initialPressPoint.y())
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
		if (currentShortenedLinePressPoint.x() < m_initialPressPoint.x())
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
		currentShortenedLinePressPoint.setX(m_initialPressPoint.x() + dx);
		currentShortenedLinePressPoint.setY(m_initialPressPoint.y() + dy);
	}

	painter->setPen(pen);
	painter->drawLine(m_initialPressPoint, currentShortenedLinePressPoint);

	// Draw the arrow
	QVector2D initPosVec(m_initialPressPoint);
	QVector2D currPosVec(m_currentPressPoint);

	QVector2D auxVec(currPosVec - initPosVec);
	double length = qSqrt(qPow(auxVec.x(), 2.0) + qPow(auxVec.y(), 2.0));

	QVector2D currPostToInitPosVec = (currPosVec - initPosVec) / length;
	QVector2D perpendVec(-currPostToInitPosVec.y(), currPostToInitPosVec.x());
	QVector2D arrowEdgeVec1 = currPosVec - m_arrowHeight*currPostToInitPosVec + halfWidth*perpendVec;
	QVector2D arrowEdgeVec2 = currPosVec - m_arrowHeight*currPostToInitPosVec - halfWidth*perpendVec;

	QPoint p1(arrowEdgeVec1.x(), arrowEdgeVec1.y());
	QPoint p2(arrowEdgeVec2.x(), arrowEdgeVec2.y());

	QPolygon p;
	p.append(m_currentPressPoint);
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
	painter->drawLine(m_initialPressPoint, m_currentPressPoint);
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
	painter->drawRect(m_initialPressPoint.x(),
	                  m_initialPressPoint.y(),
	                  m_currentPressPoint.x() - m_initialPressPoint.x(),
	                  m_currentPressPoint.y() - m_initialPressPoint.y());

}

void ScreenshotArea::drawBrush(QPainter* painter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	QPen backupPen = painter->pen();
	QBrush backupBrush = painter->brush();

	painter->setPen(Qt::NoPen);

	QBrush brush(m_pToolBar->currentColor());

	painter->setBrush(brush);

	painter->drawEllipse(m_currentPressPoint.x(),
	                     m_currentPressPoint.y(),
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

	QRect ellipseRectangle(m_initialPressPoint.x(),
	                       m_initialPressPoint.y(),
	                       m_currentPressPoint.x() - m_initialPressPoint.x(),
	                       m_currentPressPoint.y() - m_initialPressPoint.y());

	painter->drawEllipse(ellipseRectangle);
}

void ScreenshotArea::drawText(QPainter* painter)
{
	Q_UNUSED(painter)
	// TODO: Draw text here
}

void ScreenshotArea::mouseMoveEvent(QMouseEvent *e)
{
	if (m_leftButtonPressed)
	{
		m_currentPressPoint = e->pos();

		if (m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			const QPoint toolBarPositionPoint =  m_screenShotArea.normalized().bottomRight();
			const int toolBarDistance = 10;
			const int toolBarDistanceThreshold = 15;
			int x = toolBarPositionPoint.x();
			int y = toolBarPositionPoint.y();

			if(x + m_pToolBar->width() + toolBarDistanceThreshold > m_originalCapture.width())
			{
				x = m_originalCapture.width() - m_pToolBar->width() - toolBarDistanceThreshold;
			}

			if(y + m_pToolBar->height() + toolBarDistanceThreshold > m_originalCapture.height())
			{
				y = m_originalCapture.height() - m_pToolBar->height() - toolBarDistanceThreshold;
			}

			m_pToolBar->move(QPoint(x, y) + QPoint(toolBarDistance, toolBarDistance));
		}
	}
}

void ScreenshotArea::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;
		m_selectionStarted = true;

		m_initialPressPoint = e->pos();
		m_currentPressPoint = e->pos();

		m_helperBoard = m_paintBoard;

		if(m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			m_pToolBar->hide();
		}
	}
}

void ScreenshotArea::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;

		if (m_pToolBar->currentTool() == ToolBar::NoTool)
		{
			m_pToolBar->show();
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
