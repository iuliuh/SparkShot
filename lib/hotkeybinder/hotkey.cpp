#include "hotkey.h"

HotKey::HotKey()
{
}

HotKey::~HotKey()
{
}

uint HotKey::modifiers() const
{
	return m_modifiers;
}

void HotKey::setModifiers(uint modifiers)
{
	m_modifiers = modifiers;
}
uint HotKey::key() const
{
	return m_key;
}

void HotKey::setKey(uint key)
{
	m_key = key;
}
bool HotKey::valid() const
{
	return m_valid;
}

void HotKey::setValid(bool valid)
{
	m_valid = valid;
}


#ifdef Q_OS_WIN
DWORD HotKey::modifiers() const
{

}

void HotKey::setModifiers(DWORD modifiers)
{

}

DWORD HotKey::key() const
{

}

void HotKey::setKey(DWORD key)
{

}
#endif


