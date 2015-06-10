// MakeQCT.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include "OziGen.h"
#include "OziGenDlg.h"

#include <windows.h>
#include <stdio.h>
#include "MakeQCT.h"

#include "IGN.h"
#include "OSPro.h"
#include "NGI.h"

#include "TileDatabase.h"
#include "Database.h"

#include <io.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../MapLib/zconf.h"
#include "../MapLib/zlib.h"
#include "../MapLib/palette.h"
#include "qct.h"
#include "qctcoef.h"
#include "../MapLib/mmmath.h"

#include "../MapLib/Lambert93.h"


#include "ProgressWindow.h"
#include "func.h"
#include "CustomMap.h"

extern CCustomMap MyMap;

extern CoordinateSystems g_CoordinateSystem;

unsigned char MakeQCT_ign_palette[1024] = {
	0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0x00, 0xFE, 0xFF, 0xFE, 0x32, 0x00, 0xFF, 0xFE, 0x7F, 0x00, 0xFF, 
	0xFE, 0xD8, 0xB2, 0xFF, 0xFE, 0xFE, 0x00, 0xFF, 0xFE, 0xFE, 0x65, 0xFF, 0x00, 0xFE, 0x00, 0xFF, 
	0x4B, 0xFE, 0x65, 0xFF, 0x00, 0xFE, 0x54, 0xFF, 0x3F, 0xFE, 0x7F, 0xFF, 0xB9, 0xFE, 0xD0, 0xFF, 
	0xD0, 0xFE, 0xDF, 0xFF, 0x00, 0xFE, 0xFE, 0xFF, 0x4B, 0x98, 0xFE, 0xFF, 0x7F, 0x98, 0xE5, 0xFF, 
	0x32, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0xF8, 0xF8, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 
	0xEC, 0xEC, 0xEC, 0xFF, 0xE6, 0xE6, 0xE6, 0xFF, 0xE0, 0xE0, 0xE0, 0xFF, 0xDA, 0xDA, 0xDA, 0xFF, 
	0xD4, 0xD4, 0xD4, 0xFF, 0xCE, 0xCE, 0xCE, 0xFF, 0xC8, 0xC8, 0xC8, 0xFF, 0xC2, 0xC2, 0xC2, 0xFF, 
	0xBC, 0xBC, 0xBC, 0xFF, 0xB6, 0xB6, 0xB6, 0xFF, 0xB0, 0xB0, 0xB0, 0xFF, 0xAA, 0xAA, 0xAA, 0xFF, 
	0xA4, 0xA4, 0xA4, 0xFF, 0x62, 0xA7, 0x79, 0xFF, 0x67, 0xAC, 0x7E, 0xFF, 0x6C, 0xB1, 0x83, 0xFF, 
	0x74, 0xB9, 0x8B, 0xFF, 0x79, 0xBE, 0x90, 0xFF, 0x7E, 0xC3, 0x95, 0xFF, 0x83, 0xC8, 0x9A, 0xFF, 
	0x88, 0xCD, 0x9F, 0xFF, 0x90, 0xD5, 0xA7, 0xFF, 0x95, 0xDA, 0xAC, 0xFF, 0x9A, 0xDF, 0xB1, 0xFF, 
	0xA2, 0xE7, 0xB9, 0xFF, 0xA7, 0xEC, 0xBE, 0xFF, 0xAC, 0xF1, 0xC3, 0xFF, 0xB4, 0xF9, 0xCB, 0xFF, 
	0x77, 0xA5, 0x86, 0xFF, 0x7D, 0xAB, 0x8C, 0xFF, 0x83, 0xB1, 0x92, 0xFF, 0x89, 0xB7, 0x98, 0xFF, 
	0x8F, 0xBD, 0x9E, 0xFF, 0x95, 0xC3, 0xA4, 0xFF, 0x9B, 0xC9, 0xAA, 0xFF, 0xA1, 0xCF, 0xB0, 0xFF, 
	0xA7, 0xD5, 0xB6, 0xFF, 0xAD, 0xDB, 0xBC, 0xFF, 0xB3, 0xE1, 0xC2, 0xFF, 0xB9, 0xE7, 0xC8, 0xFF, 
	0xBF, 0xED, 0xCE, 0xFF, 0xC5, 0xF3, 0xD5, 0xFF, 0xCB, 0xF8, 0xDA, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 
	0x07, 0x07, 0x07, 0xFF, 0x0F, 0x0F, 0x30, 0xFF, 0x11, 0x22, 0x11, 0xFF, 0x18, 0x28, 0x30, 0xFF, 
	0x28, 0x28, 0x28, 0xFF, 0x39, 0x0F, 0x07, 0xFF, 0x30, 0x07, 0x30, 0xFF, 0x07, 0x30, 0x51, 0xFF, 
	0x28, 0x41, 0x39, 0xFF, 0x30, 0x72, 0x6A, 0xFF, 0x7A, 0x49, 0x0F, 0xFF, 0x59, 0x59, 0x59, 0xFF, 
	0x49, 0x51, 0x62, 0xFF, 0x49, 0x6A, 0x51, 0xFF, 0x51, 0x72, 0x6A, 0xFF, 0x7A, 0x7A, 0x7A, 0xFF, 
	0x59, 0x30, 0x07, 0xFF, 0x49, 0x07, 0x59, 0xFF, 0x62, 0x7A, 0xC4, 0xFF, 0x51, 0xF6, 0x6A, 0xFF, 
	0x7A, 0xB4, 0x8B, 0xFF, 0xF6, 0x83, 0x0F, 0xFF, 0x92, 0x93, 0x93, 0xFF, 0x93, 0xA4, 0xD5, 0xFF, 
	0x8B, 0xC4, 0x93, 0xFF, 0x91, 0xD5, 0xA4, 0xFF, 0x8B, 0xF6, 0x93, 0xFF, 0x93, 0xE5, 0xB4, 0xFF, 
	0xA4, 0xF6, 0xB4, 0xFF, 0xB9, 0xCC, 0x83, 0xFF, 0x95, 0xD6, 0xC6, 0xFF, 0xAB, 0xB5, 0xE6, 0xFF, 
	0x93, 0xE5, 0xC4, 0xFF, 0xB4, 0xF6, 0xC4, 0xFF, 0xC1, 0xF6, 0xE5, 0xFF, 0xC3, 0xCD, 0xDF, 0xFF, 
	0xB4, 0xC4, 0xE5, 0xFF, 0xF6, 0xD5, 0xB4, 0xFF, 0xBC, 0x07, 0xCD, 0xFF, 0xD5, 0xD5, 0xE5, 0xFF, 
	0xD5, 0xF2, 0xD9, 0xFF, 0xD5, 0xF6, 0xE5, 0xFF, 0xF6, 0xE8, 0xE2, 0xFF, 0xE5, 0xE5, 0xF6, 0xFF, 
	0xEB, 0xD7, 0xCB, 0xFF, 0xDD, 0xBC, 0x9B, 0xFF, 0xF6, 0x9B, 0xF6, 0xFF, 0xE5, 0x62, 0x0F, 0xFF, 
	0xF6, 0x07, 0xF6, 0xFF, 0xB4, 0xF6, 0xD5, 0xFF, 0x0F, 0x9B, 0xCD, 0xFF, 0xE5, 0xF6, 0xF6, 0xFF, 
	0x30, 0x07, 0xEE, 0xFF, 0xD5, 0xDD, 0xF6, 0xFF, 0xE6, 0xF5, 0xE6, 0xFF, 0xB4, 0x83, 0x0F, 0xFF, 
	0x07, 0xB4, 0x0F, 0xFF, 0x09, 0x07, 0x16, 0xFF, 0x18, 0x07, 0x18, 0xFF, 0x07, 0x18, 0x18, 0xFF, 
	0x18, 0x18, 0x18, 0xFF, 0x07, 0x62, 0x18, 0xFF, 0xF6, 0xBC, 0xF6, 0xFF, 0xC4, 0xD2, 0xF6, 0xFF, 
	0x0F, 0xEE, 0x20, 0xFF, 0x29, 0x38, 0x28, 0xFF, 0x93, 0x83, 0x28, 0xFF, 0x0F, 0x8B, 0x28, 0xFF, 
	0x62, 0x41, 0x30, 0xFF, 0x39, 0x39, 0x39, 0xFF, 0x9B, 0x51, 0x39, 0xFF, 0x72, 0x62, 0x39, 0xFF, 
	0xBC, 0x9B, 0x39, 0xFF, 0x20, 0xBC, 0x39, 0xFF, 0x28, 0x28, 0x41, 0xFF, 0x41, 0x59, 0x41, 0xFF, 
	0xF6, 0x9B, 0x41, 0xFF, 0x49, 0x49, 0x49, 0xFF, 0x30, 0x51, 0x49, 0xFF, 0x30, 0x6A, 0x49, 0xFF, 
	0xCD, 0x7A, 0x49, 0xFF, 0x30, 0xEE, 0x49, 0xFF, 0x93, 0xAC, 0x51, 0xFF, 0x0F, 0xF6, 0x51, 0xFF, 
	0x18, 0x49, 0x59, 0xFF, 0x30, 0x93, 0x59, 0xFF, 0xB4, 0xB4, 0x59, 0xFF, 0x41, 0xBC, 0x59, 0xFF, 
	0x30, 0x41, 0x62, 0xFF, 0x9B, 0x72, 0x62, 0xFF, 0x6A, 0x83, 0x62, 0xFF, 0x62, 0x30, 0x6A, 0xFF, 
	0x0F, 0x51, 0x6A, 0xFF, 0x6A, 0x6A, 0x6A, 0xFF, 0xE5, 0x9B, 0x6A, 0xFF, 0x30, 0xF6, 0x6A, 0xFF, 
	0x51, 0x8B, 0x72, 0xFF, 0x51, 0xAC, 0x72, 0xFF, 0xF6, 0xB4, 0x72, 0xFF, 0x51, 0xCD, 0x72, 0xFF, 
	0x6A, 0x93, 0x7A, 0xFF, 0xBC, 0x93, 0x7A, 0xFF, 0x8B, 0x94, 0xD5, 0xFF, 0x6A, 0x07, 0x83, 0xFF, 
	0x59, 0x62, 0x83, 0xFF, 0x0F, 0x6A, 0x83, 0xFF, 0x6A, 0x7A, 0x83, 0xFF, 0x8B, 0x9B, 0x83, 0xFF, 
	0x18, 0x0F, 0x8B, 0xFF, 0x19, 0x32, 0x7A, 0xFF, 0x41, 0x59, 0x8B, 0xFF, 0x8B, 0x8B, 0x8B, 0xFF, 
	0x49, 0x93, 0x8B, 0xFF, 0x7A, 0x93, 0x8B, 0xFF, 0x6A, 0xA4, 0x8B, 0xFF, 0x6A, 0xB4, 0x8C, 0xFF, 
	0x72, 0xE5, 0x8B, 0xFF, 0x51, 0xF6, 0x8B, 0xFF, 0x83, 0x51, 0x93, 0xFF, 0x20, 0x7A, 0x93, 0xFF, 
	0x58, 0xA6, 0xF6, 0xFF, 0x51, 0xAC, 0x93, 0xFF, 0xAC, 0x49, 0x07, 0xFF, 0x8B, 0xB4, 0x93, 0xFF, 
	0x7A, 0xC4, 0x93, 0xFF, 0xF6, 0xC4, 0x93, 0xFF, 0x62, 0x49, 0xEE, 0xFF, 0xB4, 0xDD, 0x93, 0xFF, 
	0x6A, 0xF6, 0x93, 0xFF, 0x6A, 0x72, 0x9B, 0xFF, 0x8B, 0x83, 0x9B, 0xFF, 0x30, 0x8B, 0x9B, 0xFF, 
	0x6A, 0xA4, 0x9B, 0xFF, 0x7A, 0xA4, 0x9B, 0xFF, 0x7A, 0xB4, 0x9B, 0xFF, 0x62, 0xCD, 0x9B, 0xFF, 
	0xE5, 0xBC, 0xE5, 0xFF, 0x93, 0x07, 0xA4, 0xFF, 0x51, 0x6A, 0xA4, 0xFF, 0xBC, 0xA4, 0xA4, 0xFF, 
	0xA4, 0xB4, 0xA4, 0xFF, 0xBC, 0xB4, 0xA4, 0xFF, 0x8B, 0xC4, 0xA4, 0xFF, 0xF6, 0x59, 0xF6, 0xFF, 
	0x07, 0xF6, 0xF6, 0xFF, 0xF6, 0xD5, 0xA4, 0xFF, 0x8E, 0xE5, 0xA4, 0xFF, 0xE5, 0x20, 0x0F, 0xFF, 
	0xA4, 0xE5, 0xA4, 0xFF, 0xB4, 0xE5, 0xA3, 0xFF, 0x7A, 0xF6, 0xA4, 0xFF, 0x8C, 0xF6, 0xA4, 0xFF, 
	0x83, 0x18, 0x07, 0xFF, 0xB3, 0xE7, 0xD5, 0xFF, 0xF6, 0x6A, 0xAC, 0xFF, 0x49, 0x93, 0xAC, 0xFF, 
	0x93, 0x93, 0xAC, 0xFF, 0x51, 0xAC, 0xAC, 0xFF, 0x93, 0xAC, 0xAC, 0xFF, 0x6A, 0x0F, 0xB4, 0xFF, 
	0xB7, 0x39, 0xC7, 0xFF, 0x72, 0x8B, 0xC5, 0xFF, 0x62, 0xB4, 0xB4, 0xFF, 0xA4, 0xB4, 0xB4, 0xFF, 
	0x7A, 0xC4, 0xB4, 0xFF, 0xCD, 0xC5, 0xB4, 0xFF, 0xEE, 0xC4, 0xB4, 0xFF, 0x90, 0xD5, 0xB4, 0xFF, 
	0xF6, 0xF6, 0x07, 0xFF, 0xE5, 0xD5, 0xB4, 0xFF, 0x83, 0xE5, 0xB4, 0xFF, 0xB4, 0xE5, 0xB4, 0xFF, 
	0xCD, 0xE5, 0xB4, 0xFF, 0xF6, 0xE5, 0xB4, 0xFF, 0x92, 0xF6, 0xB4, 0xFF, 0x59, 0x93, 0x18, 0xFF, 
	0xB4, 0xF6, 0xB4, 0xFF, 0xB8, 0x6E, 0xC9, 0xFF, 0x3E, 0x7C, 0xCE, 0xFF, 0xA4, 0xA4, 0xBC, 0xFF, 
	0x8B, 0xC4, 0xBC, 0xFF, 0xBC, 0x9B, 0xC4, 0xFF, 0x78, 0xA9, 0xC8, 0xFF, 0xAA, 0xB6, 0xD3, 0xFF, 
	0xD5, 0xC4, 0xC4, 0xFF, 0xC4, 0xDF, 0xCB, 0xFF, 0xD5, 0xDB, 0xCA, 0xFF, 0xA4, 0xE5, 0xC4, 0xFF, 
	0x51, 0xEE, 0xEE, 0xFF, 0xC4, 0xE5, 0xEA, 0xFF, 0xA4, 0xF6, 0xC4, 0xFF, 0xC4, 0xF6, 0xC4, 0xFF
};


