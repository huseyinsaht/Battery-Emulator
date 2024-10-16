#include "../include.h"
#ifdef FISKER_BATTERY
#include "../datalayer/datalayer.h"
#include "../devboard/utils/events.h"
#include "FISKER-BATTERY.h"


CAN_frame FISKER_93 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 16,
                     .ID = 0x93,
                     .data = {0x00, 0x20, 0x11, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

static unsigned long previousMillis20 = 0;  // will store last time a 20ms CAN Message was sent

void update_values_battery() {  //This function maps all the values fetched via CAN to the correct parameters used for modbus


}

void receive_can_battery(CAN_frame rx_frame) {
  datalayer.battery.status.CAN_battery_still_alive = CAN_STILL_ALIVE;
  switch (rx_frame.ID) {
    case 0x2F5: 
    	datalayer.battery.info.battery_soc = rx_frame.data.u8[0];
		datalayer.battery.info.battery_actual_capacity = (rx_frame.data.u8[3] <<8)  | (rx_frame.data.u8[4]>> 8) ;
		datalayer.battery.info.battery_soh = rx_frame.data.u8[5];
		datalayer.battery.info.battery_soc_real = rx_frame.data.u8[6];
     break;
    case 0xE9:
    	datalayer.battery.info.battery_voltage = (rx_frame.data.u8[6] <<8)  | (rx_frame.data.u8[7] >> 8);
    	datalayer.battery.info.battery_current = (rx_frame.data.u8[4] <<8)  | (rx_frame.data.u8[5] >> 8);
     break;
    case 0x2F7:
    	datalayer.battery.info.battery_allowed_max_cell_voltage = (rx_frame.data.u8[0] <<8)  | (rx_frame.data.u8[1]>> 8) ;
    	datalayer.battery.info.battery_allowed_min_cell_voltage = (rx_frame.data.u8[2] <<8)  |  (rx_frame.data.u8[3] >> 8);
    	datalayer.battery.info.battery_discharge_max_current = (rx_frame.data.u8[4] <<8)  | (rx_frame.data.u8[5]>> 8) ;
    	datalayer.battery.info.battery_charge_max_current = (rx_frame.data.u8[6] <<8)   | (rx_frame.data.u8[7]>> 8) ;
    	break;
    case 0xF2:
    	datalayer.battery.info.battery_max_cell_voltage = (rx_frame.data.u8[0]<<8 )  | (rx_frame.data.u8[1]>> 8) ;
    	datalayer.battery.info.battery_min_cell_voltage = (rx_frame.data.u8[2] <<8)  | (rx_frame.data.u8[3] >> 8);
    	datalayer.battery.info.battery_average_cell_voltage = (rx_frame.data.u8[4]<<8 ) | (rx_frame.data.u8[5] >> 8);
    	datalayer.battery.info.battery_t30c_voltage = (rx_frame.data.u8[6] <<8)  | (rx_frame.data.u8[7]>> 8) ;
		break;
	case 0x372:
		datalayer.battery.info.battery_min_cell_temperature = rx_frame.data.u8[2];
    	datalayer.battery.info.battery_max_cell_temperature = rx_frame.data.u8[4];
		datalayer.battery.info.battery_average_cell_temperature = rx_frame.data.u8[5];
		datalayer.battery.info.battery_thermal_management_required = (rx_frame.data.u8[6] & 0x3);
		break; 
    case 0x3A5:
    	datalayer.battery.info.battery_target_coolant_inlet_temperature = (rx_frame.data.u8[5] >> 8);
    	datalayer.battery.info.battery_target_coolant_inlet_min_temperature = (rx_frame.data.u8[6]>> 8);
    	datalayer.battery.info.battery_target_coolant_inlet_max_temperature = (rx_frame.data.u8[7]>> 8);
    	break;
    case 0x215:
    	datalayer.battery.info.battery_cell_over_voltage_fault = (rx_frame.data.u8[2] &0x20) >> 5;
		datalayer.battery.info.battery_cell_under_voltage_fault = (rx_frame.data.u8[2] &0x10) >> 4;
    	datalayer.battery.info.battery_precharge_relay_blocked = (rx_frame.data.u8[3] &0x08) >> 3;
		datalayer.battery.info.battery_outlet_water_temperature_sensor_fault = (rx_frame.data.u8[3] &0x04) >> 2;
		datalayer.battery.info.battery_inlet_water_temperature_sensor_fault = (rx_frame.data.u8[3] &0x02) >> 1;
    	datalayer.battery.info.battery_functional_safety_fault = rx_frame.data.u8[6];
		break;
    case 0x330:
    	datalayer.battery.info.battery_remaining_soc = rx_frame.data.u8[3];
    	datalayer.battery.info.battery_capacity = rx_frame.data.u8[5] << 8 || rx_frame.data.u8[4];
    	datalayer.battery.info.battery_insulation_state = (rx_frame.data.u8[6] &0x80) >> 7;
    	datalayer.battery.info.battery_cell_temp_too_high = (rx_frame.data.u8[6] &0x20) >> 5;
		datalayer.battery.info.battery_cell_temp_too_low = (rx_frame.data.u8[6] & 0x1) ;
    	break;
    case 0xED:
    	datalayer.battery.info.battery_balancing_status = (rx_frame.data.u8[2] & 0xF0) >> 4;
		datalayer.battery.info.battery_operating_states = (rx_frame.data.u8[2] & 0xF);

		datalayer.battery.info.battery_cat7_failure = (rx_frame.data.u8[3] & 0x80) >> 7;
		datalayer.battery.info.battery_cat6_failure = (rx_frame.data.u8[3] & 0x40) >> 6;
		datalayer.battery.info.battery_cat5_failure = (rx_frame.data.u8[3] & 0x20) >> 5;
		datalayer.battery.info.battery_cat4_failure = (rx_frame.data.u8[3] & 0x10) >> 4;
		datalayer.battery.info.battery_cat3_failure = (rx_frame.data.u8[3] & 0x8) >> 3;
		datalayer.battery.info.battery_cat2_failure = (rx_frame.data.u8[3] & 0x4) >> 2;
		datalayer.battery.info.battery_cat1_failure = (rx_frame.data.u8[3] & 0x2) >> 1;
		datalayer.battery.info.battery_cat0_failure = (rx_frame.data.u8[3] & 0x1);
		
		
		datalayer.battery.info.battery_t30c_status = (rx_frame.data.u8[4] & 0x80) >> 7;
		datalayer.battery.info.battery_service_disconnect_status = (rx_frame.data.u8[4] & 0x40) >> 6;
		datalayer.battery.info.battery_general_status = (rx_frame.data.u8[4] & 0x20) >> 5;
		datalayer.battery.info.battery_measurement_module_status = (rx_frame.data.u8[4] & 0x10) >> 4;
		datalayer.battery.info.battery_configuration_status = (rx_frame.data.u8[4] & 0x8) >> 3;
		datalayer.battery.info.battery_memory_empty_status = (rx_frame.data.u8[4] & 0x4) >> 2;
		datalayer.battery.info.battery_hardware_status = (rx_frame.data.u8[4] & 0x2) >> 1;
		datalayer.battery.info.battery_cat8_failure = (rx_frame.data.u8[4] & 0x1);


		datalayer.battery.info.battery_t15_status = (rx_frame.data.u8[5] & 0x1);
		datalayer.battery.info.battery_hv_interlock_status = (rx_frame.data.u8[5] & 0x2) >> 1;
		datalayer.battery.info.battery_hv_contactor_status = (rx_frame.data.u8[5] & 0x4) >> 2;
		datalayer.battery.info.battery_software_status = (rx_frame.data.u8[5] & 0x8) >> 3;
		datalayer.battery.info.battery_current_sensor_status = (rx_frame.data.u8[3] & 0x10) >> 4;
		datalayer.battery.info.battery_isolation_status = (rx_frame.data.u8[3] & 0x20) >> 5;

		datalayer.battery.info.battery_state_machine_status = rx_frame.data.u8[6];
		datalayer.battery.info.battery_bdu_status = rx_frame.data.u8[7];

    	break;
    default:
      break;
  }
}

void send_can_battery() {
  unsigned long currentMillis = millis();
  // Send 100ms CAN Message
  if (currentMillis - previousMillis20 >= INTERVAL_20_MS) {
    // Check if sending of CAN messages has been delayed too much.
    if ((currentMillis - previousMillis20 >= INTERVAL_20_MS_DELAYED) && (currentMillis > BOOTUP_TIME)) {
      set_event(EVENT_CAN_OVERRUN, (currentMillis - previousMillis20));
    }
    previousMillis20 = currentMillis;
	
	FISKER_93.data.u8[0] = calculateCRC(FISKER_93, 8, 0x0);
    transmit_can(&FISKER_93, can_config.battery);
	
  }
}

#endif
