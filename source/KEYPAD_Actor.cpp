// KEYPAD_Actor.cpp : Defines the entry point for the console application.
//

#ifdef WIN32
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#endif

#include <stdio.h>
#include <iostream>
#include <NDSE_SDK.h>
#include "ioregs.h"

#define EXPORT
#include "KEYPAD_Actor.h"

#undef D_INPUT

unsigned long g_kflags = 0x3FF;
unsigned long g_exkflags = 0x47;
keyio* regs_keyio9;
keyio* regs_keyio7;

#ifdef WIN32
// alternative input methods (winapi + dinput)
BYTE diks_a[256];  // DirectInput keyboard state buffer
BYTE diks_b[256];
BYTE* diks_f = diks_a;
BYTE* diks_s = diks_b;

HWND wnd = 0;

void swap_buffers()
{
	BYTE* tmp = diks_f;
	diks_f = diks_s;
	diks_s = tmp;
}

#ifdef D_INPUT
#include <dinput.h>
LPDIRECTINPUT8       g_pDI       = 0; // The DirectInput object         
LPDIRECTINPUTDEVICE8 g_pKeyboard = 0; // The keyboard device 

HRESULT init_dinput()
{
	HRESULT hr;
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, 0 ) ) )
        return hr;
	if( FAILED( hr = g_pDI->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
        return hr;
	if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;
	if( FAILED( hr = g_pKeyboard->SetCooperativeLevel( wnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
		return hr;
	g_pKeyboard->Acquire();
	return S_OK;
}
#endif


void init()
{
	memset( diks_a, 0, sizeof(diks_a) );
	memset( diks_b, 0, sizeof(diks_b) );
#ifdef D_INPUT
	HRESULT hr;
	if FAILED( hr = init_dinput() )
	{
		char buf[256];
		sprintf_s( buf, sizeof(buf), "Failed to intialize DirectInput: 0x%08X", hr);
		MessageBoxA( 0, buf, 0, MB_ICONERROR );
		exit(-1);
	}
#endif
}


int key_down(unsigned char key)
{
#ifdef D_INPUT
	return (diks[key] & 0x80) == 0x80;
#else
	//return ((diks_a[key] ^ diks_b[key]) & 0x80) != 0;
	//return (diks_s[key] & 0x80) != 0;
	return (GetAsyncKeyState( key ) & 0x8000) != 0;
#endif
}

void process_input()
{
	unsigned long kflags = 0x3FF; //keyio::START | keyio::SELECT;
	//static unsigned long kflags = 0;
#ifdef D_INPUT
	if (key_down(203))
		kflags ^= KeyIO::LEFT;
	if (key_down(205))
		kflags ^= KeyIO::RIGHT;
	if (key_down(200))
		kflags ^= KeyIO::UP;
	if (key_down(208))
		kflags ^= KeyIO::DOWN;
	if (key_down(16))
		kflags ^= KeyIO::BUTTON_L;
	if (key_down(17))
		kflags ^= KeyIO::BUTTON_R;
	if (key_down(30))
		kflags ^= KeyIO::BUTTON_A;
	if (key_down(31))
		kflags ^= KeyIO::BUTTON_B;
	if (key_down(28))
		kflags ^= KeyIO::START;
	if (key_down(14))
		kflags ^= KeyIO::SELECT;
#else
	if (/*wnd &&*/ (wnd != GetForegroundWindow()))
		return;

	if (key_down(37))
		kflags ^= KeyIO::LEFT;
	if (key_down(39))
		kflags ^= KeyIO::RIGHT;
	if (key_down(38))
		kflags ^= KeyIO::UP;
	if (key_down(40))
		kflags ^= KeyIO::DOWN;
	if (key_down(81))
		kflags ^= KeyIO::BUTTON_L;
	if (key_down(87))
		kflags ^= KeyIO::BUTTON_R;
	if (key_down(65))
		kflags ^= KeyIO::BUTTON_A;
	if (key_down(83))
		kflags ^= KeyIO::BUTTON_B;
	if (key_down(13))
		kflags ^= KeyIO::START;
	if (key_down(8))
		kflags ^= KeyIO::SELECT;
#endif

#ifdef _DEBUG
	for (int i = 0; i < 255; i++)
	{
		static unsigned long lastflags;
		static char tmp[256];
		//if (diks[i] & 0x80)
		/*
		if (key_down(i))
		{
			sprintf_s(tmp, sizeof(tmp), "Key down: %i\n", i);
			OutputDebugStringA(tmp);
		}
		if (kflags != lastflags)
		{
			lastflags = kflags;
			sprintf_s(tmp, sizeof(tmp), "KFlags: %04X\n", kflags);
			OutputDebugStringA(tmp);
		}*/
	}
#endif
	

	regs_keyio7->keyinput = (unsigned short)kflags;
	regs_keyio9->keyinput = (unsigned short)kflags;
}

void read_state()
{
	for (int i = 0; i < 256; i++)
		diks_f[i] = GetAsyncKeyState(i) >> 8;
}

void update_input()
{
#ifdef D_INPUT
	HRESULT hr;
	hr = g_pKeyboard->GetDeviceState( sizeof(diks_a), diks_f );
	if( FAILED(hr) ) 
    {
		regs_keyio->keyinput = keyio::START | keyio::SELECT;
		hr = g_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pKeyboard->Acquire();
		return; 
	}
	swap_buffers();
#else
	//read_state();
	swap_buffers();
#endif
	process_input();
}

#endif // WIN32


void TriggerKey0(KeyIO::keyin key, bool down)
{
	if (down)
		g_kflags &= ~key;
	else g_kflags |= key;
	regs_keyio7->keyinput = (unsigned short)g_kflags;
	regs_keyio9->keyinput = (unsigned short)g_kflags;
}

void TriggerKey1(KeyIO::extkeyin key, bool down)
{
	if (down)
		g_exkflags &= ~key;
	else g_exkflags |= key;
	regs_keyio7->keyxy = (unsigned short)g_exkflags;
	regs_keyio9->keyxy = (unsigned short)g_exkflags;

}

void STDCALL TriggerKey(Keys::key key, bool down)
{
	// careful this is _not_ threadsafe

	// down = 1 => bit = 0
	// down = 0 => bit = 1
	

	switch (key)
	{
	case Keys::BUTTON_A: return TriggerKey0(KeyIO::BUTTON_A, down);
	case Keys::BUTTON_B: return TriggerKey0(KeyIO::BUTTON_B, down);
	case Keys::SELECT:   return TriggerKey0(KeyIO::SELECT, down);
	case Keys::START:    return TriggerKey0(KeyIO::START, down);
	case Keys::RIGHT:    return TriggerKey0(KeyIO::RIGHT, down);
	case Keys::LEFT:     return TriggerKey0(KeyIO::LEFT, down);
	case Keys::UP:       return TriggerKey0(KeyIO::UP, down);
	case Keys::DOWN:     return TriggerKey0(KeyIO::DOWN, down);
	case Keys::BUTTON_L: return TriggerKey0(KeyIO::BUTTON_L, down);
	case Keys::BUTTON_R: return TriggerKey0(KeyIO::BUTTON_R, down);
	case Keys::BUTTON_X: return TriggerKey1(KeyIO::BUTTON_X, down);
	case Keys::BUTTON_Y: return TriggerKey1(KeyIO::BUTTON_Y, down);
	case Keys::BUTTON_DEBUG: return TriggerKey1(KeyIO::BUTTON_DEBUG, down);
	case Keys::PEN:      return TriggerKey1(KeyIO::PEN, down);
	case Keys::HINGE:    return TriggerKey1(KeyIO::HINGE, down);
	}
}

void STDCALL TouchMove(int x, int y)
{
	TouchSet(x, y);
}

IMPORT void STDCALL InputInit2()
{
#ifdef WIN32
	init();
#endif
	regs_keyio7 = reinterpret_cast<keyio*>(ARM7_GetPage(0x04000130)->mem + (0x4000130 & PAGE_MASK));
	regs_keyio9 = reinterpret_cast<keyio*>(ARM9_GetPage(0x04000130)->mem + (0x4000130 & PAGE_MASK));
	regs_keyio7->keyinput = (unsigned short)g_kflags;
	regs_keyio9->keyinput = (unsigned short)g_kflags;
	regs_keyio7->keyxy = (unsigned short)g_exkflags;
	regs_keyio9->keyxy = (unsigned short)g_exkflags;
}
