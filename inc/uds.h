#ifndef _UDS_H_
#define _UDS_H_
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n",\
        __FILE__, __LINE__, ##__VA_ARGS__)

typedef enum
{
    /* service sid */
    UDS_SID_SESSION_CONTROL = 0x10,
    UDS_SID_ECU_RESET = 0x11,
    UDS_SID_SECURITY_ACCESS = 0x27,
    UDS_SID_COMUNICATION_CTROL = 0x28,
    UDS_SID_TESTER_PRESET = 0x3E,
    UDS_SID_ACCESS_TIMING_PARAMETER = 0x83,
    UDS_SID_SECRED_DATA_TRANSMISSION = 0x84,
    UDS_SID_CONTROL_DTC_SETTING = 0x85,
    UDS_SID_RESPONSE_ON_EVENT = 0x86,
    UDS_SID_LINK_CONTROL = 0x87,
    UDS_SID_READ_DATA_BY_IDENTIFIER = 0x22,
    UDS_SID_WRITE_DATA_BY_IDENTIFIER = 0x2E,
    UDS_SID_READ_MEMORY_BY_ADDRESS = 0x23,
    UDS_SID_WRITE_MEMORY_BY_ADDRESS = 0x3D,
    UDS_SID_READ_SCALING_DATA_BY_IDENTIFIER = 0x24,
    UDS_SID_READ_DATA_BY_PERIOD_IDENTIFIER = 0x2A,
    UDS_SID_DYNAMICAL_DEFINE_DATA_IDENTIFIER = 0x2C,
    UDS_SID_CLEAR_DTC = 0x14,
    UDS_SID_READ_DTC = 0x19,
    UDS_SID_IO_CONTROL_BY_IDENTIFIER = 0x2F,
    UDS_SID_ROUTINE_CONTROL = 0x31,
    UDS_SID_REQUEST_DOWNLOAD = 0x34,
    UDS_SID_REQUEST_UPLOAD = 0x35,
    UDS_SID_TRANSFER_DATA = 0x36,
    UDS_SID_REQUEST_TRANSFER_EXIT = 0x37,
    UDS_SID_REQUEST_FILE_TRANSFER = 0x38
} UDS_SID;

typedef enum 
{
    UDS_NRC_OK = 0,
    UDS_NRC_GENERAL_REJECT = 0x11,
    UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED = 0x12,
    UDS_NRC_INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT = 0x13,
    UDS_NRC_CONDITION_NOT_CORRECT = 0x22,
    UDS_NRC_REQUEST_OUT_OF_RANGE = 0x31,
    UDS_NRC_RESPONSE_PENDING = 0x78
    /* ... */
} UDS_NRC;

typedef enum
{
    /* DTC for chery*/
    CIRCUIT_VOLTAGE_BELOW_THRESHOLD = 0X950016,
    CIRCUIT_VOLTAGE_ABOVE_THRESHOLD = 0X950116
    /* ... */
} UDS_DTC;

typedef enum
{
    /* DID for chery*/
    VEHICLE_MANUFACTURE_SPARE_PART_NUMBER = 0xF187,
    VEHICLE_MANUFACTURE_ECU_HARD_WARE_NUMBER = 0xF191,
    VEHICLE_MANUFACTURE_ECU_SOFT_WARE_NUMBER = 0xF188,
    SYSTEM_SUPPLIER_IDENTIFIER = 0xF18A
    /* ... */
} UDS_DID;

typedef enum
{
    DEFAULT_SESSION = 0x1,
    PROGRAMMING_SESSION = 0x2,
    EXTEND_SESSION = 0x3
    /* ... */
} UDS_SESSION;

typedef enum
{
    HRAD_RESET = 0x1,
    KEY_OFF_ON_RESET = 0x2,
    SOFT_RESET = 0x3
    /* ... */
} UDS_ECU_RESET;

#define UDS_MESSAGE_BUFFER_SIZE 64

typedef struct
{
    uint8_t rxMsgData[UDS_MESSAGE_BUFFER_SIZE];
    uint8_t rxMsgLength;

    uint8_t txMsgData[UDS_MESSAGE_BUFFER_SIZE];
    uint8_t txMsgLength;

    UDS_SID sid;            // service id
    uint16_t did;           // data id
    uint8_t subFunction;    // subFunction
} UdsDataType;

/* client */
static void udsClient_responseProcess(UdsDataType *p_udsServerData);

/* server */
static void udsServer_requestProcess(UdsDataType *p_udsServerData);
static void udsServer_sendResponse(UdsDataType *p_udsServerData, UDS_NRC nrc);

/* service function */
static void udsServer_sessionControl(UdsDataType *p_udsServerData);
static void udsServer_ecuReset(UdsDataType *p_udsServerData);

#endif