#define	TILE_WIDTH	400

unsigned char mm_ign_palette[1024] = {
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFE, 0xFE, 0xFE, 0x00, 0xF8, 0xF8, 0xF8, 0x00, 
	0xB9, 0xFE, 0xD0, 0x00, 0xF2, 0xF2, 0xF2, 0x00, 0xEC, 0xEC, 0xEC, 0x00, 0xB4, 0xF9, 0xCB, 0x00, 
	0xE6, 0xE6, 0xE6, 0x00, 0xFE, 0xD8, 0xB2, 0x00, 0xE0, 0xE0, 0xE0, 0x00, 0xDA, 0xDA, 0xDA, 0x00, 
	0xD4, 0xD4, 0xD4, 0x00, 0xA4, 0xA4, 0xA4, 0x00, 0xAC, 0xF1, 0xC3, 0x00, 0xCE, 0xCE, 0xCE, 0x00, 
	0xC8, 0xC8, 0xC8, 0x00, 0xC2, 0xC2, 0xC2, 0x00, 0xBC, 0xBC, 0xBC, 0x00, 0xB6, 0xB6, 0xB6, 0x00, 
	0xAA, 0xAA, 0xAA, 0x00, 0xB0, 0xB0, 0xB0, 0x00, 0xA2, 0xE7, 0xB9, 0x00, 0x9A, 0xDF, 0xB1, 0x00, 
	0xA7, 0xEC, 0xBE, 0x00, 0x93, 0xA4, 0xD5, 0x00, 0x72, 0x8B, 0xC5, 0x00, 0x07, 0x07, 0x07, 0x00, 
	0x18, 0x18, 0x18, 0x00, 0x41, 0x59, 0x8B, 0x00, 0x28, 0x28, 0x28, 0x00, 0x39, 0x39, 0x39, 0x00, 
	0x62, 0xA7, 0x79, 0x00, 0x59, 0x59, 0x59, 0x00, 0x6A, 0x6A, 0x6A, 0x00, 0x62, 0x7A, 0xC4, 0x00, 
	0x88, 0xCD, 0x9F, 0x00, 0x92, 0x93, 0x93, 0x00, 0x7A, 0xC4, 0xB4, 0x00, 0x49, 0x49, 0x49, 0x00, 
	0xB4, 0xF6, 0xC4, 0x00, 0x30, 0x8B, 0x9B, 0x00, 0x8B, 0x94, 0xD5, 0x00, 0x6A, 0x72, 0x9B, 0x00, 
	0x20, 0x7A, 0x93, 0x00, 0xC3, 0xCD, 0xDF, 0x00, 0xB4, 0xC4, 0xE5, 0x00, 0xE5, 0xE5, 0xF6, 0x00, 
	0x95, 0xDA, 0xAC, 0x00, 0x51, 0x6A, 0xA4, 0x00, 0x90, 0xD5, 0xA7, 0x00, 0xAA, 0xB6, 0xD3, 0x00, 
	0x6C, 0xB1, 0x83, 0x00, 0x62, 0xB4, 0xB4, 0x00, 0x93, 0x93, 0xAC, 0x00, 0xAB, 0xB5, 0xE6, 0x00, 
	0x19, 0x32, 0x7A, 0x00, 0x83, 0xC8, 0x9A, 0x00, 0x7A, 0x7A, 0x7A, 0x00, 0x30, 0x72, 0x6A, 0x00, 
	0x74, 0xB9, 0x8B, 0x00, 0x51, 0xAC, 0xAC, 0x00, 0xD5, 0xD5, 0xE5, 0x00, 0x49, 0x93, 0xAC, 0x00, 
	0xFE, 0x00, 0xFE, 0x00, 0xFE, 0x32, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 
	0x32, 0x00, 0xFE, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0xF6, 0x9B, 0x41, 0x00, 0x93, 0x07, 0xA4, 0x00, 
	0xF6, 0x6A, 0xAC, 0x00, 0x83, 0x18, 0x07, 0x00, 0x30, 0xF6, 0x6A, 0x00, 0x59, 0x93, 0x18, 0x00, 
	0xB4, 0x83, 0x0F, 0x00, 0xFE, 0xFE, 0x65, 0x00, 0x51, 0xEE, 0xEE, 0x00, 0x20, 0xBC, 0x39, 0x00, 
	0xF6, 0x59, 0xF6, 0x00, 0xB4, 0xB4, 0x59, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xB7, 0x39, 0xC7, 0x00, 
	0x9B, 0x51, 0x39, 0x00, 0x4B, 0x98, 0xFE, 0x00, 0x07, 0x62, 0x18, 0x00, 0xF6, 0x9B, 0xF6, 0x00, 
	0x62, 0x49, 0xEE, 0x00, 0x0F, 0x9B, 0xCD, 0x00, 0x49, 0x07, 0x59, 0x00, 0xF6, 0xB4, 0x72, 0x00, 
	0xAC, 0x49, 0x07, 0x00, 0x00, 0xFE, 0x54, 0x00, 0xCD, 0x7A, 0x49, 0x00, 0xB8, 0x6E, 0xC9, 0x00, 
	0x6A, 0x07, 0x83, 0x00, 0x6A, 0xF6, 0x93, 0x00, 0xBC, 0x07, 0xCD, 0x00, 0x7A, 0x49, 0x0F, 0x00, 
	0x07, 0xB4, 0x0F, 0x00, 0xB9, 0xCC, 0x83, 0x00, 0xBC, 0x93, 0x7A, 0x00, 0xDD, 0xBC, 0x9B, 0x00, 
	0x41, 0xBC, 0x59, 0x00, 0x62, 0x30, 0x6A, 0x00, 0x07, 0x30, 0x51, 0x00, 0x0F, 0x8B, 0x28, 0x00, 
	0x6A, 0x0F, 0xB4, 0x00, 0x59, 0x30, 0x07, 0x00, 0x83, 0x51, 0x93, 0x00, 0x72, 0x62, 0x39, 0x00, 
	0x9B, 0x72, 0x62, 0x00, 0x93, 0x83, 0x28, 0x00, 0xE5, 0x62, 0x0F, 0x00, 0xBC, 0x9B, 0x39, 0x00, 
	0x18, 0x0F, 0x8B, 0x00, 0x0F, 0xEE, 0x20, 0x00, 0x39, 0x0F, 0x07, 0x00, 0x3E, 0x7C, 0xCE, 0x00, 
	0x30, 0x93, 0x59, 0x00, 0x0F, 0x51, 0x6A, 0x00, 0x62, 0x41, 0x30, 0x00, 0x30, 0x41, 0x62, 0x00, 
	0x30, 0x07, 0x30, 0x00, 0x93, 0xAC, 0x51, 0x00, 0xE5, 0x20, 0x0F, 0x00, 0x51, 0xCD, 0x72, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char mm_osgb_palette[1024] = {
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xF8, 0xE8, 0xD8, 0x00, 0xE8, 0xF8, 0xF8, 0x00, 
	0xA4, 0xE8, 0xE3, 0x00, 0xD8, 0xE8, 0xE8, 0x00, 0xF8, 0xF8, 0xF8, 0x00, 0x08, 0x08, 0x08, 0x00, 
	0xF8, 0xD8, 0xB4, 0x00, 0xA4, 0xD8, 0xF8, 0x00, 0xF8, 0xF8, 0xE8, 0x00, 0xD8, 0xE8, 0xD8, 0x00, 
	0xF7, 0xCB, 0x8C, 0x00, 0xE8, 0xB8, 0x08, 0x00, 0x85, 0x88, 0x88, 0x00, 0xC7, 0xC8, 0xC8, 0x00, 
	0xE8, 0xE8, 0xE8, 0x00, 0x98, 0x98, 0x98, 0x00, 0x47, 0x48, 0x48, 0x00, 0x68, 0x98, 0x08, 0x00, 
	0xE8, 0xF8, 0xE8, 0x00, 0xD8, 0xF8, 0xE8, 0x00, 0x58, 0x58, 0x58, 0x00, 0xA7, 0xD2, 0xA0, 0x00, 
	0xC8, 0xE8, 0xF8, 0x00, 0x70, 0xAE, 0xD0, 0x00, 0x18, 0x18, 0x18, 0x00, 0x78, 0x78, 0x7A, 0x00, 
	0xB8, 0xB8, 0xB8, 0x00, 0xB8, 0xD8, 0xF8, 0x00, 0x87, 0xB8, 0xF8, 0x00, 0x93, 0xC9, 0xF8, 0x00, 
	0x77, 0xA8, 0xF8, 0x00, 0xC8, 0xE8, 0xD8, 0x00, 0x28, 0x28, 0x28, 0x00, 0x08, 0xF8, 0xF8, 0x00, 
	0x94, 0xC8, 0x81, 0x00, 0x38, 0x38, 0x38, 0x00, 0x95, 0xC8, 0xCC, 0x00, 0xB8, 0xE8, 0xF8, 0x00, 
	0x68, 0x68, 0x68, 0x00, 0x4F, 0x96, 0xED, 0x00, 0xA8, 0xA8, 0xA8, 0x00, 0xD8, 0xD8, 0xD8, 0x00, 
	0x7C, 0xE0, 0xDF, 0x00, 0x84, 0xB9, 0xE8, 0x00, 0xBD, 0xD8, 0xBE, 0x00, 0xA8, 0xC8, 0xF8, 0x00, 
	0xF6, 0xC8, 0x6C, 0x00, 0xD8, 0xE8, 0xF8, 0x00, 0x54, 0xA9, 0xF6, 0x00, 0x69, 0xAA, 0x09, 0x00, 
	0x08, 0xC8, 0xF8, 0x00, 0x78, 0x08, 0xE8, 0x00, 0xC8, 0xE8, 0xC8, 0x00, 0xD9, 0xD7, 0xA0, 0x00, 
	0x67, 0x88, 0xDE, 0x00, 0xD8, 0xF8, 0xF8, 0x00, 0x88, 0xBE, 0x6A, 0x00, 0x6D, 0xA4, 0xAA, 0x00, 
	0xF8, 0xE8, 0xC2, 0x00, 0xA8, 0xC8, 0xD8, 0x00, 0x7A, 0xB6, 0x59, 0x00, 0xCB, 0xC7, 0x78, 0x00, 
	0x53, 0x32, 0xA5, 0x00, 0x1D, 0x95, 0x97, 0x00, 0x3F, 0x09, 0x71, 0x00, 0xA9, 0x8B, 0x3F, 0x00, 
	0x73, 0x18, 0x3B, 0x00, 0x3A, 0xC0, 0x6E, 0x00, 0x38, 0x09, 0xDA, 0x00, 0x24, 0xD0, 0xAD, 0x00, 
	0xA3, 0x78, 0xEA, 0x00, 0x18, 0x78, 0xE7, 0x00, 0x2F, 0x5D, 0x8A, 0x00, 0x87, 0x41, 0xE5, 0x00, 
	0x45, 0xE8, 0xD8, 0x00, 0xBA, 0x99, 0x08, 0x00, 0x7F, 0x67, 0x0D, 0x00, 0xC8, 0xB7, 0x43, 0x00, 
	0x34, 0x51, 0x09, 0x00, 0x78, 0x54, 0xB3, 0x00, 0x3C, 0x7E, 0xB8, 0x00, 0x6C, 0x09, 0xA9, 0x00, 
	0x0F, 0xB2, 0xCF, 0x00, 0x50, 0x7F, 0x3F, 0x00, 0xB7, 0x9E, 0x65, 0x00, 0x95, 0x82, 0xC2, 0x00, 
	0x5D, 0x1D, 0xCE, 0x00, 0x47, 0xCC, 0x95, 0x00, 0x63, 0x76, 0x9F, 0x00, 0x49, 0xAD, 0xC2, 0x00, 
	0x69, 0xA7, 0x37, 0x00, 0x8B, 0x7A, 0x54, 0x00, 0x5A, 0x8D, 0x6B, 0x00, 0x68, 0x59, 0xE6, 0x00, 
	0x61, 0xD0, 0xF6, 0x00, 0x96, 0x68, 0x96, 0x00, 0xEB, 0xB8, 0x33, 0x00, 0x37, 0xF8, 0xF8, 0x00, 
	0x45, 0x72, 0x08, 0x00, 0xC1, 0xAD, 0xF6, 0x00, 0x6A, 0x58, 0x89, 0x00, 0x66, 0xF4, 0xF8, 0x00, 
	0x0C, 0xD5, 0xCC, 0x00, 0x40, 0xC8, 0xF5, 0x00, 0x90, 0xF8, 0xF8, 0x00, 0xD8, 0xC7, 0xF8, 0x00, 
	0x58, 0x08, 0xE8, 0x00, 0x2E, 0x8F, 0xE9, 0x00, 0x66, 0x82, 0xBC, 0x00, 0x20, 0xC8, 0xE4, 0x00, 
	0x78, 0x3E, 0xC9, 0x00, 0xC1, 0x9E, 0x32, 0x00, 0x49, 0xCA, 0xCD, 0x00, 0xB2, 0x92, 0xF5, 0x00, 
	0xA5, 0x83, 0x0A, 0x00, 0x4E, 0x6D, 0x8D, 0x00, 0x4B, 0x94, 0xAD, 0x00, 0x8B, 0x96, 0xEF, 0x00, 
	0x4D, 0x58, 0x72, 0x00, 0x89, 0x98, 0xCF, 0x00, 0x65, 0x94, 0x52, 0x00, 0x37, 0x35, 0x09, 0x00, 
	0x56, 0x8E, 0x28, 0x00, 0x5F, 0x58, 0x3D, 0x00, 0xBE, 0xB1, 0x83, 0x00, 0x35, 0x48, 0x5D, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
};

int QCT_NGI_read_bmp(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width, int nMapScale)
{
long tilewidth;

	if (nMapScale == MAP_SCALE_50K) {
		NGI_read_png_10K(y*1000, x*1000, db, bitmap_offset + /*TILE_WIDTH/2*/0*bitmap_width, bitmap_width);
		tilewidth = 200;
	} else if (nMapScale == MAP_SCALE_10K) {

		NGI_read_png_10K(y*1000, x*1000, db, bitmap_offset+750*bitmap_width, bitmap_width);
		NGI_read_png_10K(y*1000, x*1000+250, db, bitmap_offset+750*bitmap_width+250, bitmap_width);
		NGI_read_png_10K(y*1000, x*1000+500, db, bitmap_offset+750*bitmap_width+500, bitmap_width);
		NGI_read_png_10K(y*1000, x*1000+750, db, bitmap_offset+750*bitmap_width+750, bitmap_width);

		NGI_read_png_10K(y*1000+250, x*1000, db, bitmap_offset+500*bitmap_width, bitmap_width);
		NGI_read_png_10K(y*1000+250, x*1000+250, db, bitmap_offset+500*bitmap_width+250, bitmap_width);
		NGI_read_png_10K(y*1000+250, x*1000+500, db, bitmap_offset+500*bitmap_width+500, bitmap_width);
		NGI_read_png_10K(y*1000+250, x*1000+750, db, bitmap_offset+500*bitmap_width+750, bitmap_width);

		NGI_read_png_10K(y*1000+500, x*1000, db, bitmap_offset+250*bitmap_width, bitmap_width);
		NGI_read_png_10K(y*1000+500, x*1000+250, db, bitmap_offset+250*bitmap_width+250, bitmap_width);
		NGI_read_png_10K(y*1000+500, x*1000+500, db, bitmap_offset+250*bitmap_width+500, bitmap_width);
		NGI_read_png_10K(y*1000+500, x*1000+750, db, bitmap_offset+250*bitmap_width+750, bitmap_width);

		NGI_read_png_10K(y*1000+750, x*1000, db, bitmap_offset+0*bitmap_width, bitmap_width);
		NGI_read_png_10K(y*1000+750, x*1000+250, db, bitmap_offset+0*bitmap_width+250, bitmap_width);
		NGI_read_png_10K(y*1000+750, x*1000+500, db, bitmap_offset+0*bitmap_width+500, bitmap_width);
		NGI_read_png_10K(y*1000+750, x*1000+750, db, bitmap_offset+0*bitmap_width+750, bitmap_width);

		tilewidth = 1000;
	}

	unsigned char	palette_convert[256];
	unsigned char * tracklog_palette = get_tracklog_palette();
	int i;
	for (i=0; i<256; i++)
		palette_convert[i] = (unsigned char)RGBtoPalette(mm_osgb_palette, 128, tracklog_palette[i*4+0], tracklog_palette[i*4+1], tracklog_palette[i*4+2]);

	for (y=0; y < tilewidth/2; y++) {
		for (x=0; x<tilewidth; x++) {
			// Flip image in the Y axis and convert to the 128 colour memory map palette
			unsigned char t = palette_convert[bitmap_offset[x+y*bitmap_width]];
			bitmap_offset[x+y*bitmap_width] = palette_convert[bitmap_offset[x+(tilewidth-1-y)*bitmap_width]];
			bitmap_offset[x+(tilewidth-1-y)*bitmap_width] = t;
		}
	}
	return 0;
}

int QCT_OSPro_read_bmp(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width, int nMapScale)
{
long tilewidth;

	if (nMapScale == MAP_SCALE_50K) {
		read_png(y*1000, x*1000, db, bitmap_offset + /*TILE_WIDTH/2*/0*bitmap_width, bitmap_width);
		tilewidth = 200;
	} else if (nMapScale == MAP_SCALE_25K) {
		read_png(y*1000, x*1000, db, bitmap_offset+TILE_WIDTH/2*bitmap_width, bitmap_width);
		read_png(y*1000, x*1000+500, db, bitmap_offset+TILE_WIDTH/2*bitmap_width+TILE_WIDTH/2, bitmap_width);
		read_png(y*1000+500, x*1000, db, bitmap_offset, bitmap_width);
		read_png(y*1000+500, x*1000+500, db, bitmap_offset+TILE_WIDTH/2, bitmap_width);
		tilewidth = 400;
	} else if (nMapScale == MAP_SCALE_10K) {

		read_png_10K(y*1000, x*1000, db, bitmap_offset+750*bitmap_width, bitmap_width);
		read_png_10K(y*1000, x*1000+250, db, bitmap_offset+750*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000, x*1000+500, db, bitmap_offset+750*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000, x*1000+750, db, bitmap_offset+750*bitmap_width+750, bitmap_width);

		read_png_10K(y*1000+250, x*1000, db, bitmap_offset+500*bitmap_width, bitmap_width);
		read_png_10K(y*1000+250, x*1000+250, db, bitmap_offset+500*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000+250, x*1000+500, db, bitmap_offset+500*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000+250, x*1000+750, db, bitmap_offset+500*bitmap_width+750, bitmap_width);

		read_png_10K(y*1000+500, x*1000, db, bitmap_offset+250*bitmap_width, bitmap_width);
		read_png_10K(y*1000+500, x*1000+250, db, bitmap_offset+250*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000+500, x*1000+500, db, bitmap_offset+250*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000+500, x*1000+750, db, bitmap_offset+250*bitmap_width+750, bitmap_width);

		read_png_10K(y*1000+750, x*1000, db, bitmap_offset+0*bitmap_width, bitmap_width);
		read_png_10K(y*1000+750, x*1000+250, db, bitmap_offset+0*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000+750, x*1000+500, db, bitmap_offset+0*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000+750, x*1000+750, db, bitmap_offset+0*bitmap_width+750, bitmap_width);

		tilewidth = 1000;

	}
/*
	else if (nMapScale == MAP_SCALE_SV) {

		read_png(y*1000, x*1000, db, bitmap_offset+750*bitmap_width, bitmap_width);
		read_png(y*1000, x*1000+250, db, bitmap_offset+750*bitmap_width+250, bitmap_width);
		read_png(y*1000, x*1000+500, db, bitmap_offset+750*bitmap_width+500, bitmap_width);
		read_png(y*1000, x*1000+750, db, bitmap_offset+750*bitmap_width+750, bitmap_width);

		read_png(y*1000+250, x*1000, db, bitmap_offset+500*bitmap_width, bitmap_width);
		read_png(y*1000+250, x*1000+250, db, bitmap_offset+500*bitmap_width+250, bitmap_width);
		read_png(y*1000+250, x*1000+500, db, bitmap_offset+500*bitmap_width+500, bitmap_width);
		read_png(y*1000+250, x*1000+750, db, bitmap_offset+500*bitmap_width+750, bitmap_width);

		read_png(y*1000+500, x*1000, db, bitmap_offset+250*bitmap_width, bitmap_width);
		read_png(y*1000+500, x*1000+250, db, bitmap_offset+250*bitmap_width+250, bitmap_width);
		read_png(y*1000+500, x*1000+500, db, bitmap_offset+250*bitmap_width+500, bitmap_width);
		read_png(y*1000+500, x*1000+750, db, bitmap_offset+250*bitmap_width+750, bitmap_width);

		read_png(y*1000+750, x*1000, db, bitmap_offset+0*bitmap_width, bitmap_width);
		read_png(y*1000+750, x*1000+250, db, bitmap_offset+0*bitmap_width+250, bitmap_width);
		read_png(y*1000+750, x*1000+500, db, bitmap_offset+0*bitmap_width+500, bitmap_width);
		read_png(y*1000+750, x*1000+750, db, bitmap_offset+0*bitmap_width+750, bitmap_width);

		tilewidth = 1000;

	} else if (nMapScale == MAP_SCALE_VML) {

		read_png_10K(y*1000, x*1000, db, bitmap_offset+750*bitmap_width, bitmap_width);
		read_png_10K(y*1000, x*1000+250, db, bitmap_offset+750*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000, x*1000+500, db, bitmap_offset+750*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000, x*1000+750, db, bitmap_offset+750*bitmap_width+750, bitmap_width);

		read_png_10K(y*1000+250, x*1000, db, bitmap_offset+500*bitmap_width, bitmap_width);
		read_png_10K(y*1000+250, x*1000+250, db, bitmap_offset+500*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000+250, x*1000+500, db, bitmap_offset+500*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000+250, x*1000+750, db, bitmap_offset+500*bitmap_width+750, bitmap_width);

		read_png_10K(y*1000+500, x*1000, db, bitmap_offset+250*bitmap_width, bitmap_width);
		read_png_10K(y*1000+500, x*1000+250, db, bitmap_offset+250*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000+500, x*1000+500, db, bitmap_offset+250*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000+500, x*1000+750, db, bitmap_offset+250*bitmap_width+750, bitmap_width);

		read_png_10K(y*1000+750, x*1000, db, bitmap_offset+0*bitmap_width, bitmap_width);
		read_png_10K(y*1000+750, x*1000+250, db, bitmap_offset+0*bitmap_width+250, bitmap_width);
		read_png_10K(y*1000+750, x*1000+500, db, bitmap_offset+0*bitmap_width+500, bitmap_width);
		read_png_10K(y*1000+750, x*1000+750, db, bitmap_offset+0*bitmap_width+750, bitmap_width);

		tilewidth = 1000;

	} else if (nMapScale == MAP_SCALE_VMD) {

		read_png(y*1000, x*1000, db, bitmap_offset+200*bitmap_width, bitmap_width);
		read_png(y*1000, x*1000+500, db, bitmap_offset+200*bitmap_width+200, bitmap_width);
		read_png(y*1000+500, x*1000, db, bitmap_offset, bitmap_width);
		read_png(y*1000+500, x*1000+500, db, bitmap_offset+200, bitmap_width);
		tilewidth = 400;

	} else if (nMapScale == MAP_SCALE_250K) {
//		read_png(y*1000, x*1000, &db, bitmap_offset + (maxy-5-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
		tilewidth = 40;
	} else if (nMapScale == MAP_SCALE_MS) {
//		read_png(y*1000, x*1000, &db, bitmap_offset + (maxy-20-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
		tilewidth = 10;
	}
*/

	unsigned char	palette_convert[256];
	unsigned char * tracklog_palette = get_tracklog_palette();
	int i;
	for (i=0; i<256; i++)
		palette_convert[i] = (unsigned char)RGBtoPalette(mm_osgb_palette, 128, tracklog_palette[i*4+0], tracklog_palette[i*4+1], tracklog_palette[i*4+2]);

	for (y=0; y < tilewidth/2; y++) {
		for (x=0; x<tilewidth; x++) {
			// Flip image in the Y axis and convert to the 128 colour memory map palette
			unsigned char t = palette_convert[bitmap_offset[x+y*bitmap_width]];
			bitmap_offset[x+y*bitmap_width] = palette_convert[bitmap_offset[x+(tilewidth-1-y)*bitmap_width]];
			bitmap_offset[x+(tilewidth-1-y)*bitmap_width] = t;
		}
	}
	return 0;
}

int QCT_IGN_read_bmp(long northing, long easting, CIGNTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize=0;
	LPBYTE data = db->ReadTile(northing*1000, easting*1000, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 0
		for (long ty=0; ty<TILE_WIDTH; ty++)
			memset(bm_ptr+ty*bitmap_width, 0, TILE_WIDTH);
		return 1;		// couldn't load file
	}
//	char strTilename[256];
//	sprintf(strTilename, "%d-%d\r", easting, northing);
//	printf("%s\r", strTilename);

	unsigned char * bmp = (unsigned char *)malloc(161078);
	if (bmp == NULL) {
		free(data);
		printf("Couldn't allocate 161078 bytes for bitmap\n");
		return 1;
	}


	// decode zlib encoded data

	int ret;
	z_stream strm;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK) {
		free(bmp);
		free(data);
		return ret;
	}

	strm.avail_in = tileSize;
	strm.next_in = data;
	strm.avail_out = 161078;
	strm.next_out = bmp;
	ret = inflate(&strm, Z_NO_FLUSH);
	(void)inflateEnd(&strm);

	if (memcmp(bmp+0x36, MakeQCT_ign_palette, 0x400) != 0) {
		printf("tile %d %d has a different palette\n", easting, northing);
	}

	unsigned char	palette_convert[256];
	int i;
	for (i=0; i<256; i++)
		palette_convert[i] = (unsigned char)RGBtoPalette(mm_ign_palette, 128, bmp[0x36+i*4+0], bmp[0x36+i*4+1], bmp[0x36+i*4+2]);

/*
	FILE * fp = fopen("out.bmp", "wb+");
	fwrite(bmp, 1,161078, fp);
	fclose(fp);
*/
	long y,x;
	for (y=0; y < TILE_WIDTH; y++) {
		for (x=0; x<TILE_WIDTH; x++) {
			bm_ptr[x+y*bitmap_width] = palette_convert[bmp[y*TILE_WIDTH+x+0x436]];
		}
	}

	free(bmp);
	free(data);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

long compress(unsigned char * data, unsigned char * output);
long compressHuffman(unsigned char * data, unsigned char * output);
long compressHuffmanTest(unsigned char * data, unsigned char * output);

#pragma pack(push,1)
	struct smsg_hdr {
		unsigned long length;		// 0
		unsigned long unknown1;		// 4
		unsigned short type;		// 8
		unsigned long id;			// 0x0a
		unsigned long unknown2[2];	// 0x0e
		unsigned short revision;	// 0x16
		unsigned short unknown3[3];	// 0x18
//		unsigned short unknown3[3+14];	// 0x18
		unsigned long width;		// 0x1e
		unsigned short unknown4[3];	// 0x22
		unsigned long height;		// 0x28
		unsigned short unknown5[3];	// 0x2c
		unsigned long format;		// 0x32
		unsigned short unknown6;	// 0x36
		unsigned long palette_size;	// 0x38
		unsigned char palette[1024];// 0x3c
		unsigned short unknown7;	// 0x43c
		unsigned long raster_size;	// 0x43e
		unsigned char raster[1];	// 0x442
	};
#pragma pack(pop)


#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        fprintf(stderr, "%s error: %d\n", msg, err); \
        exit(0); \
    } \
}


