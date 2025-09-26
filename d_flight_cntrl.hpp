//KEEP IN MIND, THIS DRONE IS A SOPHISTICATED TOY, NOT A WEAPON!

#ifndef D_FLIGHT_CNTRL_HPP
#define D_FLIGHT_CNTRL_HPP

#include <stdbool.h>

//Drone system state
typedef struct {
	bool motors_armed;
	double wattage;
	double voltage;
	int altitude;
} DroneState;

void flight_control();
void arm_motors(DroneState *drone);
void takeoff(DroneState *drone, int altitude);
void land(DroneState *drone, int altitude);

#endif