#include <CommandLine.h>
#include <ClickButton.h>

#define NR_BUTTONS 3


const char *eventString(int event);
void handleClickEvent(int button_id, int event);

// ClickButton settings
ClickButton buttons[NR_BUTTONS] = {
  ClickButton(4, LOW, CLICKBTN_PULLUP),
  ClickButton(5, LOW, CLICKBTN_PULLUP),
  ClickButton(6, LOW, CLICKBTN_PULLUP)
};

// CommandLine settings
CommandLine commandLine(Serial, "> ");
Command cmdPrint = Command("print", &cmdHandlePrint);


void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  commandLine.add(cmdPrint);

  // Setup button timers (all in milliseconds)
  for (int i = 0; i < NR_BUTTONS; ++i) {
    buttons[i].debounceTime   = 20;   // Debounce timer in ms
    buttons[i].multiclickTime = 250;  // Time limit for multi clicks
    buttons[i].longClickTime  = 600;  // time until "held-down clicks" register
  }
}


void loop() {
  commandLine.update();

  for (int i = 0; i < NR_BUTTONS; ++i) {
    buttons[i].Update();
    handleClickEvent(i, buttons[i].event);
  }

  delay(5);
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

void handleClickEvent(int button_id, ClickEvent event) {
  char line[50];
  if (event == kClickEventNone)
    return;  // no event

  snprintf(line, 50, "Event on button %d: %s", button_id,
      eventString(event));
  Serial.println(line);
}

void cmdHandlePrint(char* tokens) {
  Serial.println("Hello World!");
}

