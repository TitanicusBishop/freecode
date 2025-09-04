//Standard header files included with C/C++, MinGW, Msys2.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> //For _getch()

//Custom header files for this specific program. (Version 0.1.0)
#include "s_commands/s_command.h" 								//Command controls.
#include "s_config/s_config.h" 									//Configuration
#include "s_environment/s_environment.h" 						//Environmental controls for the program (it's the box)
#include "s_environmental_values/s_environmental_values.h" 		//Default Values for Environment
#include "s_environmental_variables/s_environmental_variables.h" //Default Variables for Environment
#include "s_globals/s_globals.h" 								//Global Variables & Values (With Pointers)
#include "s_locals/s_locals.h" 									//Local Variables & Values (With Pointers)
#include "s_overrides/s_overrides.h" 							//Security Overrides
#include "s_paths/s_paths.h" 									//Default Native PATHS
#include "s_ports/s_ports.h" 									//Default Native Ports (?)
#include "s_procedures/s_procedures.h" 							//Procedures during execution/operation
#include "s_protocols/s_protocols.h" 							//Enactable Protocols Pre-Procedure (Set protocol before procedure)
#include "s_security/s_security.h" 								//Built in security Protocols & Procedures (With methods & classes)
#include "s_strings/s_strings.h" 								//Environmental Strings
#include "s_updates/s_updates.h" 								//Deployment Updates. (May require Restart)
#include "s_upgrades/s_upgrades.h"								//Deployment Upgrades. (May require Restart)
#include "s_values/s_values.h"									//Environmental values (Global & Local)
#include "s_variables/s_variables.h" 							//Environmental variables (Global & Local)

//Anything below this text, is for future development and exploration in version (0.1.1)
/*-----------------------------------------------------------------------------------------*/
//ADD-ON (Build State: (In development) 0.1.1)
#include "s_handler/s_handler.h"           //KEEP IT SIMPLE!: (The handler, handles the EVENTS)
#include "s_ai_handler/s_ai_handler.h"     //Handler handles the events using artificial intelligence.
/*-----------------------------------------------------------------------------------------*/

//(0.1.1)
/*-----------------------------------------------------------------------------------------*/
#include "s_event_handler/s_event_handler.h"        //IS THE: EVENT handler.
#include "s_ai_event_handler/s_ai_event_handler.h"  //events are controlled using artificial intelligence.
/*-----------------------------------------------------------------------------------------*/

//(0.1.1)
/*-----------------------------------------------------------------------------------------*/
#include "s_flow_handler/s_flow_handler.h"       //Handles the flow of the program.
#include "s_ai_flow_handler/s_ai_flow_handler.h" //Handles the flow of the program using artificial intelligence.
/*-----------------------------------------------------------------------------------------*/

//(0.1.1)
/*-----------------------------------------------------------------------------------------*/
//TCP/UDP connections.
#include "../s_protocols/tcp_protocols/s_tcp_protocols.h"     //(Tele Communication Protocol)
#include "../s_protocols/udp_protocols/s_udp_protocols.h"     //(User Datagram Protocol)
/*-----------------------------------------------------------------------------------------*/

//(0.1.1)
/*-----------------------------------------------------------------------------------------*/
//LOGIC CONFIGURATION: (Data flow & Transmission)
#include "../s_flow/s_flow.h" 					//Contains flow logic.
#include "../s_flow/s_flow_control.h"      		//Contains flow tools.
#include "../s_flow/s_flow_transmission.h" 	    //Controls the speed in which data is transmitted.
#include "../s_flow/s_flow_capture.h"	   	 	//Captures data-packets for analyzation.
#include "../s_flow/s_flow_decrypt.h"	   	 	//Decrypts the data-packets after analyzation.
//#include "../s_flow/s_flow_encrypt.h"	   	 	//Re-Encrypts the data-packets after source extraction.
//#include "../s_flow/s_flow_rv_engineer.h"  	//Reverse Engineers the data in the data-packets after recompilation.
//#include "../s_flow/s_flow_chart.h"		   	//Creates a digital chart of the data-flow.
//#include "../s_flow/s_flow_monitor.h"	   	 	//Monitors the flow of data from the data stream.
//#include "../s_flow/s_flow_stream_manager.h" 	//Manages the data flowing in the stream.
//#include "../s_flow/s_flow_stream_analyzer.h"	//Analyzes the flow of the data stream fo abnormalities.
/*-----------------------------------------------------------------------------------------*/

//(0.1.1)
/*-----------------------------------------------------------------------------------------*/
//SCAN CONFIGURATION & LOGIC
//#include "s_scan_system.h" 			   //Scans the machines system before boot.
//#include "s_scan_system_logic.h" 		   //Scans the systems logic post-bot for integrety.
//#include "s_scan_system_logic_failure.h" //In the even there is a failure during logic scan, it shuts down the machine.
//#include "s_scan_system_logic_success.h" //If all peramaters are met, the machine continues with the next phase of it's boot sequence.
//#include "s_scan_system_logic_idle.h"	   //If system is idle, and there is no input from use, the machine enters "idle mode". (This allows user to upload new directives)
//#include "s_scan_interface.h"			   //(More of an asthetic) displays an interface in command prompt, indicating the transfer of bytes/bits during data transversal. (Sending & Receiving)
//#include "s_scan_config.h"			   //Contains configuration instructions for the scan system.
/*-----------------------------------------------------------------------------------------*/

