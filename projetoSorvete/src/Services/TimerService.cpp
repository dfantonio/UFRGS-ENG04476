#include "../../include/Services/TimerService.h"

TimerService TimerService::instance;

TimerService &TimerService::GetInstance()
{
  return instance;
}

bool TimerService::StartTimer(HWND hwnd, UINT_PTR id, UINT interval)
{
  return SetTimer(hwnd, id, interval, NULL) != 0;
}

bool TimerService::StopTimer(HWND hwnd, UINT_PTR id)
{
  return KillTimer(hwnd, id) != 0;
}