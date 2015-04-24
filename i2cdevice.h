#ifndef PIROVERD_I2C_H
#define PIROVERD_I2C_H

#include <linux/i2c-dev.h>

#define BYTESWAP(x) ( ((x&0xff)<<8) | ((x&0xff00)>>8) )

class I2CDevice
{
    public:
        I2CDevice(int adapter, int address);
        ~I2CDevice();

    protected:
        __u8 read8(__u8 reg);
        void write8(__u8 reg, __u8 val);

        __u16 read16(__u8 reg);
        void write16(__u8 reg, __u16 val);

    private:
        int file_des;
};

#endif // PIROVERD_I2C_H
