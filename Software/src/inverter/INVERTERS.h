#ifndef INVERTERS_H
#define INVERTERS_H

#include "../../USER_SETTINGS.h"

#ifdef BYD_MODBUS
#include "BYD-MODBUS.h"
#endif


#ifdef CAN_INVERTER_SELECTED
void update_values_can_inverter();
void receive_can_inverter(CAN_frame rx_frame);
void send_can_inverter();
#endif

#ifdef MODBUS_INVERTER_SELECTED
void update_modbus_registers_inverter();
#endif

#endif
