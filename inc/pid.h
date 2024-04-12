#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include "global.h"

// PID Object
typedef struct pid {
    float Kp;
    float Ki;
    float Kd;
    uint32_t lastTime;
    float setPoint;
    float ITerm, lastInput;
    int sampleTime;
    float outMin, outMax;
    float output;
}PID_TypeDef;

void PID_init(PID_TypeDef *pid, float Kp, float Ki, float Kd, float outMin, float outMax, int sampleTime);

void PID_setSetPoint(PID_TypeDef *pid, float setPoint);

void PID_setOutMin(PID_TypeDef *pid, float outMin);

void PID_setOutMax(PID_TypeDef *pid, float outMax);

float PID_compute(PID_TypeDef *pid, float input);


#endif