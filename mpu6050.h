#ifndef PIROVER_MPU6050_H
#define PIROVER_MPU6050_H

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_SMPLRT_DIV 0x19

int mpu6050_open(int, int);
void mpu6050_close(int);

void mpu6050_enable_passthrough(int);
void mpu6050_disable_passthrough(int);

void mpu6050_get_acceleration(int, float *);
void mpu6050_get_rotation(int, float *);

#endif // PIROVER_MPU6050_H
