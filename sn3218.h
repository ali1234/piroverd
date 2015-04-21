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


int sn3218_open(int, int);
void sn3218_close(int);
void sn3218_set_light(int, int, int);
