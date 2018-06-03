#include <CommandLine.h>
#include <ClickButton.h>
#include "window_ctl.h"

// Prototypes
const char *eventString(int event);
void clickEvent(int button_id, int event);
void cmdHelpClick();


WindowControl win_ctl;

// ClickButton settings
const int kNrButtons = 18;
ClickButton buttons[kNrButtons] = {
  ClickButton(22, LOW, CLICKBTN_PULLUP),  //  0
  ClickButton(23, LOW, CLICKBTN_PULLUP),  //  1
  ClickButton(24, LOW, CLICKBTN_PULLUP),  //  2
  ClickButton(25, LOW, CLICKBTN_PULLUP),  //  3
  ClickButton(26, LOW, CLICKBTN_PULLUP),  //  4
  ClickButton(27, LOW, CLICKBTN_PULLUP),  //  5
  ClickButton(28, LOW, CLICKBTN_PULLUP),  //  6
  ClickButton(29, LOW, CLICKBTN_PULLUP),  //  7
  ClickButton(30, LOW, CLICKBTN_PULLUP),  //  8
  ClickButton(31, LOW, CLICKBTN_PULLUP),  //  9
  ClickButton(32, LOW, CLICKBTN_PULLUP),  // 10
  ClickButton(33, LOW, CLICKBTN_PULLUP),  // 11
  ClickButton(34, LOW, CLICKBTN_PULLUP),  // 12
  ClickButton(35, LOW, CLICKBTN_PULLUP),  // 13
  ClickButton(36, LOW, CLICKBTN_PULLUP),  // 14
  ClickButton(37, LOW, CLICKBTN_PULLUP),  // 15
  ClickButton(38, LOW, CLICKBTN_PULLUP),  // 16
  ClickButton(39, LOW, CLICKBTN_PULLUP),  // 17
};

//          Window Nr:    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
#define MAP_NONE         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_EG_OG_ALLES  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
#define MAP_EG_ALLES     {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}
#define MAP_OG_ALLES     {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1}
#define MAP_KUECHE_ALLES {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_KUECHE_FENST {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_KUECHE_TUERE {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_ESSZ_ALLES   {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_ESSZ_FENSTER {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_ESSZ_TUERE   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_WOHNZ_FENSTE {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_SUED_EG_ALLE {0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_WEST_EG_ALLE {0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
#define MAP_BUEROFENSTER {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}

#define MAP_KZ1_OST      {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}
#define MAP_KZ2_BEIDE    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0}
#define MAP_KZ2_OST      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}
#define MAP_KZ2_SUED     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}
#define MAP_SZ_ALLES     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0}
#define MAP_SZ_SUED      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0}
#define MAP_SZ_WEST      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}
#define MAP_BAD_TUERE    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}

