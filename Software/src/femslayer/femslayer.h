#ifndef _FEMSLAYER_H_
#define _FEMSLAYER_H_

#include "../include.h"

typedef struct {
  /*Battery interface channels*/
  uint16_t battery_soc;
  uint16_t battery_soh;
  uint16_t battery_voltage;
  uint16_t battery_current;
  uint16_t battery_capacity;
  uint16_t battery_charge_max_voltage ;
  uint16_t battery_charge_max_current ;
  uint16_t battery_discharge_min_voltage ;
  uint16_t battery_discharge_max_current ;
  uint16_t battery_min_cell_temperature;
  uint16_t battery_max_cell_temperature;
  uint16_t battery_min_cell_voltage;
  uint16_t battery_max_cell_voltage;
  uint16_t battery_inner_resistance;

  /*Battery FISKER BMS channels*/
  uint16_t battery_balancing_status;
  uint16_t battery_predicted_charge_voltage_long_time;//[for short time 3000s value]
  uint16_t battery_predicted_charge_voltage_middle_time;//[for short time 30s value]
  uint16_t battery_predicted_charge_voltage_short_time;//[for short time 10s value]
  uint16_t battery_inlet_temperature;
  uint16_t battery_outlet_temperature;
  uint16_t battery_predicted_discharge_current_long_time;//[for short time 3000s value]
  uint16_t battery_predicted_discharge_current_middle_time;//[for short time 30s value]
  uint16_t battery_predicted_discharge_current_short_time;//[for short time 10s value]
  uint16_t battery_predicted_discharge_voltage_long_time;//[for short time 3000s value]
  uint16_t battery_predicted_discharge_voltage_middle_time;//[for short time 30s value]
  uint16_t battery_predicted_discharge_voltage_short_time;//[for short time 10s value]
  uint16_t battery_rolling_counter;
  uint16_t battery_average_temperature;
  uint16_t battery_balancing_activation_status;
  uint16_t battery_cell_voltage_unbalanced_warning;
  uint16_t battery_general_status;
  uint16_t battery_target_coolant_inlet_max_temperature;
  uint16_t battery_target_coolant_inlet_min_temperature;
  uint16_t battery_target_coolant_inlet_temperature;;
  uint16_t battery_water_inlet_temperature_requirements_of_pack;
  uint16_t battery_inlet_water_temperature_sensor_fault;
  uint16_t battery_outlet_water_temperature_sensor_fault;
  uint16_t battery_over_voltage_fault_pack_1;
  uint16_t battery_under_voltage_fault_pack_1;
  uint16_t battery_high_and_low_voltage_interlock;
  uint16_t battery_bcu_eeprom_read_write_failure;
  uint16_t battery_bcu_status;
  uint16_t battery_average_cell_voltage;
  uint16_t battery_cell_temperature_min_warning;
  uint16_t battery_cell_tempratures[MAX_AMOUNT_TEMPERATURE_CELLS];
  uint16_t battery_cell_voltages[MAX_AMOUNT_VOLTAGE_CELLS];
  uint16_t battery_t15_status;
  uint16_t battery_t30c_status;
  uint16_t battery_t30c_voltage;
  uint16_t battery_and_bobc_communication_failure;//FAULT
  uint16_t battery_and_ccu_communication_failure;//FAULT
  uint16_t battery_and_ecc_communication_failure;//FAULT
  uint16_t battery_and_vcu_communication_failure;//FAULT
  uint16_t battery_and_slave_measurement_communication_failure;//FAULT
  uint16_t battery_and_bobc_communication_timeout;//FAULT
  uint16_t battery_and_ecc_communication_timeout;//FAULT
  uint16_t battery_main_contactor_pos_status;
  uint16_t battery_main_contactor_neg_status;
  uint16_t battery_pre_charge_relay_status;
  uint16_t battery_main_contactor_pos_error;
  uint16_t battery_main_contactor_neg_error;
  uint16_t battery_pre_charge_relay_error;
  uint16_t battery_dc_charging_socket_overtemp_fault;
  uint16_t battery_cat0_failure;
  uint16_t battery_cat1_failure;
  uint16_t battery_cat2_failure;
  uint16_t battery_cat3_failure;
  uint16_t battery_cat4_failure;
  uint16_t battery_cat5_failure;
  uint16_t battery_cat6_failure;
  uint16_t battery_cat7_failure;
  uint16_t battery_cat8_failure;
  uint16_t battery_cell_over_voltage_fault;
  uint16_t battery_cell_temp_high_fault;
  uint16_t battery_cell_temp_low_fault;
  uint16_t battery_cell_under_voltage_fault;
  uint16_t battery_cell_voltage_difference_over_limit_fault;
  uint16_t battery_hv_overv_oltage;
  uint16_t battery_hv_under_voltage;
  uint16_t battery_isolation_resistance_under_operational_limit_warning;
  uint16_t battery_over_charge_current_fault;
  uint16_t battery_over_discharge_current_fault;
  uint16_t battery_soc_low_warning;
  uint16_t battery_soc_high_warning;
  uint16_t battery_functional_safety_fault;
  uint16_t battery_general_hardware_fault;
  uint16_t battery_hardware_status;
  uint16_t battery_hv_contactor_status;
  uint16_t battery_hv_interlock_status;


  uint16_t CAN_error_counter;
  /** uint8_t */
  /** A counter set each time a new message comes from battery.
   * This value then gets decremented each 5 seconds. Incase we reach 0
   * we report the battery as missing entirely on the CAN bus.
   */
  uint8_t CAN_battery_still_alive = CAN_STILL_ALIVE;

  /** Other */
  /** The current BMS status */
  bms_status_enum bms_status = ACTIVE;
} FEMSLAYER_BATTERY_STATUS_TYPE;

