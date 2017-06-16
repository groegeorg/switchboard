
#include "window_ctl.h"

WindowControl::WindowControl(int nr_of_windows) {
  nr_windows_ = nr_of_windows;
  last_time_ = 0;
  for (int i = 0; i < kMaxNrWindows; ++i) {
    run_timer_[i] = 0;
    pause_timer_[i] = 0;
    next_dir_[i] = kDirStop;
    current_dir_[i] = kDirStop;
  }
}

void WindowControl::ExecuteAction(const WindowAction *action) {
  if (action->dir == kDirNone)
    return;

  for (int i = 0; i < kMaxNrWindows; ++i) {
    if (action->windows[i]) {
      // set timer and direction for window
      next_dir_[i] = action->dir
      run_timer_[i] = kMaxRunTime;
    }
  }
}

void WindowControl::Update() {
  long now = (long)millis();      // get current time
  long time_since_last = now - last_time_;
  last_time_ = now;

  for (int i = 0; i < kMaxNrWindows; ++i) {

    // check pause timer
    pause_timer_[i] -= time_since_last;
    if (pause_timer_[i] > 0) {
      continue;  // we are pausing, don't send any command
    } else {
      pause_timer_[i] = 0;
    }

    // check run timer
    if (run_timer_[i] > 0) {
      run_timer_[i] -= time_since_last;
      if (run_timer_[i] <= 0) {
        // timer expired, send stop command
        next_dir_[i] = kDirStop;
        run_timer_[i] = 0;
      }
    }

    // check if we need to send a command
    if (current_dir_[i] != next_dir_[i]) {
      if (current_dir_[i] != kDirStop) {
        // this is a direction change - enter pause
        SendCommand(i, kDirStop);
        current_dir_[i] = kDirStop;
        pause_timer_[i] = kPauseTime;
      } else {
        SendCommand(i, next_dir_[i]);
        current_dir_[i] = next_dir_[i];
      }
    }
  }
}

void SendCommand(int window_nr, WinCtlDirection dir) {

}

