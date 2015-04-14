#include "toolbar.h"

#include <QPainter>
#include <QPaintEvent>
#include <QIcon>
#include <QSpacerItem>
#include <QStyleOption>

ToolBar::ToolBar(ToolBar::Type type, QWidget *parent) : QWidget(parent), m_type(type)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	setStyleSheet("background-color: red; border: 1px solid cyan; border-radius: 5px;");
	if(type == Horizontal)
	{
		resize(200, 25);
		m_pLayout = new QHBoxLayout;
	}
	else
	{
		resize(25, 200);
		m_pLayout = new QVBoxLayout;
	}

//	m_pLayout->setMargin(6);

	m_pUploadButton = new QPushButton(QIcon(":/images/cloud-upload.png"), "");
	m_pUploadButton->setMaximumSize(16, 16);
	m_pUploadButton->setFlat(true);
	m_pUploadButton->setStyleSheet("QPushButton { background: transparent; outline: 0px; border: 0px; margin: 0px; padding: 0px; }");

	m_pBrushButton = new QPushButton(QIcon(":/images/brush.png"), "");
	m_pBrushButton->setMaximumSize(16, 16);
	m_pBrushButton->setFlat(true);
	m_pBrushButton->setStyleSheet("QPushButton { background: transparent; outline: 0px; border: 0px; margin: 0px; padding: 0px; }");

	m_pLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
	m_pLayout->addWidget(m_pUploadButton);
	m_pLayout->addWidget(m_pBrushButton);

	setLayout(m_pLayout);
}

ToolBar::~ToolBar()
{

}

void ToolBar::paintEvent(QPaintEvent *pEvent)
{
	QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