extern int abortProgram;


int MakeQCT_compare_palettes(const void * palette1, const void * palette2, int palette_len)
{
const DWORD * pal1 = (DWORD*)palette1;
const DWORD * pal2 = (DWORD*)palette2;
	palette_len >>= 2;
	while (palette_len--) {
		if ((*pal1 & 0xffffff) != (*pal2++ & 0xffffff))
			return 0;
	}
	return 1;
}



struct sQCT	qct_header;

void writePoint(int fh_qct, long x, long y)
{
	double lat, lon;

	switch (g_CoordinateSystem) {
	case COORD_BELGIUM_LAMBERT_2008:
//		LambertBelgium2008ToWGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_FRANCE_LAMBERT_1993:
		Lambert93ToWGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_SPAIN_ED50:
		Spain_WGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_GERMANY_GAUSS_KRUEGER:
		Germany_WGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_CZECH:
		Czech_WGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_SLOVENIA:
		Slovenia_WGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_CATALONIA:
		Catalonia_WGS84(y*1000, x*1000, &lat, &lon);
		break;
	case COORD_OSGB:
	case COORD_SCOTLAND:
	default:
		NorthingEasting_to_LatLon(y*1000, x*1000, &lat, &lon);
		break;
	}

	_write(fh_qct, &lat, sizeof(lat));
	_write(fh_qct, &lon, sizeof(lon));
}

