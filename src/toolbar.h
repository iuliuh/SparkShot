#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "colorpicker.h"
#include "quicksettingsdialog.h"

class ToolBar : public QWidget
{
	Q_OBJECT

public:
	enum Type {
		Horizontal,
		Vertical
	};

	enum Tool {
		Crop,
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

public Q_SLOTS:
	void hide();
	void show();

Q_SIGNALS:
	void discardButtonPressed();
	void settingsButtonPressed();
	void uploadButtonPressed();
	void saveButtonPressed();

	void overlayColorChanged(QColor color);
	void rubberBandColorChanged(QColor color);
	void rubberBandWidthChanged(int width);
	void penWidthChanged(int width);
	void dotsRadiusChanged(int radius);
	void fontSizeChanged(int size);

protected:
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);
	void mouseMoveEvent(QMouseEvent *pEvent);
	void paintEvent(QPaintEvent *pEvent);

private Q_SLOTS:
	void onColorButtonClicked();
	void onTextToolButtonPressed();
	void onBrushToolButtonPressed();
	void onSquareToolButtonPressed();
	void onEllipseToolButtonPressed();
	void onLineToolButtonPressed();
	void onArrowToolButtonPressed();
	void onCropToolButtonPressed();
	void onSettingsButtonPressed();
	void onDrawingColorChanged(QColor newColor);

private:
	void autoPositionColorPicker();
	void autoPositionSettingsDialog();
	void setTheme();

private:
	Type m_type;
	QLayout *m_pLayout;

	Tool m_currentTool;
	ColorPicker m_colorPickerDialog;
	QuickSettingsDialog m_settingsDialog;

	QButtonGroup *m_pButtonGroup;
	QPushButton *m_pSettingsButton;
	QPushButton *m_pColorButton;
	QPushButton *m_pCropButton;
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
	QRect m_screenRect;
};

#endif // TOOLBAR_H