WinActionGroup action_mapping[kNrButtons][kClickEventMax] = {
  { // Button 0
    { kDirUp,   MAP_EG_OG_ALLES  }, // Single Click
    { kDirUp,   MAP_EG_ALLES     }, // Double Click
    { kDirUp,   MAP_OG_ALLES     }, // Triple Click
  },
  { // Button 1
    { kDirDown, MAP_EG_OG_ALLES  }, // Single Click
    { kDirDown, MAP_EG_ALLES     }, // Double Click
    { kDirDown, MAP_OG_ALLES     }, // Triple Click
  },
  { // Button 2
    { kDirUp,   MAP_KUECHE_ALLES }, // Single Click
    { kDirUp,   MAP_KUECHE_FENST }, // Double Click
    { kDirUp,   MAP_KUECHE_TUERE }, // Triple Click
  },
  { // Button 3
    { kDirDown, MAP_KUECHE_ALLES }, // Single Click
    { kDirDown, MAP_KUECHE_FENST }, // Double Click
    { kDirDown, MAP_KUECHE_TUERE }, // Triple Click
  },
  { // Button 4
    { kDirUp,   MAP_ESSZ_ALLES   }, // Single Click
    { kDirUp,   MAP_ESSZ_FENSTER }, // Double Click
    { kDirUp,   MAP_ESSZ_TUERE   }, // Triple Click
  },
  { // Button 5
    { kDirDown, MAP_ESSZ_ALLES   }, // Single Click
    { kDirDown, MAP_ESSZ_FENSTER }, // Double Click
    { kDirDown, MAP_ESSZ_TUERE   }, // Triple Click
  },
  { // Button 6
    { kDirUp,   MAP_WOHNZ_FENSTE }, // Single Click
    { kDirUp,   MAP_SUED_EG_ALLE }, // Double Click
    { kDirUp,   MAP_WEST_EG_ALLE }, // Triple Click
  },
  { // Button 7
    { kDirDown, MAP_WOHNZ_FENSTE }, // Single Click
    { kDirDown, MAP_SUED_EG_ALLE }, // Double Click
    { kDirDown, MAP_WEST_EG_ALLE }, // Triple Click
  },
  { // Button 8
    { kDirUp,   MAP_BUEROFENSTER }, // Single Click
    { kDirUp,   MAP_NONE         }, // Double Click
    { kDirUp,   MAP_NONE         }, // Triple Click
  },
  { // Button 9
    { kDirDown, MAP_BUEROFENSTER }, // Single Click
    { kDirDown, MAP_NONE         }, // Double Click
    { kDirDown, MAP_NONE         }, // Triple Click
  },
  { // Button 10
    { kDirUp,   MAP_KZ1_OST      }, // Single Click
    { kDirUp,   MAP_NONE         }, // Double Click
    { kDirUp,   MAP_NONE         }, // Triple Click
  },
  { // Button 11
    { kDirDown, MAP_KZ1_OST      }, // Single Click
    { kDirDown, MAP_NONE         }, // Double Click
    { kDirDown, MAP_NONE         }, // Triple Click
  },
  { // Button 12
    { kDirUp,   MAP_KZ2_BEIDE    }, // Single Click
    { kDirUp,   MAP_KZ2_OST      }, // Double Click
    { kDirUp,   MAP_KZ2_SUED     }, // Triple Click
  },
  { // Button 13
    { kDirDown, MAP_KZ2_BEIDE     }, // Single Click
    { kDirDown, MAP_KZ2_OST       }, // Double Click
    { kDirDown, MAP_KZ2_SUED      }, // Triple Click
  },
  { // Button 14
    { kDirUp,   MAP_SZ_ALLES      }, // Single Click
    { kDirUp,   MAP_SZ_SUED       }, // Double Click
    { kDirUp,   MAP_SZ_WEST       }, // Triple Click
  },
  { // Button 15
    { kDirDown, MAP_SZ_ALLES      }, // Single Click
    { kDirDown, MAP_SZ_SUED       }, // Double Click
    { kDirDown, MAP_SZ_WEST       }, // Triple Click
  },
  { // Button 16
    { kDirUp,   MAP_BAD_TUERE     }, // Single Click
    { kDirUp,   MAP_OG_ALLES      }, // Double Click
    { kDirUp,   MAP_EG_ALLES      }, // Triple Click
  },
  { // Button 17
    { kDirDown, MAP_BAD_TUERE     }, // Single Click
    { kDirDown, MAP_OG_ALLES      }, // Double Click
    { kDirDown, MAP_EG_ALLES      }, // Triple Click
  }
};


// CommandLine settings
CommandLine commandLine(Serial, "> ");
Command cmdPrint = Command("print", &cmdHandlePrint);
Command cmdClick = Command("click", &cmdHandleClick);
Command cmdHelp  = Command("help",  &cmdHelpClick);


void setup() {
  Serial.begin(9600);
  Serial.print("Starting...");

  commandLine.add(cmdPrint);
  commandLine.add(cmdClick);
  commandLine.add(cmdHelp);

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
  Serial.println("");
  Serial.println("Arguments:");
  Serial.println("   button_id  ... number of the button");
  Serial.println("   event_type ... c (click)");
  Serial.println("                  h (hold)");
  Serial.println("                  r (release)");
  Serial.println("   click_cnt  ... number of times clicked");
}

// vim: set ft=c:
