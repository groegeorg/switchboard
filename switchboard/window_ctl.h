

const int kMaxNrWindows = 16;
const long kMaxRunTime = 20000;     // maximum time to run in one direction (in ms)
const long kPauseTime = 100;

#define WINDOW_ACTION_IGNORE                                          \
  {                                                                   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },   \
    { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }    \
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
  WindowControl(int nr_of_windows);
  void Update();
  void ExecuteAction(const WindowAction *action);

 private:
  int nr_windows_;
  long last_time_;
  long run_timer_[kMaxNrWindows];
  long pause_timer_[kMaxNrWindows];
  WinCtlDirection next_dir_[kMaxNrWindows];
  WinCtlDirection current_dir_[kMaxNrWindows];
};

