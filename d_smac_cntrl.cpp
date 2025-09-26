//KEEP IN MIND, THIS DRONE IS A SOPHISTICATED TOY, NOT A WEAPON!

#include <cstdio>
#include "../hpp/d_flight_cntrl.hpp"
bool motors_armed = false;   // declare global from flight control

#include "d_smac_cntrl.hpp"

// Initialize drone to cold start (everything off)
void smac_init(DroneStatus* status) {
    if (!status) return;

    status->drone_roll_left  = 0.0f;
    status->drone_roll_right = 0.0f;
    status->drone_turn_left  = 0.0f;
    status->drone_turn_right = 0.0f;
    status->drone_pitch_up   = 0.0f;
    status->drone_pitch_dwn  = 0.0f;

    status->altitude         = 0.0f;
    status->altitude_heading = 0.0f;

    status->target_altitude_x = 0.0f;
    status->target_altitude_y = 0.0f;
    status->target_altitude_z = 0.0f;

    status->battery_power_max = 0.0f;
    status->battery_power_min = 0.0f;

    status->sensor_health   = 0.0f;
    status->sensor_range    = 0.0f;
    status->sensor_depth    = 0.0f;
    status->sensor_strength = 0.0f;

    status->power_redirect   = 0.0f;

    status->current_speed    = 0.0f;
    status->stationary_speed = 0.0f;
    status->cruise_speed     = 0.0f;
    status->hover_speed      = 0.0f;

    status->high_speed_2x = 0.0f;
    status->high_speed_3x = 0.0f;
    status->high_speed_4x = 0.0f;
    status->high_speed_5x = 0.0f;
}

// Update drone status
void smac_update(DroneStatus* status, const DroneState* drone_state, float max_wattage) {
    if (!status) return;

    if (!motors_armed) {
        // Keep everything at zero if motors not armed
        status->altitude       = 0.0f;
        status->battery_power_max = 0.0f;
        status->current_speed  = 0.0f;
        status->sensor_health  = 0.0f;
        status->power_redirect = 0.0f;
        return;
    }

    // Motors armed: normal updates
    if (drone_state && max_wattage > 0.0f) {
        status->power_redirect = (drone_state->wattage / max_wattage) * 100.0f;
    }

    // Update altitude if below target
    if (status->altitude < status->target_altitude_z) {
        float delta = 0.5f;
        status->altitude += delta;
        if (status->altitude > status->target_altitude_z)
            status->altitude = status->target_altitude_z;
    }

    // Battery drain
    if (status->battery_power_max > status->battery_power_min) {
        status->battery_power_max -= 0.1f;
        if (status->battery_power_max < status->battery_power_min)
            status->battery_power_max = status->battery_power_min;
    }
}

// Print single row of SMAC status
static void smac_print_row(const DroneStatus* status) {
    if (!status) return;
    std::printf("|%7.1f |%7.1f%% |%5.1f |%6.1f |   %6.1f%%|\n",
                status->altitude,
                status->battery_power_max,
                status->current_speed,
                status->sensor_health,
                status->power_redirect);
}

// Detailed report
void smac_report(const DroneStatus* status) {
    if (!status) return;
    std::printf("\tDrone Altitude: %.6f\n", status->altitude);
    std::printf("\tBattery Power: %.2f%%\n", status->battery_power_max);
    std::printf("\tCurrent Speed: %.2f\n", status->current_speed);
    std::printf("\tSensor Health: %.2f%%\n", status->sensor_health);
}

// SMAC test control
void smac_control(DroneStatus* smac_drone, const DroneState* flight_drone, float max_wattage) {
    if (!smac_drone) return;

    std::printf("Running SMAC test sequence..\n");
    smac_init(smac_drone);

    std::printf("|Altitude |Battery |Speed |Sensor |Pwr Rdrct |\n");
    std::printf("----------------------------------------------\n");

    // Cold start row
    smac_print_row(smac_drone);

    // Run updates (altitude, battery, etc.) only if motors armed
    for (int i = 0; i < 5; ++i) {
        smac_update(smac_drone, flight_drone, max_wattage);
        smac_print_row(smac_drone);
    }

    std::printf("\nSMAC test sequence complete...\n\n");
}