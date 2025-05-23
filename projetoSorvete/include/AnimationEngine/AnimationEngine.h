#pragma once

#include <windows.h>
#include "../StateModel.h"
#include "../Controllers/ProcessController.h"

class AnimationEngine
{
public:
  static AnimationEngine &GetInstance();

  // Controle de animação
  bool Start();  // Inicia o motor de animação
  bool Stop();   // Para o motor de animação
  bool Pause();  // Pausa as animações
  bool Resume(); // Retoma as animações

  // Renderização
  void Render(HDC hdc); // Renderiza o estado atual

private:
  AnimationEngine();
  ~AnimationEngine();
  AnimationEngine(const AnimationEngine &) = delete;
  AnimationEngine &operator=(const AnimationEngine &) = delete;

  // Constantes de desenho
  static const int TANK_WIDTH = 100;
  static const int TANK_HEIGHT = 200;
  static const int TANK_SPACING = 120;
  static const int VALVE_SIZE = 20;
  static const int SENSOR_SIZE = 15;

  // Métodos de desenho
  void DrawTank(HDC hdc, int x, int y, float level, float capacity, const wchar_t *label);
  void DrawValve(HDC hdc, int x, int y, bool isOpen);
  void DrawSensor(HDC hdc, int x, int y, float value, float minValue, float maxValue, const wchar_t *label);
  void DrawText(HDC hdc, int x, int y, const wchar_t *text);

  static AnimationEngine *instance;
};