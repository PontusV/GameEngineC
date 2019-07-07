#ifndef CLIPBOARD_H
#define CLIPBOARD_H
#include <string>
namespace Core {
	namespace Clipboard {
		void copy(std::wstring text);
		std::wstring paste();
	};
}
#endif