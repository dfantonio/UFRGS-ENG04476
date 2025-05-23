#pragma once

#include <windows.h>
#include "Controllers/ProcessController.h"

class MainWindow
{
public:
  static const wchar_t *WINDOW_CLASS_NAME;
  static const wchar_t *WINDOW_TITLE;
  static const int WINDOW_WIDTH = 800;
  static const int WINDOW_HEIGHT = 600;

  static MainWindow &GetInstance();
  bool Create();
  HWND GetHandle() const { return hwnd; }

private:
  MainWindow();
  ~MainWindow();
  MainWindow(const MainWindow &) = delete;
  MainWindow &operator=(const MainWindow &) = delete;

  static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
  void CreateControls();
  void UpdateDisplay();

  // Handles dos controles
  HWND hwnd;
  HWND hwndStartButton;
  HWND hwndStopButton;
  HWND hwndPauseButton;
  HWND hwndResumeButton;

  // Controles de válvulas
  HWND hwndBaseValveButton;
  HWND hwndFlavor1ValveButton;
  HWND hwndFlavor2ValveButton;
  HWND hwndOutputValveButton;

  // Labels para exibição de valores
  HWND hwndBaseTankLevel;
  HWND hwndFlavor1TankLevel;
  HWND hwndFlavor2TankLevel;
  HWND hwndOutputTankLevel;
  HWND hwndTemperature;
  HWND hwndPH;

  static MainWindow *instance;
};