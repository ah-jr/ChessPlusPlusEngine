#include "pch.h"
#include "CppUnitTest.h"
#include "../ChessPlusPlusEngine/src/ChessPlusPlusEngine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ChessPlusPlusEngineTest
{
	TEST_CLASS(ChessPlusPlusEngineTest)
	{
	public:

		TEST_METHOD(TestInitialization)
		{
			Assert::AreEqual(true, init_game());
		}

		TEST_METHOD(MoveValidation)
		{
			Assert::AreEqual(true, init_game());
			Assert::AreEqual(false, validate_move(-1,-1,0,0));
		}
	};
}
