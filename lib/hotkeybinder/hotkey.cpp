#include "hotkey.h"
#include "defines.h"

#include <QString>
#include <QStringList>

HotKey::HotKey() : m_valid(true)
{
	initializeHotKeyMap();
}

HotKey::~HotKey()
{
}

#ifdef Q_OS_WINDOWS
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
#elif defined Q_OS_LINUX
bool HotKey::isValid() const
{
	return m_valid;
}

int HotKey::key() const
{
	return m_key;
}

uint HotKey::modifiers() const
{
	return m_modifiers;
}
#endif

HotKey HotKey::fromString(const QString& hotKeyString)
{
	HotKey hotKey;

	QStringList keyList = hotKeyString.split(KEY_SEPARATOR);

#ifdef Q_OS_LINUX
	QString keyString = keyList.takeLast();
	if(hotKey.m_keyMap.contains(keyString))
	{
		hotKey.m_key = hotKey.m_keyMap[keyString];
	}
	else
	{
		hotKey.setValid(false);
	}
#endif

	Q_FOREACH(QString key, keyList)
	{
#ifdef Q_OS_WINDOWS
		if(hotKey.m_keyMap.contains(key))
		{
			hotKey.m_keys.push_back(hotKey.m_keyMap[key]);
		}
		else
		{
			hotKey.setValid(false);
		}
#elif defined Q_OS_LINUX
		if(hotKey.m_keyMap.contains(key))
		{
			hotKey.m_modifiers |= hotKey.m_keyMap[key];
		}
		else
		{
			hotKey.setValid(false);
		}
#endif
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

#elif defined Q_OS_LINUX
	m_keyMap["A"]          = XK_A;
	m_keyMap["B"]          = XK_B;
	m_keyMap["C"]          = XK_C;
	m_keyMap["D"]          = XK_D;
	m_keyMap["E"]          = XK_E;
	m_keyMap["F"]          = XK_F;
	m_keyMap["G"]          = XK_G;
	m_keyMap["H"]          = XK_H;
	m_keyMap["I"]          = XK_I;
	m_keyMap["J"]          = XK_J;
	m_keyMap["K"]          = XK_K;
	m_keyMap["L"]          = XK_L;
	m_keyMap["M"]          = XK_M;
	m_keyMap["N"]          = XK_N;
	m_keyMap["O"]          = XK_O;
	m_keyMap["P"]          = XK_P;
	m_keyMap["Q"]          = XK_Q;
	m_keyMap["R"]          = XK_R;
	m_keyMap["S"]          = XK_S;
	m_keyMap["T"]          = XK_T;
	m_keyMap["U"]          = XK_U;
	m_keyMap["V"]          = XK_V;
	m_keyMap["W"]          = XK_W;
	m_keyMap["X"]          = XK_X;
	m_keyMap["Y"]          = XK_Y;
	m_keyMap["Z"]          = XK_Z;

	m_keyMap["0"]          = XK_0;
	m_keyMap["1"]          = XK_1;
	m_keyMap["2"]          = XK_2;
	m_keyMap["3"]          = XK_3;
	m_keyMap["4"]          = XK_4;
	m_keyMap["5"]          = XK_5;
	m_keyMap["6"]          = XK_6;
	m_keyMap["7"]          = XK_7;
	m_keyMap["8"]          = XK_8;
	m_keyMap["9"]          = XK_9;

	m_keyMap["Ctrl"]       = ControlMask;
	m_keyMap["Shift"]      = ShiftMask;
	m_keyMap["Alt"]        = Mod1Mask;
	m_keyMap["Meta"]       = Mod4Mask;
//	m_keyMap["Tab"]        = XK_Tab;
	m_keyMap["Print"]      = XK_Print;
	m_keyMap["Esc"]        = XK_Escape;
	m_keyMap["Space"]      = XK_space;
	m_keyMap["PgUp"]       = XK_KP_Page_Up;
	m_keyMap["PgDown"]     = XK_KP_Page_Down;
	m_keyMap["End"]        = XK_KP_End;
	m_keyMap["Home"]       = XK_KP_Home;
	m_keyMap["Left"]       = XK_KP_Left;
	m_keyMap["Up"]         = XK_KP_Up;
	m_keyMap["Right"]      = XK_KP_Right;
	m_keyMap["Down"]       = XK_KP_Down;
	m_keyMap["Ins"]        = XK_KP_Insert;
	m_keyMap["Del"]        = XK_Delete;
	m_keyMap["Backspace"]  = XK_BackSpace;
	m_keyMap["Return"]     = XK_Return;
	m_keyMap["Menu"]       = XK_Menu;

	m_keyMap["F1"]         = XK_F1;
	m_keyMap["F2"]         = XK_F2;
	m_keyMap["F3"]         = XK_F3;
	m_keyMap["F4"]         = XK_F4;
	m_keyMap["F5"]         = XK_F5;
	m_keyMap["F6"]         = XK_F6;
	m_keyMap["F7"]         = XK_F7;
	m_keyMap["F8"]         = XK_F8;
	m_keyMap["F9"]         = XK_F9;
	m_keyMap["F10"]        = XK_F10;
	m_keyMap["F11"]        = XK_F11;
	m_keyMap["F12"]        = XK_F12;
	m_keyMap["F13"]        = XK_F13;
	m_keyMap["F14"]        = XK_F14;
	m_keyMap["F15"]        = XK_F15;
	m_keyMap["F16"]        = XK_F16;
	m_keyMap["F17"]        = XK_F17;
	m_keyMap["F18"]        = XK_F18;
	m_keyMap["F19"]        = XK_F19;
	m_keyMap["F20"]        = XK_F20;
	m_keyMap["F21"]        = XK_F21;
	m_keyMap["F22"]        = XK_F22;
	m_keyMap["F23"]        = XK_F23;
	m_keyMap["F24"]        = XK_F24;


	m_keyMap["`"]          = XK_grave;
	m_keyMap[":"]          = XK_colon;
	m_keyMap[";"]          = XK_semicolon;
	m_keyMap["+"]          = XK_plus;
	m_keyMap[","]          = XK_comma;
	m_keyMap["-"]          = XK_minus;
	m_keyMap["."]          = XK_period;
	m_keyMap["/"]          = XK_slash;
	m_keyMap["?"]          = XK_question;

	m_keyMap["["]          = XK_bracketleft;
	m_keyMap["{"]          = XK_braceleft;
	m_keyMap["]"]          = XK_bracketright;
	m_keyMap["}"]          = XK_braceright;
	m_keyMap["\\"]         = XK_backslash;
	m_keyMap["|"]          = XK_bar;
	m_keyMap["'"]          = XK_apostrophe;
#endif
}

