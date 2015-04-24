#ifndef PIROVERD_MAX17043_H
#define PIROVERD_MAX17043_H

#include "i2cdevice.h"

#define MAX17043_VCELL    0x02
#define MAX17043_SOC      0x04
#define MAX17043_MODE     0x06
#define MAX17043_VERSION  0x08
#define MAX17043_CONFIG   0x0D
#define MAX17043_COMMAND  0xFE

#define MAX17043_MODE_POR       0x4000
#define MAX17043_CONFIG_SLEEP   0x0080
#define MAX17043_CONFIG_ALRT    0x0020
#define MAX17043_COMMAND_RESET  0x0054

class MAX17043 : public I2CDevice
{
    public:
        MAX17043(int adapter, int address);
        ~MAX17043();

        float get_voltage();
        float get_percent();
};

#endif // PIROVERD_MAX17043_H
