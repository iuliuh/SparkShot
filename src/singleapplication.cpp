#include "singleapplication.h"

#include <QTimer>
#include <QByteArray>

SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv)
{
	m_sharedMemory.setKey(uniqueKey);

	// When can create it only if it doesn't exist
	if(m_sharedMemory.create(5000))
	{
		m_sharedMemory.lock();

		*(char*)m_sharedMemory.data() = '\0';
		m_sharedMemory.unlock();

		m_alreadyExists = false;

		// Start checking for messages of other instances.
		QTimer *pTimer = new QTimer(this);
		connect(pTimer, SIGNAL(timeout()), this, SLOT(checkForMessage()));
		pTimer->start(200);
	}
	else if(m_sharedMemory.attach())
	{
		// It exits, so we can attach it?!
		m_alreadyExists = true;
	}
	else
	{
		// Error
	}

}

bool SingleApplication::alreadyExists() const
{
	return m_alreadyExists;
}

bool SingleApplication::isMasterApp() const
{
	return !alreadyExists();
}

void SingleApplication::checkForMessage()
{
	QStringList arguments;

	m_sharedMemory.lock();
	char *pFrom = (char*)m_sharedMemory.data();

	while(*pFrom != '\0')
	{
		int sizeToRead = int(*pFrom);
		++pFrom;

		QByteArray byteArray = QByteArray(pFrom, sizeToRead);
		byteArray[sizeToRead] = '\0';
		pFrom += sizeToRead;

		arguments << QString::fromUtf8(byteArray.constData());
	}

	*(char*)m_sharedMemory.data() = '\0';
	m_sharedMemory.unlock();

	if(arguments.size())
	{
		Q_EMIT messageAvailable( arguments );
	}
}

bool SingleApplication::sendMessage(const QString &message)
{
	// We cannot send mess if we are master process!
	if (isMasterApp())
	{
		return false;
	}

	QByteArray byteArray;
	byteArray.append(char(message.size()));
	byteArray.append(message.toUtf8());
	byteArray.append('\0');

	m_sharedMemory.lock();
	char *pTo = (char*)m_sharedMemory.data();

	while(*pTo != '\0')
	{
		int sizeToRead = int(*pTo);
		pTo += sizeToRead + 1;
	}

	const char *pFrom = byteArray.data();
	memcpy(pTo, pFrom, qMin(m_sharedMemory.size(), byteArray.size()));
	m_sharedMemory.unlock();

	return true;
}
