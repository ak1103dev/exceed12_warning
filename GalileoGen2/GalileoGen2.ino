#include <pt.h>

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

#define SW 2

struct pt pt_taskSW;
struct pt pt_taskSendSerial;

int led1, led2, led3, led4;
int sw, knob;
String val;

#define CMD_LED1 0
#define CMD_LED2 1
#define CMD_LED3 2
#define CMD_LED4 3

void setValue() {

  switch(val.charAt(CMD_LED1)) {
    case '0': led1 = LOW; break;
    case '1': led1 = HIGH; break;
  }
  switch(val.charAt(CMD_LED2)) {
    case '0': led2 = LOW; break;
    case '1': led2 = HIGH; break;
  }
  switch(val.charAt(CMD_LED3)) {
    case '0': led3 = LOW; break;
    case '1': led3 = HIGH; break;
  }
  switch(val.charAt(CMD_LED4)) {
    case '0': led4 = LOW; break;
    case '1': led4 = HIGH; break;
  }
}

void sendSerial() {
  Serial1.print(sw);
  Serial1.print('\r');

  // for debug
  Serial.print(sw);
}

void serialEvent() {
  if (Serial1.available() > 0) {
    val = Serial1.readStringUntil('\r');
    Serial.print("value Recieve : ");
    Serial.println(val);
    Serial1.flush();
    setValue();
  }
}
///////////////////////////////////////////////////////
PT_THREAD(taskSendSerial(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    sendSerial();
    PT_DELAY(pt, 600, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskSW(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    sw = digitalRead(SW);
    PT_DELAY(pt, 150, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
void setup()
{
  Serial1.begin(115200);
  Serial.begin(9600);

  pinMode(SW, INPUT);

  PT_INIT(&pt_taskSW);
  PT_INIT(&pt_taskSendSerial);
}

///////////////////////////////////////////////////////
void loop()
{
  taskSW(&pt_taskSW);

  serialEvent();
  taskSendSerial(&pt_taskSendSerial);
}