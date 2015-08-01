#include <pt.h>

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_WHILE(pt, millis()-ts < (ms));

#define LED1 11
#define LED2 12

struct pt pt_taskLED1;
struct pt pt_taskLED2;

///////////////////////////////////////////////////////
PT_THREAD(taskLED1(struct pt* pt))
{
  static uint32_t ts;

  PT_BEGIN(pt);

  while (1)
  {
    digitalWrite(LED1, HIGH);
    PT_DELAY(pt, 1000, ts);
    digitalWrite(LED1, LOW);
    PT_DELAY(pt, 500, ts);
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
    digitalWrite(LED2, HIGH);
    PT_DELAY(pt, 700, ts);
    digitalWrite(LED2, LOW);
    PT_DELAY(pt, 300, ts);
  }

  PT_END(pt);
}

///////////////////////////////////////////////////////
void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  PT_INIT(&pt_taskLED1);
  PT_INIT(&pt_taskLED2);
}

///////////////////////////////////////////////////////
void loop()
{
  taskLED1(&pt_taskLED1);
  taskLED2(&pt_taskLED2);
}
