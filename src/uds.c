/*
 * uds.c
 *
 *  Created on: 2022年10月24日
 *      Author: wzw
 *      useage: uds服务端响应客户端请求
 */
#include "uds.h"

//函数名：  udsServer_requestProcess
//作者：    wzw
//日期：    2022-10-24
//功能：    服务端接收客户端请求数据并处理
//输入参数：p_udsServerData（服务端接收数据处理）
//返回值：  类型（void)
//修改记录：
static void udsServer_requestProcess(UdsDataType *p_udsServerData)
{
    // get service ID
    p_udsServerData->sid = (UDS_SID)p_udsServerData->rxMsgData[0];

    // get subFunciton
    p_udsServerData->subFunction = p_udsServerData->rxMsgData[1];

    // get did
    p_udsServerData->did = p_udsServerData->rxMsgData[1];
    p_udsServerData->did <<= 8;
    p_udsServerData->did |= p_udsServerData->rxMsgData[2];

    // switch services
    switch (p_udsServerData->sid) {
    /* session control：0x10 */
    case UDS_SID_SESSION_CONTROL:
        udsServer_sessionControl(p_udsServerData);
        break;
    /* ecu reset：0x11 */
    case UDS_SID_ECU_RESET:
        udsServer_ecuReset(p_udsServerData);
        break;

    default:
        break;
    }
}

//函数名：  udsServer_sessionControl
//作者：    wzw
//日期：    2022-10-24
//功能：    会话模式控制
//输入参数：p_udsServerData（服务端接收数据处理）
//返回值：  类型（void)
//修改记录：
static void udsServer_sessionControl(UdsDataType *p_udsServerData)
{
    if (p_udsServerData->rxMsgLength < 2) {
        udsServer_sendResponse(p_udsServerData, UDS_NRC_INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);
    } else if (p_udsServerData->subFunction != DEFAULT_SESSION || p_udsServerData->subFunction != PROGRAMMING_SESSION || p_udsServerData->subFunction != EXTEND_SESSION) {
        udsServer_sendResponse(p_udsServerData, UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
    } else {
        p_udsServerData->txMsgData[1] = p_udsServerData->subFunction;
        // 后面跟4个字节的时间参数，用来表示服务端3种状态：肯定响应，否定响应，处理中；这里取固定值，实际是由别的地方设置
        p_udsServerData->txMsgData[2] = 0x00;
        p_udsServerData->txMsgData[3] = 0x32; // p2server_max:50ms，服务端需要在该时间内完成响应，若超时也需要告知客户端，同时按照p2*时间回应
        p_udsServerData->txMsgData[4] = 0x01;
        p_udsServerData->txMsgData[5] = 0xF4; // p2*server_max:5000ms，若服务端仍未处理完，按照该周期定时发送
        p_udsServerData->txMsgLength = 6;
        udsServer_sendResponse(p_udsServerData, UDS_NRC_OK);
    }
}

//函数名：  udsServer_ecuReset
//作者：    wzw
//日期：    2022-10-25
//功能：    ecu复位
//输入参数：p_udsServerData（服务端接收数据处理）
//返回值：  类型（void)
//修改记录：
static void udsServer_ecuReset(UdsDataType *p_udsServerData)
{
    if (p_udsServerData->rxMsgLength < 2) {
        udsServer_sendResponse(p_udsServerData, UDS_NRC_INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);
    } else if (p_udsServerData->subFunction != HRAD_RESET || p_udsServerData->subFunction != KEY_OFF_ON_RESET || p_udsServerData->subFunction != SOFT_RESET) {
        udsServer_sendResponse(p_udsServerData, UDS_NRC_SUB_FUNCTION_NOT_SUPPORTED);
    } else {
        p_udsServerData->txMsgData[1] = p_udsServerData->subFunction;
        p_udsServerData->txMsgLength = 2;
        udsServer_sendResponse(p_udsServerData, UDS_NRC_OK);
    }
}

//函数名：  udsServer_sendResponse
//作者：    wzw
//日期：    2022-10-24
//功能：    服务端响应客户端请求
//输入参数：p_udsServerData（服务端发送数据处理），nrc（响应码）nrc==0表示肯定响应，nrc!=0表示否定响应
//返回值：  类型（void)
//修改记录：
static void udsServer_sendResponse(UdsDataType *p_udsServerData, UDS_NRC nrc)
{
    if (nrc == UDS_NRC_OK) {
        p_udsServerData->txMsgData[0] |= 0x40;
    } else {
        p_udsServerData->txMsgData[0] = 0x7F;
        p_udsServerData->txMsgData[1] = (uint8_t)p_udsServerData->sid;
        p_udsServerData->txMsgData[2] = (uint8_t)nrc;
        p_udsServerData->txMsgLength = 3;
    }
}