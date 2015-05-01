#include "toolbar.h"

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QSpacerItem>
#include <QStyleOption>
#include <QFile>
#include <QPoint>
#include <QColorDialog>
#include <QButtonGroup>
#include <QScreen>

ToolBar::ToolBar(Type type, QWidget *parent) :
    QWidget(parent),
    m_type(type),
    m_colorPickerDialog(this),
    m_settingsDialog(this)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	m_leftButtonPressed = false;
	m_drawColor = Qt::black;
	m_currentTool = Crop;

	m_screenRect = QGuiApplication::primaryScreen()->grabWindow(0).rect();

	m_pColorButton = new QPushButton();
	m_pCropButton = new QPushButton(QIcon(":/images/cropIcon"), "");
	m_pArrowButton = new QPushButton(QIcon(":/images/arrowIcon"), "");
	m_pLineButton = new QPushButton(QIcon(":/images/lineIcon"), "");
	m_pSquareButton = new QPushButton(QIcon(":/images/squareIcon"), "");
	m_pSettingsButton = new QPushButton(QIcon(":/images/settingsIcon"), "");
	m_pEllipseButton = new QPushButton(QIcon(":/images/ellipseIcon"), "");
	m_pTextButton = new QPushButton(QIcon(":/images/textIcon"), "");
	m_pBrushButton = new QPushButton(QIcon(":/images/brushIcon"), "");
	m_pUploadButton = new QPushButton(QIcon(":/images/uploadIcon"), "");
	m_pSaveButton = new QPushButton(QIcon(":/images/saveIcon"), "");
	m_pCloseButton = new QPushButton(QIcon(":/images/closeIcon"), "");
	m_pButtonGroup = new QButtonGroup(this);
	m_pSeparator1 = new QLabel();
	m_pSeparator2 = new QLabel();
	m_pHandler = new QLabel();

	if(type == Horizontal)
	{
		m_pLayout = new QHBoxLayout;
		setMaximumHeight(35);
		m_pLayout->setContentsMargins(5, 2, 10, 2);

		m_pSeparator1->setPixmap(QPixmap(":/images/horizontalSeparatorIcon"));
		m_pSeparator2->setPixmap(QPixmap(":/images/horizontalSeparatorIcon"));
		m_pHandler->setPixmap(QPixmap(":/images/handleIcon"));
	}
	else
	{
		m_pLayout = new QVBoxLayout;
		setMaximumWidth(35);
		m_pLayout->setContentsMargins(2, 5, 2, 10);

		m_pSeparator1->setPixmap(QPixmap(":/images/verticalSeparatorIcon"));
		m_pSeparator2->setPixmap(QPixmap(":/images/verticalSeparatorIcon"));
		m_pHandler->setPixmap(QPixmap(":/images/handleIcon"));
		m_pHandler->setStyleSheet("padding-left: 8px;");
	}

	m_pColorButton->setFlat(true);
	m_pCropButton->setFlat(true);
	m_pArrowButton->setFlat(true);
	m_pLineButton->setFlat(true);
	m_pSquareButton->setFlat(true);
	m_pSettingsButton->setFlat(true);
	m_pEllipseButton->setFlat(true);
	m_pTextButton->setFlat(true);
	m_pBrushButton->setFlat(true);
	m_pUploadButton->setFlat(true);
	m_pSaveButton->setFlat(true);
	m_pCloseButton->setFlat(true);

	m_pCropButton->setCheckable(true);
	m_pArrowButton->setCheckable(true);
	m_pLineButton->setCheckable(true);
	m_pSquareButton->setCheckable(true);
	m_pEllipseButton->setCheckable(true);
	m_pTextButton->setCheckable(true);
	m_pBrushButton->setCheckable(true);

	m_pButtonGroup->addButton(m_pCropButton);
	m_pButtonGroup->addButton(m_pArrowButton);
	m_pButtonGroup->addButton(m_pLineButton);
	m_pButtonGroup->addButton(m_pSquareButton);
	m_pButtonGroup->addButton(m_pEllipseButton);
	m_pButtonGroup->addButton(m_pTextButton);
	m_pButtonGroup->addButton(m_pBrushButton);
	m_pButtonGroup->setExclusive(true);

	// Connections
	connect(m_pColorButton, &QPushButton::clicked,
	        this, &ToolBar::onColorButtonClicked);
	connect(m_pCloseButton, &QPushButton::clicked,
	        this, &ToolBar::discardButtonPressed);
	connect(m_pSettingsButton, &QPushButton::clicked,
	        this, &ToolBar::onSettingsButtonPressed);
	connect(m_pUploadButton, &QPushButton::clicked,
	        this, &ToolBar::uploadButtonPressed);
	connect(m_pSaveButton, &QPushButton::clicked,
	        this, &ToolBar::saveButtonPressed);
	connect(m_pTextButton, &QPushButton::clicked,
	        this, &ToolBar::onTextToolButtonPressed);
	connect(m_pBrushButton, &QPushButton::clicked,
	        this, &ToolBar::onBrushToolButtonPressed);
	connect(m_pEllipseButton, &QPushButton::clicked,
	        this, &ToolBar::onEllipseToolButtonPressed);
	connect(m_pSquareButton, &QPushButton::clicked,
	        this, &ToolBar::onSquareToolButtonPressed);
	connect(m_pLineButton, &QPushButton::clicked,
	        this, &ToolBar::onLineToolButtonPressed);
	connect(m_pArrowButton, &QPushButton::clicked,
	        this, &ToolBar::onArrowToolButtonPressed);
	connect(m_pCropButton, &QPushButton::clicked,
	        this, &ToolBar::onCropToolButtonPressed);

	connect(&m_settingsDialog, &SettingsDialog::overlayColorChanged,
	        this, &ToolBar::overlayColorChanged);
	connect(&m_settingsDialog, &SettingsDialog::rubberBandColorChanged,
	        this, &ToolBar::rubberBandColorChanged);
	connect(&m_settingsDialog, &SettingsDialog::rubberBandWidthChanged,
	        this, &ToolBar::rubberBandWidthChanged);
	connect(&m_settingsDialog, &SettingsDialog::penWidthChanged,
	        this, &ToolBar::penWidthChanged);
	connect(&m_settingsDialog, &SettingsDialog::dotsRadiusChanged,
	        this, &ToolBar::dotsRadiusChanged);
	connect(&m_settingsDialog, &SettingsDialog::fontSizeChanged,
	        this, &ToolBar::fontSizeChanged);

	m_pLayout->setSpacing(1);
	m_pLayout->addWidget(m_pHandler);
	m_pLayout->addItem(new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	m_pLayout->addWidget(m_pColorButton);
	m_pLayout->addWidget(m_pCropButton);
	m_pLayout->addWidget(m_pArrowButton);
	m_pLayout->addWidget(m_pLineButton);
	m_pLayout->addWidget(m_pSquareButton);
	m_pLayout->addWidget(m_pEllipseButton);
	m_pLayout->addWidget(m_pBrushButton);
	m_pLayout->addWidget(m_pTextButton);
	m_pLayout->addWidget(m_pSeparator1);
	m_pLayout->addWidget(m_pSaveButton);
	m_pLayout->addWidget(m_pUploadButton);
	m_pLayout->addWidget(m_pSeparator2);
	m_pLayout->addWidget(m_pSettingsButton);
	m_pLayout->addWidget(m_pCloseButton);

	setLayout(m_pLayout);

	setTheme();
}