long dump_vpdata(int fh_qct, long minx, long miny, long maxx, long maxy, long width, long height, const char * missing_squares, int dx, int initial_dir)
{
long path_length=0;
int start_x = dx%(maxx-minx);
int start_y = dx/(maxx-minx);
	if (dx == (maxx-minx)*(maxy-miny) || (maxx-minx)*(maxy-miny)==1) {
		// empty map, or consisting of 1 tile
		if (fh_qct) {
			writePoint(fh_qct, minx, miny);
			writePoint(fh_qct, maxx, miny);
			writePoint(fh_qct, maxx, maxy);
			writePoint(fh_qct, minx, maxy);
		}
		return 2*(maxx-minx) + 2*(maxy-miny);
	}

	if ( (start_x==0 || missing_squares[dx-1] == 0) &&
		(start_y==0 || missing_squares[dx-(maxx-minx)] == 0) &&
		(start_x==(maxx-minx-1) || missing_squares[dx+1] == 0) &&
		(start_y==(maxy-miny-1) || missing_squares[dx+(maxx-minx)] == 0))
	{
		// outline consists of a single tile
		if (fh_qct) {
			writePoint(fh_qct, minx+start_x, miny+start_y);
			writePoint(fh_qct, minx+start_x+1, miny+start_y);
			writePoint(fh_qct, minx+start_x+1, miny+start_y+1);
			writePoint(fh_qct, minx+start_x, miny+start_y+1);
		}
		return 4;
	}

	int x,y, pdir;
	int dirs[4][2] = {
		{1,0},		// right
		{0,1},		// down
		{-1,0},		// left
		{0,-1}};	// up
	int xyadjust[16][2] = {
					//dir, pdir
		{-1,1},		//0,0
		{0,0},		//0,1		inside bottom right
		{-1,0},		//0,2
		{-1,0},		//0,3		bottom left
		{1,-1},		//1,0		bottom right
		{0,-1},		//1,1		up to down to up #1
		{1,0},		//1,2		inside top right
		{1,-1},		//1,3		up to down to up #2
		{2,1},		//2,0		east to west to east #1
		{2,1},		//2,1		top right
		{2,0},		//2,2		east to west to east #1
		{1,1},		//2,3		inside top left
		{0,1},		//3,0		inside bottom left
		{0,2},		//3,1		down to up to down #2
		{0,2},		//3,2		top left
		{1,2}		//3,3		down to up to down #1
	};
	int index=1;
	int dir = initial_dir;

	x = start_x;
	y = start_y;
	pdir=(3 + initial_dir)%4;
	for (;;) {
		dir = (dir+3)%4;
		x += dirs[dir][0];
		y += dirs[dir][1];

		if (x<0 || x>=(maxx-minx) || y<0 || y>=(maxy-miny) || missing_squares[y*(maxx-minx)+x] == 0) {
			x -= dirs[dir][0];
			y -= dirs[dir][1];
			dir += 2;
		} else {
			path_length++;
			if (dir != pdir) {
				if ((dir ^ pdir) == 2) {
					if (fh_qct) {
						writePoint(fh_qct, minx+x+xyadjust[dir*4+dir][0], miny+y+xyadjust[dir*4+dir][1]);
						writePoint(fh_qct, minx+x+xyadjust[dir*4+pdir][0], miny+y+xyadjust[dir*4+pdir][1]);
					}
					index +=2;
				} else {
					if (fh_qct)
						writePoint(fh_qct, minx+x+xyadjust[dir*4+pdir][0], miny+y+xyadjust[dir*4+pdir][1]);
					index++;
				}
				pdir = dir;
			}
		}
		if (x==start_x && y==start_y && index != 1) {
			if (pdir == 3-initial_dir)
				break;
			if (pdir == 2-initial_dir && ((maxy-miny) == 1 || (maxy-miny) == y+1 || missing_squares[(maxx-minx)*(y+1)+x] == 0)) {
				// Started on row of horizontal tiles -- missing a corner
				if (fh_qct)
					writePoint(fh_qct, minx+x, miny+y+1);
				break;
			}
			// Shape:
			// +----
			// |
		}
	}

	return path_length;
}


