#ifndef _KEYPAD_ACTOR_H_
#define _KEYPAD_ACTOR_H_

#undef STDCALL
#undef IMPORT

#ifdef WIN32
#define STDCALL __stdcall 
#else
#define STDCALL
#endif

#ifndef EXPORT
	#ifdef WIN32
	#define IMPORT __declspec(dllimport)
	#else
	#define IMPORT
	#endif
#else
	#ifdef WIN32
	#define IMPORT EXPORT
	#else
	#define IMPORT __attribute__((visibility("default")))
	#endif
#endif


struct KeyIO
{
	enum keyin // KEYIN
	{
		BUTTON_A = 0x0001, 
		BUTTON_B = 0x0002,
		SELECT   = 0x0004,
		START    = 0x0008,
		RIGHT    = 0x0010,
		LEFT     = 0x0020,
		UP       = 0x0040,
		DOWN     = 0x0080,
		BUTTON_L = 0x0100,
		BUTTON_R = 0x0200
	};
	enum extkeyin // EXTKEYIN
	{
		BUTTON_X     = 0x01,
		BUTTON_Y     = 0x02,
		BUTTON_DEBUG = 0x04,
		PEN          = 0x40,
		HINGE        = 0x80
	};
};

struct Keys
{
	enum key {
		BUTTON_A = 0, 
		BUTTON_B = 1,
		SELECT   = 2,
		START    = 3,
		RIGHT    = 4,
		LEFT     = 5,
		UP       = 6,
		DOWN     = 7,
		BUTTON_L = 8,
		BUTTON_R = 9,
		BUTTON_X = 10,
		BUTTON_Y = 11,
		BUTTON_DEBUG = 12,
		PEN      = 13,
		HINGE    = 14
	};

	enum {
		TOUCH = 1 << 12
	};
};

extern "C"{
IMPORT void STDCALL TriggerKey(Keys::key key, bool down);
IMPORT void STDCALL TouchMove(int x, int y);
IMPORT void STDCALL InputInit2();
}

#endif
