#ifndef FISKER_BATTERY_H
#define FISKER_BATTERY_H

#define BATTERY_SELECTED
#define MAX_CELL_DEVIATION_MV 250    // LED turns yellow on the board if mv delta exceeds this value

void send_can_battery(void);
void send_timeout(void);
void transmit_can(CAN_frame *tx_frame, int interface);

#endif
