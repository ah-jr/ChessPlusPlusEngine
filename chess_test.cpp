#include <windows.h>
#include <iostream>

typedef int (__stdcall *f_funci)();

int main()
{
  HINSTANCE hGetProcIDDLL = LoadLibrary("C:\\Projects\\Chess\\chess.dll");

  if (!hGetProcIDDLL) {
    std::cout << "could not load the dynamic library" << std::endl;
    return EXIT_FAILURE;
  }

  f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "chess_test");
  if (!funci) {
    std::cout << "could not locate the function" << std::endl;
    return EXIT_FAILURE;
  }

  funci();

  return EXIT_SUCCESS;
}