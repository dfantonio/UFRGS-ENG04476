#include "../include/MainWindow.h"
#include "../include/AnimationEngine/AnimationEngine.h"
#include <windowsx.h>
#include <stdio.h>

MainWindow *MainWindow::instance = nullptr;
const wchar_t *MainWindow::WINDOW_CLASS_NAME = L"MainWindowClass";
const wchar_t *MainWindow::WINDOW_TITLE = L"Simulador de Fábrica de Sorvete";

MainWindow &MainWindow::GetInstance()
{
  if (!instance)
  {
    instance = new MainWindow();
  }
  return *instance;
}

MainWindow::MainWindow() : hwnd(NULL)
{
}

MainWindow::~MainWindow()
{
  if (instance)
  {
    delete instance;
    instance = nullptr;
  }
}

bool MainWindow::Create()
{
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.lpfnWndProc = WndProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = WINDOW_CLASS_NAME;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);

  if (!RegisterClassEx(&wc))
  {
    return false;
  }

  hwnd = CreateWindowEx(
      0,
      WINDOW_CLASS_NAME,
      WINDOW_TITLE,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      800, 600,
      NULL,
      NULL,
      GetModuleHandle(NULL),
      NULL);

  if (!hwnd)
  {
    return false;
  }

  CreateControls();
  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  return true;
}

void MainWindow::CreateControls()
{
  // Botões de controle do processo
  hwndStartButton = CreateWindow(L"BUTTON", L"Iniciar",
                                 WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                 10, 10, 100, 30, hwnd, (HMENU)1, NULL, NULL);

  hwndStopButton = CreateWindow(L"BUTTON", L"Parar",
                                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                120, 10, 100, 30, hwnd, (HMENU)2, NULL, NULL);

  hwndPauseButton = CreateWindow(L"BUTTON", L"Pausar",
                                 WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                 230, 10, 100, 30, hwnd, (HMENU)3, NULL, NULL);

  hwndResumeButton = CreateWindow(L"BUTTON", L"Retomar",
                                  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                  340, 10, 100, 30, hwnd, (HMENU)4, NULL, NULL);

  // Botões de controle das válvulas
  hwndBaseValveButton = CreateWindow(L"BUTTON", L"Válvula Base",
                                     WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                     10, 50, 100, 30, hwnd, (HMENU)5, NULL, NULL);

  hwndFlavor1ValveButton = CreateWindow(L"BUTTON", L"Válvula Sabor 1",
                                        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                        120, 50, 100, 30, hwnd, (HMENU)6, NULL, NULL);

  hwndFlavor2ValveButton = CreateWindow(L"BUTTON", L"Válvula Sabor 2",
                                        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                        230, 50, 100, 30, hwnd, (HMENU)7, NULL, NULL);

  hwndOutputValveButton = CreateWindow(L"BUTTON", L"Válvula Saída",
                                       WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                       340, 50, 100, 30, hwnd, (HMENU)8, NULL, NULL);

  // Labels para exibição de valores
  hwndBaseTankLevel = CreateWindow(L"STATIC", L"Tanque Base: 0.0 L",
                                   WS_VISIBLE | WS_CHILD,
                                   10, 90, 200, 20, hwnd, NULL, NULL, NULL);

  hwndFlavor1TankLevel = CreateWindow(L"STATIC", L"Tanque Sabor 1: 0.0 L",
                                      WS_VISIBLE | WS_CHILD,
                                      10, 120, 200, 20, hwnd, NULL, NULL, NULL);

  hwndFlavor2TankLevel = CreateWindow(L"STATIC", L"Tanque Sabor 2: 0.0 L",
                                      WS_VISIBLE | WS_CHILD,
                                      10, 150, 200, 20, hwnd, NULL, NULL, NULL);

  hwndOutputTankLevel = CreateWindow(L"STATIC", L"Tanque Saída: 0.0 L",
                                     WS_VISIBLE | WS_CHILD,
                                     10, 180, 200, 20, hwnd, NULL, NULL, NULL);

  hwndTemperature = CreateWindow(L"STATIC", L"Temperatura: 0.0 °C",
                                 WS_VISIBLE | WS_CHILD,
                                 10, 210, 200, 20, hwnd, NULL, NULL, NULL);

  hwndPH = CreateWindow(L"STATIC", L"pH: 0.0",
                        WS_VISIBLE | WS_CHILD,
                        10, 240, 200, 20, hwnd, NULL, NULL, NULL);
}

void MainWindow::UpdateDisplay()
{
  auto &controller = ProcessController::GetInstance();
  char buffer[256];

  // Atualizar níveis dos tanques
  sprintf_s(buffer, "Tanque Base: %.1f L", controller.GetTankLevel(ProcessController::TANK_BASE));
  SetWindowTextA(hwndBaseTankLevel, buffer);

  sprintf_s(buffer, "Tanque Sabor 1: %.1f L", controller.GetTankLevel(ProcessController::TANK_FLAVOR1));
  SetWindowTextA(hwndFlavor1TankLevel, buffer);

  sprintf_s(buffer, "Tanque Sabor 2: %.1f L", controller.GetTankLevel(ProcessController::TANK_FLAVOR2));
  SetWindowTextA(hwndFlavor2TankLevel, buffer);

  sprintf_s(buffer, "Tanque Saída: %.1f L", controller.GetTankLevel(ProcessController::TANK_OUTPUT));
  SetWindowTextA(hwndOutputTankLevel, buffer);

  // Atualizar temperatura e pH
  sprintf_s(buffer, "Temperatura: %.1f °C", controller.GetTemperature());
  SetWindowTextA(hwndTemperature, buffer);

  sprintf_s(buffer, "pH: %.1f", controller.GetPH());
  SetWindowTextA(hwndPH, buffer);

  InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_COMMAND:
  {
    auto &controller = ProcessController::GetInstance();
    switch (LOWORD(wParam))
    {
    case 1: // Start
      controller.StartProcess();
      break;
    case 2: // Stop
      controller.StopProcess();
      break;
    case 3: // Pause
      controller.PauseProcess();
      break;
    case 4: // Resume
      controller.ResumeProcess();
      break;
    case 5: // Base Valve
      if (controller.IsValveOpen(ProcessController::VALVE_BASE))
      {
        controller.CloseValve(ProcessController::VALVE_BASE);
      }
      else
      {
        controller.OpenValve(ProcessController::VALVE_BASE);
      }
      break;
    case 6: // Flavor 1 Valve
      if (controller.IsValveOpen(ProcessController::VALVE_FLAVOR1))
      {
        controller.CloseValve(ProcessController::VALVE_FLAVOR1);
      }
      else
      {
        controller.OpenValve(ProcessController::VALVE_FLAVOR1);
      }
      break;
    case 7: // Flavor 2 Valve
      if (controller.IsValveOpen(ProcessController::VALVE_FLAVOR2))
      {
        controller.CloseValve(ProcessController::VALVE_FLAVOR2);
      }
      else
      {
        controller.OpenValve(ProcessController::VALVE_FLAVOR2);
      }
      break;
    case 8: // Output Valve
      if (controller.IsValveOpen(ProcessController::VALVE_OUTPUT))
      {
        controller.CloseValve(ProcessController::VALVE_OUTPUT);
      }
      else
      {
        controller.OpenValve(ProcessController::VALVE_OUTPUT);
      }
      break;
    }
    break;
  }
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    AnimationEngine::GetInstance().Render(hdc);
    EndPaint(hwnd, &ps);
    break;
  }
  case WM_TIMER:
    if (wParam == 2)
    { // Timer de atualização da interface (20ms)
      GetInstance().UpdateDisplay();
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}