void create_outline(int fh_qct, long minx, long miny, long maxx, long maxy, long width, long height, const char * missing_squares)
{
//	easting_northing_to_OSref5(minx, maxy, OSvalue);
//	easting_northing_to_OSref5(maxx, miny, OSvalue+strlen(OSvalue));

	// try making polygons from top and bottom of area and chose the polygon with the largest border
	int dx1;
	for (dx1=0; dx1<(maxx-minx)*(maxy-miny); dx1++) {
		if (missing_squares[dx1])
			break;
	}
	long length1 = dump_vpdata(0, minx, miny, maxx, maxy, width, height, missing_squares, dx1, 0);

	int dx2;
	for (dx2=(maxx-minx)*(maxy-miny)-1; dx2>=0; dx2--) {
		if (missing_squares[dx2])
			break;
	}
	long length2 = dump_vpdata(0, minx, miny, maxx, maxy, width, height, missing_squares, dx2, 2);

	_lseek(fh_qct, 0, SEEK_END);
	__int64 pointsOffset = _telli64(fh_qct);

	if (length1 > length2)
		dump_vpdata(fh_qct, minx, miny, maxx, maxy, width, height, missing_squares, dx1, 0);
	else
		dump_vpdata(fh_qct, minx, miny, maxx, maxy, width, height, missing_squares, dx2, 2);
	__int64 nPoints = (_telli64(fh_qct) - pointsOffset)/16;

	_lseek(fh_qct, (long)(&((struct sQCT*)NULL)->nOutlinePoints), SEEK_SET);
	_write(fh_qct, &nPoints, 4);
	_write(fh_qct, &pointsOffset, 4);
	_lseek(fh_qct, 0, SEEK_END);
}





unsigned char MakeQCT_uncompressed[1500*1500];
//long	uncompressed_length;
int cvt[64] = {63,31,47,15,55,23,39,7,59,27,43,11,51,19,35,3,61,29,45,13,53,21,37,5,57,25,41,9,49,17,33,1,62,30,46,14,54,22,38,6,58,26,42,10,50,18,34,2,60,28,44,12,52,20,36,4,56,24,40,8,48,16,32,0};

unsigned long TileCache00dd[256];
long g_optimize = 0;
long optimizer1Saved = 0;
long optimizer1Saved_count = 0;
long optimizer2Saved = 0;
long optimizer2Saved_count = 0;
long g_nTiles;
unsigned long * g_TileCache = NULL;
#define MAX_CACHE_TILES	10000

void OptimizerInit()
{
	memset(TileCache00dd, 0, sizeof(TileCache00dd));
	if (g_optimize) {
		g_nTiles = 0;
		g_TileCache = (unsigned long*)malloc(MAX_CACHE_TILES*8);
		if (g_TileCache == NULL) {
			printf("Couldn't allocate %d byte for optimizer\n");
		} else {
			memset(g_TileCache, 0, MAX_CACHE_TILES*8);
		}
	}
}

void OptimizerClose()
{
	if (g_TileCache) {
		free(g_TileCache);
		g_TileCache = NULL;
	}
}

