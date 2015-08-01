#include <pt.h>

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

#define LED1 11
#define LED2 12
#define LED3 13
#define LED4 14
#define SW 2
#define KNOB A3

struct pt pt_taskLED1;
struct pt pt_taskLED2;
struct pt pt_taskLED3;
struct pt pt_taskLED4;
struct pt pt_taskSW;
struct pt pt_taskKNOB;
struct pt pt_taskRecieveSerial;
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
//  Serial1.print(led1);
//  Serial1.print(led2);
//  Serial1.print(led3);
//  Serial1.print(led4);
//  Serial1.print("|");
  Serial1.print(sw);

  int print_zero_before = 4 - String(knob).length();
  while (print_zero_before > 0) {
    Serial1.print(0);
    print_zero_before--;
  }

  Serial1.print(knob);
  Serial1.print('\r');
  
  // for debug
  Serial.print("value in Galileo : ");
  Serial.print(led1);
  Serial.print(led2);
  Serial.print(led3);
  Serial.print(led4);
  Serial.print("|");
  Serial.print(sw);

  print_zero_before = 4 - String(knob).length();
  while (print_zero_before > 0) {
    Serial.print(0);
    print_zero_before--;
  }

  Serial.print(knob);
  Serial.print('\n');
  // for debug
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
PT_THREAD(taskKNOB(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    knob = analogRead(KNOB);
    PT_DELAY(pt, 150, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED1(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED1, led1);
    PT_DELAY(pt, 300, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED2(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED2, led2);
    PT_DELAY(pt, 300, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED3(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED3, led3);
    PT_DELAY(pt, 300, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
PT_THREAD(taskLED4(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED4, led4);
    PT_DELAY(pt, 300, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
void setup()
{
  Serial1.begin(115200);
  Serial.begin(9600);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(SW, INPUT);

  PT_INIT(&pt_taskLED1);
  PT_INIT(&pt_taskLED2);
  PT_INIT(&pt_taskLED3);
  PT_INIT(&pt_taskLED4);
  PT_INIT(&pt_taskSW);
  PT_INIT(&pt_taskKNOB);
  PT_INIT(&pt_taskRecieveSerial);
  PT_INIT(&pt_taskSendSerial);

  led1 = HIGH;
  led2 = HIGH;
  led3 = LOW;
  led4 = LOW;
}

///////////////////////////////////////////////////////
void loop()
{
  taskLED1(&pt_taskLED1);
  taskLED2(&pt_taskLED2);
  taskLED3(&pt_taskLED3);
  taskLED4(&pt_taskLED4);
  taskSW(&pt_taskSW);
  taskKNOB(&pt_taskKNOB);

  serialEvent();
  taskSendSerial(&pt_taskSendSerial);
}