ToolBar::~ToolBar()
{
}

ToolBar::Tool ToolBar::currentTool() const
{
	return m_currentTool;
}

QColor ToolBar::currentColor() const
{
	return m_drawColor;
}

void ToolBar::hide()
{
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();

	QWidget::hide();
}

void ToolBar::show()
{
	autoPositionColorPicker();
	autoPositionSettingsDialog();

	QWidget::show();
}

void ToolBar::mousePressEvent(QMouseEvent* pEvent)
{
	if(pEvent->button() == Qt::LeftButton)
	{
		m_mousePosOnBar = pEvent->pos();
		m_leftButtonPressed = true;
	}

	pEvent->accept();
}

void ToolBar::mouseReleaseEvent(QMouseEvent* pEvent)
{
	if(pEvent->button() == Qt::LeftButton)
	{
		m_leftButtonPressed = false;
	}

	pEvent->accept();
}

void ToolBar::mouseMoveEvent(QMouseEvent* pEvent)
{
	if(m_leftButtonPressed)
	{
		move(pEvent->pos().x() - m_mousePosOnBar.x() + geometry().x(),
		     pEvent->pos().y() - m_mousePosOnBar.y() + geometry().y());

		autoPositionColorPicker();
		autoPositionSettingsDialog();
	}


	pEvent->accept();
}

