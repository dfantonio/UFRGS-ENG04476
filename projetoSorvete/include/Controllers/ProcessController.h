#pragma once

#include "../StateModel.h"
#include "../HardwareComms/HardwareComms.h"
#include "../SimulationService/SimulationService.h"

class ProcessController
{
public:
  static ProcessController &GetInstance();

  // Controle do processo
  bool StartProcess();  // Inicia o processo de fabricação
  bool StopProcess();   // Para o processo
  bool PauseProcess();  // Pausa o processo
  bool ResumeProcess(); // Retoma o processo

  // Controle de válvulas
  bool OpenValve(int valveId);  // Abre uma válvula específica
  bool CloseValve(int valveId); // Fecha uma válvula específica

  // Monitoramento
  float GetTankLevel(int tankId); // Retorna o nível de um tanque
  float GetTemperature();         // Retorna a temperatura atual
  float GetPH();                  // Retorna o pH atual
  bool IsValveOpen(int valveId);  // Verifica se uma válvula está aberta

  // Constantes para identificação de tanques
  static const int TANK_BASE = 0;
  static const int TANK_FLAVOR1 = 1;
  static const int TANK_FLAVOR2 = 2;
  static const int TANK_OUTPUT = 3;

  // Constantes para identificação de válvulas
  static const int VALVE_BASE = HardwareComms::PIN_BASE_VALVE;
  static const int VALVE_FLAVOR1 = HardwareComms::PIN_FLAVOR1_VALVE;
  static const int VALVE_FLAVOR2 = HardwareComms::PIN_FLAVOR2_VALVE;
  static const int VALVE_OUTPUT = HardwareComms::PIN_OUTPUT_VALVE;

private:
  ProcessController();
  ~ProcessController() = default;
  ProcessController(const ProcessController &) = delete;
  ProcessController &operator=(const ProcessController &) = delete;

  static ProcessController instance;
};