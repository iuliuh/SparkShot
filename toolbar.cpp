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

ToolBar::ToolBar(Type type, QWidget *parent) : QWidget(parent), m_type(type)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	m_leftButtonPressed = false;
	m_drawColor = Qt::black;

	m_pColorButton = new QPushButton();
	m_pArrowButton = new QPushButton(QIcon(":/images/arrowIcon"), "");
	m_pLineButton = new QPushButton(QIcon(":/images/lineIcon"), "");
	m_pSquareButton = new QPushButton(QIcon(":/images/squareIcon"), "");
	m_pSettingsButton = new QPushButton(QIcon(":/images/settingsIcon"), "");
	m_pPencilButton = new QPushButton(QIcon(":/images/pencilIcon"), "");
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
	m_pArrowButton->setFlat(true);
	m_pLineButton->setFlat(true);
	m_pSquareButton->setFlat(true);
	m_pSettingsButton->setFlat(true);
	m_pPencilButton->setFlat(true);
	m_pTextButton->setFlat(true);
	m_pBrushButton->setFlat(true);
	m_pUploadButton->setFlat(true);
	m_pSaveButton->setFlat(true);
	m_pCloseButton->setFlat(true);

	m_pArrowButton->setCheckable(true);
	m_pLineButton->setCheckable(true);
	m_pSquareButton->setCheckable(true);
	m_pPencilButton->setCheckable(true);
	m_pTextButton->setCheckable(true);
	m_pBrushButton->setCheckable(true);

	m_pButtonGroup->addButton(m_pArrowButton);
	m_pButtonGroup->addButton(m_pLineButton);
	m_pButtonGroup->addButton(m_pSquareButton);
	m_pButtonGroup->addButton(m_pPencilButton);
	m_pButtonGroup->addButton(m_pTextButton);
	m_pButtonGroup->addButton(m_pBrushButton);
	m_pButtonGroup->setExclusive(true);

	// Connections
	connect(m_pColorButton, &QPushButton::clicked,
			this, &ToolBar::onColorButtonClicked);
	connect(m_pCloseButton, &QPushButton::clicked,
			this, &ToolBar::discardButtonPressed);
	connect(m_pSettingsButton, &QPushButton::clicked,
			this, &ToolBar::settingsButtonPressed);
	connect(m_pUploadButton, &QPushButton::clicked,
			this, &ToolBar::uploadButtonPressed);
	connect(m_pSaveButton, &QPushButton::clicked,
			this, &ToolBar::saveButtonPressed);
	connect(m_pTextButton, &QPushButton::clicked,
			this, &ToolBar::textToolButtonPressed);
	connect(m_pBrushButton, &QPushButton::clicked,
			this, &ToolBar::brushToolButtonPressed);
	connect(m_pPencilButton, &QPushButton::clicked,
			this, &ToolBar::pencilToolButtonPressed);
	connect(m_pSquareButton, &QPushButton::clicked,
			this, &ToolBar::squareToolButtonPressed);
	connect(m_pLineButton, &QPushButton::clicked,
			this, &ToolBar::lineToolButtonPressed);
	connect(m_pArrowButton, &QPushButton::clicked,
			this, &ToolBar::arrowToolButtonPressed);

	m_pLayout->setSpacing(1);
	m_pLayout->addWidget(m_pHandler);
	m_pLayout->addItem(new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	m_pLayout->addWidget(m_pColorButton);
	m_pLayout->addWidget(m_pArrowButton);
	m_pLayout->addWidget(m_pLineButton);
	m_pLayout->addWidget(m_pSquareButton);
	m_pLayout->addWidget(m_pPencilButton);
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
	}

	pEvent->accept();
}

void ToolBar::paintEvent(QPaintEvent *pEvent)
{
	Q_UNUSED(pEvent)

	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ToolBar::onColorButtonClicked()
{
	m_drawColor = QColorDialog::getColor(m_drawColor,
										 0, /* Not quite safe but resolves the style issues */
										 "Pick your color",
										 QColorDialog::ShowAlphaChannel);

	m_pColorButton->setStyleSheet(QString("background-color: rgba(%1, %2, %3, %4)")
								  .arg(m_drawColor.red())
								  .arg(m_drawColor.green())
								  .arg(m_drawColor.blue())
								  .arg(m_drawColor.alpha()));
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

