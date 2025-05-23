#include <windows.h>
#include "../include/App.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
  auto &app = App::GetInstance();
  if (!app.Init())
  {
    return 1;
  }
  app.Run();
  return 0;
}