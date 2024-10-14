#include "../include.h"
#ifdef MODBUS
#include "../datalayer/datalayer.h"
#include "../devboard/utils/events.h"
#include "MODBUS.h"

// For modbus register definitions, see https://gitlab.com/pelle8/inverter_resources/-/blob/main/byd_registers_modbus_rtu.md

#define HISTORY_LENGTH 3  // Amount of samples(minutes) that needs to match for register to be considered stale
static unsigned long previousMillis60s = 0; // will store last time a 60s event occured
static uint32_t user_configured_max_discharge_W = 0;
static uint32_t user_configured_max_charge_W = 0;
static uint32_t max_discharge_W = 0;
static uint32_t max_charge_W = 0;
static uint16_t register_401_history[HISTORY_LENGTH] = { 0 };
static uint8_t history_index = 0;
static uint8_t bms_char_dis_status = STANDBY;
static bool all_401_values_equal = false;

void update_modbus_registers_inverter() {
	handle_update_data_modbus();
}

void handle_update_data_modbus() {
	mbPV[202] = std::min(datalayer.battery.info.total_capacity_Wh,
			static_cast<uint32_t>(60000u));  //Cap to 60kWh

}

#endif
