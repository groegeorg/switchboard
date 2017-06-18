
#include <inttypes.h>

const int kMaxNrWindows = 16;
const long kMaxRunTime = 20000;     // maximum time to run in one direction (in ms)
const long kPauseTime = 100;

#define WINDOW_ACTION_IGNORE                                          \
  {                                                                                      \
    /* Button 0        Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */   \
    /* Single Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    /* Double Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    /* Triple Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
  }

enum WinCtlDirection {
  kDirNone = -1,  // used to disable this action
  kDirDown = 0,
  kDirUp = 1,
  kDirStop = 2
};

struct WindowAction {
  int8_t dir;
  int8_t windows[kMaxNrWindows];          // if we get memory problems, this could be converted to a bit array
};

class WindowControl {
 public:
  WindowControl();
  void Update();
  void ExecuteAction(const WindowAction *action, bool stop);

 private:
  long last_time_;
  long run_timer_[kMaxNrWindows];
  long pause_timer_[kMaxNrWindows];
  WinCtlDirection next_dir_[kMaxNrWindows];
  WinCtlDirection current_dir_[kMaxNrWindows];
  void SendCommand(int window_nr, WinCtlDirection dir);
};

