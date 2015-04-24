#ifndef PIROVERD_AK8975_H
#define PIROVERD_AK8975_H

#include "i2cdevice.h"

class AK8975 : public I2CDevice
{
    public:
        AK8975(int adapter, int address);
        ~AK8975();
        void get_magnetism(float*);
};

#endif // PIROVERD_AK8975_H
