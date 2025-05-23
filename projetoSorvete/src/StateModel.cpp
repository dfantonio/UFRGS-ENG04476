#include "../include/StateModel.h"

StateModel StateModel::instance;

StateModel &StateModel::GetInstance()
{
  return instance;
}

StateModel::StateModel()
{
  // Inicializar tanques
  baseTank.level = 0.0f;
  baseTank.capacity = 100.0f;
  baseTank.valveOpen = false;

  flavorTank1.level = 0.0f;
  flavorTank1.capacity = 30.0f;
  flavorTank1.valveOpen = false;

  flavorTank2.level = 0.0f;
  flavorTank2.capacity = 30.0f;
  flavorTank2.valveOpen = false;

  outputTank.level = 0.0f;
  outputTank.capacity = 40.0f;
  outputTank.valveOpen = false;

  // Inicializar sensores
  temperature.value = 25.0f; // Temperatura ambiente
  temperature.minValue = 0.0f;
  temperature.maxValue = 100.0f;

  ph.value = 7.0f; // pH neutro
  ph.minValue = 0.0f;
  ph.maxValue = 14.0f;

  // Inicializar flags
  isRunning = false;
  isPaused = false;
}