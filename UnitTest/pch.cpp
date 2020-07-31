// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
std::string ConvertStdString(LPCTSTR target)
{
	USES_CONVERSION;
	CString tmpStr = T2CW(target);
	std::string result((CStringA)tmpStr);
	return result;
}

const char* RetrieveCode(const char* filePath)
{
	std::string tmpStr;
	std::ifstream file;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open vertex shader, fragment shader code file
		file.open(filePath);
		// read file's buffer contents into streams
		std::stringstream stream;
		stream << file.rdbuf();
		// close internal stream buffer
		file.close();
		// convert stream into string
		tmpStr = stream.str();
		return tmpStr.c_str();
	}
	catch (std::ifstream::failure e)
	{
		return nullptr;
	}
}