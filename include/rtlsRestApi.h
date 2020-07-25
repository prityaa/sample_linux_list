#ifndef _RTSL_REST_API_H_
#define _RTSL_REST_API_H_

#include <stdio.h>
#include <stdlib.h>

#define UID_NAME_SIZE_MAX 256
#define UID_EMAIL_SIZE_MAX 256
#define UID_IMEI_SIZE_MAX 256

#define CELL_CHANNEL_SIZE_MAX 256
#define MAC_ADDRESS_SIZE_MAX 256
#define BLE_NAME_SIZE_MAX 256
#define WIFI_SSID_SIZE_MAX 256

#define SERVER_POLICY_KEY_VALUE_SIZE 256

#define TIME_STAMP_SIZE_MAX 256
#define SERVER_SERVICE_ID_SIZE 256
#define SERVER_STATUS_MSG_SIZE 256

#define API_KEY_SIZE_MAX 256

//One of name or email or IMEI is mandatory. Othes to be set to null.
struct userId_t {
    char name[UID_NAME_SIZE_MAX];
    char email[UID_EMAIL_SIZE_MAX];
    char imei[UID_IMEI_SIZE_MAX];
};

//Policy Parameters. Parameters are described in getapikey API.
//Users are to apply these parameters for subsequent requests.
//Currently 2 policy parameters supported
//maxRequestPerDay - Maximum number of requests allowed per day
//minRequestIntervalSeconds - The minimum interval between 2 requests in seconds, when API is invoked periodically.
struct serverPolicy_t {
    char key[SERVER_POLICY_KEY_VALUE_SIZE];
    int value;
};

//User identification such as name, email or imei.
struct getApiKeyReq_t {
    struct userId_t uid; //As provided by user in request
};

//Get API key response received from server.
struct getApiKeyResp_t {
    struct userId_t uid;
    struct serverPolicy_t *policy;
    char time[TIME_STAMP_SIZE_MAX]; //Time at that instant in UTC
    int respCode; //Response Code (200, 400, 500 etc.)
    char apiKey[API_KEY_SIZE_MAX]; //Key to access location services
};

struct gpsCoords_t {
    float lat; //Latitude as determined from GPS
    float lng; //Longitude as determined from GPS
};

//LTE Cells. Array of this struct can be provided for multiple cells.
struct lteCell_t {
    int mcc; //The Mobile Country Code. ** Mandatory **
    int mnc; //The Mobile Network Code. ** Mandatory **
    int tac; //The Tracking Area Code (TAC). ** Mandatory **
    int cellId; //The cell id. ** Mandatory **
    int age; //milliseconds since this cell was detected
    int pci; //Physical cell id
    int rssi; //The received signal strength. ** Mandatory **
    int rscp; //Received Signal Code Power
    int ta; //The timing advance value for this cell
    bool serving; //true indicates that the device is currently associated with this cell tower.
    char channel[CELL_CHANNEL_SIZE_MAX]; //Channel Number (ARFCN)
};

//WCDMA (3G) Cells. Array of this struct can be provided for multiple cells.
struct wcdmsCell_t {
    int mcc; //The Mobile Country Code. ** Mandatory **
    int mnc; //The Mobile Network Code. ** Mandatory **
    int lac; //The Location Area Code (TAC). ** Mandatory **
    int cellId; //The cell id. ** Mandatory **
    int age; //milliseconds since this cell was detected
    int psc; //Primary Scrambling Code
    int rssi; //The received signal strength. ** Mandatory **
    int ta; //The timing advance value for this cell
    bool serving; //true indicates that the device is currently associated with this cell tower.
    char channel[CELL_CHANNEL_SIZE_MAX]; //Channel Number
};

//GSM (2G) Cells. Array of this struct can be provided for multiple cells.
struct gsmCell_t {
    int mcc; //The Mobile Country Code. ** Mandatory **
    int mnc; //The Mobile Network Code. ** Mandatory **
    int lac; //The Location Area Code (TAC). ** Mandatory **
    int cellId; //The cell id. ** Mandatory **
    int age; //milliseconds since this cell was detected
    int bsic; //Base Station Identity Code
    int rssi; //The received signal strength. ** Mandatory **
    int ta; //The timing advance value for this cell
    bool serving; //true indicates that the device is currently associated with this cell tower.
    char channel[CELL_CHANNEL_SIZE_MAX]; //Channel Number
};

//Wi-Fi Access Points. Array of this struct can be provided for multiple cells.
struct wifiAp_t {
    char macAddress[MAC_ADDRESS_SIZE_MAX]; //The BSSID of the WiFi network. ** Mandatory **
    int age; //The number of milliseconds since this network was last detected.
    int channel; //The WiFi channel for networks in the 2.4GHz range. This often ranges from 1 to 13.
    int frequency; //The frequency in MHz of the channel over which the client is communicating with the access point.
    int rssi; //The received signal strength (RSSI) in dBm. ** Mandatory **
    int snr; //The current signal to noise ratio measured in dB.
    char ssid[WIFI_SSID_SIZE_MAX]; //The SSID of the Wifi network. Wifi networks with a SSID ending in _nomap must not be collected.
    bool serving; //true indicates that the device is currently connected to this access point.
};

