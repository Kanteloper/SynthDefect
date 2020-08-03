// GlobMyProc.cpp for common Global utility procedures

#include "pch.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/// <summary>
/// Convert LPCTSTR to std::string
/// </summary>
/// <param name="target">: Target LPCTSTR of conversion </param>
/// <returns> Converted value of std::string type </returns>
std::string ConvertStdString(LPCTSTR target)
{
	// In order to for the macros to have space to store the a temporary length, 
	// See https://docs.microsoft.com/ko-kr/cpp/mfc/tn059-using-mfc-mbcs-unicode-conversion-macros?view=vs-2019
	// convert lpctstr(MBCS) to lpcstr(MBCS), convert lpcstr(MBCS) to CString
	USES_CONVERSION;
	CString tmpStr = T2CW(target);
	std::string result((CStringA)tmpStr);
	return result;
}


/// <summary>
/// Retrieve the vertex/fragment shader code from file
/// </summary>
/// <param name="filePath">: the file of vertex/fragment shader</param>
/// <returns>vertex/fragment shader code</returns>
std::string RetrieveCode(const char* filePath)
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
		return tmpStr;
	}
	catch (std::ifstream::failure e)
	{
		TRACE(e.what());
		return nullptr;
	}
}