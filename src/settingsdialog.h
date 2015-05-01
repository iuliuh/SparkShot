#ifndef SETTINGS_H
#define SETTINGS_H

#include "dialog.h"
#include "preferences.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QHBoxLayout;
class QSpacerItem;
class ColorPicker;

class SettingsDialog : public Dialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

	QColor overlayColor() const;
	void setOverlayColor(const QColor &overlayColor);

	QColor rubberBandColor() const;
	void setRubberBandColor(const QColor &rubberBandColor);

	int rubberBandWidth() const;
	void setRubberBandWidth(int rubberBandWidth);

	int penWidth() const;
	void setPenWidth(int penWidth);

	int dotsRadius() const;
	void setDotsRadius(int dotsRadius);

	int fontSize() const;
	void setFontSize(int fontSize);

signals:
	void overlayColorChanged(QColor color);
	void rubberBandColorChanged(QColor color);
	void rubberBandWidthChanged(int width);
	void penWidthChanged(int width);
	void dotsRadiusChanged(int radius);
	void fontSizeChanged(int size);

protected:
	void moveEvent(QMoveEvent *pEvent);
	void keyPressEvent(QKeyEvent* pEvent);

private slots:
	void onOverlayColorButtonClicked();
	void onOverlayColorChanged(QColor newColor);

	void onRubberBandColorButtonClicked();
	void onRubberBandColorChanged(QColor newColor);

	void onRubberBandWidthChanged(int width);
	void onPenWidthChanged(int width);
	void onFontSizeChanged(int size);

	void onResetButtonClicked();

private:
	QGridLayout *m_pLayout;

	QLabel *m_pOverlayColorLabel;
	QPushButton *m_pOverlayColorButton;

	QLabel *m_pRubberBandColorLabel;
	QPushButton *m_pRubberBandColorButton;

	QLabel *m_pPenWidthLabel;
	QSpinBox *m_pPenWidthSpinBox;

	QLabel *m_pRubberBandWidthLabel;
	QSpinBox *m_pRubberBandWidthSpinBox;

	QLabel *m_pFontSizeLabel;
	QSpinBox *m_pFontSizeSpinBox;

	QHBoxLayout *m_pResetDefaultLayout;
	QPushButton *m_pResetDefaultsButton;
	QSpacerItem *m_pResetDefaultsSpacer;

	QSpacerItem *m_pVerticalSpacer;

	ColorPicker *m_pOverlayColorPicker;
	ColorPicker *m_pRubberBandColorPicker;

	int m_dotsRadius;
};

#endif // SETTINGS_H
