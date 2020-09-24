#pragma once

#include <string>

std::wstring getOpenFileName(const wchar_t* title, const wchar_t* filter, unsigned long filterCount);

std::wstring getSaveFileName(const wchar_t* title, const wchar_t* filter, unsigned long filterCount, const wchar_t* root = nullptr);

std::wstring getOpenFolderName(const wchar_t* title, const wchar_t* root = nullptr);

std::wstring getApplicationDirectoryPath();