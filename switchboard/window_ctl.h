
#include <inttypes.h>

const int kNrModules = 2;
const int kNrWindowsPerModule = 8;
const int kMaxNrWindows = 16;
const long kMaxRunTime = 20000;     // maximum time to run in one direction (in ms)
const long kPauseTime = 100;        // maximum time to pause between a direction change (in ms)

#define WINDOW_ACTION_IGNORE                                          \
  {                                                                                      \
    /*                 Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */   \
    /* Single Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    /* Double Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    /* Triple Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
  }

enum WinCtlDirection {
  kDirNone = -1,  // used to disable this action
  kDirUp   =  0,
  kDirDown =  1,
  kDirStop =  2
};

struct WindowAction {
  int8_t dir;
  int8_t windows[kMaxNrWindows];  // if we get memory problems, this could be converted to a bit array
};

class WindowControl {
 public:
  Setup();
  void Update();
  void ExecuteAction(const WindowAction *action, bool stop);

 private:
  uint16_t gp_reg[kNrModules];   // actual value of the general purpuse registers GP0 (lower 8 bits) and GP1 (upper 8 bits)
  long last_time_;
  long run_timer_[kMaxNrWindows];
  long pause_timer_[kMaxNrWindows];
  WinCtlDirection next_dir_[kMaxNrWindows];
  WinCtlDirection current_dir_[kMaxNrWindows];
  void SendCommand(int window_nr, WinCtlDirection dir);
};

