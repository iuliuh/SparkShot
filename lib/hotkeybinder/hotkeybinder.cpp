#include "hotkeybinder.h"

#include "qxtglobalshortcut.h"

#include <QDebug>

HotKeyBinder::HotKeyBinder(QObject *parent) :
    QObject(parent),
    m_pShortcut(new QxtGlobalShortcut())
{
    m_pShortcut->setShortcut(QKeySequence("Shift+1")); // Default

    connect(m_pShortcut, &QxtGlobalShortcut::activated, [this]() {
        Q_EMIT hotKeyTriggered();
    });
}

HotKeyBinder::~HotKeyBinder()
{
}

void HotKeyBinder::setKeySequence(const QKeySequence& keySequence)
{
	qDebug() << Q_FUNC_INFO << "Setting shortcut" << keySequence.toString();
	m_pShortcut->setShortcut(keySequence);
}

QKeySequence HotKeyBinder::keySequence()
{
	return m_pShortcut->shortcut();
}

