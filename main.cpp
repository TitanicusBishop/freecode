//KEEP IN MIND, THIS DRONE IS A SOPHISTICATED TOY, NOT A WEAPON!

//Standard C/CPP Directives.
#include <cstdio>

//Implements a "stamp" of sorts to show it's version.
#define BUILD_VERSION "App v0.1.0"

/*Please read document included in this folder carefully.*/
//Custom CPP Directives.
#include "d_flight_cntrl.hpp"    	//Handles flight control and manuvering.
//#include "d_vtol_cntrl.hpp"		//Vertical take off and land declarations.
//#include "d_vtol_idle.hpp"		//Handles the drone while idling in the air.
//#include "d_vtol_ptrl.hpp"		//Handles patrolling when drone is active.
//#include "d_vtol_prtcl.hpp"		//Handles vtol protocols.
//#include "d_vtol_prcdr.hpp"		//Handles vtol procedures.
//#include "d_vtol_peram.hpp"		//Handles vtol peramaters.
//#include "d_vtol_mem.hpp"			//Handles the vtol modules memory.
#include "d_smac_cntrl.hpp"			//Handles Self Monitoring And Correction (SMAC)
#include "d_sensor_cntrl.hpp"    	//Handles flight sensors.
#include "d_optic_cntrl.hpp"     	//Handles drone optics   (Visual)
#include "d_audio_cntrl.hpp"	 	//Handles drone audio    (Sound)
#include "d_roll_cntrl.hpp"	 		//Handles drone rolling  (left/Right)
#include "d_pitch_cntrl.hpp"        //Handles drone pitching (Up/down)
#include "d_hover_cntrl.hpp"        //Handles drone hovering (hovering)
#include "d_motor_cntrl.hpp"        //Handles drone motor systems.
#include "d_hydrolic_cntrl.hpp"		//Handles drone hydrolic systems.
#include "d_pwr_cntrl.hpp"          //Handles drone power controls.
#include "d_pwr_rdrct.hpp"			//Handles redirecting power if the main power module is damaged.
#include "d_sys_cntrl.hpp"       	//Handles the entirety of the drones sytems.
#include "d_bios.hpp"		     	//Handles the bios of the drone.
#include "d_chk_memory.hpp"			//Handles pre-flight memory checks.


//############################WARNING##########################
//The header files below, handle the drones weapon systems.
//Please read the documents included with this drone carefully.
//#############################################################
//#include "chk_wpn_iff.hpp"		 //Checks weapons IFF (Identify Friend/Foe Peramaters)
//#include "chk_fire_cntrl.hpp"		 //Checks fire control patterns & safety.
#include "chk_wpn_diagnostics.hpp" 	 //Before arming, a diagnostics is run.
#include "chk_ammo.hpp"		 	 	 //Checks how much ammo is in the weapon.
#include "chk_wpn_integ.hpp"	 	 //Checks the weapons structure integrity.
#include "chk_wpn_init.hpp"	 	 	 //Checks initialization protocols.
#include "wpn_arm.hpp" 		 	 	 //Arms the weapon.

int main(){
std::printf("Hunter Killer Drone (Prototype)\n");
std::printf("Build version (%s)\n", BUILD_VERSION);
std::printf("Press anykey to continue.\n");
std::getchar(); //Waits for user to press enter.. (Obviously)

	//Initializes flight control system.	
	DroneState flight_drone;
	DroneStatus smac_drone;
	
	float max_wattage = 120.5f;
	
	smac_control(&smac_drone, &flight_drone, max_wattage);
	
	
		flight_control();
		arm_motors(&flight_drone);
		takeoff(&flight_drone, 0);
		land(&flight_drone, 0);				
		
		//Explicit disarm after landing.
		flight_drone.motors_armed = false;
		std::printf("\t[MOTOR RESPONSE] - (Motors disarmed after landing...)\n\n");

	smac_control(&smac_drone, &flight_drone, max_wattage);
	sensor_control();
	optic_control();
	audio_control();
	roll_control();
	pitch_control();
	hover_control();
	motor_control();
	hydrolic_control();
	power_control();
	power_redirect_control();
	system_control();
	bios_control();
	chk_wpn_diagnostics();
	chk_ammo();
	chk_memory();
	chk_wpn_integ();
	chk_wpn_init();
	wpn_arm();

	
	
	//do we need to add more crap, moe?
	
return 0;
}