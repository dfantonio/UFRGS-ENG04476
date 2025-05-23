#pragma once

#include <windows.h>

// Estrutura para representar um tanque
struct Tank
{
  float level;    // Nível atual em litros
  float capacity; // Capacidade máxima em litros
  bool valveOpen; // Estado da válvula (aberta/fechada)
};

// Estrutura para representar um sensor
struct Sensor
{
  float value;    // Valor atual do sensor
  float minValue; // Valor mínimo
  float maxValue; // Valor máximo
};

class StateModel
{
public:
  static StateModel &GetInstance();

  // Tanques
  Tank baseTank;    // Tanque de base (100L)
  Tank flavorTank1; // Tanque de sabor 1 (30L)
  Tank flavorTank2; // Tanque de sabor 2 (30L)
  Tank outputTank;  // Tanque de saída (40L)

  // Sensores
  Sensor temperature; // Sensor de temperatura (0-100°C)
  Sensor ph;          // Sensor de pH (0-14)

  // Flags de estado
  bool isRunning; // Simulação está rodando
  bool isPaused;  // Simulação está pausada

private:
  StateModel();
  ~StateModel() = default;
  StateModel(const StateModel &) = delete;
  StateModel &operator=(const StateModel &) = delete;

  static StateModel instance;
};