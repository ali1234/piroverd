#ifndef PIROVER_AK8975_H
#define PIROVER_AK8975_H

int ak8975_open(int, int);
void ak8975_close(int);

void ak8975_get_magnetism(int, float *);

#endif // PIROVER_AK8975_H
