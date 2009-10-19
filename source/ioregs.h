#ifndef _IOREGS_H_
#define _IOREGS_H_

struct keyio
{
	// uses Keys::key
	unsigned short keyinput; // 0x04000130
	unsigned short keycnt;   // 0x04000132
	unsigned short rcnt;     // 0x04000134
	unsigned short keyxy;    // 0x04000136
};


#endif
