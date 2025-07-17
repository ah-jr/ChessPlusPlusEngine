#pragma once

#ifdef CHESSPLUSPLUSENGINE_EXPORTS
  #define CHESS_API __declspec(dllexport)
#else
  #define CHESS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

CHESS_API bool play_move(int ox, int oy, int dx, int dy);
CHESS_API bool validate_move(int ox, int oy, int dx, int dy);
CHESS_API bool init_game();
CHESS_API bool get_piece(int x, int y, int* team, int* type);
CHESS_API void get_next_move(int team, int* ox, int* oy, int* dx, int* dy);

#ifdef __cplusplus
}
#endif
