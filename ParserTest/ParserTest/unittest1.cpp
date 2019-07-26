#include "stdafx.h"
#include "CppUnitTest.h"
#include "RMC_Parser.h" //А вот и библиотека для функции

#pragma comment(lib, "LibRMCParser.lib")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParserTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		std::string escape = "$GPRMC,081218.77,A,5165.48,N,03922.58,E,0.x,x.x,240719,x.x,a,a*hh<CR><LF>";

		TEST_METHOD(TestMethod1)
		{
			std::string Date = "240719";
			RMC Typical = Parse_RMC(escape);
			Assert::AreEqual(Date, Parse_RMC(escape).Date);
		}
		TEST_METHOD(TestMethod2)
		{
			std::string Time = "081218.77";
			RMC Typical = Parse_RMC(escape);
			Assert::AreEqual(Time, Parse_RMC(escape).Time);
		}
		TEST_METHOD(TestMethod3)
		{
			std::string Latitude = "5165.48";
			RMC Typical = Parse_RMC(escape);
			Assert::AreEqual(Latitude, Parse_RMC(escape).Latitude);
		}
		TEST_METHOD(TestMethod4)
		{
			std::string Longitude = "03922.58";
			RMC Typical = Parse_RMC(escape);
			Assert::AreEqual(Longitude, Parse_RMC(escape).Longitude);
		}
	};
}