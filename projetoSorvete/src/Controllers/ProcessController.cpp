#include "../../include/Controllers/ProcessController.h"
#include <stdio.h>

ProcessController ProcessController::instance;

ProcessController &ProcessController::GetInstance()
{
  return instance;
}

ProcessController::ProcessController()
{
}

bool ProcessController::StartProcess()
{
  auto &sim = SimulationService::GetInstance();
  return sim.Start();
}

bool ProcessController::StopProcess()
{
  auto &sim = SimulationService::GetInstance();
  return sim.Stop();
}

bool ProcessController::PauseProcess()
{
  auto &sim = SimulationService::GetInstance();
  return sim.Pause();
}

bool ProcessController::ResumeProcess()
{
  auto &sim = SimulationService::GetInstance();
  return sim.Resume();
}

bool ProcessController::OpenValve(int valveId)
{
  auto &hw = HardwareComms::GetInstance();
  return hw.WriteDigitalOutput(valveId, true);
}

bool ProcessController::CloseValve(int valveId)
{
  auto &hw = HardwareComms::GetInstance();
  return hw.WriteDigitalOutput(valveId, false);
}

float ProcessController::GetTankLevel(int tankId)
{
  auto &state = StateModel::GetInstance();
  switch (tankId)
  {
  case TANK_BASE:
    return state.baseTank.level;
  case TANK_FLAVOR1:
    return state.flavorTank1.level;
  case TANK_FLAVOR2:
    return state.flavorTank2.level;
  case TANK_OUTPUT:
    return state.outputTank.level;
  default:
    return 0.0f;
  }
}

float ProcessController::GetTemperature()
{
  return StateModel::GetInstance().temperature.value;
}

float ProcessController::GetPH()
{
  return StateModel::GetInstance().ph.value;
}

bool ProcessController::IsValveOpen(int valveId)
{
  auto &hw = HardwareComms::GetInstance();
  return hw.ReadDigitalInput(valveId);
}