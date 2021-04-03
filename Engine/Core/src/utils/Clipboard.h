#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include <string>
#include <vector>
namespace Core {
	namespace Clipboard {
		void setDataAsText(std::string text);
		std::string getDataAsText();

		void setDataAsFiles(std::vector<std::wstring> path);
		std::vector<std::wstring> getDataAsFiles();

		bool clear();
		unsigned long getSequenceNumber();
	};
}
#endif