unsigned long OptimizerGenerateHash(long length, unsigned char * data)
{
unsigned long csum = 0;
long l;
	l = length;
	while (--l)
		csum = _rotl(csum, 2) ^ *data++;
	return (csum << 13) + length;
}
unsigned long longest = 0;
unsigned long OptimizerGetOffset(int fh_qct, long length, unsigned char * data)
{
unsigned long hash = OptimizerGenerateHash(length, data);
long n;
	// Unlikely to match a similar tile more than 540 bytes in length
	if (length >= 540)
		return 0;
	for (n=0; n<MAX_CACHE_TILES; n++) {
		if (g_TileCache[n*2] == hash) {
			// possible match
			_lseeki64(fh_qct, g_TileCache[n*2+1], SEEK_SET);
			_read(fh_qct, MakeQCT_uncompressed+65536, length);
			if (memcmp(data, MakeQCT_uncompressed+65536, length) == 0) {
				long oldest = (g_nTiles+9999)%MAX_CACHE_TILES;
				long t1 = g_TileCache[n*2];
				long t2 = g_TileCache[n*2+1];
				g_TileCache[n*2] = g_TileCache[oldest*2];
				g_TileCache[n*2+1] = g_TileCache[oldest*2+1];
				g_TileCache[oldest*2] = t1;
				g_TileCache[oldest*2+1] = t2;
				return t2;
			}
		}
	}
	_lseek(fh_qct, 0, SEEK_END);
	g_TileCache[g_nTiles*2] = hash;
	g_TileCache[g_nTiles*2+1] = (unsigned long)_telli64(fh_qct);
	g_nTiles = (g_nTiles+1)%MAX_CACHE_TILES;
	return 0;
}


void SaveTile(int fh_qct, long tile)
{
long tile_length1 = compressHuffman(MakeQCT_uncompressed, MakeQCT_uncompressed+16384);
long tile_length2 = compress(MakeQCT_uncompressed, MakeQCT_uncompressed+32768);
__int64 file_offset = _telli64(fh_qct);
unsigned long file_offset_opt;

	if (tile_length1 < tile_length2) {
		if (tile_length1 == 2) {
			if (TileCache00dd[MakeQCT_uncompressed[16384+1]]) {
				file_offset = TileCache00dd[MakeQCT_uncompressed[16384+1]];
				optimizer1Saved += 2;
				optimizer1Saved_count++;
			} else {
				_write(fh_qct, MakeQCT_uncompressed+16384, tile_length1);
				TileCache00dd[MakeQCT_uncompressed[16384+1]] = (unsigned long)(file_offset);
			}
		} else {
			if (g_TileCache && (file_offset_opt = OptimizerGetOffset(fh_qct, tile_length1, MakeQCT_uncompressed+16384))) {
				file_offset = file_offset_opt;
				optimizer2Saved += tile_length1;
				optimizer2Saved_count++;
			} else {
				_write(fh_qct, MakeQCT_uncompressed+16384, tile_length1);
			}
		}
	} else {
		if (g_TileCache && (file_offset_opt = OptimizerGetOffset(fh_qct, tile_length2, MakeQCT_uncompressed+32768))) {
			file_offset = file_offset_opt;
			optimizer2Saved += tile_length2;
			optimizer2Saved_count++;
		} else {
			_write(fh_qct, MakeQCT_uncompressed+32768, tile_length2);
		}
	}
	_lseek(fh_qct, sizeof(struct sQCT)+ tile*4, SEEK_SET);
	_write(fh_qct, &file_offset, 4);
	_lseek(fh_qct, 0, SEEK_END);
}

/*
bool NGI_CreateQCT(LPCSTR mapname, long minx, long miny, long maxx, long maxy, int nMapScale)
{
int width, height, tile;
long x,y;

unsigned char	white_colour = 1;		// Memory Map palette entry 1 is {0xff, 0xff, 0xff}
unsigned char * bitmap_memory = NULL;
long bitmap_memory_size;
long mm_bitmap_width, bitmap_offset;
long mm_bitmap_height;
long square_width;
long square_height;

int fh_qct;

	tile = GetTileDatabaseScale(nMapScale);
	width = GetTileWidth(nMapScale);
	height = GetTileWidth(nMapScale);

	CTileDatabase db(MAP_DATUM_NGI,tile);

	CProgressWindow wndProgress;
	wndProgress.Initialize();


	// shrink extents to remove blank data
wndProgress.ResetProgressBar("Shrink1:", (maxy-miny)*(maxx-minx));

	char * squares = (char*)malloc((maxy-miny)*(maxx-minx));
	for (y=miny; y<maxy; y++) {
		for (x=minx; x<maxx; x++) {
			squares[(y-miny)*(maxx-minx)+x-minx] = db.TileLoaded(y*1000, x*1000);

wndProgress.ProgressBar();


		}
	}
	for (x=(maxy-miny)*(maxx-minx)-1; x>=0; x--)
		if (squares[x])
			break;
	if (x<0) {
		printf("Nothing to do!\n");
		free(squares);
		return false;
	}
	long maxy_new = miny+x/(maxx-minx)+1;

	for (x=0; x<(maxy-miny)*(maxx-minx); x++)
		if (squares[x])
			break;
	long miny_new = miny+x/(maxx-minx);

	long minx_new;
	for (x=0; x<maxx-minx; x++) {
		for (y=0; y<maxy-miny; y++) {
			if (squares[y*(maxx-minx) + x]) {
				minx_new = x+minx;
				x=maxx-minx;
				break;
			}
		}
	}

	long maxx_new;
	for (x=maxx-minx-1; x>=0; x--) {
		for (y=0; y<maxy-miny; y++) {
			if (squares[y*(maxx-minx) + x]) {
				maxx_new = x+minx+1;
				x=0;
				break;
			}
		}
	}


wndProgress.ResetProgressBar("Shrink2:", (maxy_new-miny_new)*(maxx_new-minx_new));

	char * squares_new = (char*)malloc((maxy_new-miny_new)*(maxx_new-minx_new));
	for (y=0; y<maxy_new-miny_new; y++) {
		for (x=0; x<maxx_new-minx_new; x++) {

			if (!wndProgress.ProgressBar()) return false;

			squares_new[y*(maxx_new-minx_new)+x]=squares[(y+miny_new-miny)*(maxx-minx)+x+minx_new-minx];
		}
	}

	minx = minx_new;
	maxx = maxx_new;
	miny = miny_new;
	maxy = maxy_new;
	free(squares);



			if (bitmap_memory == NULL) {
				// First square found so allocate memory for bitmap
				mm_bitmap_width = (((maxx-minx) * width)+63)/64*64;
				mm_bitmap_height = (((maxy-miny) * height)+63)/64*64;

				OptimizerInit();
//				OptimizerInit((mm_bitmap_width/64)*(mm_bitmap_height)/64);

				bitmap_offset = 0;
				bitmap_memory_size = (height+64) * mm_bitmap_width;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);
				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					exit(0);
				}
				memset(bitmap_memory, white_colour, bitmap_memory_size);		// assume 0xff is white

				square_width = width;
				square_height = height;

				if (mm_bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", mm_bitmap_width);
				if (mm_bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", mm_bitmap_height);

				memset(&qct_header, 0, sizeof(qct_header));
				qct_header.magic = 0x1423D5FF;
				qct_header._unk0004 = 2;
				qct_header.width = mm_bitmap_width/64;
				qct_header.height = mm_bitmap_height/64;
				qct_header._unk0040 = 2;

				for (int i=0; i<128; i++) {
					for (int j=0; j<128; j++) {
						qct_header.paletteMerge[i*128+j] = (unsigned char)RGBtoPalette(mm_osgb_palette, 128,
								(mm_osgb_palette[i*4+0]+mm_osgb_palette[j*4+0])/2,
								(mm_osgb_palette[i*4+1]+mm_osgb_palette[j*4+1])/2,
								(mm_osgb_palette[i*4+2]+mm_osgb_palette[j*4+2])/2);
					}
				}


				memcpy(&qct_header.palette, mm_osgb_palette, sizeof(qct_header.palette));

				CalculateCoefficients(LambertBelgium2008ToWGS84,
									  qct_header.calibration_data,
									  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
									  0, (maxx-minx) * width,
									  0, height*(maxy-miny));

				// Create QCT header and write out palette
				fh_qct = _open(mapname, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR, _S_IREAD | _S_IWRITE);
				if (fh_qct == -1) {
					printf("Couldn't open %s\n", mapname);
					exit(0);
				}
				_write(fh_qct, &qct_header, sizeof(qct_header));
				unsigned long zero = 0;
				for (int i=0; i<mm_bitmap_width/64*mm_bitmap_height/64; i++)
					_write(fh_qct, &zero, 4);

				// Create outline
				create_outline(fh_qct, minx, miny, maxx, maxy, (maxx-minx) * width, height*(maxy-miny), squares_new);
				free(squares_new);
			}

	// Create QCT

wndProgress.ResetProgressBar("QCT:", (maxy-miny)*(maxx-minx));

	long mm_tile_y=0;

int iterator = GetTileIterator(nMapScale);

	for (y=maxy-1; y>=miny; y -= iterator) {
		if (abortProgram)
			break;
		for (x=minx; x<maxx; x+= iterator) {
			if (abortProgram)
				break;
			if (!wndProgress.ProgressBar()) return false;

			QCT_NGI_read_bmp(y, x, &db, bitmap_memory + (x-minx)*width, mm_bitmap_width, nMapScale);
		}
		// create row of QCT
		long mmy;
		for (mmy=height+bitmap_offset-64; mmy>=0; mmy-=64) {
			long mmx;
			for (mmx=0; mmx<mm_bitmap_width; mmx+=64) {
				long ty;
				for (ty=0; ty<64; ty++)
					memcpy(MakeQCT_uncompressed+ty*64, bitmap_memory + (mmy+cvt[ty])*mm_bitmap_width+mmx, 64);
				SaveTile(fh_qct, mm_tile_y*mm_bitmap_width/64+mmx/64);
			}
			mm_tile_y++;
		}
		if (mmy < 0) {
			bitmap_offset = mmy+64;
			memmove(bitmap_memory+height*mm_bitmap_width, bitmap_memory, mm_bitmap_width*bitmap_offset);
			memset(bitmap_memory, white_colour, height*mm_bitmap_width);
		}
	}

	if (bitmap_offset != 0) {
		// write final row
		memset(bitmap_memory, white_colour, height*mm_bitmap_width);
		memmove(bitmap_memory+(64-bitmap_offset)*mm_bitmap_width, bitmap_memory+height*mm_bitmap_width, mm_bitmap_width*bitmap_offset);
		for (long mmx=0; mmx<mm_bitmap_width; mmx+=64) {
			for (long ty=0; ty<64; ty++)
				memcpy(MakeQCT_uncompressed+ty*64, bitmap_memory + (cvt[ty])*mm_bitmap_width+mmx, 64);

			SaveTile(fh_qct, mm_tile_y*mm_bitmap_width/64+mmx/64);
		}
		mm_tile_y++;
	}

	// close the file
	_close(fh_qct);


	OptimizerClose();
	free(bitmap_memory);
	return true;
}
*/
bool OSProCreateQCT(LPCSTR mapname, long minx, long miny, long maxx, long maxy, int nMapScale)
{
int width, height;
long x,y;

unsigned char	white_colour = 1;		// Memory Map palette entry 1 is {0xff, 0xff, 0xff}
unsigned char * bitmap_memory = NULL;
long bitmap_memory_size;
long mm_bitmap_width, bitmap_offset;
long mm_bitmap_height;
long square_width;
long square_height;

int fh_qct;

width = MyMap.m_nTileWidth;
height = MyMap.m_nTileWidth;
	
CTileDatabase db(MyMap.m_nDatum, MyMap.m_nMetresPerTile);

	CProgressWindow wndProgress;
	wndProgress.Initialize();


	// shrink extents to remove blank data
wndProgress.ResetProgressBar("Shrink1:", (maxy-miny)*(maxx-minx));

	char * squares = (char*)malloc((maxy-miny)*(maxx-minx));
	for (y=miny; y<maxy; y++) {
		for (x=minx; x<maxx; x++) {
			squares[(y-miny)*(maxx-minx)+x-minx] = db.TileLoaded(y*1000, x*1000);

wndProgress.ProgressBar();


		}
	}
	for (x=(maxy-miny)*(maxx-minx)-1; x>=0; x--)
		if (squares[x])
			break;
	if (x<0) {
		printf("Nothing to do!\n");
		free(squares);
		return false;
	}
	long maxy_new = miny+x/(maxx-minx)+1;

	for (x=0; x<(maxy-miny)*(maxx-minx); x++)
		if (squares[x])
			break;
	long miny_new = miny+x/(maxx-minx);

	long minx_new;
	for (x=0; x<maxx-minx; x++) {
		for (y=0; y<maxy-miny; y++) {
			if (squares[y*(maxx-minx) + x]) {
				minx_new = x+minx;
				x=maxx-minx;
				break;
			}
		}
	}

	long maxx_new;
	for (x=maxx-minx-1; x>=0; x--) {
		for (y=0; y<maxy-miny; y++) {
			if (squares[y*(maxx-minx) + x]) {
				maxx_new = x+minx+1;
				x=0;
				break;
			}
		}
	}


wndProgress.ResetProgressBar("Shrink2:", (maxy_new-miny_new)*(maxx_new-minx_new));

	char * squares_new = (char*)malloc((maxy_new-miny_new)*(maxx_new-minx_new));
	for (y=0; y<maxy_new-miny_new; y++) {
		for (x=0; x<maxx_new-minx_new; x++) {

			if (!wndProgress.ProgressBar()) return false;

			squares_new[y*(maxx_new-minx_new)+x]=squares[(y+miny_new-miny)*(maxx-minx)+x+minx_new-minx];
		}
	}

	minx = minx_new;
	maxx = maxx_new;
	miny = miny_new;
	maxy = maxy_new;
	free(squares);



			if (bitmap_memory == NULL) {
				// First square found so allocate memory for bitmap
				mm_bitmap_width = (((maxx-minx) * width)+63)/64*64;
				mm_bitmap_height = (((maxy-miny) * height)+63)/64*64;

				OptimizerInit();
//				OptimizerInit((mm_bitmap_width/64)*(mm_bitmap_height)/64);

				bitmap_offset = 0;
				bitmap_memory_size = (height+64) * mm_bitmap_width;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);
				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					exit(0);
				}
				memset(bitmap_memory, white_colour, bitmap_memory_size);		// assume 0xff is white

				square_width = width;
				square_height = height;

				/*
				if (mm_bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", mm_bitmap_width);
				if (mm_bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", mm_bitmap_height);
*/
				memset(&qct_header, 0, sizeof(qct_header));
				qct_header.magic = 0x1423D5FF;
				qct_header._unk0004 = 2;
				qct_header.width = mm_bitmap_width/64;
				qct_header.height = mm_bitmap_height/64;
				qct_header._unk0040 = 2;

