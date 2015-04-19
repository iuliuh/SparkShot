#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class Dialog : public QDialog
{
	Q_OBJECT

public:
	enum ArrowLocation
	{
		Top,
		Right,
		Bottom,
		Left
	};

	explicit Dialog(QWidget *parent = 0);
	~Dialog();

	void setArrowLocation(ArrowLocation arrowLocation);
	ArrowLocation arrowLocation() const;

protected:
	void paintEvent(QPaintEvent *e);

private:
	ArrowLocation m_arrowLocation;
};

#endif // DIALOG_H
