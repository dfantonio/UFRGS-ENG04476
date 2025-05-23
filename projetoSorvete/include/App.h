#pragma once

#include <windows.h>
#include "MainWindow.h"
#include "Services/TimerService.h"
#include "HardwareComms/HardwareComms.h"
#include "SimulationService/SimulationService.h"

class App
{
public:
  static App &GetInstance();
  bool Init();
  void Run();

private:
  App();
  ~App();
  App(const App &) = delete;
  App &operator=(const App &) = delete;

  static App *instance;

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
  bool RegisterWindowClass();

  // Timer de 5ms para comunicação
  static const UINT_PTR TIMER_COMM_ID = 1;
  static const UINT TIMER_COMM_INTERVAL = 5;
  int commTickCount = 0;

  // Timer de 20ms para animação
  static const UINT_PTR TIMER_ANIM_ID = 2;
  static const UINT TIMER_ANIM_INTERVAL = 20;
  int animTickCount = 0;
};