void ToolBar::paintEvent(QPaintEvent *pEvent)
{
	Q_UNUSED(pEvent)

	QPainter p(this);

	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void ToolBar::autoPositionColorPicker()
{
	const int threshold = 5;
	const int xCorrection = 10;

	int colorPickerXPosition = -m_pColorButton->mapFromGlobal(QPoint(0, 0)).x();
	int colorPickerYPosition = -m_pColorButton->mapFromGlobal(QPoint(0, 0)).y();

	if(m_colorPickerDialog.height() + height() + y() > m_screenRect.height())
	{
		m_colorPickerDialog.setArrowLocation(ColorPicker::Bottom);
		colorPickerXPosition -= m_pColorButton->width() - threshold + xCorrection;
		colorPickerYPosition -= m_colorPickerDialog.height() + threshold;
	}
	else
	{
		m_colorPickerDialog.setArrowLocation(ColorPicker::Top);
		colorPickerXPosition -= m_pColorButton->width() - threshold + xCorrection;
		colorPickerYPosition -= -m_pColorButton->height() - threshold;
	}

	m_colorPickerDialog.setGeometry(colorPickerXPosition,
	                                colorPickerYPosition,
	                                m_colorPickerDialog.width(),
	                                m_colorPickerDialog.height());
}

void ToolBar::autoPositionSettingsDialog()
{
	const int threshold = 5;
	const int xCorrection = 80;

	int settingsXPosition = -m_pSettingsButton->mapFromGlobal(QPoint(0, 0)).x();
	int settingsYPosition = -m_pSettingsButton->mapFromGlobal(QPoint(0, 0)).y();

	if(m_settingsDialog.height() + height() + y() > m_screenRect.height())
	{
		m_settingsDialog.setArrowLocation(ColorPicker::Bottom);
		settingsXPosition -= m_pSettingsButton->width() - threshold + xCorrection;
		settingsYPosition -= m_settingsDialog.height() + threshold;
	}
	else
	{
		m_settingsDialog.setArrowLocation(ColorPicker::Top);
		settingsXPosition -= m_pSettingsButton->width() - threshold + xCorrection;
		settingsYPosition -= -m_pSettingsButton->height() - threshold;
	}

	m_settingsDialog.setGeometry(settingsXPosition,
	                             settingsYPosition,
	                             m_settingsDialog.width(),
	                             m_settingsDialog.height());
}

void ToolBar::onColorButtonClicked()
{
	autoPositionColorPicker();

	m_settingsDialog.hide();
	m_colorPickerDialog.isVisible() ? m_colorPickerDialog.hide() : m_colorPickerDialog.show();
}

void ToolBar::onTextToolButtonPressed()
{
	m_currentTool = Text;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onBrushToolButtonPressed()
{
	m_currentTool = Brush;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onEllipseToolButtonPressed()
{
	m_currentTool = Ellipse;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onSquareToolButtonPressed()
{
	m_currentTool = Square;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onLineToolButtonPressed()
{
	m_currentTool = Line;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onArrowToolButtonPressed()
{
	m_currentTool = Arrow;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onCropToolButtonPressed()
{
	m_currentTool = Crop;
	m_colorPickerDialog.hide();
	m_settingsDialog.hide();
}

void ToolBar::onSettingsButtonPressed()
{
	m_colorPickerDialog.hide();

	autoPositionSettingsDialog();

	m_settingsDialog.isVisible() ? m_settingsDialog.hide() : m_settingsDialog.show();
}

void ToolBar::onDrawingColorChanged(QColor newColor)
{
	m_drawColor = newColor;
	m_pColorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
	                              .arg(newColor.red())
	                              .arg(newColor.green())
	                              .arg(newColor.blue()));
}

void ToolBar::setTheme()
{
	m_pColorButton->setStyleSheet("background-color: black;");

	QFile stylesheetFile(":/theme/lightTheme");
	if(stylesheetFile.open(QFile::ReadOnly))
	{
		QString styleSheet = QLatin1String(stylesheetFile.readAll());
		qApp->setStyleSheet(styleSheet);
	}

	stylesheetFile.close();
}
