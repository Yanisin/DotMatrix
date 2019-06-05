#pragma once
#include <chtypes.h>

void stInit(void);

/* The functions below are not needed in periodic mode */
void stStartAlarm(uint32_t abstime);
void stStopAlarm(void);
void stSetAlarm(uint32_t abstime);
uint32_t stGetAlarm(void);
uint32_t stGetCounter(void);
bool stIsAlarmActive(void);
