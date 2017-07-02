
#include "window_ctl.h"
#include "Arduino.h"
#include "Wire.h"

enum RelayMask {
  W4D = 0x0001, W4U = 0x0002, W3D = 0x0004, W3U = 0x0008,
  W2D = 0x0010, W2U = 0x0020, W1D = 0x0040, W1U = 0x0080,
  W5U = 0x0100, W5D = 0x0200, W6U = 0x0400, W6D = 0x0800,
  W7U = 0x1000, W7D = 0x2000, W8U = 0x4000, W8D = 0x8000
};

uint8_t module_addr[kNrModules] = { 0x20, 0x21 };

const uint16_t gp_bit_mask[kNrWindowsPerModule][2] = {
  {W1U, W1D},
  {W2U, W2D},
  {W3U, W3D},
  {W4U, W4D},
  {W5U, W5D},
  {W6U, W6D},
  {W7U, W7D},
  {W8U, W8D},
};


WindowControl::Setup() {
  last_time_ = 0;
  for (int i = 0; i < kMaxNrWindows; ++i) {
    run_timer_[i] = 0;
    pause_timer_[i] = 0;
    next_dir_[i] = kDirStop;
    current_dir_[i] = kDirStop;
  }

  Wire.begin();
  for (int i = 0; i < kNrModules; ++i) {
    gp_reg[i] = 0x0000;

    Wire.beginTransmission(module_addr[i]);
    Wire.write(0x06);            // command byte (0x06 = IODIR0)
    Wire.write(0x00);            // data byte 0 (all output)
    Wire.write(0x00);            // data byte 1 (all output)
    Wire.endTransmission();      // stop transmitting

    Wire.beginTransmission(module_addr[i]);
    Wire.write(0x00);            // command byte (0x00 = GP0)
    Wire.write(0x00);            // data byte 0 (all off)
    Wire.write(0x00);            // data byte 1 (all off)
    Wire.endTransmission();      // stop transmitting
  }
}

void WindowControl::ExecuteAction(const WindowAction *action, bool stop) {
  WinCtlDirection direction;
  if (stop)
    direction = kDirStop;
  else
    direction = action->dir;

  if (direction == kDirNone)
    return;

  for (int i = 0; i < kMaxNrWindows; ++i) {
    if (action->windows[i]) {
      // set timer and direction for window
      next_dir_[i] = direction;
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
      if (current_dir_[i] == kDirStop) {
        SendCommand(i, next_dir_[i]);
        current_dir_[i] = next_dir_[i];
      } else {
        // this is a direction change - enter pause
        SendCommand(i, kDirStop);
        current_dir_[i] = kDirStop;
        pause_timer_[i] = kPauseTime;
      }
    }
  }
}

void WindowControl::SendCommand(int window_nr, WinCtlDirection dir) {
  char line[50];

  snprintf(line, 50, "  win %d: %s", window_nr,
      dir == kDirDown ? "down" :
      dir == kDirUp   ? "up" :
      dir == kDirStop ? "stop" : "error!!");
  Serial.println(line);

  int mod_nr = window_nr / kNrWindowsPerModule;
  int win_id = window_nr % kNrWindowsPerModule;
  uint8_t i2c_addr = module_addr[mod_nr];
  switch (dir) {
    case kDirUp:
      gp_reg[mod_nr] |= gp_bit_mask[win_id][kDirUp];
      gp_reg[mod_nr] &= ~gp_bit_mask[win_id][kDirDown];
      break;
    case kDirDown:
      gp_reg[mod_nr] |= gp_bit_mask[win_id][kDirDown];
      gp_reg[mod_nr] &= ~gp_bit_mask[win_id][kDirUp];
      break;
    default:
      gp_reg[mod_nr] &= ~gp_bit_mask[win_id][kDirDown];
      gp_reg[mod_nr] &= ~gp_bit_mask[win_id][kDirUp];
      break;
  }

  uint8_t gp0 = gp_reg[mod_nr] & 0x00ff;
  uint8_t gp1 = (gp_reg[mod_nr] >> 8) & 0x00ff;
  Wire.beginTransmission(i2c_addr);
  Wire.write(0x00);            // command byte (0x00 = general purpose register 0)
  Wire.write(gp0);             // data byte 0
  Wire.write(gp1);             // data byte 1
  Wire.endTransmission();      // stop transmitting
}

