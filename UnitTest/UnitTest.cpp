#include "pch.h"
#include "CppUnitTest.h"
#include "../SynthDefect/GlobMyProc.h"x

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestMyProc)
	{
	public:
		
		TEST_METHOD(GetFileFormat)
		{
			CString filePath = getFileFormat("D:\\test\\test.obj");
			Assert::AreEqual(filePath, _T("obj"));
		}
	};
}
