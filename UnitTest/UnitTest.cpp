#include "pch.h"
#include "CppUnitTest.h"
#include "../SynthDefect/GlobMyProc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestMyProc)
	{
	public:
		
		TEST_METHOD(TestGetFileFormat)
		{
			CString filePath = getFileFormat("D:\\test\\test.obj");
			Assert::AreEqual(filePath, _T("obj"));
		}

		TEST_METHOD(TestMyFileOpen)
		{
			FILE* file = MyFileOpen(_T("D:\Research\UnitTest\testMesh.obj"), _T("r"));
			Assert::IsNull(file, _T("File not found"));
		}
	};
}