typedef struct {
  FEMSLAYER_BATTERY_STATUS_TYPE status;
} FEMSLAYER_BATTERY_TYPE;

typedef struct {
#ifdef FUNCTION_TIME_MEASUREMENT
  /** Core task measurement variable */
  int64_t core_task_max_us = 0;
  /** Core task measurement variable, reset each 10 seconds */
  int64_t core_task_10s_max_us = 0;
  /** MQTT sub-task measurement variable, reset each 10 seconds */
  int64_t mqtt_task_10s_max_us = 0;
  /** Wifi sub-task measurement variable, reset each 10 seconds */
  int64_t wifi_task_10s_max_us = 0;
  /** loop() task measurement variable, reset each 10 seconds */
  int64_t loop_task_10s_max_us = 0;

  /** OTA handling function measurement variable */
  int64_t time_ota_us = 0;
  /** CAN RX or serial link function measurement variable */
  int64_t time_comm_us = 0;
  /** 10 ms function measurement variable */
  int64_t time_10ms_us = 0;
  /** 5 s function measurement variable */
  int64_t time_5s_us = 0;
  /** CAN TX function measurement variable */
  int64_t time_cantx_us = 0;

  /** Function measurement snapshot variable.
   * This will show the performance of OTA handling when the total time reached a new worst case
   */
  int64_t time_snap_ota_us = 0;
  /** Function measurement snapshot variable.
   * This will show the performance of CAN RX or serial link when the total time reached a new worst case
   */
  int64_t time_snap_comm_us = 0;
  /** Function measurement snapshot variable.
   * This will show the performance of the 10 ms functionality of the core task when the total time reached a new worst case
   */
  int64_t time_snap_10ms_us = 0;
  /** Function measurement snapshot variable.
   * This will show the performance of the 5 s functionality of the core task when the total time reached a new worst case
   */
  int64_t time_snap_5s_us = 0;
  /** Function measurement snapshot variable.
   * This will show the performance of CAN TX when the total time reached a new worst case
   */
  int64_t time_snap_cantx_us = 0;
#endif
  /** True if the battery allows for the contactors to close */
  bool battery_allows_contactor_closing = false;
  /** True if the second battery allows for the contactors to close */
  bool battery2_allows_contactor_closing = false;
  /** True if the inverter allows for the contactors to close */
  bool inverter_allows_contactor_closing = true;
#ifdef CONTACTOR_CONTROL
  /** True if the contactor controlled by battery-emulator is closed */
  bool contactor_control_closed = false;
#endif
} FEMSLAYER_SYSTEM_STATUS_TYPE;

typedef struct {
  FEMSLAYER_SYSTEM_STATUS_TYPE status;
} FEMSLAYER_SYSTEM_TYPE;


class FemsLayer {
 public:
  FEMSLAYER_BATTERY_TYPE battery;
  FEMSLAYER_SYSTEM_TYPE system;
};

extern FemsLayer femslayer;

#endif
