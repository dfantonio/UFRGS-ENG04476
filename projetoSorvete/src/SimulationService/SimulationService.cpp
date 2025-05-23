#include "../../include/SimulationService/SimulationService.h"
#include "../../include/HardwareComms/HardwareComms.h"
#include <stdio.h>

SimulationService SimulationService::instance;

// Definição das constantes estáticas
const float SimulationService::FLOW_RATE = 0.1f;        // Taxa de fluxo em L/s
const float SimulationService::TEMP_CHANGE_RATE = 0.1f; // Taxa de mudança de temperatura em °C/s
const float SimulationService::PH_CHANGE_RATE = 0.01f;  // Taxa de mudança de pH por segundo

SimulationService &SimulationService::GetInstance()
{
  return instance;
}

SimulationService::SimulationService()
{
}

bool SimulationService::Start()
{
  StateModel::GetInstance().isRunning = true;
  StateModel::GetInstance().isPaused = false;
  OutputDebugStringA("SimulationService: Simulação iniciada\n");
  return true;
}

bool SimulationService::Stop()
{
  StateModel::GetInstance().isRunning = false;
  StateModel::GetInstance().isPaused = false;
  OutputDebugStringA("SimulationService: Simulação parada\n");
  return true;
}

bool SimulationService::Pause()
{
  if (StateModel::GetInstance().isRunning)
  {
    StateModel::GetInstance().isPaused = true;
    OutputDebugStringA("SimulationService: Simulação pausada\n");
    return true;
  }
  return false;
}

bool SimulationService::Resume()
{
  if (StateModel::GetInstance().isRunning)
  {
    StateModel::GetInstance().isPaused = false;
    OutputDebugStringA("SimulationService: Simulação retomada\n");
    return true;
  }
  return false;
}

void SimulationService::Update()
{
  if (!StateModel::GetInstance().isRunning || StateModel::GetInstance().isPaused)
  {
    return;
  }

  UpdateTanks();
  UpdateSensors();
  UpdateValves();
}

void SimulationService::UpdateTanks()
{
  auto &state = StateModel::GetInstance();
  float deltaTime = 0.005f; // 5ms em segundos

  // Atualizar tanque base
  if (state.baseTank.valveOpen && state.baseTank.level > 0)
  {
    float flow = FLOW_RATE * deltaTime;
    state.baseTank.level = max(0.0f, state.baseTank.level - flow);
  }

  // Atualizar tanque de sabor 1
  if (state.flavorTank1.valveOpen && state.flavorTank1.level > 0)
  {
    float flow = FLOW_RATE * deltaTime;
    state.flavorTank1.level = max(0.0f, state.flavorTank1.level - flow);
  }

  // Atualizar tanque de sabor 2
  if (state.flavorTank2.valveOpen && state.flavorTank2.level > 0)
  {
    float flow = FLOW_RATE * deltaTime;
    state.flavorTank2.level = max(0.0f, state.flavorTank2.level - flow);
  }

  // Atualizar tanque de saída
  if (state.baseTank.valveOpen || state.flavorTank1.valveOpen || state.flavorTank2.valveOpen)
  {
    float flow = FLOW_RATE * deltaTime;
    state.outputTank.level = min(state.outputTank.capacity, state.outputTank.level + flow);
  }
}

void SimulationService::UpdateSensors()
{
  auto &state = StateModel::GetInstance();
  float deltaTime = 0.005f; // 5ms em segundos

  // Atualizar temperatura
  if (state.baseTank.valveOpen || state.flavorTank1.valveOpen || state.flavorTank2.valveOpen)
  {
    state.temperature.value += TEMP_CHANGE_RATE * deltaTime;
  }
  else
  {
    state.temperature.value -= TEMP_CHANGE_RATE * deltaTime;
  }
  state.temperature.value = max(state.temperature.minValue,
                                min(state.temperature.maxValue, state.temperature.value));

  // Atualizar pH
  if (state.baseTank.valveOpen || state.flavorTank1.valveOpen || state.flavorTank2.valveOpen)
  {
    state.ph.value += PH_CHANGE_RATE * deltaTime;
  }
  else
  {
    state.ph.value -= PH_CHANGE_RATE * deltaTime;
  }
  state.ph.value = max(state.ph.minValue,
                       min(state.ph.maxValue, state.ph.value));
}

void SimulationService::UpdateValves()
{
  auto &state = StateModel::GetInstance();
  auto &hw = HardwareComms::GetInstance();

  // Ler estado das válvulas do hardware
  state.baseTank.valveOpen = hw.ReadDigitalInput(HardwareComms::PIN_BASE_VALVE);
  state.flavorTank1.valveOpen = hw.ReadDigitalInput(HardwareComms::PIN_FLAVOR1_VALVE);
  state.flavorTank2.valveOpen = hw.ReadDigitalInput(HardwareComms::PIN_FLAVOR2_VALVE);
  state.outputTank.valveOpen = hw.ReadDigitalInput(HardwareComms::PIN_OUTPUT_VALVE);

  // Atualizar saídas do hardware
  hw.WriteDigitalOutput(HardwareComms::PIN_BASE_VALVE, state.baseTank.valveOpen);
  hw.WriteDigitalOutput(HardwareComms::PIN_FLAVOR1_VALVE, state.flavorTank1.valveOpen);
  hw.WriteDigitalOutput(HardwareComms::PIN_FLAVOR2_VALVE, state.flavorTank2.valveOpen);
  hw.WriteDigitalOutput(HardwareComms::PIN_OUTPUT_VALVE, state.outputTank.valveOpen);
}