#pragma once

#include <windows.h>

class TimerService
{
public:
  static TimerService &GetInstance();

  bool StartTimer(HWND hwnd, UINT_PTR id, UINT interval);
  bool StopTimer(HWND hwnd, UINT_PTR id);

private:
  TimerService() = default;
  ~TimerService() = default;
  TimerService(const TimerService &) = delete;
  TimerService &operator=(const TimerService &) = delete;

  static TimerService instance;
};