#ifndef PIROVERD_SN3218_H
#define PIROVERD_SN3218_H

#include "i2cdevice.h"

#define HEADLIGHT_RIGHT_OUTER  0
#define HEADLIGHT_RIGHT_INNER  1
#define INDICATOR_RIGHT_FRONT  2
#define INDICATOR_RIGHT_REAR   3
#define TAILLIGHT_RIGHT        4
#define TAILLIGHT_LEFT        13
#define INDICATOR_LEFT_REAR   14
#define INDICATOR_LEFT_FRONT  15
#define HEADLIGHT_LEFT_INNER  16
#define HEADLIGHT_LEFT_OUTER  17

class SN3218 : public I2CDevice
{
    public:
        SN3218(int adapter, int address);
        ~SN3218();

        void set_light(int, int);
};

#endif // PIROVERD_SN3218_H
