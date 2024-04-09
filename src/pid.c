#include "pid.h"
#include "systick.h"

void PID_init(PID_TypeDef *pid, float Kp, float Ki, float Kd) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->ITerm = 0;
    pid->lastInput = 0;
    pid->lastTime = 0;
    pid->sampleTime = 10;
    pid->outMin = -25;
    pid->outMax = 25;
    pid->output = 0;
}

void PID_setSetPoint(PID_TypeDef *pid, float setPoint) {
    pid->setPoint = setPoint;
}

float PID_compute(PID_TypeDef *pid, float input) {
    uint32_t now = get_millis();
    int timeChange = (now-pid->lastTime);
    if (timeChange >= pid->sampleTime) {
        float error = (pid->setPoint - input);
        pid->ITerm += (pid->Ki * error);
        if (pid->ITerm > pid->outMax) pid->ITerm = pid->outMax;
        else if (pid->ITerm < pid->outMin) pid->ITerm = pid->outMin;

        float dInput = (input - pid->lastInput);

        pid->output = pid->Kp * error + pid->ITerm - pid->Kd*dInput;
        if (pid->output > pid->outMax) pid->output = pid->outMax;
        else if (pid->output < pid->outMin) pid->output = pid->outMin;

        pid->lastInput = input;
        pid->lastTime = now;
        // printf("Current Weight: %f\r\n", input);
        // printf("KP: %f, KI: %f, KD: %f\r\n", pid->Kp*error, pid->ITerm, pid->Kd*dInput);
        // printf("Output: %f\r\n", Output);
    }
    return pid->output;
}