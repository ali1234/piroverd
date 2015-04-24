#ifndef PIROVERD_SENSORS_H
#define PIROVERD_SENSORS_H

#include <glib.h>

#include "max17043.h"
#include "mpu6050.h"
#include "ak8975.h"

class Sensors
{
    public:
        Sensors(int adapter);
        ~Sensors();

        void update_battery(void);
        void update_orientation(void);
        void get_annotation(char *);

    private:
        MAX17043 *max17043;
        MPU6050 *mpu6050;
        AK8975 *ak8975;

        float acceleration[3];
        float rotation[3];
        float magnetism[3];
        float voltage, percent;
        float heading, pitch, roll;

        GMutex mutex;
};

#endif // PIROVERD_SENSORS_H
