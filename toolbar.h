#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

class ToolBar : public QWidget
{
	Q_OBJECT

public:
	enum Type {
		Horizontal,
		Vertical
	};

	explicit ToolBar(Type type = Horizontal, QWidget *parent = 0);
	~ToolBar();

protected:
	void paintEvent(QPaintEvent *pEvent);

private:
	Type m_type;
	QLayout *m_pLayout;

	QPushButton *m_pUploadButton;
	QPushButton *m_pBrushButton;
};

#endif // TOOLBAR_H
