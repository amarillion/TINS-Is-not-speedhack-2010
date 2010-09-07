#pragma once

enum Blending 
{
	Blending_None, 
	Blending_Add,
	Blending_Frozen,
	Blending_Lighten
};

extern void setBlending(Blending aBlending);

extern unsigned long add24(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long add16(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long add15(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long ice24(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long ice16(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long ice15(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long lighten24(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long lighten16(unsigned long x, unsigned long y, unsigned long n);

extern unsigned long lighten15(unsigned long x, unsigned long y, unsigned long n);