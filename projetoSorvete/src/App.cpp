#include "../include/App.h"
#include "../include/MainWindow.h"
#include "../include/Services/TimerService.h"
#include "../include/SimulationService/SimulationService.h"
#include "../include/HardwareComms/HardwareComms.h"
#include <stdio.h>

App *App::instance = nullptr;

App &App::GetInstance()
{
  if (!instance)
  {
    instance = new App();
  }
  return *instance;
}

App::App()
{
}

App::~App()
{
  if (instance)
  {
    delete instance;
    instance = nullptr;
  }
}

bool App::Init()
{
  // Criar janela principal
  if (!MainWindow::GetInstance().Create())
  {
    return false;
  }

  // Iniciar timers
  TimerService::GetInstance().StartTimer(MainWindow::GetInstance().GetHandle(), TIMER_COMM_ID, TIMER_COMM_INTERVAL); // Timer de comunicação
  TimerService::GetInstance().StartTimer(MainWindow::GetInstance().GetHandle(), TIMER_ANIM_ID, TIMER_ANIM_INTERVAL); // Timer de atualização da interface

  // Iniciar simulação
  SimulationService::GetInstance().Start();

  return true;
}

void App::Run()
{
  MSG msg = {0};
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

bool App::RegisterWindowClass()
{
  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.lpszClassName = MainWindow::WINDOW_CLASS_NAME;

  return RegisterClassEx(&wc) != 0;
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (instance)
  {
    switch (msg)
    {
    case WM_TIMER:
      if (wParam == TIMER_COMM_ID)
      {
        instance->commTickCount++;
        // Ler entradas do hardware
        HardwareComms::GetInstance().ReadInputs();
        // Atualizar simulação
        SimulationService::GetInstance().Update();
        // Atualizar saídas do hardware
        HardwareComms::GetInstance().WriteOutputs();

        if (instance->commTickCount % 200 == 0)
        { // A cada 1 segundo (200 * 5ms)
          char buffer[100];
          sprintf_s(buffer, "Comm Ticks: %d\n", instance->commTickCount);
          OutputDebugStringA(buffer);
        }
      }
      else if (wParam == TIMER_ANIM_ID)
      {
        instance->animTickCount++;
        if (instance->animTickCount % 50 == 0)
        { // A cada 1 segundo (50 * 20ms)
          char buffer[100];
          sprintf_s(buffer, "Anim Ticks: %d\n", instance->animTickCount);
          OutputDebugStringA(buffer);
        }
      }
      return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}