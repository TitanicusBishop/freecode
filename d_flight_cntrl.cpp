//KEEP IN MIND, THIS DRONE IS A SOPHISTICATED TOY, NOT A WEAPON!

//Windows functions
#include <windows.h>
#include <unistd.h>

#include "d_flight_cntrl.hpp"
#include <cstdio>
#include <cstdlib>

void flight_control() {
    std::printf("Running test sequence..\n");
}

void arm_motors(DroneState *drone) {
    int set_pwr = 1;
    int set_main_pwr = 0;
    int set_rdrct_pwr = 0;
    int set_pwr_transversal = 0;
    
    // Simulated power checks
    if (set_pwr == 1) {
        set_main_pwr = 1;
        std::printf("\t[MOTOR RESPONSE] - (Power check passed)\n");
    }
    
    if (set_main_pwr == 1) {
        set_rdrct_pwr = 1;
        std::printf("\t[MOTOR RESPONSE] - (Main power engaged...)\n");
    }
    
    if (set_rdrct_pwr == 1) {
        set_pwr_transversal = 1;
        drone->wattage = 120.5;
        drone->voltage = 12.0;
        std::printf("\t[MOTOR RESPONSE] - (Power redirect engaged...) (%.1fV / %.1fW)\n",
                    drone->voltage, drone->wattage);
    }
    
    if (set_pwr_transversal == 1) {
        std::printf("\t[MOTOR RESPONSE] - (Motors ready for arming...)\n");
    }
    
    // User confirmation
    std::printf("\tExecute command? (Y/N): ");
    char choice;
    scanf(" %c", &choice); // space before %c eats newline
    std::printf("\n");
    
    if (choice == 'Y' || choice == 'y') {
        // Capacitor charging animation
        std::printf("\t[POWER SYSTEM] - (Charging capacitor...)\n");
        for (int i = 0; i <= 100; i += 1) {
            std::printf("\t[CAPACITOR] - %d%% charged\n", i);
            #ifdef _WIN32
                Sleep(300);
            #else
                usleep(300000);
            #endif
        }
		getchar();

        // After charging, arm the motors
        drone->motors_armed = true;
        std::printf("\t[MOTOR RESPONSE] - (Execution confirmed. Motors armed)\n");
    } else {
        drone->motors_armed = false;
        std::printf("\t[MOTOR RESPONSE] - (Execution Aborted. Motors disarmed)\n");
    }
}

void takeoff(DroneState *drone, int altitude) {
    if (drone->motors_armed) {
        drone->altitude = altitude;
        std::printf("\tSetting ascension to altitude: %d\n", altitude);
    } else {
        std::printf("\t[ERROR] - (Unable to take off, motors disarmed...)\n");
    }
}

void land(DroneState *drone, int altitude) {
    if (drone->motors_armed) {
        drone->altitude = altitude;
        std::printf("\tSetting descent to altitude: %d\n", altitude);
    } else {
        std::printf("\t[ERROR] - (Cannot land, motors are disarmed...)\n");
    }
}