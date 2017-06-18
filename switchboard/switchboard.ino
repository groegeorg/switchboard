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
  {
    /* Button 0        Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */
    /* Single Click */ { kDirUp,   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* Double Click */ { kDirUp,   {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1} },
    /* Triple Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
  },
  {
    /* Button 1        Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */
    /* Single Click */ { kDirDown, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* Double Click */ { kDirDown, {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1} },
    /* Triple Click */ { kDirNone, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
  },
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
  win_ctl.Update();

  for (int i = 0; i < kNrButtons; ++i) {
    buttons[i].Update();
    handleClickEvent(i, buttons[i].clickType, buttons[i].clickCount);
  }

  delay(5);
}

void handleClickEvent(int button_id, ClickEventType type, uint8_t count) {
  char line[50];
  if (count == 0)
    return;  // no event

  snprintf(line, 50, "Event on button %d: %d-%s",
           button_id, count, eventString(type));
  Serial.println(line);

  if (button_id >= 0 && button_id < kNrButtons && count > 0 && count <= 3) {
    if (type == kClickEventRelease) {
      win_ctl.ExecuteAction(&action_mapping[button_id][count-1], true);
    } else {
      win_ctl.ExecuteAction(&action_mapping[button_id][count-1], false);
    }
  }
}

const char *eventString(ClickEventType type) {
  switch (type) {
    case kClickEventNone:    return "none";
    case kClickEventClick:   return "click";
    case kClickEventHold:    return "hold";
    case kClickEventRelease: return "release";
    default:                 return "invalid";
  }
}

void cmdHandlePrint(char* tokens) {
  Serial.println("Hello World!");
}

// vim: set ft=c:
