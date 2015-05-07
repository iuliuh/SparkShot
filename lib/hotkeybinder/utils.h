#ifndef UTILS
#define UTILS

#include "hotkey.h"

#include <QDebug>

#define KEY_SEPARATOR "+"

namespace Utils
{
	HotKey HOTKEYBINDERSHARED_EXPORT stringToHotKey(const QString& key)
	{
		HotKey hotKey;

		QStringList keyList = key.split(KEY_SEPARATOR);
		hotKey.setValid(true);

#ifdef Q_OS_WIN
#endif

#ifdef Q_OS_LINUX
		qDebug() << keyList;
#endif

		return hotKey;
	}
}

#endif // UTILS
