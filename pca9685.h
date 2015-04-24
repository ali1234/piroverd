#ifndef PIROVERD_PCA9685_H
#define PIROVERD_PCA9685_H

#include "i2cdevice.h"

#define PCA9685_LED0_ON_L      0x06
#define PCA9685_LED0_ON_H      0x07
#define PCA9685_LED0_OFF_L     0x08
#define PCA9685_LED0_OFF_H     0x09
#define PCA9685_ALL_LED_ON_L   0xFA
#define PCA9685_ALL_LED_ON_H   0xFB
#define PCA9685_ALL_LED_OFF_L  0xFC
#define PCA9685_ALL_LED_OFF_H  0xFD

class PCA9685 : public I2CDevice
{
    public:
        PCA9685(int adapter, int address);
        ~PCA9685();

        void set_pwm(int, int, int);
        void set_pin(int, int);
        void set_motor_mode(int, int);
        void set_motor_speed(int, int);
};

#endif // PIROVERD_PCA9685_H
