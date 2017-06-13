#include <CommandLine.h>
#include <ClickButton.h>

// ClickButton settings
const int buttonPin1 = 4;
ClickButton button1(buttonPin1, LOW, CLICKBTN_PULLUP);

// CommandLine settings
CommandLine commandLine(Serial, "> ");
Command cmdPrint = Command("print", &cmdHandlePrint);


void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT_PULLUP);

  commandLine.add(cmdPrint);

  // Setup button timers (all in milliseconds / ms)
  // (These are default if not set, but changeable for convenience)
  button1.debounceTime   = 20;   // Debounce timer in ms
  button1.multiclickTime = 250;  // Time limit for multi clicks
  button1.longClickTime  = 600;  // time until "held-down clicks" register
}


void loop()
{
  button1.Update();
  commandLine.update();

  if (button1.clicks == 1) Serial.println("SINGLE click");

  if (button1.clicks == 2) Serial.println("DOUBLE click");

  if (button1.clicks == 3) Serial.println("TRIPLE click");

  if (button1.clicks == -1) Serial.println("SINGLE LONG click");

  if (button1.clicks == -2) Serial.println("DOUBLE LONG click");

  if (button1.clicks == -3) Serial.println("TRIPLE LONG click");

  if (button1.longClickReleased == true) Serial.println("LONG click released");

  delay(5);
}


void cmdHandlePrint(char* tokens)
{
  Serial.println("Hello World!");
}