//Bluetooth Low Energy Beacons. Array of this struct can be provided for multiple cells.
struct bleBeacon_t {
    char macAddress[MAC_ADDRESS_SIZE_MAX]; //The address of the Bluetooth Low Energy (BLE) beacon. ** Mandatory **
    char name[BLE_NAME_SIZE_MAX]; //The name of the BLE beacon.
    int age; //The number of milliseconds since this BLE beacon was last seen.
    int rssi; //The measured signal strength of the BLE beacon in dBm. ** Mandatory **
};
    
//GeoLocate input parameters for Get location.
struct geoLocReq_t {
    char apiKey[API_KEY_SIZE_MAX]; //Key to access location services
    char serviceId[SERVER_SERVICE_ID_SIZE]; //serviceId of the provider
    
    int hasGpsCoords;
    int numLteCells;
    int numWcdmsCells;
    int numGsmCells;
    int numWifiAps;
    int numBleBeacons;
    
    struct gpsCoord_t *gpsCoords;
    struct lteCell_t *lteCells;
    struct wcdmsCell_t *wcdmaCells;
    struct gsmCell_t *gsmCells;
    struct wifiAp_t *wifiAps; //For WiFi based position estimate at least two networks need to be provided.
    struct bleBeacon_t *bleBeaons; //For Bluetooth based position at least three networks to be provided.
};

//List of locations of this client as calculated by various methods/services within RTLS
struct respLocation_t {
    char provider[SERVER_SERVICE_ID_SIZE]; //serviceId of the provider
    int statusCode; //Indicates success/failure of this provider. 200 means success
    char statusMsg[SERVER_STATUS_MSG_SIZE]; //Contains error details in case of error.
    float latitude; //Latitude
    float longitude; //Longitude
    float accuracy; //Accuracy in meters
    char fallback; //Loc in resp corresponds to LAC or TAC or IP. Values can be acf (area code fall back) (if lac or tac) or ipf (IP).
    struct serverPolicy_t *policy; //Parameters are described in getapikey API. Users are to apply these parameters for subsequent requests.
    int code; //Response code
    char time[TIME_STAMP_SIZE_MAX]; //Time at that instant in UTC
};

//GeoLocate api Response from RTLS server
struct geoLocResp_t {
    char statusMessage[SERVER_STATUS_MSG_SIZE]; //Message showing the result of API Call
    char startTime[TIME_STAMP_SIZE_MAX]; //Start time of Api Call
    char endTime[TIME_STAMP_SIZE_MAX]; //End time of Api Call
    struct respLocation_t *locations; //list of locations from server
};

//Known position submission to server
struct geoPosition_t {
    float lat; //The latitude of the observation (WSG 84). 
    float lng; //The longitude of the observation (WSG 84). 
    float accuracy; //The accuracy of the observed position in meters.
    int age; //The age of the position data (in milliseconds).
    int altitude; //The altitude at which the data was observed in meters above sea-level.
    int altitudeAccuracy; //The accuracy of the altitude-estimate in meters.
    float heading; /*The heading field denotes the direction of travel of the device and is specified in degrees, 
                    where 0° ≤ heading < 360°, counting clockwise relative to the true north. */
    float pressure; //The air pressure in hPa (millibar).
    float speed; /*The speed field denotes the magnitude of the horizontal component of the device’s 
                    current velocity and is specified in meters per second. */
    char source[SERVER_SERVICE_ID_SIZE]; /* The source of the position information. If the field is omitted, “gps” is assumed. 
                        The term gps is used to cover all types of satellite based positioning systems 
                        including Galileo and Glonass. Other possible values are manual for a position 
                        entered manually into the system and fused for a position obtained from a 
                        combination of other sensors or outside service queries. */
};

//Items list in GeoSubmit api
struct geoSubItem_t {
    struct geoPosition_t position;
    
    int numLteCells;
    int numWcdmsCells;
    int numGsmCells;
    int numWifiAps;
    int numBleBeacons;
    
    struct lteCell_t *lteCells;
    struct wcdmsCell_t *wcdmaCells;
    struct gsmCell_t *gsmCells;
    struct wifiAp_t *wifiAps; //For WiFi based position estimate at least two networks need to be provided.
    struct bleBeacon_t *bleBeaons; //For Bluetooth based position at least three networks to be provided.
};

//GeoSubmit api request parameters
struct geoSubReq_t {
    char apiKey[API_KEY_SIZE_MAX]; //Key to access location services
    char serviceId[SERVER_SERVICE_ID_SIZE]; //serviceId of the provider

    struct geoSubItem_t *items;
};

//GeoSubmit api response parameters
struct geoSubResp_t {
    char time[TIME_STAMP_SIZE_MAX]; //Time at that instant in UTC
    int code; //Response code
    char message[SERVER_STATUS_MSG_SIZE]; //API message
};

/* This API to be used by every user to obtain a key to use in other APIs.
 * One of name or email or IMEI is mandatory.
 */
int rtlsGetApiKey(struct getApiKeyReq_t reqParams, getApiKeyResp_t *respOut);

/* This API provides the location (latitude and longitude) to a User.
 * This API is also used to compare the location known to the User such as via GPS and that calculated by the service.
 */
int rtlsGeoLocate(struct geoLocReq_t *reqParams, 
		struct geoLocResp_t *respOut);

/* This API is used to submit the cell towers, wifi access points and Bluetooth beacons information to the location service DB.
 * Each request must contain at least one entry in the bluetoothBeacons or cells array or two entries in the wifiAccessPoints array.
 */
int rtlsGeoSubmit(struct geoSubReq_t *reqParams, struct geoSubResp_t *respOut);

#endif //_RTSL_REST_API_H_
