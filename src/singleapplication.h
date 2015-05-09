#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <QSharedMemory>
#include <QStringList>

//! \brief Class ment to stop running two instances
//!        of the same application at the same time.
class SingleApplication : public QApplication
{
	Q_OBJECT

public:
	//! \brief Constructs a SingleApplication object.
	//! \param argc Arguments count.
	//! \param argv Arguments values.
	//! \param uniqueKey Application unique key.
	SingleApplication(int &argc, char *argv[], const QString uniqueKey);

	//! \brief Checks if application is already running.
	//! \return true if application is already runing, false otherwise.
	bool alreadyExists() const;

	//! \brief Checks if application is the master application. The master
	//!        application is the application started first.
	bool isMasterApp() const;

	//! \brief Sends message to the already running application.
	//! \param message The message to send.
	bool sendMessage(const QString& message);

public Q_SLOTS:
	//! \brief Slot called when a message has arrived.
	//! \see SingleApplication::messageAvailable
	void checkForMessage();

Q_SIGNALS:
	//! \brief Signal emitted when a message is available.
	//! \brief message The message to read.
	void messageAvailable(const QStringList& messages);

private:
	bool m_alreadyExists;
	QSharedMemory m_sharedMemory;
};

#endif // SINGLEAPPLICATION_H
