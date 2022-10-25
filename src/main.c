/*
 * main.c
 *
 *  Created on: 2022年10月24日
 *      Author: wzw
 *      useage: 控制台模拟发送客户端服务请求
 */
#include <stdio.h>
#include <string.h>
#include "uds.h"

int main(int argc, char const *argv[])
{
    UdsDataType udsClientData;
    for(int i = 0; i < argc; i ++)
    {   
        udsClientData.txMsgData[i] = argv[i + 2];
    }
    udsClientData.subFunctionFlag = argv[1];
    msg_send(udsClientData);
    return 0;
}