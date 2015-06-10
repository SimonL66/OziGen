#include "stdafx.h"
#include <gdiplus.h>

#include "jpeg.h"

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT	num = 0;		// number of image encoders
	UINT	size = 0;		// size of the image encoder array in bytes

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;		// Failure

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;		// Failure
	
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	
	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;	// Success
		}
	}

	free(pImageCodecInfo);
	return -1;			// Failure
}


DWORD appendJPG(CString fname, FILE * fid)
{
	FILE * fp = fopen(fname, "rb");
	if (fp == NULL) {
		printf("Couldn't open %s\n", fname);
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	DWORD length = ftell(fp)-2;
	fseek(fp, 2, SEEK_SET);
	BYTE * buffer = (BYTE*)malloc(length);
	if (buffer == NULL) {
		printf("Couldn't allocate %d bytes of memory to read %s\n", length, fname);
		return 0;
	}
	fread(buffer, 1, length, fp);
	fclose(fp);
	fwrite(buffer, 1, length, fid);
	free(buffer);
	return length;
}
