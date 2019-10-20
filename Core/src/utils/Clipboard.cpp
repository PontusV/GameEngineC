#include "Clipboard.h"
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#endif
using namespace Core;

void Clipboard::copy(std::wstring text) {
	if (text.empty()) return;
#ifdef WIN32
	int memSize = (text.length() + 1) * sizeof(wchar_t);

	HANDLE hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, memSize);
	memcpy((void*)GlobalLock(hData), text.c_str(), memSize);
	GlobalUnlock(hData);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hData);
	CloseClipboard();
#endif
}

std::wstring Clipboard::paste() {
#ifdef WIN32
	OpenClipboard(NULL);
	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	std::wstring text = (wchar_t*)GlobalLock(hData);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
#endif
}