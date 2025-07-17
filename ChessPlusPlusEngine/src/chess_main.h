//---------------------------------------------------------------------------
#ifndef CHESS_MAIN_HEADER
#define CHESS_MAIN_HEADER
//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif

__declspec (dllexport) int  chess_test();
__declspec (dllexport) bool play_move(int ox, int oy, int dx, int dy);
__declspec (dllexport) bool validate_move(int ox, int oy, int dx, int dy);
__declspec (dllexport) bool init_game();
__declspec (dllexport) bool get_piece(int x, int y, int* team, int* type);
__declspec (dllexport) void get_next_move(int team, int* ox, int* oy, int* dx, int* dy);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
#endif