#ifndef FISKER_BATTERY_H
#define FISKER_BATTERY_H
#include "../include.h"

#define BATTERY_SELECTED

void send_can_battery(void);
void transmit_can(CAN_frame* tx_frame, int interface);

#endif
