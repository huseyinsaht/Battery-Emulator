#ifndef __USER_SETTINGS_H__
#define __USER_SETTINGS_H__
#include <WiFi.h>
#include <stdint.h>

#define FISKER_BATTERY
#define MODBUS  //Enable this line to emulate over Modbus RTU

/* Select hardware used for Battery-Emulator */
#define HW_LILYGO
//#define HW_STARK

/* Other options */
//#define DEBUG_VIA_USB  //Enable this line to have the USB port output serial diagnostic data while program runs (WARNING, raises CPU load, do not use for production)
//#define DEBUG_CANFD_DATA    //Enable this line to have the USB port output CAN-FD data while program runs (WARNING, raises CPU load, do not use for production)
//#define INTERLOCK_REQUIRED  //Nissan LEAF specific setting, if enabled requires both high voltage conenctors to be seated before starting
//#define CONTACTOR_CONTROL     //Enable this line to have pins 25,32,33 handle automatic precharge/contactor+/contactor- closing sequence
//#define PWM_CONTACTOR_CONTROL //Enable this line to use PWM for CONTACTOR_CONTROL, which lowers power consumption and heat generation. CONTACTOR_CONTROL must be enabled.
//#define DUAL_CAN  //Enable this line to activate an isolated secondary CAN Bus using add-on MCP2515 chip (Needed for some inverters / double battery)
#define CAN_FD  //Enable this line to activate an isolated secondary CAN-FD bus using add-on MCP2518FD chip / Native CANFD on Stark board
//#define USE_CANFD_INTERFACE_AS_CLASSIC_CAN // Enable this line if you intend to use the CANFD as normal CAN
//#define SERIAL_LINK_RECEIVER  //Enable this line to receive battery data over RS485 pins from another Lilygo (This LilyGo interfaces with inverter)
//#define SERIAL_LINK_TRANSMITTER  //Enable this line to send battery data over RS485 pins to another Lilygo (This LilyGo interfaces with battery)
#define WIFI
#define WIFICONFIG  //Enable this line to set a static IP address / gateway /subnet mask for the device. see USER_SETTINGS.cpp for the settings
#define WEBSERVER  //Enable this line to enable WiFi, and to run the webserver. See USER_SETTINGS.cpp for the Wifi settings.
#define WEBSERVER_AUTH_REQUIRED \
  false         //Enable this line to enable webserver authentication. See USER_SETTINGS.cpp  setting the credentials.
#define WIFIAP  //Disable this line to permanently disable WIFI AP mode (make sure to hardcode ssid and password of you home wifi network). When enabled WIFI AP can still be disabled by a setting in the future.
#define MDNSRESPONDER  //Enable this line to enable MDNS, allows battery monitor te be found by .local address. Requires WEBSERVER to be enabled.
#define LOAD_SAVED_SETTINGS_ON_BOOT  //Enable this line to read settings stored via the webserver on boot (overrides Wifi/battery settings set below)
//#define FUNCTION_TIME_MEASUREMENT  // Enable this to record execution times and present them in the web UI (WARNING, raises CPU load, do not use for production)

/* MQTT options */
// #define MQTT  // Enable this line to enable MQTT
#define MQTT_SERVER "192.168.xxx.yyy"
#define MQTT_PORT 1883

/* Home Assistant options */
#define HA_AUTODISCOVERY  // Enable this line to send Home Assistant autodiscovery messages. If not enabled manual configuration of Home Assitant is required

/* Event options*/
#define DUMMY_EVENT_ENABLED false  //Enable this line to have a dummy event that gets logged to test the interface

/* Battery settings */
// Predefined total energy capacity of the battery in Watt-hours
#define BATTERY_WH_MAX 30000
// Increases battery life. If true will rescale SOC between the configured min/max-percentage
#define BATTERY_USE_SCALED_SOC true
// 8000 = 80.0% , Max percentage the battery will charge to (Inverter gets 100% when reached)
#define BATTERY_MAXPERCENTAGE 8000
// 2000 = 20.0% , Min percentage the battery will discharge to (Inverter gets 0% when reached)
#define BATTERY_MINPERCENTAGE 2000
// 300 = 30.0A , BYD CAN specific setting, Max charge in Amp (Some inverters needs to be limited)
#define BATTERY_MAX_CHARGE_AMP 300
// 300 = 30.0A , BYD CAN specific setting, Max discharge in Amp (Some inverters needs to be limited)
#define BATTERY_MAX_DISCHARGE_AMP 300

/* Do not change any code below this line unless you are sure what you are doing */
/* Only change battery specific settings in "USER_SETTINGS.h" */
typedef enum { CAN_NATIVE = 0, CANFD_NATIVE = 1, CAN_ADDON_MCP2515 = 2, CAN_ADDON_FD_MCP2518 = 3 } CAN_Interface;
typedef struct {
  CAN_Interface battery;
  CAN_Interface inverter;
  CAN_Interface battery_double;
  CAN_Interface charger;
} CAN_Configuration;
extern volatile CAN_Configuration can_config;
extern volatile uint8_t AccessPointEnabled;
extern const uint8_t wifi_channel;
extern volatile float charger_setpoint_HV_VDC;
extern volatile float charger_setpoint_HV_IDC;
extern volatile float charger_setpoint_HV_IDC_END;
extern volatile float CHARGER_SET_HV;
extern volatile float CHARGER_MAX_HV;
extern volatile float CHARGER_MIN_HV;
extern volatile float CHARGER_MAX_POWER;
extern volatile float CHARGER_MAX_A;
extern volatile float CHARGER_END_A;
extern bool charger_HV_enabled;
extern bool charger_aux12V_enabled;

#ifdef WIFICONFIG
extern IPAddress local_IP;
// Set your Gateway IP address
extern IPAddress gateway;
// Set your Subnet IP address
extern IPAddress subnet;
#endif

#endif  // __USER_SETTINGS_H__
