#ifndef PIROVER_I2C_H
#define PIROVER_I2C_H

int i2c_open(int adapter, int address);
void i2c_close(int device);

#define BYTESWAP(x) ( ((x&0xff)<<8) | ((x&0xff00)>>8) )

#endif // PIROVER_I2C_H
