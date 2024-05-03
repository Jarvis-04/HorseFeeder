#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include "global.h"

typedef struct pidData PID_TypeDef;

PID_TypeDef *PID_init(float Kp, float Ki, float Kd, float outMin, float outMax, int sampleTime);

void PID_destroy(PID_TypeDef *pid);

void PID_setSetPoint(PID_TypeDef *pid, float setPoint);

void PID_setOutMin(PID_TypeDef *pid, float outMin);

float PID_getOutMin(PID_TypeDef *pid);

void PID_setOutMax(PID_TypeDef *pid, float outMax);

float PID_getOutMax(PID_TypeDef *pid);

float PID_compute(PID_TypeDef *pid, float input);


#endif