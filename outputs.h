#ifndef PIROVERD_OUTPUTS_H
#define PIROVERD_OUTPUTS_H

#include "pca9685.h"
#include "sn3218.h"

class Outputs
{
    public:
        Outputs(int adapter);
        ~Outputs();

        void handle_control_packet(char *buf);

    private:
        PCA9685 *pca9685;
        SN3218 *sn3218;
};

#endif // PIROVERD_OUTPUTS_H