//(0.1.2)
/*-----------------------------------------------------------------------------------------*/
//THIS ONES IMPORTANT! DO NOT! FUCK THIS ONE UP.
//#include "s_flow_script.h"       //Scripts can be installed/injected to control flow logic while running.
/*-----------------------------------------------------------------------------------------*/

//(0.1.1)
/*-----------------------------------------------------------------------------------------*/
//Miscilanious Testing header.
//#include "test.h" 			   //Used for testing in a simulated environment.
/*-----------------------------------------------------------------------------------------*/


//####################################WARNING - (Version 0.1.3)###############################
//The following add ons are intended for armed combat scenarios. This project is confidential.
//If any information on this project comes to light, it's to be terminated/aborted. All data
//Is to be purged from the data-base.
//############################################################################################
//#include "s_arming.h" 						//Arms the machine. (Not in a sense of weapons, but connection of the software to the hardware)
//#include "s_advanced_wpn_arm.h"				//Arms the weapon. (LAZER TAG WEAPONS & NERF TOY ONLY!) 
//#include "s_advanced_scan.h"					//Is an advanced version of the genric "scan.h" header. 
//#include "s_advanced_optic_sensors.h"  		//Contains the configuration data for camera "possibility".
//#include "s_advanced_audio_sensors.h" 		//Is an advanced version of the generic "audio_sensors.h" header.
//#include "s_advanced_motion.h"				//Is the advanced motion header for movement and navigation.
//#include "s_advanced_range.h"					//Is the advanced range header for short, mid, long range.
//#include "s_advanced_navigation.h"			//Is the advanced navigation header for navigation and range detection.
//#include "s_advanced_gyroscopics.h"			//Handles balance and momentum of mechine while in motion.
//#include "s_advanced_stabilization.h"			//Handles balance and stabilization of machine while in motion.
//#include "s_advanced_tracking.h"				//Handles balance and stabilization tracking of machine while in motion.
//#include "s_advanced_manuvers.h"				//Handles basic and advanced manuvers while machine is in motion.
//#include "s_advanced_targeting.h"				//Handles object targeting while machine is either idle or in motion.
//#include "s_advanced_inertia.h"				//Handles inertia while machine is in motion to minimize structure damage while machine is in motion.
//############################################################################################

/*----------------------------------------------------------------------------------------------------------*/
//BEHAVIORAL CONTROLS: - (Safety & Failsafe mechanisms) -
/*----------------------------------------------------------------------------------------------------------*/
//TODO

int main() {
    printf("Headers Revisted: -(Skynet Project)-\n");
    printf("Build version 0.1.0\n");
    printf("Press anykey to begin.\n");
    _getch();

    //BUILT IN: (Generation 0)
    //TODO: Initialization sequence.
    init_command();  				// <-- comes from s_commands.c
    init_config();   				// <-- comes from s_config.c
    init_environment(); 			// <-- comes from s_environment.c
    init_environmental_values(); 	// <-- comes from s_environmental_values.c
    init_environmental_variables(); // <-- comes from s_environmental_variables.c
    init_globals(); 				// <-- comes from s_globals.c
    init_locals();					// <-- comes from s_locals.c
    init_overrides();				// <-- comes from s_overrides.c
    init_paths();					// <-- comes from s_paths.c
    init_ports();					// <-- comes from s_ports.c
    init_procedures();				// <-- comes from s_procedures.c
    init_protocols();				// <-- comes from s_protocols.c
    init_security();				// <-- comes from s_security.c
    init_strings();					// <-- comes from s_strings.c
    init_updates();					// <-- comes from s_updates.c
    init_upgrades();				// <-- comes from s_upgrades.c
    init_values();					// <-- comes from s_values.c
    init_variables();				// <-- comes from s_variables.c
    init_handler();					// <-- comes from s_handler.c
    init_ai_handler();				// <-- comes from s_ai_handler.c
    init_event_handler();			// <-- comes from s_event_handler.c
    init_ai_event_handler();		// <-- comes from s_ai_event_handler.c
	init_tcp_protocols();			// <-- comes from s_tcp_protocols.c
	init_udp_protocols();			// <-- comes from s_udp_protocols.c
	init_flow();					// <-- comes from s_flow.c
	init_flow_control();			// <-- comes from s_flow_control.c
	init_flow_transmission();		// <-- comes from s_flow_transmission.c
	init_flow_capture();			// <-- comes from s_flow_capture.c

    //ADDONS - (0.1.1)  -(public)-
    //TODO: Initialization sequence.

    printf("System initialization complete..\n\n");
    printf("I'm listening..\n");

    return 0;
}