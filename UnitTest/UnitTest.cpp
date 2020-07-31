#include "pch.h"
#include "CppUnitTest.h"
#include "../SynthDefect/GlobMyProc.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestMyProc)
	{
	public:
		TEST_METHOD(TestConvertStdString)
		{
			LPCTSTR target = _T("test");
			std::string compare("test");
			std::string result = ConvertStdString(target);
			Assert::AreEqual(result, compare);
		}

		TEST_METHOD(TestRetrieveCode)
		{
			const char* result = RetrieveCode("D:\Project\Research\SynthDefect\UnitTest\\test.vs");
			Assert::IsNull(result);
		}
	};
}
