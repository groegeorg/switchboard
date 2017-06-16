#include <CommandLine.h>
#include <ClickButton.h>
#include "window_ctl.h"


const char *eventString(int event);
void handleClickEvent(int button_id, int event);


WindowControl win_ctl;

// ClickButton settings
const int kNrButtons = 3;
ClickButton buttons[kNrButtons] = {
  ClickButton(4, LOW, CLICKBTN_PULLUP),
  ClickButton(5, LOW, CLICKBTN_PULLUP),
  ClickButton(6, LOW, CLICKBTN_PULLUP)
};


WindowAction action_mapping[kNrButtons][kClickEventMax] = {
  { // Button 0
    /*                                 Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */
    /* kClickEventSingle            */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventDouble            */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventTriple            */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventSingleLong        */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventDoubleLong        */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventTripleLong        */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventSingleLongRelease */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventDoubleLongRelease */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* kClickEventTripleLongRelease */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} }
  },
  WINDOW_ACTION_IGNORE,
  WINDOW_ACTION_IGNORE
};


// CommandLine settings
CommandLine commandLine(Serial, "> ");
Command cmdPrint = Command("print", &cmdHandlePrint);


void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  commandLine.add(cmdPrint);

  // Setup button timers (all in milliseconds)
  for (int i = 0; i < kNrButtons; ++i) {
    buttons[i].debounceTime   = 20;   // Debounce timer in ms
    buttons[i].multiclickTime = 250;  // Time limit for multi clicks
    buttons[i].longClickTime  = 600;  // time until "held-down clicks" register
  }
}


void loop() {
  commandLine.update();

  for (int i = 0; i < kNrButtons; ++i) {
    buttons[i].Update();
    handleClickEvent(i, buttons[i].event);
  }

  delay(5);
}

void handleClickEvent(int button_id, ClickEvent event) {
  char line[50];
  if (event == kClickEventNone)
    return;  // no event

  snprintf(line, 50, "Event on button %d: %s", button_id,
      eventString(event));
  Serial.println(line);

  if (button_id >= 0 && button_id < kNrButtons &&
      event >= 0 && event < kClickEventMax) {
    win_ctl.ExecuteAction(&action_mapping[button_id][event]);
  }
}

const char *eventString(ClickEvent event) {
  switch (event) {
    case kClickEventNone:               return "none";
    case kClickEventSingle:             return "single click";
    case kClickEventDouble:             return "double click";
    case kClickEventTriple:             return "triple click";
    case kClickEventSingleLong:         return "single long click";
    case kClickEventDoubleLong:         return "double long click";
    case kClickEventTripleLong:         return "triple long click";
    case kClickEventSingleLongRelease:  return "single long release";
    case kClickEventDoubleLongRelease:  return "double long release";
    case kClickEventTripleLongRelease:  return "triple long release";
    default:                            return "invalid";
  }
}

void cmdHandlePrint(char* tokens) {
  Serial.println("Hello World!");
}

// vim: set ft=c:
