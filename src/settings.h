#ifndef SETTINGS_H
#define SETTINGS_H

#include "dialog.h"

class QGridLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QHBoxLayout;
class QSpacerItem;
class ColorPicker;

class Settings : public Dialog
{
	Q_OBJECT

public:
	explicit Settings(QWidget *parent = 0);
	~Settings();

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

public slots:
	void hide();

signals:
	void overlayColorChanged(QColor color);
	void rubberBandColorChanged(QColor color);
	void rubberBandWidthChanged(int width);
	void penWidthChanged(int width);
	void dotsRadiusChanged(int radius);
	void fontSizeChanged(int size);

protected:
	void moveEvent(QMoveEvent *pEvent);

private:
	void onOverlayColorButtonClicked();
	void autopositionOverlayColorPicker();
	void onOverlayColorChanged(QColor newColor);

	void onRubberBandColorButtonClicked();
	void autopositionRubberBandColorPicker();
	void onRubberBandColorChanged(QColor newColor);

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

	QColor m_overlayColor;
	QColor m_rubberBandColor;
	int m_rubberBandWidth;
	int m_penWidth;
	int m_dotsRadius;
	int m_fontSize;
};

#endif // SETTINGS_H
