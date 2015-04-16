#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "colorpickerdialog.h"

class ToolBar : public QWidget
{
	Q_OBJECT

public:
	enum Type {
		Horizontal,
		Vertical
	};

	enum Tool {
		NoTool,
		Arrow,
		Line,
		Square,
		Ellipse,
		Brush,
		Text
	};

	explicit ToolBar(Type type = Vertical, QWidget *parent = 0);
	~ToolBar();

	Tool currentTool() const;
	QColor currentColor() const;

signals:
	void discardButtonPressed();
	void settingsButtonPressed();
	void uploadButtonPressed();
	void saveButtonPressed();

protected:
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);
	void mouseMoveEvent(QMouseEvent *pEvent);
	void paintEvent(QPaintEvent *pEvent);

private slots:
	void onColorButtonClicked();
	void onTextToolButtonPressed();
	void onBrushToolButtonPressed();
	void onSquareToolButtonPressed();
	void onEllipseToolButtonPressed();
	void onLineToolButtonPressed();
	void onArrowToolButtonPressed();

private:
	void autoPositionColorPicker();
	// Todo: Move this away from here
	void setTheme();

private:
	Type m_type;
	QLayout *m_pLayout;

	Tool m_currentTool;
	ColorPickerDialog m_colorPickerDialog;

	QButtonGroup *m_pButtonGroup;
	QPushButton *m_pSettingsButton;
	QPushButton *m_pColorButton;
	QPushButton *m_pArrowButton;
	QPushButton *m_pLineButton;
	QPushButton *m_pSquareButton;
	QPushButton *m_pEllipseButton;
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
