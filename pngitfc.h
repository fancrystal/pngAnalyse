#pragma once
#include "stdafx.h"
#include <png.h>


enum VEPngReadFlag
{
	NORMAL,
	NOTPNG32,
	OPENERROR,
	OTHER
	
};

struct VSColor
{
	
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	
};

struct VSPngOutPut
{
	VEPngReadFlag flag; 
	VNVECTOR2I size;
	VSConstBuffer< VSColor > info;
};