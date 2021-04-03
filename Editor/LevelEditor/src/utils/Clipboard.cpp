#include "Clipboard.h"
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#include <Shlobj.h>
#endif
using namespace Editor;

void Clipboard::setDataAsText(std::string text) {
	if (text.empty()) return;
#ifdef WIN32
	int memSize = (text.length() + 1) * sizeof(char);
	HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, memSize);
	memcpy((void*)GlobalLock(hData), text.c_str(), memSize);
	GlobalUnlock(hData);
	if (OpenClipboard(NULL)) {
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hData);
		CloseClipboard();
	}
#endif
}

std::string Clipboard::getDataAsText() {
	std::string text;
#ifdef WIN32
	if (OpenClipboard(NULL)) {
		if (IsClipboardFormatAvailable(CF_TEXT)) {
			HANDLE hData = GetClipboardData(CF_TEXT);
			text = static_cast<char*>(GlobalLock(hData));
			GlobalUnlock(hData);
		}
		CloseClipboard();
	}
#endif
	return text;
}

void Clipboard::setDataAsFiles(std::vector<std::wstring> filePaths) {
#ifdef WIN32
	int clpSize = sizeof(DROPFILES);
	for (int i = 0; i < filePaths.size(); i++)
		clpSize += sizeof(wchar_t) * (wcslen(filePaths[i].c_str()) + 1);
	clpSize += sizeof(wchar_t);

	HDROP hdrop = (HDROP)GlobalAlloc(GHND, clpSize);
	DROPFILES* df = (DROPFILES*)GlobalLock(hdrop);
	df->pFiles = sizeof(DROPFILES);
	df->fWide = TRUE;

	wchar_t* dstStart = (wchar_t*)&df[1];
	for (int i = 0; i < filePaths.size(); i++) {
		wcscpy_s(dstStart, filePaths[i].length() + 1, filePaths[i].c_str());
		dstStart = &dstStart[filePaths[i].length() + 1];
	}
	GlobalUnlock(hdrop);
	if (OpenClipboard(NULL)) {
		EmptyClipboard();
		SetClipboardData(CF_HDROP, hdrop);
		CloseClipboard();
	}
#endif
}

std::vector<std::wstring> Clipboard::getDataAsFiles() {
	std::vector<std::wstring> filePaths;
#ifdef WIN32
	if (OpenClipboard(NULL)) {
		if (IsClipboardFormatAvailable(CF_HDROP)) {
			HDROP hdrop = (HDROP)GetClipboardData(CF_HDROP);
			DROPFILES* df = (DROPFILES*)GlobalLock(hdrop);
			wchar_t* ptr = (wchar_t*)&df[1];
			while (*ptr != '\0') {
				wchar_t* path = ptr;
				filePaths.push_back(std::wstring(path, wcslen(path)));
				ptr += wcslen(path) + 1;
			}
			GlobalUnlock(hdrop);
		}
		CloseClipboard();
	}
#endif
	return filePaths;
}

unsigned long Clipboard::getSequenceNumber() {
	return GetClipboardSequenceNumber();
}

bool Clipboard::clear() {
	if (OpenClipboard(NULL)) {
		EmptyClipboard();
		CloseClipboard();
		return true;
	}
	return false;
}