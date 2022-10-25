/*
 * uds.c
 *
 *  Created on: 2022年10月24日
 *      Author: wzw
 *      useage: uds服务端响应客户端请求
 */
#include <stdint.h>
#include "uds.h"

//函数名：  udsServer_requestProcess
//作者：    wzw
//日期：    2022-10-24
//功能：    服务端接收客户端请求数据并处理
//输入参数：
//返回值：  类型（void)
//修改记录：
static void udsServer_requestProcess(void)
{
    // get service ID
	udsServerData.sid = (UDS_SID)udsServerData.rxMsgData[0];

    // get subFunciton
    udsServerData.subFunction = udsServerData.rxMsgData[1];

	// get did
	udsServerData.did = udsServerData.rxMsgData[1];
	udsServerData.did <<= 8;
	udsServerData.did |= udsServerData.rxMsgData[2];

	// switch services
	switch(udsServerData.sid)
	{
		/* session control：0x10 */
		case UDS_SID_SESSION_CONTROL:
			udsServer_sessionControl();
			break;

		default:
			break;
	}
}

//函数名：  udsServer_sessionControl
//作者：    wzw
//日期：    2022-10-24
//功能：    会话模式控制
//输入参数：
//返回值：  类型（void)
//修改记录：
static void udsServer_sessionControl(void)
{
    if( udsServerData.rxMsgLength < 2) {
        udsServer_sendResponse(UDS_NRC_INCORRECT_MESSAGE_LENGTH_OR_INVALID_FORMAT);
    } else {
        udsServer_sendResponse(UDS_NRC_OK);
    }
}

//函数名：  udsServer_sendResponse
//作者：    wzw
//日期：    2022-10-24
//功能：    服务端响应客户端请求
//输入参数：nrc（响应码）nrc==0表示肯定响应，nrc!=0表示否定响应
//返回值：  类型（void)
//修改记录：
static void udsServer_sendResponse(UDS_NRC nrc)
{
    if(nrc == UDS_NRC_OK) {
		udsServerData.txMsgData[0] |= 0x40;
        if (udsServerData.subFunctionFlag)
        {
            udsServerData.txMsgLength = 2;
        }
        udsServerData.txMsgLength = 1;
		msg_send(udsServerData);
	} else {
		udsServerData.txMsgData[0] = 0x7F;
		udsServerData.txMsgData[1] = (uint8_t)udsServerData.sid;
		udsServerData.txMsgData[2] = (uint8_t)nrc;
		udsServerData.txMsgLength = 3;
		msg_send(udsServerData);
	}
}

//函数名：  msg_send
//作者：    wzw
//日期：    2022-10-24
//功能：    报文发送
//输入参数：msg（发送数据）
//返回值：  类型（void)
//修改记录：
static void msg_send(UdsDataType msg)
{
    /* to do */
}

//函数名：  msg_recieve
//作者：    wzw
//日期：    2022-10-24
//功能：    报文接收
//输入参数：msg（接收数据）
//返回值：  类型（void)
//修改记录：
static void msg_recieve(UdsDataType* rxmsg, UdsDataType txMsg)
{
    for (int i = 0; i < txMsg.txMsgLength; i++)
    {
        rxmsg->rxMsgData[i] = txMsg.txMsgData[i];
    }
    rxmsg->rxMsgLength = txMsg.txMsgLength;
    rxmsg->txMsgData[0] = txMsg.txMsgData[0]; 
    rxmsg->subFunctionFlag = txMsg.subFunctionFlag;
    /* to do */
}