#include <CommandLine.h>
#include <ClickButton.h>
#include "window_ctl.h"

// Prototypes
const char *eventString(int event);
void clickEvent(int button_id, int event);
void cmdHelpClick();


WindowControl win_ctl;

// ClickButton settings
const int kNrButtons = 3;
ClickButton buttons[kNrButtons] = {
  ClickButton(4, LOW, CLICKBTN_PULLUP),
  ClickButton(5, LOW, CLICKBTN_PULLUP),
  ClickButton(6, LOW, CLICKBTN_PULLUP)
};


WinActionGroup action_mapping[kNrButtons][kClickEventMax] = {
  {
    /* Button 0        Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */
    /* Single Click */ { kDirUp,   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* Double Click */ { kDirUp,   {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1} },
    /* Triple Click */ { kDirUp,   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} },
  },
  {
    /* Button 1        Window Nr:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15  */
    /* Single Click */ { kDirDown, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} },
    /* Double Click */ { kDirDown, {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1} },
    /* Triple Click */ { kDirDown, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} },
  },
  WINDOW_ACTION_IGNORE
};


// CommandLine settings
CommandLine commandLine(Serial, "> ");
Command cmdPrint = Command("print", &cmdHandlePrint);
Command cmdClick = Command("click", &cmdHandleClick);


void setup() {
  Serial.begin(9600);
  Serial.print("Starting...");

  commandLine.add(cmdPrint);
  commandLine.add(cmdClick);

  win_ctl.Setup();

  // Setup button timers (all in milliseconds)
  for (int i = 0; i < kNrButtons; ++i) {
    buttons[i].debounceTime   = 20;   // Debounce timer in ms
    buttons[i].multiclickTime = 250;  // Time limit for multi clicks
    buttons[i].longClickTime  = 600;  // time until "held-down clicks" register
  }
  Serial.println("ok");
}


void loop() {
  commandLine.update();
  win_ctl.Update();

  for (int i = 0; i < kNrButtons; ++i) {
    buttons[i].Update();
    clickEvent(i, buttons[i].clickType, buttons[i].clickCount);
  }

  delay(5);
}

void clickEvent(int button_id, ClickEventType type, uint8_t nr_clicks) {
  char line[50];
  if (nr_clicks == 0)
    return;  // no event

  snprintf(line, 50, "Event on button %d: %d-%s",
           button_id, nr_clicks, eventString(type));
  Serial.println(line);

  if (button_id >= 0 && button_id < kNrButtons && nr_clicks <= 3) {
    if (type == kClickEventRelease) {
      win_ctl.SetActionGroup(&action_mapping[button_id][nr_clicks-1], true);
    } else {
      win_ctl.SetActionGroup(&action_mapping[button_id][nr_clicks-1], false);
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

void cmdHandleClick(int argc, char *argv[]) {
  int button_id = -1;
  int click_cnt = -1;
  ClickEventType ev_type = kClickEventNone;

  if (argc < 3) {
    Serial.println("Error: at least three arguments required!");
    cmdHelpClick();
    return;
  }
  if (sscanf(argv[0], "%d", &button_id) != 1 ||
      button_id < 0 ||
      button_id >= kNrButtons) {
    Serial.println("Error: invalid argument button id");
    cmdHelpClick();
    return;
  }
  switch (argv[1][0]) {
    case 'c': ev_type = kClickEventClick; break;
    case 'h': ev_type = kClickEventHold; break;
    case 'r': ev_type = kClickEventRelease; break;
    default:
      Serial.println("Error: invalid argument event type");
      cmdHelpClick();
      return;
  }
  if (sscanf(argv[2], "%d", &click_cnt) != 1 ||
      click_cnt < 1 ||
      click_cnt > 3) {
    Serial.println("Error: invalid argument click count");
    cmdHelpClick();
    return;
  }

  clickEvent(button_id, ev_type, click_cnt);
}

void cmdHelpClick() {
  Serial.println("Syntax:");
  Serial.println("   click button_id event_type click_cnt");
  Serial.println("   button_id  ... number of the button");
  Serial.println("   event_type ... c (click)");
  Serial.println("                  h (hold)");
  Serial.println("                  r (release)");
  Serial.println("   click_cnt  ... number of times clicked");
}

// vim: set ft=c:
