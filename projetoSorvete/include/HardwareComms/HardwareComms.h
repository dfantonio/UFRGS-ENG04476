#pragma once

#include <windows.h>
#include "../StateModel.h"

class HardwareComms
{
public:
  static HardwareComms &GetInstance();

  // Constantes para mapeamento de pinos
  static const int PIN_BASE_VALVE = 1;    // Válvula do tanque base
  static const int PIN_FLAVOR1_VALVE = 2; // Válvula do tanque de sabor 1
  static const int PIN_FLAVOR2_VALVE = 3; // Válvula do tanque de sabor 2
  static const int PIN_OUTPUT_VALVE = 4;  // Válvula do tanque de saída
  static const int PIN_TEMP_SENSOR = 5;   // Sensor de temperatura
  static const int PIN_PH_SENSOR = 6;     // Sensor de pH

  // Leitura de sinais
  bool ReadInputs();              // Lê todos os sinais de entrada (sensores, válvulas)
  bool ReadDigitalInput(int pin); // Lê um sinal digital específico
  float ReadAnalogInput(int pin); // Lê um sinal analógico específico

  // Escrita de sinais
  bool WriteOutputs();                          // Atualiza todos os sinais de saída
  bool WriteDigitalOutput(int pin, bool value); // Escreve um sinal digital
  bool WriteAnalogOutput(int pin, float value); // Escreve um sinal analógico

private:
  HardwareComms();
  ~HardwareComms() = default;
  HardwareComms(const HardwareComms &) = delete;
  HardwareComms &operator=(const HardwareComms &) = delete;

  static HardwareComms instance;
};