//				for (i=0; i<256; i++) {
//					unsigned char t = mm_osgb_palette[i*4+0];
//					mm_osgb_palette[i*4+0] = mm_osgb_palette[i*4+2];
//					mm_osgb_palette[i*4+2] = t;
//				}
//				long i;
				for (int i=0; i<128; i++) {
					for (int j=0; j<128; j++) {
						qct_header.paletteMerge[i*128+j] = (unsigned char)RGBtoPalette(mm_osgb_palette, 128,
								(mm_osgb_palette[i*4+0]+mm_osgb_palette[j*4+0])/2,
								(mm_osgb_palette[i*4+1]+mm_osgb_palette[j*4+1])/2,
								(mm_osgb_palette[i*4+2]+mm_osgb_palette[j*4+2])/2);
					}
				}

//				printf("%d %d %d %d %d %d\n", minx*1000, miny*1000,
//										maxx*1000, maxy*1000,
//										(maxx-minx) * width,
//										(maxy-miny) * height);


				memcpy(&qct_header.palette, mm_osgb_palette, sizeof(qct_header.palette));

				if ((MyMap.m_nCountry == COUNTRY_UK) || (MyMap.m_nCountry == COUNTRY_SCO)) {

					CalculateCoefficients(NorthingEasting_to_LatLon,
										  qct_header.calibration_data,
										  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
										  0, (maxx-minx) * width,
										  0, height*(maxy-miny));

				} else if (MyMap.m_nCountry == COUNTRY_BE) {
					
//					CalculateCoefficients(LambertBelgium2008ToWGS84,
	//									  qct_header.calibration_data,
		//								  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
			//							  0, (maxx-minx) * width,
				//						  0, height*(maxy-miny));

				} else if (MyMap.m_nCountry == COUNTRY_ES) {
					
					CalculateCoefficients(Spain_WGS84,
										  qct_header.calibration_data,
										  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
										  0, (maxx-minx) * width,
										  0, height*(maxy-miny));

				} else if (MyMap.m_nCountry == COUNTRY_DE) {
					CalculateCoefficients(Germany_WGS84,
										  qct_header.calibration_data,
										  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
										  0, (maxx-minx) * width,
										  0, height*(maxy-miny));

				} else if (MyMap.m_nCountry == COUNTRY_CZ) {
					
					CalculateCoefficients(Czech_WGS84,
										  qct_header.calibration_data,
										  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
										  0, (maxx-minx) * width,
										  0, height*(maxy-miny));

				} else if (MyMap.m_nCountry == COUNTRY_SI) {
					
					CalculateCoefficients(Slovenia_WGS84,
										  qct_header.calibration_data,
										  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
										  0, (maxx-minx) * width,
										  0, height*(maxy-miny));
				
				} else if (MyMap.m_nCountry == COUNTRY_CAT) {
					
					CalculateCoefficients(Catalonia_WGS84,
										  qct_header.calibration_data,
										  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
										  0, (maxx-minx) * width,
										  0, height*(maxy-miny));
				}

				// Create QCT header and write out palette
				fh_qct = _open(mapname, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR, _S_IREAD | _S_IWRITE);
				if (fh_qct == -1) {
					printf("Couldn't open %s\n", mapname);
					exit(0);
				}
				_write(fh_qct, &qct_header, sizeof(qct_header));
				unsigned long zero = 0;
				for (int i=0; i<mm_bitmap_width/64*mm_bitmap_height/64; i++)
					_write(fh_qct, &zero, 4);

				// Create outline
				create_outline(fh_qct, minx, miny, maxx, maxy, (maxx-minx) * width, height*(maxy-miny), squares_new);
				free(squares_new);
			}

	// Create QCT

wndProgress.ResetProgressBar("QCT:", (maxy-miny)*(maxx-minx));

//	printf("(%d,%d) -> (%d,%d)\n", minx, miny, maxx, maxy);
	long mm_tile_y=0;

int iterator = GetTileIterator(nMapScale);

	for (y=maxy-1; y>=miny; y -= iterator) {
		if (abortProgram)
			break;
		for (x=minx; x<maxx; x+= iterator) {
			if (abortProgram)
				break;
//			printf("%04d%04d\r", x, y);
			if (!wndProgress.ProgressBar()) return false;

//			QCT_OSPro_read_bmp(y, x, &db, bitmap_memory + (x-minx)*width, mm_bitmap_width, nMapScale);
			
			(void) OSPro_ReadTiles_QCT(y*1000, x*1000, &db, bitmap_memory + (x-minx)*width, mm_bitmap_width);

		}
		// create row of QCT
		long mmy;
		for (mmy=height+bitmap_offset-64; mmy>=0; mmy-=64) {
			long mmx;
			for (mmx=0; mmx<mm_bitmap_width; mmx+=64) {
				long ty;
				for (ty=0; ty<64; ty++)
					memcpy(MakeQCT_uncompressed+ty*64, bitmap_memory + (mmy+cvt[ty])*mm_bitmap_width+mmx, 64);
				SaveTile(fh_qct, mm_tile_y*mm_bitmap_width/64+mmx/64);
			}
			mm_tile_y++;
		}
		if (mmy < 0) {
			bitmap_offset = mmy+64;
			memmove(bitmap_memory+height*mm_bitmap_width, bitmap_memory, mm_bitmap_width*bitmap_offset);
			memset(bitmap_memory, white_colour, height*mm_bitmap_width);
		}
	}

	if (bitmap_offset != 0) {
		// write final row
		memset(bitmap_memory, white_colour, height*mm_bitmap_width);
		memmove(bitmap_memory+(64-bitmap_offset)*mm_bitmap_width, bitmap_memory+height*mm_bitmap_width, mm_bitmap_width*bitmap_offset);
		for (long mmx=0; mmx<mm_bitmap_width; mmx+=64) {
			for (long ty=0; ty<64; ty++)
				memcpy(MakeQCT_uncompressed+ty*64, bitmap_memory + (cvt[ty])*mm_bitmap_width+mmx, 64);

			SaveTile(fh_qct, mm_tile_y*mm_bitmap_width/64+mmx/64);
		}
		mm_tile_y++;
	}

	/* close the file */
	_close(fh_qct);


	OptimizerClose();
	free(bitmap_memory);
	return true;
}

