struct sQCT
{
	unsigned long	magic;				// 00 = 0x1423D5FF
	unsigned long	_unk0004;			// 04 = 0x2
	unsigned long	width;				// 08
	unsigned long	height;				// 0c
	unsigned long	ptrLongTitle;		// 10
	unsigned long	ptrName;			// 14
	unsigned long	ptrIdent;			// 18
	unsigned long	ptrEdition;			// 1c
	unsigned long	ptrRevision;		// 20
	unsigned long	ptrKeywords;		// 24
	unsigned long	ptrCopyright;		// 28
	unsigned long	ptrScale;			// 2c
	unsigned long	ptrDatum;			// 30
	unsigned long	ptrDepths;			// 34
	unsigned long	ptrHeights;			// 38
	unsigned long	ptrProjection;		// 3c
	unsigned long	_unk0040;			// 40 = 0x1=must have original file; 0x2=allow calibration
	unsigned long	ptrOriginalFilename;// 44
	unsigned long	_unk0048;			// 48 = 0x9fcbc3
	unsigned long	_unk004c;			// 4c = 0x40eafe14
	unsigned long	_unk0050;			// 50
	unsigned long	ptrExtraInfo;		// 54
	unsigned long	nOutlinePoints;		// 58
	unsigned long	ptrOutline;			// 5c
	double			calibration_data[40];// 60
	unsigned long	palette[256];		// 01a0
	unsigned char	paletteMerge[16384];// 05a0
//	unsigned long	_tileOffsets[0];	// 45a0
};

struct sQCT_ExtraInfo
{
	unsigned long	ptrType;
	unsigned long	ptrDatumShift;
	unsigned long	ptrDiskName;
	unsigned long	_unk0c;
	unsigned long	_unk10;
	unsigned long	_unk14;
	unsigned long	ptrAssociatedData;
	unsigned long	_unk1c;
};
