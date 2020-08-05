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
			std::string testFile("D:\\Project\\Research\\SynthDefect\\UnitTest\\test.vs");
			std::string result = RetrieveCode(testFile.c_str());
			Assert::IsFalse(result.empty());
		}
	};
}
