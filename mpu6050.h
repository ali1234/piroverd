#ifndef PIROVERD_MPU6050_H
#define PIROVERD_MPU6050_H

#include "i2cdevice.h"

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_SMPLRT_DIV 0x19

class MPU6050 : public I2CDevice
{
    public:
        MPU6050(int adapter, int address);
        ~MPU6050();

        void enable_passthrough(void);
        void disable_passthrough(void);

        void get_acceleration(float *);
        void get_rotation(float *);
};

#endif // PIROVERD_MPU6050_H
