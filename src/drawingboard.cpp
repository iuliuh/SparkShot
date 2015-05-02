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

	m_darkOverlayColor = Preferences::instance().overlayColor();
	m_rubberBandColor = Preferences::instance().rubberBandColor();
	m_rubberBandWidth = Preferences::instance().rubberBandWidth();
	m_penWidth = Preferences::instance().penWidth();
	m_fontSize = Preferences::instance().fontSize();
	m_rubberBandPointRadius = 3;
	m_arrowBaseWidth = m_penWidth * 4;
	m_arrowHeight = m_arrowBaseWidth * qSqrt(3.0);

	m_leftButtonPressed = false;
	m_moveCroppedArea = false;

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

	if(pEvent->key() == Qt::Key_Backspace)
	{
		m_currentText.remove(m_currentText.length() - 1, 1);
	}
	else
	{
		m_currentText.append(pEvent->text());
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
		m_temporarySketchesBoard = QPixmap(m_originalCapture.width(), m_originalCapture.height());
		m_temporarySketchesBoard.fill(QColor(0, 0, 0, 0));
		m_currentSketchesBoard = m_temporarySketchesBoard;
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

	m_arrowBaseWidth = m_penWidth * 4;
	m_arrowHeight = m_arrowBaseWidth * qSqrt(3.0);

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

void DrawingBoard::onToolbarToolChanged(ToolBar::Tool tool)
{
	switch(tool)
	{
	case ToolBar::Text: setCursor(Qt::IBeamCursor); break;
	default: setCursor(Qt::ArrowCursor);
	}
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

	QPixmap newPixmap = grab(m_cropRect);

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
	QPixmap newPixmap = grab(m_cropRect);

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

		QString str = jsonData.take("link").toString();
		qDebug() << str;
		m_pUploadDialog->setLink(str);
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
	qDebug() << "Error: " << error;

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &DrawingBoard::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &DrawingBoard::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &DrawingBoard::onSslErrors);
}
#include <QSslError>

void DrawingBoard::onSslErrors(QList<QSslError> errorList)
{
	Q_FOREACH (QSslError e, errorList)
	{
		qDebug() << e.errorString();
	}

	disconnect(m_pNetworkReply, &QNetworkReply::finished,
	           this, &DrawingBoard::replyFinished);
	disconnect(m_pNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
	           this, &DrawingBoard::onError);
	disconnect(m_pNetworkReply, &QNetworkReply::sslErrors,
	           this, &DrawingBoard::onSslErrors);
}

