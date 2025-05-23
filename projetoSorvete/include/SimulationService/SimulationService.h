#pragma once

#include <windows.h>
#include "../StateModel.h"
#include "../HardwareComms/HardwareComms.h"

class SimulationService
{
public:
  static SimulationService &GetInstance();

  // Controle da simulação
  bool Start();  // Inicia a simulação
  bool Stop();   // Para a simulação
  bool Pause();  // Pausa a simulação
  bool Resume(); // Retoma a simulação

  // Atualização do estado
  void Update(); // Atualiza o estado da simulação

private:
  SimulationService();
  ~SimulationService() = default;
  SimulationService(const SimulationService &) = delete;
  SimulationService &operator=(const SimulationService &) = delete;

  // Constantes da simulação
  static const float FLOW_RATE;        // Taxa de fluxo em L/s
  static const float TEMP_CHANGE_RATE; // Taxa de mudança de temperatura em °C/s
  static const float PH_CHANGE_RATE;   // Taxa de mudança de pH por segundo

  // Métodos auxiliares
  void UpdateTanks();   // Atualiza os níveis dos tanques
  void UpdateSensors(); // Atualiza os valores dos sensores
  void UpdateValves();  // Atualiza o estado das válvulas

  static SimulationService instance;
};