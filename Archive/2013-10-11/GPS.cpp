#include "stdafx.h"

#include <stdio.h>
#include <windows.h>

#define MAX 256

/*
int main(void) {
	int dr_type=99;
	char dr_avail[MAX];
	char *temp=dr_avail;
	
	// 1st we fill the buffer
	GetLogicalDriveStrings(MAX,dr_avail);
	while(*temp!=NULL) { // Split the buffer by null
		dr_type=GetDriveType(temp);
		switch(dr_type) {
			case 0: // Unknown
			printf("%s : Unknown Drive type\n",temp);
			break;
			case 1: // Invalid
			printf("%s : Drive is invalid\n",temp);
			break;
			case 2: // Removable Drive
			printf("%s : Removable Drive\n",temp);
			break;
			case 3: // Fixed
			printf("%s : Hard Disk (Fixed)\n",temp);
			break;
			case 4: // Remote
			printf("%s : Remote (Network) Drive\n",temp);
			break;
			case 5: // CDROM
			printf("%s : CD-Rom/DVD-Rom\n",temp);
			break;
			case 6: // RamDrive
			printf("%s : Ram Drive\n",temp);
			break;
		}
		temp += lstrlen(temp) +1; // incriment the buffer
	}
	return 0;
}
*/

/*
	if ((GetFileAttributes(cLocati.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		cout << "Directory doesn't exist\";
		CreateDirectory(cLocati.c_str(), 0);
		cout << "Directory Created\";
	}
*/

CString GetTrackFolderFromGPS(void)
{
	CString strTrackFolder;

	int dr_type=99;
	char dr_avail[MAX];
	char *temp=dr_avail;
	
	strTrackFolder.Empty();
	// 1st we fill the buffer
	GetLogicalDriveStrings(MAX,dr_avail);
	while(*temp!=NULL) { // Split the buffer by null
		dr_type=GetDriveType(temp);
		switch(dr_type) {
			case 2: // Removable Drive
//			printf("%s : Removable Drive\n",temp);
			strTrackFolder.Format("%sGarmin\\GPX",temp);
			if (GetFileAttributes(strTrackFolder) != INVALID_FILE_ATTRIBUTES)
			{
				return strTrackFolder;
			}
			break;
		}
		temp += lstrlen(temp) +1; // incriment the buffer
	}

	strTrackFolder.Empty();
	return strTrackFolder;
}

CString GetMapFolderFromGPS(CString strCustomMapType)
{
	CString strMapFolder;

	int dr_type=99;
	char dr_avail[MAX];
	char *temp=dr_avail;
	
	strMapFolder.Empty();
	// 1st we fill the buffer
	GetLogicalDriveStrings(MAX,dr_avail);
	while(*temp!=NULL) { // Split the buffer by null
		dr_type=GetDriveType(temp);
		switch(dr_type) {
			case 2: // Removable Drive
//			printf("%s : Removable Drive\n",temp);
			strMapFolder.Format("%sGarmin\\%s",temp,strCustomMapType);
			if (GetFileAttributes(strMapFolder) != INVALID_FILE_ATTRIBUTES)
			{
				return strMapFolder;
			}
			break;
		}
		temp += lstrlen(temp) +1; // incriment the buffer
	}

	strMapFolder.Empty();
	return strMapFolder;
}