void DrawingBoard::drawRubberBand(QPainter* pPainter)
{
	QPen pen(m_rubberBandColor);
	pen.setWidth(m_rubberBandWidth);

	pPainter->setPen(pen);
	pPainter->setBrush(Qt::NoBrush);
	pPainter->drawRect(m_cropRect);

	pPainter->setBrush(m_rubberBandColor);

	QPoint topMiddle(m_cropRect.topLeft().x() + m_cropRect.width() / 2, m_cropRect.y());
	QPoint rightMiddle(m_cropRect.topLeft().x() + m_cropRect.width(), m_cropRect.y() + m_cropRect.height() / 2);
	QPoint bottomMiddle(m_cropRect.topLeft().x() + m_cropRect.width() / 2, m_cropRect.y() + m_cropRect.height());
	QPoint leftMiddle(m_cropRect.topLeft().x(), m_cropRect.y() + m_cropRect.height() / 2);

	pPainter->drawEllipse(m_cropRect.topLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(m_cropRect.topRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(m_cropRect.bottomLeft(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(m_cropRect.bottomRight(), m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(topMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(rightMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(bottomMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
	pPainter->drawEllipse(leftMiddle, m_rubberBandPointRadius, m_rubberBandPointRadius);
}

void DrawingBoard::drawCroppedArea(QPainter* pPainter)
{
	pPainter->drawPixmap(m_cropRect.normalized(), m_originalCapture, m_cropRect.normalized());
}

void DrawingBoard::drawDarkOverlay(QPainter* pPainter)
{
	QBrush darkOverlayBrush(m_darkOverlayColor);

	pPainter->setBrush(darkOverlayBrush);
	pPainter->drawRect(m_originalCapture.rect());
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
	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	QPoint currentShortenedLinePressPoint = m_finalMousePosition;

	if (m_initialMousePosition == currentShortenedLinePressPoint)
	{
		// Not a line
		return;
	}

	double dx = currentShortenedLinePressPoint.x() - m_initialMousePosition.x();
	double dy = currentShortenedLinePressPoint.y() - m_initialMousePosition.y();
	double shorteningFactor = -m_arrowHeight;
	if (dx == 0)
	{
		// Vertical line
		if (currentShortenedLinePressPoint.y() < m_initialMousePosition.y())
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
		if (currentShortenedLinePressPoint.x() < m_initialMousePosition.x())
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
		currentShortenedLinePressPoint.setX(m_initialMousePosition.x() + dx);
		currentShortenedLinePressPoint.setY(m_initialMousePosition.y() + dy);
	}

	pPainter->setPen(pen);
	pPainter->drawLine(m_initialMousePosition, currentShortenedLinePressPoint);

	// Draw the arrow
	QVector2D initPosVec(m_initialMousePosition);
	QVector2D currPosVec(m_finalMousePosition);

	QVector2D auxVec(currPosVec - initPosVec);
	double length = qSqrt(qPow(auxVec.x(), 2.0) + qPow(auxVec.y(), 2.0));

	QVector2D currPostToInitPosVec = (currPosVec - initPosVec) / length;
	QVector2D perpendVec(-currPostToInitPosVec.y(), currPostToInitPosVec.x());
	QVector2D arrowEdgeVec1 = currPosVec - m_arrowHeight*currPostToInitPosVec + halfWidth*perpendVec;
	QVector2D arrowEdgeVec2 = currPosVec - m_arrowHeight*currPostToInitPosVec - halfWidth*perpendVec;

	QPoint p1(arrowEdgeVec1.x(), arrowEdgeVec1.y());
	QPoint p2(arrowEdgeVec2.x(), arrowEdgeVec2.y());

	QPolygon p;
	p.append(m_finalMousePosition);
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

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	pPainter->setPen(pen);
	pPainter->drawLine(m_initialMousePosition, m_finalMousePosition);
}

void DrawingBoard::drawSquare(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	pPainter->setPen(pen);
	pPainter->drawRect(QRect(m_initialMousePosition, m_finalMousePosition).normalized());

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

	m_brushInitialPosition = m_brushFinalPosition;
	m_brushFinalPosition = m_currentMousePosition;

	pPainter->drawLine(m_brushInitialPosition, m_brushFinalPosition);

	commitSketches();

	pPainter->setPen(backupPen);
	pPainter->setBrush(backupBrush);
}

void DrawingBoard::drawEllipse(QPainter* pPainter)
{
	if(!m_leftButtonPressed)
	{
		return;
	}

	QPen pen;
	pen.setBrush(QBrush(m_pToolBar->currentColor()));
	pen.setWidth(m_penWidth);

	pPainter->setPen(pen);

	QRect ellipseRectangle(QRect(m_initialMousePosition, m_finalMousePosition));

	pPainter->drawEllipse(ellipseRectangle);
}

void DrawingBoard::drawText(QPainter* pPainter)
{
	QPen backupPen = pPainter->pen();
	QBrush backupBrush = pPainter->brush();

	QPen p;
	p.setStyle(Qt::DotLine);
	p.setColor(Qt::black);
	p.setWidth(1);

	pPainter->setPen(p);

	QFont textFont;
	textFont.setPointSize(m_fontSize);
	pPainter->setFont(textFont);

	QFontMetrics fontMetrics(textFont);

	const int padding = 10;
	QRect textRect(m_textPoint.x(),
	               m_textPoint.y(),
	               fontMetrics.width(m_currentText) + padding,
	               fontMetrics.height());

	pPainter->drawText(textRect,
	                   Qt::AlignLeft,
	                   m_currentText);

	textRect.adjust(-2, -2, 2, 2);

	pPainter->drawRect(textRect);

	pPainter->setPen(backupPen);
	pPainter->setBrush(backupBrush);
}

void DrawingBoard::commitSketches()
{
	m_temporarySketchesBoard = m_currentSketchesBoard;
}

void DrawingBoard::mousePressEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = true;

		m_initialMousePosition = e->pos();
		m_finalMousePosition = e->pos();

		if(m_pToolBar->currentTool() == ToolBar::Crop)
		{
			m_pToolBar->hide();

			if(m_cropRect.contains(e->pos(), true))
			{
				m_moveCroppedArea = true;
				m_initialCropRectMovePoint = e->pos();
				m_cropRectBeforeMove = m_cropRect;

				setCursor(Qt::ClosedHandCursor);
			}
		}
		else if(m_pToolBar->currentTool() == ToolBar::Text)
		{
			commitSketches();

			m_currentText.clear();
		}

		m_brushInitialPosition = e->pos();
		m_brushFinalPosition = e->pos();
		m_textPoint = e->pos();
	}

	update();
}

void DrawingBoard::mouseMoveEvent(QMouseEvent *e)
{
	if(m_leftButtonPressed)
	{
		m_finalMousePosition = e->pos();
	}

	if(m_pToolBar->currentTool() == ToolBar::Crop)
	{
		if(m_moveCroppedArea && m_leftButtonPressed)
		{
			m_cropRect = m_cropRectBeforeMove.translated(e->pos() - m_initialCropRectMovePoint);
		}

		if(!m_moveCroppedArea && m_leftButtonPressed)\
		{
			m_cropRect = QRect(m_initialMousePosition, m_finalMousePosition).normalized();
		}
	}

	if(m_pToolBar->currentTool() == ToolBar::Crop &&
	   m_cropRect.contains(e->pos()))
	{
		setCursor(m_leftButtonPressed ? Qt::ClosedHandCursor : Qt::OpenHandCursor);
	}
	else if(m_pToolBar->currentTool() == ToolBar::Text)
	{
		setCursor(Qt::IBeamCursor);
	}
	else
	{
		setCursor(Qt::ArrowCursor);
	}

	m_currentMousePosition = e->pos();

	update();
}

void DrawingBoard::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
		m_moveCroppedArea = false;

		if(m_pToolBar->currentTool() != ToolBar::Text)
		{
			commitSketches();
		}

		if(m_pToolBar->currentTool() == ToolBar::Crop &&
		   m_cropRect.contains(e->pos(), true))
		{
			setCursor(Qt::OpenHandCursor);
		}

		m_pToolBar->move(m_cropRect.bottomRight() + QPoint(20, 20));
		m_pToolBar->show();
	}

	update();
}

void DrawingBoard::paintEvent(QPaintEvent *pEvent)
{
	Q_UNUSED(pEvent)

	m_screenshotBoard = m_originalCapture;
	QPainter paintBoardPainter(&m_screenshotBoard);
	paintBoardPainter.setRenderHint(QPainter::Antialiasing);

	drawDarkOverlay(&paintBoardPainter);
	drawCroppedArea(&paintBoardPainter);
	drawRubberBand(&paintBoardPainter);

	m_currentSketchesBoard = m_temporarySketchesBoard;
	QPainter sketchesBoardPainter(&m_currentSketchesBoard);
	sketchesBoardPainter.setRenderHint(QPainter::Antialiasing);

	switch (m_pToolBar->currentTool())
	{
	case ToolBar::Crop:
		break;
	case ToolBar::Arrow:
		drawArrow(&sketchesBoardPainter);
		break;
	case ToolBar::Line:
		drawLine(&sketchesBoardPainter);
		break;
	case ToolBar::Square:
		drawSquare(&sketchesBoardPainter);
		break;
	case ToolBar::Brush:
		drawBrush(&sketchesBoardPainter);
		break;
	case ToolBar::Ellipse:
		drawEllipse(&sketchesBoardPainter);
		break;
	case ToolBar::Text:
		drawText(&sketchesBoardPainter);
		break;
	default:
		break;
	}

	QPainter formPainter(this);
	formPainter.drawPixmap(m_screenshotBoard.rect(), m_screenshotBoard, m_screenshotBoard.rect());
	formPainter.drawPixmap(m_currentSketchesBoard.rect(), m_currentSketchesBoard, m_currentSketchesBoard.rect());
}
