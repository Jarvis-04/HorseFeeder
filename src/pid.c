#include "pid.h"
#include "systick.h"

void PID_init(PID_TypeDef *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->ITerm = 0;
    pid->lastInput = 0;
    pid->lastTime = 0;
    pid->sampleTime = 1000;
    pid->outMin = -1000;
    pid->outMax = 1000;
}

void PID_setSetPoint(PID_TypeDef *pid, float setPoint) {
    pid->setPoint = setPoint;
}

float PID_compute(PID_TypeDef *pid, float input) {
    float Output = 0;
    uint32_t now = get_millis();
    int timeChange = (now-pid->lastTime);
    if (timeChange >= pid->sampleTime) {
        float error = (pid->setPoint - input);
        pid->ITerm += (pid->Ki * error);
        if (pid->ITerm > pid->outMax) pid->ITerm = pid->outMax;
        else if (pid->ITerm < pid->outMin) pid->ITerm = pid->outMin;

        float dInput = (input - pid->lastInput);

        Output = pid->Kp * error + pid->ITerm - pid->Kd*dInput;
        if (Output > pid->outMax) Output = pid->outMax;
        else if (Output < pid->outMin) Output = pid->outMin;

        pid->lastInput = input;
        pid->lastTime = now;
        printf("KP: %f, KI: %f, KD: %f\r\n", pid->Kp*error, pid->ITerm, pid->Kd*dInput);
        printf("Output: %f\r\n", Output);
    }
    return Output;
}