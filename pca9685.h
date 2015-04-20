#define PCA9685_LED0_ON_L      0x06
#define PCA9685_LED0_ON_H      0x07
#define PCA9685_LED0_OFF_L     0x08
#define PCA9685_LED0_OFF_H     0x09
#define PCA9685_ALL_LED_ON_L   0xFA
#define PCA9685_ALL_LED_ON_H   0xFB
#define PCA9685_ALL_LED_OFF_L  0xFC
#define PCA9685_ALL_LED_OFF_H  0xFD

int pca9685_open(int, int);
void pca9685_close(int);

void pca9685_set_pwm(int, int, int, int);
void pca9685_set_pin(int, int, int);
void pca9685_set_motor_mode(int, int, int);
void pca9685_set_motor_speed(int, int, int);
