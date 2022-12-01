#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "Game/LIb/GLFW.h"
#include <codecvt>

class Clipboard {
	Clipboard() {}
	~Clipboard() {}
public:

	static std::wstring to_wstring(std::string & narrow) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(narrow);
	}

	static bool get(std::string & str) {
		const char* buffer = glfwGetClipboardString(NULL);
		if (buffer)
		{
			str = buffer;
			return 1;
		}
		return 0;
	}

	static void push(const std::string & str) {
		glfwSetClipboardString(NULL, str.c_str());
	}
};

#endif

