#include "pid.h"
#include "systick.h"

// Initialize PID values
void PID_init(PID_TypeDef *pid, float Kp, float Ki, float Kd, float outMin, float outMax, int sampleTime) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->sampleTime = sampleTime;
    pid->outMin = outMin;
    pid->outMax = outMax;

    pid->ITerm = 0;
    pid->lastInput = 0;
    pid->lastTime = 0;

    pid->output = 0;
}

// Set the PID set point
void PID_setSetPoint(PID_TypeDef *pid, float setPoint) {
    pid->setPoint = setPoint;
}

// Set the PID output minimum
void PID_setOutMin(PID_TypeDef *pid, float outMin) {
    pid->outMin = outMin;
}

// Set the PID output maximum
void PID_setOutMax(PID_TypeDef *pid, float outMax) {
    pid->outMax = outMax;
}

// Run the PID loop
float PID_compute(PID_TypeDef *pid, float input) {
    // Find the time change to see if the loop should be run
    uint32_t now = get_millis();
    int timeChange = (now-pid->lastTime);
    if (timeChange >= pid->sampleTime) {
        // Find the error and ITerm
        float error = (pid->setPoint - input);
        pid->ITerm += (pid->Ki * error);
        // Adjust ITerm to prevent windup
        if (pid->ITerm > pid->outMax) pid->ITerm = pid->outMax;
        else if (pid->ITerm < pid->outMin) pid->ITerm = pid->outMin;

        // Calculate the derivative input
        float dInput = (input - pid->lastInput);

        // Calculate the output and cap it to the output min and max
        pid->output = pid->Kp * error + pid->ITerm - pid->Kd*dInput;
        if (pid->output > pid->outMax) pid->output = pid->outMax;
        else if (pid->output < pid->outMin) pid->output = pid->outMin;

        // Set terms for next loop
        pid->lastInput = input;
        pid->lastTime = now;
    }
    return pid->output;
}