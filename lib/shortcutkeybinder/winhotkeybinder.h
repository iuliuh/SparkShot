#ifndef WINHOTKEYBINDER_H
#define WINHOTKEYBINDER_H

#include <QObject>

class WinHotKeyBinder : public QObject
{
	Q_OBJECT
public:
	explicit WinHotKeyBinder(QObject *parent = 0);
	~WinHotKeyBinder();

signals:

public slots:
};

#endif // WINHOTKEYBINDER_H