bool IGNCreateQCT(LPCSTR mapname, long minx, long miny, long maxx, long maxy)
{
int width, height;
long x,y;
//	char * missing_squares;		// Not used SNL 01/02/2013?
unsigned char	white_colour = 1;
unsigned char * bitmap_memory = NULL;
long bitmap_memory_size;
long mm_bitmap_width, bitmap_offset;
long mm_bitmap_height;
long square_width;
long square_height;
//	unsigned char bitmap_palette[1024];	// Not used SNL 01/02/2013?
//	long bitmap_palette_length;	// Not used SNL 01/02/2013?

int fh_qct;

CIGNTileDatabase db;

	CProgressWindow wndProgress;
	wndProgress.Initialize();


	// shrink extents to remove blank data
wndProgress.ResetProgressBar("Shrink:", (maxy-miny)*(maxx-minx));

	char * squares = (char*)malloc((maxy-miny)*(maxx-minx));
	for (y=miny; y<maxy; y++) {
		for (x=minx; x<maxx; x++) {
			squares[(y-miny)*(maxx-minx)+x-minx] = db.TileLoaded(y*1000, x*1000);

if (!wndProgress.ProgressBar()) return false;


		}
	}
	for (x=(maxy-miny)*(maxx-minx)-1; x>=0; x--)
		if (squares[x])
			break;
	if (x<0) {
		printf("Nothing to do!\n");
		free(squares);
		return false;
	}
	long maxy_new = miny+x/(maxx-minx)+1;

	for (x=0; x<(maxy-miny)*(maxx-minx); x++)
		if (squares[x])
			break;
	long miny_new = miny+x/(maxx-minx);

	long minx_new;
	for (x=0; x<maxx-minx; x++) {
		for (y=0; y<maxy-miny; y++) {
			if (squares[y*(maxx-minx) + x]) {
				minx_new = x+minx;
				x=maxx-minx;
				break;
			}
		}
	}

	long maxx_new;
	for (x=maxx-minx-1; x>=0; x--) {
		for (y=0; y<maxy-miny; y++) {
			if (squares[y*(maxx-minx) + x]) {
				maxx_new = x+minx+1;
				x=0;
				break;
			}
		}
	}


wndProgress.ResetProgressBar("Shrink:", (maxy-miny)*(maxx-minx));

	char * squares_new = (char*)malloc((maxy_new-miny_new)*(maxx_new-minx_new));
	for (y=0; y<maxy_new-miny_new; y++) {
		for (x=0; x<maxx_new-minx_new; x++) {

if (!wndProgress.ProgressBar()) return false;

			squares_new[y*(maxx_new-minx_new)+x]=squares[(y+miny_new-miny)*(maxx-minx)+x+minx_new-minx];
		}
	}

/*
//	printf("%d %d %d %d\n", miny,minx,maxy,maxx);
	for (y=maxy-1; y>=miny; y--) {
		for (x=minx; x<maxx; x++) {
//			printf(squares[(y-miny)*(maxx-minx)+x-minx] ? "x" : ".");
		}
//		printf("\n");
	}
//	printf("\n");


//	printf("%d %d %d %d\n", miny_new,minx_new,maxy_new,maxx_new);
	for (y=maxy_new-1; y>=miny_new; y--) {
		for (x=minx_new; x<maxx_new; x++) {
//			printf(squares_new[(y-miny_new)*(maxx_new-minx_new)+x-minx_new] ? "x" : ".");



		}
//		printf("\n");
	}
*/
	minx = minx_new;
	maxx = maxx_new;
	miny = miny_new;
	maxy = maxy_new;
	free(squares);



	width = TILE_WIDTH;
	height = TILE_WIDTH;

			if (bitmap_memory == NULL) {
				// First square found so allocate memory for bitmap
				mm_bitmap_width = (((maxx-minx) * width)+63)/64*64;
				mm_bitmap_height = (((maxy-miny) * height)+63)/64*64;

				OptimizerInit();
//				OptimizerInit((mm_bitmap_width/64)*(mm_bitmap_height)/64);

				bitmap_offset = 0;
				bitmap_memory_size = (height+64) * mm_bitmap_width;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);
				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					exit(0);
				}
				memset(bitmap_memory, white_colour, bitmap_memory_size);		// assume 0xff is white

				square_width = width;
				square_height = height;

				/*
				if (mm_bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", mm_bitmap_width);
				if (mm_bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", mm_bitmap_height);
*/
				memset(&qct_header, 0, sizeof(qct_header));
				qct_header.magic = 0x1423D5FF;
				qct_header._unk0004 = 2;
				qct_header.width = mm_bitmap_width/64;
				qct_header.height = mm_bitmap_height/64;
				qct_header._unk0040 = 2;

//				for (i=0; i<256; i++) {
//					unsigned char t = mm_ign_palette[i*4+0];
//					mm_ign_palette[i*4+0] = mm_ign_palette[i*4+2];
//					mm_ign_palette[i*4+2] = t;
//				}
//				long i;
				for (int i=0; i<128; i++) {
					for (int j=0; j<128; j++) {
						qct_header.paletteMerge[i*128+j] = (unsigned char)RGBtoPalette(mm_ign_palette, 128,
								(mm_ign_palette[i*4+0]+mm_ign_palette[j*4+0])/2,
								(mm_ign_palette[i*4+1]+mm_ign_palette[j*4+1])/2,
								(mm_ign_palette[i*4+2]+mm_ign_palette[j*4+2])/2);
					}
				}

//				printf("%d %d %d %d %d %d\n", minx*1000, miny*1000,
//										maxx*1000, maxy*1000,
//										(maxx-minx) * width,
//										(maxy-miny) * height);


				memcpy(&qct_header.palette, mm_ign_palette, sizeof(qct_header.palette));

				CalculateCoefficients(Lambert93ToWGS84,
									  qct_header.calibration_data,
									  miny*1000, (maxy-miny)*1000, minx*1000, (maxx-minx)*1000,
									  0, (maxx-minx) * width,
									  0, height*(maxy-miny));

				// Create QCT header and write out palette
				fh_qct = _open(mapname, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR, _S_IREAD | _S_IWRITE);
				if (fh_qct == -1) {
					printf("Couldn't open %s\n", mapname);
					exit(0);
				}
				_write(fh_qct, &qct_header, sizeof(qct_header));
				unsigned long zero = 0;
				for (int i=0; i<mm_bitmap_width/64*mm_bitmap_height/64; i++)
					_write(fh_qct, &zero, 4);

				// Create outline
				create_outline(fh_qct, minx, miny, maxx, maxy, (maxx-minx) * width, height*(maxy-miny), squares_new);
				free(squares_new);
			}

	// Create QCT

wndProgress.ResetProgressBar("QCT:", (maxy-miny)*(maxx-minx));


//	printf("(%d,%d) -> (%d,%d)\n", minx, miny, maxx, maxy);
	long mm_tile_y=0;
	for (y=maxy-1; y>=miny; y--) {
		if (abortProgram)
			break;
		for (x=minx; x<maxx; x++) {
			if (abortProgram)
				break;
//			printf("%04d%04d\r", x, y);
if (wndProgress.ProgressBar()) return false;


			QCT_IGN_read_bmp(y, x, &db, bitmap_memory + (x-minx)*width, mm_bitmap_width);
		}
		// create row of QCT
		long mmy;
		for (mmy=height+bitmap_offset-64; mmy>=0; mmy-=64) {
			long mmx;
			for (mmx=0; mmx<mm_bitmap_width; mmx+=64) {
				long ty;
				for (ty=0; ty<64; ty++)
					memcpy(MakeQCT_uncompressed+ty*64, bitmap_memory + (mmy+cvt[ty])*mm_bitmap_width+mmx, 64);
				SaveTile(fh_qct, mm_tile_y*mm_bitmap_width/64+mmx/64);
			}
			mm_tile_y++;
		}
		if (mmy < 0) {
			bitmap_offset = mmy+64;
			memmove(bitmap_memory+height*mm_bitmap_width, bitmap_memory, mm_bitmap_width*bitmap_offset);
			memset(bitmap_memory, white_colour, height*mm_bitmap_width);
		}
	}

	if (bitmap_offset != 0) {
		// write final row
		memset(bitmap_memory, white_colour, height*mm_bitmap_width);
		memmove(bitmap_memory+(64-bitmap_offset)*mm_bitmap_width, bitmap_memory+height*mm_bitmap_width, mm_bitmap_width*bitmap_offset);
		for (long mmx=0; mmx<mm_bitmap_width; mmx+=64) {
			for (long ty=0; ty<64; ty++)
				memcpy(MakeQCT_uncompressed+ty*64, bitmap_memory + (cvt[ty])*mm_bitmap_width+mmx, 64);

			SaveTile(fh_qct, mm_tile_y*mm_bitmap_width/64+mmx/64);
		}
		mm_tile_y++;
	}

	/* close the file */
	_close(fh_qct);

	OptimizerClose();
	free(bitmap_memory);
	return true;
}

bool MakeQCT_main(long miny, long minx, long maxy, long maxx, LPCSTR mapname, int nMapScale)
{
//	SetConsoleCtrlHandler((PHANDLER_ROUTINE)MakeQCT_CtrlCHandler, TRUE);

		g_optimize = 1;

if (g_CoordinateSystem == COORD_FRANCE_LAMBERT_1993) {
	if (miny<1000000) {
		minx*=1000;
		miny*=1000;
		maxx*=1000;
		maxy*=1000;
	}
}

	if (minx > maxx) {
		long t = minx;
		minx = maxx;
		maxx = t;
	}
	if (miny > maxy) {
		long t = miny;
		miny = maxy;
		maxy = t;
	}

	minx = minx/1000;
	miny = miny/1000;
	maxx = (maxx+999)/1000;
	maxy = (maxy+999)/1000;

	if (g_CoordinateSystem == COORD_FRANCE_LAMBERT_1993) {
		
		return (IGNCreateQCT(mapname, minx, miny, maxx, maxy));

	} else {
		
		 return (OSProCreateQCT(mapname, minx, miny, maxx, maxy, nMapScale));
	}

	return false;
}
