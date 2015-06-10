#pragma once

void GetNationalParkExtents (int nExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap);
int GetNationalParkMapNumber(int nMapNumber);
CString GetNationalParkMapName (int nExplorer);

void GetRegionalAreaExtents (int nExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap);
int GetRegionalAreaMapNumber(int nMapNumber);
CString GetRegionalAreaMapName (int nExplorer);

void GetLargeRegionalAreaExtents (int nExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap);
int GetLargeRegionalAreaMapNumber(int nMapNumber);
CString GetLargeRegionalAreaMapName (int nExplorer);

void GetExplorerExtents (long lngExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap);
int GetExplorerMapNumber(int nMapNumber, int iDelta);
CString GetExplorerMapName (long lngExplorer);

void GetLandrangerExtents (long lngExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap);
int GetLandrangerMapNumber(int nMapNumber);
CString GetLandrangerMapName (long lngExplorer);
