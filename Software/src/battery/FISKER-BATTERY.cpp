#ifdef FISKER_BATTERY
#include <algorithm>  // For std::min and std::max
#include "../datalayer/datalayer.h"
#include "../devboard/utils/events.h"
#include "FISKER-BATTERY.h"


CAN_frame FISKER_93 = {.FD = true,
                     .ext_ID = false,
                     .DLC = 8,
                     .ID = 0x93,
                     .data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};


static unsigned long previousMillis100 = 0;  // will store last time a 100ms CAN Message was sent

void update_values_battery() {  //This function maps all the values fetched via CAN to the correct parameters used for modbus
  datalayer.battery.status.soh_pptt = LB_SOH;
  datalayer.battery.status.real_soc = SOC_polled;
  datalayer.battery.status.current_dA = LB_Current;

  //Calculate the remaining Wh amount from SOC% and max Wh value.
  datalayer.battery.status.remaining_capacity_Wh = static_cast<uint32_t>(
      (static_cast<double>(datalayer.battery.status.real_soc) / 10000) * datalayer.battery.info.total_capacity_Wh);

  datalayer.battery.status. = 5000;  //TODO: Take from CAN
  datalayer.battery.status.max_charge_power_W = 5000;  //TODO: Take from CAN

  //Power in watts, Negative = charging batt
  datalayer.battery.status.active_power_W =
      ((datalayer.battery.status.voltage_dV * datalayer.battery.status.current_dA) / 100);

  int16_t temperatures[] = {cell_1_temperature_polled,  cell_2_temperature_polled,  cell_3_temperature_polled,
                            cell_4_temperature_polled,  cell_5_temperature_polled,  cell_6_temperature_polled,
                            cell_7_temperature_polled,  cell_8_temperature_polled,  cell_9_temperature_polled,
                            cell_10_temperature_polled, cell_11_temperature_polled, cell_12_temperature_polled};

  // Find the minimum and maximum temperatures
  int16_t min_temperature = *std::min_element(temperatures, temperatures + 12);
  int16_t max_temperature = *std::max_element(temperatures, temperatures + 12);

  datalayer.battery.status.temperature_min_dC = min_temperature * 10;

  datalayer.battery.status.temperature_max_dC = max_temperature * 10;

  //Map all cell voltages to the global array
  memcpy(datalayer.battery.status.cell_voltages_mV, cellvoltages, 96 * sizeof(uint16_t));

  // Initialize min and max, lets find which cells are min and max!
  uint16_t min_cell_mv_value = std::numeric_limits<uint16_t>::max();
  uint16_t max_cell_mv_value = 0;
  calculated_total_pack_voltage_mV =
      datalayer.battery.status.cell_voltages_mV
          [0];  // cell96 issue, this value should be initialized to 0, but for now it is initialized to cell0
  // Loop to find the min and max while ignoring zero values
  for (uint8_t i = 0; i < datalayer.battery.info.number_of_cells; ++i) {
    uint16_t voltage_mV = datalayer.battery.status.cell_voltages_mV[i];
    calculated_total_pack_voltage_mV += voltage_mV;
    if (voltage_mV != 0) {  // Skip unread values (0)
      min_cell_mv_value = std::min(min_cell_mv_value, voltage_mV);
      max_cell_mv_value = std::max(max_cell_mv_value, voltage_mV);
    }
  }
  // If all array values are 0, reset min/max to 3700
  if (min_cell_mv_value == std::numeric_limits<uint16_t>::max()) {
    min_cell_mv_value = 3700;
    max_cell_mv_value = 3700;
    calculated_total_pack_voltage_mV = 370000;
  }

  datalayer.battery.status.cell_min_voltage_mV = min_cell_mv_value;
  datalayer.battery.status.cell_max_voltage_mV = max_cell_mv_value;
  datalayer.battery.status.voltage_dV =
      static_cast<uint32_t>((calculated_total_pack_voltage_mV / 100));  // Convert from mV to dV

  if (datalayer.battery.status.cell_max_voltage_mV >= ABSOLUTE_CELL_MAX_VOLTAGE) {
    set_event(EVENT_CELL_OVER_VOLTAGE, 0);
  }
  if (datalayer.battery.status.cell_min_voltage_mV <= ABSOLUTE_CELL_MIN_VOLTAGE) {
    set_event(EVENT_CELL_UNDER_VOLTAGE, 0);
  }

#ifdef DEBUG_VIA_USB

#endif
}

void receive_can_battery(CAN_frame rx_frame) {
  datalayer.battery.status.CAN_battery_still_alive = CAN_STILL_ALIVE;
  switch (rx_frame.ID) {
    case 0x2F5: 
    	datalayer.battery.status.real_soc = rx_frame.data.u8[5];
     break;
    case 0xE9:
    	datalayer.battery.status.battery_voltage = rx_frame.data.u8[7] << 8 || rx_frame.data.u8[8];
    	datalayer.battery.status.battery_current = rx_frame.data.u8[5] << 8 || rx_frame.data.u8[6];
     break;
     case 0x360
      	datalayer.battery.status.battery_insulation_resistance_pos_pole =  rx_frame.data.u8[5] << 8 || rx_frame.data.u8[4]; 
        datalayer.battery.status.battery_insulation_resistance_neg_pole =  rx_frame.data.u8[7] << 8 || rx_frame.data.u8[6]; 
     break;
     case 0x2EE
        datalayer.battery.status.battery_main_contactor_pos_status =  (rx_frame.data.u8[0] & 0x00);
     break;
      case 0x2EF
        datalayer.battery.status.battery_main_contactor_neg_status =  (rx_frame.data.u8[0] & 0x00);
     break;
     case 0x2EF
        datalayer.battery.status.battery_soc =  rx_frame.data.u8[0] << 8;
        datalayer.battery.status.battery_soh =  rx_frame.data.u8[5] << 8;
     break;
    default:
      break;
  }
}

void send_can_battery() {
  unsigned long currentMillis = millis();
  // Send 100ms CAN Message
  if (currentMillis - previousMillis100 >= INTERVAL_100_MS) {
    // Check if sending of CAN messages has been delayed too much.
    if ((currentMillis - previousMillis100 >= INTERVAL_100_MS_DELAYED) && (currentMillis > BOOTUP_TIME)) {
      set_event(EVENT_CAN_OVERRUN, (currentMillis - previousMillis100));
    }
    previousMillis100 = currentMillis;
    transmit_can(&FISKER_93, can_config.battery);
  }
}

#endif
