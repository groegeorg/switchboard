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
    handleClickEvent(i, buttons[i].clicks);
  }

  delay(5);
}

const char *eventString(int event) {
  switch (event) {
    case  1: return "single click"; break;
    case  2: return "double click"; break;
    case  3: return "triple click"; break;
    case -1: return "single long click"; break;
    case -2: return "double long click"; break;
    case -3: return "triple long click"; break;
    case CLICKBTN_LONG_RELEASE: return "long click released"; break;
  }
}

void handleClickEvent(int button_id, int event) {
  char line[50];
  if (event == 0)
    return;  // no event

  snprintf(line, 50, "Event on button %d: %s", button_id,
      eventString(event));
  Serial.println(line);
}

void cmdHandlePrint(char* tokens) {
  Serial.println("Hello World!");
}

