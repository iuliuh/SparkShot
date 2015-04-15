#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class ToolBar : public QWidget
{
	Q_OBJECT

public:
	enum Type {
		Horizontal,
		Vertical
	};

	enum Tool {
		Arrow,
		Line,
		Square,
		Pencil,
		Brush,
		Text
	};

	explicit ToolBar(Type type = Vertical, QWidget *parent = 0);
	~ToolBar();

signals:
	void discardButtonPressed();
	void settingsButtonPressed();
	void uploadButtonPressed();
	void saveButtonPressed();
	void toolButtonChanged(const Tool& tool);

protected:
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);
	void mouseMoveEvent(QMouseEvent *pEvent);
	void paintEvent(QPaintEvent *pEvent);

private slots:
	void onColorButtonClicked();
	void onTextToolButtonPressed();
	void onBrushToolButtonPressed();
	void onPencilToolButtonPressed();
	void onSquareToolButtonPressed();
	void onLineToolButtonPressed();
	void onArrowToolButtonPressed();

private:
	// Todo: Move this away from here
	void setTheme();

private:
	Type m_type;
	QLayout *m_pLayout;

	QButtonGroup *m_pButtonGroup;
	QPushButton *m_pSettingsButton;
	QPushButton *m_pColorButton;
	QPushButton *m_pArrowButton;
	QPushButton *m_pLineButton;
	QPushButton *m_pSquareButton;
	QPushButton *m_pPencilButton;
	QPushButton *m_pSaveButton;
	QPushButton *m_pBrushButton;
	QPushButton *m_pTextButton;
	QPushButton *m_pUploadButton;
	QPushButton *m_pCloseButton;
	QLabel *m_pSeparator1;
	QLabel *m_pSeparator2;
	QLabel *m_pHandler;

	bool m_leftButtonPressed;
	QPoint m_mousePosOnBar;
	QColor m_drawColor;
};

#endif // TOOLBAR_H
