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

			// Invalid Squares
			Assert::AreEqual(false, validate_move(-1, -1, 0, 0));
			Assert::AreEqual(false, validate_move(8, 8, 0, 0));

			// Knight (a2) possible moves
			for (int x = 0; x < 8; x++)
				for (int y = 0; y < 8; y++)
					if ((x == 0 && y == 2) || (x == 2 && y == 2))
						Assert::AreEqual(true, validate_move(1, 0, x, y));
					else
						Assert::AreEqual(false, validate_move(1, 0, x, y));
		}

		TEST_METHOD(MoveMaking)
		{
			Assert::AreEqual(true, init_game());

			Assert::AreEqual(false, play_move(1, 0, 0, 0));
			Assert::AreEqual(false, play_move(1, 0, 1, 0));
			Assert::AreEqual(false, play_move(0, 0, 0, 1));
		}

		TEST_METHOD(MoveGeneration)
		{
			Assert::AreEqual(true, init_game());

			int moves[128];
			int count = 0;

			// Pawn
			get_valid_moves(0, 1, moves, &count);

			Assert::AreEqual(3, count);

			Assert::AreEqual(0, moves[0]);
			Assert::AreEqual(2, moves[1]);

			Assert::AreEqual(0, moves[2]);
			Assert::AreEqual(3, moves[3]);

			Assert::AreEqual(1, moves[4]);
			Assert::AreEqual(2, moves[5]); // REMOVE THESE

			// Knight
			get_valid_moves(1, 0, moves, &count);

			Assert::AreEqual(2, count);

			Assert::AreEqual(0, moves[0]);
			Assert::AreEqual(2, moves[1]);

			Assert::AreEqual(2, moves[2]);
			Assert::AreEqual(2, moves[3]);
		}
	};
}
