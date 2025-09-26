//KEEP IN MIND, THIS DRONE IS A SOPHISTICATED TOY, NOT A WEAPON!

#pragma once
#include <cstdio>

//Data type (typedef + struct)
struct DroneStatus {
	
	//Advanced movement function. (Possibly menuver?)
	float drone_roll_left;  //Roll left.
	float drone_roll_right; //Roll right.
	
	float drone_turn_left;  //Turn left.
	float drone_turn_right; //Turn right.
	
	float drone_pitch_up;   //Pitch up.
	float drone_pitch_dwn;  //Pitch down.
	
	//General drone control.
	float altitude;          	 //Current altitude.
	float altitude_heading;   	 //Current altitude heading.
	
	float target_altitude_x; 	 //Desired altitude X axis.
	float target_altitude_y; 	 //Desired altitude Y axis.
	float target_altitude_z; 	 //Desired altitude Z axis.
	
	float power_redirect; 		 //Motor power as percentage.
	
	float battery_power_max;     //Battery power level % MAX.
	float battery_power_min;	 //Battery power level % MIN.
	
	float motor_power_max; 	     //Motor power allocation % MAX.
	float motor_power_min;		 //Motor power allocation % MIN.
	
	float sensor_health;   		 //0 to 100%
	float sensor_range;			 //0 to 100%
	float sensor_depth;			 //0 to 100%
	float sensor_strength;		 //0 to 100%
	
	
	float current_speed; 	     //Current speed heading.
	float stationary_speed;		 //If the drone does not move at all, it's stationary.
	
	float cruise_speed;			 //Cruise speed.
	float hover_speed;			 //Hoverse and follows.
	
	float high_speed_2x;		 //Speed boost by 2x (Per motor strength)
	float high_speed_3x;		 //Speed boost by 3x (per motor strength)
	float high_speed_4x;		 //Speed boost by 4x (per motor strength)
	float high_speed_5x;		 //Speed boost by 5x (per motor strength)

	//Add more fields later (voltage, temp, etc..)
};

//Function prototypes.
void smac_init(DroneStatus* status);
void smac_update(DroneStatus* status, const DroneState* drone_state, float max_wattage);
void smac_report(const DroneStatus* status);

//smac control function prototype.
void smac_control(DroneStatus* smac_drone, const DroneState* flight_drone, float max_wattage);