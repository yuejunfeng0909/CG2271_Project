#ifndef US_SENSOR_H_
#define US_SENSOR_H_

void initUS_Sensor(void);
void generate_10us_impulse(void);
void wait_until_echo(void);
void wait_until_echo_end(void);
int measure_distance_cm(void);

#endif
