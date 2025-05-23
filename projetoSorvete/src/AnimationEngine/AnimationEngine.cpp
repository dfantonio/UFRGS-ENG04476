#include "../../include/AnimationEngine/AnimationEngine.h"
#include <stdio.h>

AnimationEngine *AnimationEngine::instance = nullptr;

AnimationEngine &AnimationEngine::GetInstance()
{
  if (!instance)
  {
    instance = new AnimationEngine();
  }
  return *instance;
}

AnimationEngine::AnimationEngine()
{
}

AnimationEngine::~AnimationEngine()
{
  if (instance)
  {
    delete instance;
    instance = nullptr;
  }
}

bool AnimationEngine::Start()
{
  OutputDebugStringA("AnimationEngine: Iniciado\n");
  return true;
}

bool AnimationEngine::Stop()
{
  OutputDebugStringA("AnimationEngine: Parado\n");
  return true;
}

bool AnimationEngine::Pause()
{
  OutputDebugStringA("AnimationEngine: Pausado\n");
  return true;
}

bool AnimationEngine::Resume()
{
  OutputDebugStringA("AnimationEngine: Retomado\n");
  return true;
}

void AnimationEngine::Render(HDC hdc)
{
  auto &controller = ProcessController::GetInstance();
  int startX = 50;
  int startY = 100;

  // Desenhar tanques
  DrawTank(hdc, startX, startY,
           controller.GetTankLevel(ProcessController::TANK_BASE), 100.0f, L"Base");
  DrawTank(hdc, startX + TANK_SPACING, startY,
           controller.GetTankLevel(ProcessController::TANK_FLAVOR1), 30.0f, L"Sabor 1");
  DrawTank(hdc, startX + TANK_SPACING * 2, startY,
           controller.GetTankLevel(ProcessController::TANK_FLAVOR2), 30.0f, L"Sabor 2");
  DrawTank(hdc, startX + TANK_SPACING * 3, startY,
           controller.GetTankLevel(ProcessController::TANK_OUTPUT), 40.0f, L"Saída");

  // Desenhar válvulas
  DrawValve(hdc, startX + TANK_WIDTH / 2, startY + TANK_HEIGHT + 10,
            controller.IsValveOpen(ProcessController::VALVE_BASE));
  DrawValve(hdc, startX + TANK_SPACING + TANK_WIDTH / 2, startY + TANK_HEIGHT + 10,
            controller.IsValveOpen(ProcessController::VALVE_FLAVOR1));
  DrawValve(hdc, startX + TANK_SPACING * 2 + TANK_WIDTH / 2, startY + TANK_HEIGHT + 10,
            controller.IsValveOpen(ProcessController::VALVE_FLAVOR2));
  DrawValve(hdc, startX + TANK_SPACING * 3 + TANK_WIDTH / 2, startY + TANK_HEIGHT + 10,
            controller.IsValveOpen(ProcessController::VALVE_OUTPUT));

  // Desenhar sensores
  DrawSensor(hdc, startX + TANK_SPACING * 3 + TANK_WIDTH + 20, startY,
             controller.GetTemperature(), 0.0f, 100.0f, L"Temp");
  DrawSensor(hdc, startX + TANK_SPACING * 3 + TANK_WIDTH + 20, startY + 50,
             controller.GetPH(), 0.0f, 14.0f, L"pH");
}

void AnimationEngine::DrawTank(HDC hdc, int x, int y, float level, float capacity, const wchar_t *label)
{
  // Desenhar contorno do tanque
  Rectangle(hdc, x, y, x + TANK_WIDTH, y + TANK_HEIGHT);

  // Calcular altura do nível
  int levelHeight = (int)((level / capacity) * TANK_HEIGHT);

  // Desenhar nível do tanque
  HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Rectangle(hdc, x + 2, y + TANK_HEIGHT - levelHeight, x + TANK_WIDTH - 2, y + TANK_HEIGHT - 2);
  SelectObject(hdc, hOldBrush);
  DeleteObject(hBrush);

  // Desenhar label
  DrawText(hdc, x, y - 20, label);
}

void AnimationEngine::DrawValve(HDC hdc, int x, int y, bool isOpen)
{
  HBRUSH hBrush = CreateSolidBrush(isOpen ? RGB(0, 255, 0) : RGB(255, 0, 0));
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Ellipse(hdc, x - VALVE_SIZE / 2, y, x + VALVE_SIZE / 2, y + VALVE_SIZE);
  SelectObject(hdc, hOldBrush);
  DeleteObject(hBrush);
}

void AnimationEngine::DrawSensor(HDC hdc, int x, int y, float value, float minValue, float maxValue, const wchar_t *label)
{
  // Desenhar contorno do sensor
  Rectangle(hdc, x, y, x + SENSOR_SIZE, y + SENSOR_SIZE);

  // Calcular cor baseada no valor
  int r = (int)((value - minValue) / (maxValue - minValue) * 255);
  int g = 255 - r;
  HBRUSH hBrush = CreateSolidBrush(RGB(r, g, 0));
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
  Rectangle(hdc, x + 1, y + 1, x + SENSOR_SIZE - 1, y + SENSOR_SIZE - 1);
  SelectObject(hdc, hOldBrush);
  DeleteObject(hBrush);

  // Desenhar label
  DrawText(hdc, x, y - 20, label);
}

void AnimationEngine::DrawText(HDC hdc, int x, int y, const wchar_t *text)
{
  TextOutW(hdc, x, y, text, wcslen(text));
}