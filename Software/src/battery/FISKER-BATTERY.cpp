#include "../include.h"
#ifdef FISKER_BATTERY
#include "../datalayer/datalayer.h"
#include "../devboard/utils/events.h"
#include "FISKER-BATTERY.h"


CAN_frame FISKER_93 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 16,
                     .ID = 0x93,
                     .data = {0x00, 0x21, 0x11, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
CAN_frame FISKER_94 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 16,
                     .ID = 0x94,
                     .data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
CAN_frame FISKER_644 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 8,
                     .ID = 0x644,
                     .data = {0x49, 0x00, 0x44, 0x40, 0x44, 0x44, 0x44, 0x00}};
CAN_frame FISKER_260 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 16,
                     .ID = 0x260,
                     .data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
CAN_frame FISKER_358 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 16,
                     .ID = 0x358,
                     .data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};					 

static unsigned long previousMillis20 = 0;  // will store last time a 20ms CAN Message was sent
static unsigned long previousSecond1 = 0;
static unsigned long previousMillis100 = 0;
  // will store last time a 20ms CAN Message was sent
static unsigned long initalValue = 0;  // will store last time a 20ms CAN Message was sent
static unsigned long aliveCounter = 0;  // will store last time a 20ms CAN Message was sent


const unsigned char crc8_table[256] =
    {  // CRC8_SAE_J1850_ZER0 formula,0x1D Poly,initial value 0x3F,Final XOR value varies
        0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53, 0xE8, 0xF5, 0xD2, 0xCF, 0x9C, 0x81, 0xA6, 0xBB, 0xCD, 0xD0,
        0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E, 0x25, 0x38, 0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76, 0x87, 0x9A, 0xBD, 0xA0,
        0xF3, 0xEE, 0xC9, 0xD4, 0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C, 0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23,
        0x04, 0x19, 0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1, 0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40,
        0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8, 0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D, 0x36, 0x2B,
        0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65, 0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7, 0x7C, 0x61, 0x46, 0x5B,
        0x08, 0x15, 0x32, 0x2F, 0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A, 0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8,
        0xFF, 0xE2, 0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75, 0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D,
        0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8, 0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50, 0xA1, 0xBC,
        0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2, 0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A, 0x6C, 0x71, 0x56, 0x4B,
        0x18, 0x05, 0x22, 0x3F, 0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7, 0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C,
        0x7B, 0x66, 0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E, 0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB,
        0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43, 0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1, 0x5A, 0x47,
        0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09, 0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C, 0x97, 0x8A, 0xAD, 0xB0,
        0xE3, 0xFE, 0xD9, 0xC4};

static uint8_t calculateCRC(CAN_frame rx_frame, uint8_t length, uint8_t initial_value) {
  uint8_t crc = initial_value;
  for (uint8_t j = 1; j < length; j++) {  //start at 1, since 0 is the CRC
    crc = crc8_table[(crc ^ static_cast<uint8_t>(rx_frame.data.u8[j])) % 256];
  }
  return crc;
}
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
	case 0x2F9:
		datalayer.battery.info.battery_charge_max_voltage = (rx_frame.data.u8[0] <<8)  | (rx_frame.data.u8[1]>> 8) ;
    	datalayer.battery.info.battery_discharge_min_voltage = (rx_frame.data.u8[2] <<8)  |  (rx_frame.data.u8[3] >> 8);
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
    	datalayer.battery.info.battery_capacity = (rx_frame.data.u8[4] << 8) || (rx_frame.data.u8[5]>>8);
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
		
		datalayer.battery.info.battery_cat8_failure = (rx_frame.data.u8[4] & 0x1);

		datalayer.battery.info.battery_t30c_status = (rx_frame.data.u8[4] & 0x80) >> 7;
		datalayer.battery.info.battery_service_disconnect_status = (rx_frame.data.u8[4] & 0x40) >> 6;
		datalayer.battery.info.battery_general_status = (rx_frame.data.u8[4] & 0x20) >> 5;
		datalayer.battery.info.battery_measurement_module_status = (rx_frame.data.u8[4] & 0x10) >> 4;
		datalayer.battery.info.battery_configuration_status = (rx_frame.data.u8[4] & 0x8) >> 3;
		datalayer.battery.info.battery_memory_empty_status = (rx_frame.data.u8[4] & 0x4) >> 2;
		datalayer.battery.info.battery_hardware_status = (rx_frame.data.u8[4] & 0x2) >> 1;
		


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
	FISKER_93.data.u8[0] = calculateCRC(FISKER_93, 8, 0xF3);
    transmit_can(&FISKER_93, can_config.battery);
	send_timeout();
	FISKER_260.data.u8[0] = initalValue;
	FISKER_260.data.u8[1] = aliveCounter;
	transmit_can(&FISKER_260, can_config.battery);
	FISKER_94.data.u8[0] = initalValue;
	transmit_can(&FISKER_94, can_config.battery);
  }
  
  if (currentMillis - previousSecond1 >= INTERVAL_1_S) {
    // Check if sending of CAN messages has been delayed too much.
    if ((currentMillis - previousSecond1 >= INTERVAL_1_S_DELAYED) && (currentMillis > BOOTUP_TIME)) {
      set_event(EVENT_CAN_OVERRUN, (currentMillis - previousSecond1));
    }
    previousSecond1 = currentMillis;
	FISKER_644.data.u8[0] = calculateCRC(FISKER_644, 8, 0xB9);
	transmit_can(&FISKER_644, can_config.battery);
  }

  
}

void send_timeout(){
	unsigned long currentMillis = millis();
if (currentMillis - previousMillis100 >= INTERVAL_100_MS) {
    // Check if sending of CAN messages has been delayed too much.
    if ((currentMillis - previousMillis100 >= INTERVAL_100_MS_DELAYED) && (currentMillis > BOOTUP_TIME)) {
      set_event(EVENT_CAN_OVERRUN, (currentMillis - previousMillis100));
    }
    previousMillis100 = currentMillis;
	//FISKER_260.data.u8[0] = calculateCRC(FISKER_644, 8, initalValue);
	if(initalValue == 255){
		initalValue = 0;
		aliveCounter++;
	}
	initalValue++ ;
  }
}

#endif
