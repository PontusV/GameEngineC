#include "file.h"
#include <Windows.h>
#include <commdlg.h>
#include <ShlObj.h>

std::wstring getOpenFileName(const wchar_t* title, const wchar_t* filter, unsigned long filterCount)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	wchar_t f1[MAX_PATH] = { 0 };
	ZeroMemory(&f1, sizeof(f1));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = filterCount;
	ofn.lpstrFile = (wchar_t*)f1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn)) {
		return std::wstring(f1);
	}
	return std::wstring();
}

std::wstring getSaveFileName(const wchar_t* title, const wchar_t* filter, unsigned long filterCount, const wchar_t* root)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	wchar_t f1[MAX_PATH] = { 0 };
	ZeroMemory(&f1, sizeof(f1));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrTitle = title;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = filterCount;
	ofn.lpstrFile = (wchar_t*)f1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (root) {
		/*LPITEMIDLIST folderID;
		SHParseDisplayName(root, NULL, &folderID, NULL, NULL);
		ofn.pidlRoot = folderID;*/
	}

	if (GetSaveFileName(&ofn)) {
		return std::wstring(f1);
	}
	return std::wstring();
}

std::wstring getOpenFolderName(const wchar_t* title, const wchar_t* root)
{
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	wchar_t szDir[MAX_PATH] = { 0 };
	ZeroMemory(&szDir, sizeof(szDir));
	bInfo.pszDisplayName = szDir; // Address of a buffer to receive the display name of the folder selected by the user
	bInfo.lpszTitle = title; // Title of the dialog
	bInfo.ulFlags = 0;
	bInfo.lpfn = NULL;
	if (root) {
		LPITEMIDLIST folderID;
		SHParseDisplayName(root, NULL, &folderID, NULL, NULL);
		bInfo.pidlRoot = folderID;
	}
	
	LPITEMIDLIST lpItem = SHBrowseForFolder(&bInfo);
	if (lpItem != NULL)
	{
		SHGetPathFromIDList(lpItem, szDir);
		return std::wstring(szDir);
	}
	return std::wstring();
}

std::wstring getApplicationDirectoryPath() {
	wchar_t szDir[MAX_PATH] = { 0 };
	if (GetModuleFileName(NULL, szDir, sizeof(szDir))) {
		std::wstring result = szDir;
		return result.substr(0, result.find_last_of(L"\\"));
	}
	return std::wstring();
}