#include "hotkey.h"
#include "defines.h"

#include <QString>
#include <QStringList>
#include <QDebug>

HotKey::HotKey() : m_valid(true)
{
	initializeHotKeyMap();
}

HotKey::~HotKey()
{
}

bool HotKey::isValid() const
{
	return m_valid;
}

QVector<DWORD> HotKey::keys() const
{
	return m_keys;
}

int HotKey::count() const
{
	if(!m_valid)
	{
		return 0;
	}

	return m_keys.count();
}

HotKey HotKey::fromString(const QString& hotKeyString)
{
	HotKey hotKey;

	QStringList keyList = hotKeyString.split(KEY_SEPARATOR);

	Q_FOREACH(QString key, keyList)
	{
		if(hotKey.m_keyMap.contains(key))
		{
			hotKey.m_keys.push_back(hotKey.m_keyMap[key]);
		}
		else
		{
			hotKey.setValid(false);
		}
	}

	return hotKey;
}

void HotKey::setValid(bool valid)
{
	m_valid = valid;
}

void HotKey::initializeHotKeyMap()
{
#ifdef Q_OS_WIN
	m_keyMap["A"]          = 'A';
	m_keyMap["B"]          = 'B';
	m_keyMap["C"]          = 'C';
	m_keyMap["D"]          = 'D';
	m_keyMap["E"]          = 'E';
	m_keyMap["F"]          = 'F';
	m_keyMap["G"]          = 'G';
	m_keyMap["H"]          = 'H';
	m_keyMap["I"]          = 'I';
	m_keyMap["J"]          = 'J';
	m_keyMap["K"]          = 'K';
	m_keyMap["L"]          = 'L';
	m_keyMap["M"]          = 'M';
	m_keyMap["N"]          = 'N';
	m_keyMap["O"]          = 'O';
	m_keyMap["P"]          = 'P';
	m_keyMap["Q"]          = 'Q';
	m_keyMap["R"]          = 'R';
	m_keyMap["S"]          = 'S';
	m_keyMap["T"]          = 'T';
	m_keyMap["U"]          = 'U';
	m_keyMap["V"]          = 'V';
	m_keyMap["W"]          = 'W';
	m_keyMap["X"]          = 'X';
	m_keyMap["Y"]          = 'Y';
	m_keyMap["Z"]          = 'Z';

	m_keyMap["0"]          = '0';
	m_keyMap["1"]          = '1';
	m_keyMap["2"]          = '2';
	m_keyMap["3"]          = '3';
	m_keyMap["4"]          = '4';
	m_keyMap["5"]          = '5';
	m_keyMap["6"]          = '6';
	m_keyMap["7"]          = '7';
	m_keyMap["8"]          = '8';
	m_keyMap["9"]          = '9';

	m_keyMap["Ctrl"]       = VK_LCONTROL;
	m_keyMap["Shift"]      = VK_LSHIFT;
	m_keyMap["Alt"]        = VK_LMENU;
	m_keyMap["Meta"]       = VK_LWIN;
	m_keyMap["Tab"]        = VK_TAB;
	m_keyMap["Print"]      = VK_SNAPSHOT;
	m_keyMap["Esc"]        = VK_ESCAPE;
	m_keyMap["Space"]      = VK_SPACE;
	m_keyMap["PgUp"]       = VK_PRIOR;
	m_keyMap["PgDown"]     = VK_NEXT;
	m_keyMap["End"]        = VK_END;
	m_keyMap["Home"]       = VK_HOME;
	m_keyMap["Left"]       = VK_LEFT;
	m_keyMap["Up"]         = VK_UP;
	m_keyMap["Right"]      = VK_RIGHT;
	m_keyMap["Down"]       = VK_DOWN;
	m_keyMap["Ins"]        = VK_INSERT;
	m_keyMap["Del"]        = VK_DELETE;
	m_keyMap["Backspace"]  = VK_BACK;
	m_keyMap["Return"]     = VK_RETURN;
	m_keyMap["Menu"]       = VK_APPS;

	m_keyMap["F1"]         = VK_F1;
	m_keyMap["F2"]         = VK_F2;
	m_keyMap["F3"]         = VK_F3;
	m_keyMap["F4"]         = VK_F4;
	m_keyMap["F5"]         = VK_F5;
	m_keyMap["F6"]         = VK_F6;
	m_keyMap["F7"]         = VK_F7;
	m_keyMap["F8"]         = VK_F8;
	m_keyMap["F9"]         = VK_F9;
	m_keyMap["F10"]        = VK_F10;
	m_keyMap["F11"]        = VK_F11;
	m_keyMap["F12"]        = VK_F12;
	m_keyMap["F13"]        = VK_F13;
	m_keyMap["F14"]        = VK_F14;
	m_keyMap["F15"]        = VK_F15;
	m_keyMap["F16"]        = VK_F16;
	m_keyMap["F17"]        = VK_F17;
	m_keyMap["F18"]        = VK_F18;
	m_keyMap["F19"]        = VK_F19;
	m_keyMap["F20"]        = VK_F20;
	m_keyMap["F21"]        = VK_F21;
	m_keyMap["F22"]        = VK_F22;
	m_keyMap["F23"]        = VK_F23;
	m_keyMap["F24"]        = VK_F24;

	m_keyMap["`"]          = VK_OEM_3;
	m_keyMap[":"]          = VK_OEM_1;
	m_keyMap[";"]          = VK_OEM_1;
	m_keyMap["+"]          = VK_OEM_PLUS;
	m_keyMap[","]          = VK_OEM_COMMA;
	m_keyMap["-"]          = VK_OEM_MINUS;
	m_keyMap["."]          = VK_OEM_PERIOD;
	m_keyMap["/"]          = VK_OEM_2;
	m_keyMap["?"]          = VK_OEM_2;

	m_keyMap["["]          = VK_OEM_4;
	m_keyMap["{"]          = VK_OEM_4;
	m_keyMap["]"]          = VK_OEM_6;
	m_keyMap["}"]          = VK_OEM_6;
	m_keyMap["\\"]         = VK_OEM_5;
	m_keyMap["|"]          = VK_OEM_5;
	m_keyMap["\""]         = VK_OEM_7;
	m_keyMap["'"]          = VK_OEM_7;

	// Unmapped...please map them if you have them
	// on your keyboard...
	m_keyMap[""]             = VK_HELP;
	m_keyMap[""]             = VK_EXECUTE;
	m_keyMap[""]             = VK_SCROLL;
	m_keyMap[""]             = VK_OEM_8;

#if(_WIN32_WINNT >= 0x0500)
	m_keyMap[""]             = VK_BROWSER_BACK;
	m_keyMap[""]             = VK_BROWSER_FORWARD;
	m_keyMap[""]             = VK_BROWSER_REFRESH;
	m_keyMap[""]             = VK_BROWSER_STOP;
	m_keyMap[""]             = VK_BROWSER_SEARCH;
	m_keyMap[""]             = VK_BROWSER_FAVORITES;
	m_keyMap[""]             = VK_BROWSER_HOME;
	m_keyMap[""]             = VK_VOLUME_MUTE;
	m_keyMap[""]             = VK_VOLUME_DOWN;
	m_keyMap[""]             = VK_VOLUME_UP;
	m_keyMap[""]             = VK_MEDIA_NEXT_TRACK;
	m_keyMap[""]             = VK_MEDIA_PREV_TRACK;
	m_keyMap[""]             = VK_MEDIA_STOP;
	m_keyMap[""]             = VK_MEDIA_PLAY_PAUSE;
	m_keyMap[""]             = VK_LAUNCH_MAIL;;
	m_keyMap[""]             = VK_LAUNCH_MEDIA_SELECT;
	m_keyMap[""]             = VK_LAUNCH_APP1;
	m_keyMap[""]             = VK_LAUNCH_APP2;
#endif /* _WIN32_WINNT >= 0x0500 */

#endif /* Q_OS_WIN */
}

