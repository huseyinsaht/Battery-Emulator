#include "../include.h"
#ifdef MODBUS
#include "../datalayer/datalayer.h"
#include "../devboard/utils/events.h"
#include "MODBUS.h"

#define HISTORY_LENGTH 3  // Amount of samples(minutes) that needs to match for register to be considered stale
static uint32_t user_configured_max_discharge_W = 0;
static uint32_t user_configured_max_charge_W = 0;
static uint32_t max_discharge_W = 0;
static uint32_t max_charge_W = 0;
static uint8_t history_index = 0;
static uint8_t bms_char_dis_status = STANDBY;

void update_modbus_registers_inverter() {
	handle_update_data_modbus();
}

void handle_update_data_modbus() {
	/*Battery interface channels*/
	mbPV[1] = datalayer.battery.info.battery_soc ;
	mbPV[2] = datalayer.battery.info.battery_soh ;
	mbPV[3] = datalayer.battery.info.battery_voltage ;
	mbPV[4] = datalayer.battery.info.battery_current ;
	mbPV[5] = datalayer.battery.info.battery_capacity ;
	mbPV[6] = datalayer.battery.info.battery_charge_max_voltage ;
	mbPV[7] = datalayer.battery.info.battery_charge_max_current ;
	mbPV[8] = datalayer.battery.info.battery_discharge_min_voltage ;
	mbPV[9] = datalayer.battery.info.battery_discharge_max_current;
	mbPV[10] = datalayer.battery.info.battery_min_cell_temperature ;
	mbPV[11] = datalayer.battery.info.battery_max_cell_temperature ;
	mbPV[12] = datalayer.battery.info.battery_min_cell_voltage ;
	mbPV[13] = datalayer.battery.info. battery_max_cell_voltage;
	mbPV[14] = datalayer.battery.info. battery_inner_resistance;
	/*Battery FISKER BMS channels*/
	mbPV[15] = datalayer.battery.info.battery_average_cell_voltage ;
	mbPV[16] = datalayer.battery.info. battery_allowed_min_cell_voltage;
	mbPV[17] = datalayer.battery.info.battery_allowed_max_cell_voltage ;
	mbPV[18] = datalayer.battery.info. battery_balancing_status;
	mbPV[19] = datalayer.battery.info. battery_predicted_charge_voltage_long_time;
	mbPV[20] = datalayer.battery.info.battery_predicted_charge_voltage_middle_time ;
	mbPV[21] = datalayer.battery.info.battery_predicted_charge_voltage_short_time ;
	mbPV[22] = datalayer.battery.info.battery_inlet_temperature ;
	mbPV[23] = datalayer.battery.info.battery_outlet_temperature ;
	mbPV[24] = datalayer.battery.info.battery_predicted_discharge_current_long_time ;
	mbPV[25] = datalayer.battery.info.battery_predicted_discharge_current_middle_time ;
	mbPV[26] = datalayer.battery.info. battery_predicted_discharge_current_short_time;
	mbPV[27] = datalayer.battery.info.battery_predicted_discharge_voltage_long_time ;
	mbPV[28] = datalayer.battery.info.battery_predicted_discharge_voltage_middle_time ;
	mbPV[29] = datalayer.battery.info.battery_predicted_discharge_voltage_short_time ;
	mbPV[30] = datalayer.battery.info. battery_rolling_counter;
	mbPV[31] = datalayer.battery.info.battery_average_temperature ;
	mbPV[32] = datalayer.battery.info.battery_balancing_activation_status ;
	mbPV[33] = datalayer.battery.info.battery_cell_voltage_unbalanced_warning ;
	mbPV[34] = datalayer.battery.info. battery_general_status;
	mbPV[35] = datalayer.battery.info.battery_target_coolant_inlet_max_temperature ;
	mbPV[36] = datalayer.battery.info. battery_target_coolant_inlet_min_temperature;
	mbPV[37] = datalayer.battery.info.battery_target_coolant_inlet_temperature ;
	mbPV[38] = datalayer.battery.info. battery_water_inlet_temperature_requirements_of_pack;
	mbPV[39] = datalayer.battery.info.battery_inlet_water_temperature_sensor_fault ;
	mbPV[40] = datalayer.battery.info.battery_outlet_water_temperature_sensor_fault ;
	mbPV[41] = datalayer.battery.info.battery_over_voltage_fault_pack_1 ;
	mbPV[42] = datalayer.battery.info. battery_under_voltage_fault_pack_1;
	mbPV[43] = datalayer.battery.info.battery_high_and_low_voltage_interlock ;
	mbPV[44] = datalayer.battery.info.battery_bcu_eeprom_read_write_failure ;
	mbPV[45] = datalayer.battery.info.battery_bcu_status ;
	mbPV[46] = datalayer.battery.info.battery_cell_temperature_min_warning ;
	mbPV[47] = datalayer.battery.info.battery_t15_status ;
	mbPV[48] = datalayer.battery.info.battery_t30c_status ;
	mbPV[49] = datalayer.battery.info.battery_t30c_voltage ;
	mbPV[50] = datalayer.battery.info.battery_and_bobc_communication_failure ;
	mbPV[51] = datalayer.battery.info.battery_and_ccu_communication_failure ;
	mbPV[52] = datalayer.battery.info.battery_and_ecc_communication_failure ;
	mbPV[53] = datalayer.battery.info.battery_and_vcu_communication_failure ;
	mbPV[54] = datalayer.battery.info.battery_and_slave_measurement_communication_failure ;
	mbPV[55] = datalayer.battery.info.battery_and_bobc_communication_timeout ;
	mbPV[56] = datalayer.battery.info.battery_and_ecc_communication_timeout ;
	mbPV[57] = datalayer.battery.info. battery_main_contactor_pos_status;
	mbPV[58] = datalayer.battery.info.battery_main_contactor_neg_status ;
	mbPV[59] = datalayer.battery.info. battery_pre_charge_relay_status;
	mbPV[60] = datalayer.battery.info.battery_main_contactor_pos_error ;
	mbPV[61] = datalayer.battery.info.battery_main_contactor_neg_error ;
	mbPV[62] = datalayer.battery.info.battery_pre_charge_relay_error ;
	mbPV[63] = datalayer.battery.info.battery_dc_charging_socket_overtemp_fault ;
	mbPV[64] = datalayer.battery.info.battery_cat0_failure ;
	mbPV[65] = datalayer.battery.info.battery_cat1_failure ;
	mbPV[66] = datalayer.battery.info.battery_cat2_failure ;
	mbPV[67] = datalayer.battery.info.battery_cat3_failure ;
	mbPV[68] = datalayer.battery.info.battery_cat4_failure ;
	mbPV[69] = datalayer.battery.info.battery_cat5_failure ;
	mbPV[70] = datalayer.battery.info.battery_cat6_failure ;
	mbPV[71] = datalayer.battery.info.battery_cat7_failure ;
	mbPV[72] = datalayer.battery.info.battery_cat8_failure ;
	mbPV[73] = datalayer.battery.info.battery_cell_over_voltage_fault ;
	mbPV[74] = datalayer.battery.info.battery_cell_temp_high_fault ;
	mbPV[75] = datalayer.battery.info.battery_cell_temp_low_fault ;
	mbPV[76] = datalayer.battery.info.battery_cell_under_voltage_fault ;
	mbPV[77] = datalayer.battery.info.battery_cell_voltage_difference_over_limit_fault ;
	mbPV[78] = datalayer.battery.info.battery_hv_over_voltage ;
	mbPV[79] = datalayer.battery.info.battery_hv_under_voltage ;
	mbPV[80] = datalayer.battery.info.battery_isolation_resistance_under_operational_limit_warning ;
	mbPV[81] = datalayer.battery.info.battery_over_charge_current_fault ;
	mbPV[82] = datalayer.battery.info.battery_over_discharge_current_fault ;
	mbPV[83] = datalayer.battery.info.battery_soc_low_warning ;
	mbPV[84] = datalayer.battery.info.battery_soc_high_warning ;
	mbPV[85] = datalayer.battery.info.battery_functional_safety_fault ;
	mbPV[86] = datalayer.battery.info.battery_general_hardware_fault ;
	mbPV[87] = datalayer.battery.info.battery_hardware_status ;
	mbPV[88] = datalayer.battery.info.battery_hv_contactor_status ;
	mbPV[89] = datalayer.battery.info.battery_hv_interlock_status ;
	mbPV[90] = datalayer.battery.info.battery_precharge_relay_blocked ;
	mbPV[91] = datalayer.battery.info.battery_insulation_state ;
	mbPV[92] = datalayer.battery.info.battery_cell_temp_too_high ;
	mbPV[93] = datalayer.battery.info.battery_cell_temp_too_low ;
	mbPV[94] = datalayer.battery.info.battery_remaining_soc;
	mbPV[95] = std::min(datalayer.battery.info.total_capacity_Wh,static_cast<uint32_t>(60000u));  //Cap to 60kWh
    mbPV[96] =  datalayer.battery.info.battery_average_cell_temperature;
    mbPV[97] =  datalayer.battery.info.battery_thermal_management_required;
    mbPV[98] =  datalayer.battery.info.battery_soc_real;
    mbPV[99] =  datalayer.battery.info.battery_actual_capacity;
    mbPV[100] = datalayer.battery.info.battery_operating_states;
    mbPV[101] = datalayer.battery.info.battery_software_status;
    mbPV[102] = datalayer.battery.info.battery_current_sensor_status;
    mbPV[103] = datalayer.battery.info.battery_isolation_status;
    mbPV[104] = datalayer.battery.info.battery_service_disconnect_status;
    mbPV[105] = datalayer.battery.info.battery_measurement_module_status;
    mbPV[106] = datalayer.battery.info.battery_configuration_status;
    mbPV[107] = datalayer.battery.info.battery_state_machine_status;
	mbPV[108] = datalayer.battery.info.battery_bdu_status;

	datalayer.battery.info.battery_hv_contactor_status = mbPV[109];
}

#endif
