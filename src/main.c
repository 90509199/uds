/*
 * main.c
 *
 *  Created on: 2022年10月24日
 *      Author: wzw
 *      useage: 控制台模拟发送客户端服务请求
 */
#include <stdio.h>
#include <malloc.h>
#include "uds.h"

int main(int argc, char const *argv[])
{
    UdsDataType *p_udsServerData = (UdsDataType*)malloc(sizeof(UdsDataType));
    memset(p_udsServerData, 0, sizeof(UdsDataType));
    for(int i = 0; i < argc; i ++)
    {
        /* 模拟服务端接收客户端发来的服务请求 */
        p_udsServerData->rxMsgData[i] = (uint8_t)argv[i + 2];
    }
    /* 是否有子功能用第一个参数标志：0-无，1-有 */
    p_udsServerData->subFunctionFlag = argv[1];
    p_udsServerData->rxMsgLength = argc - 2;

    /* 等待服务端响应 */
    udsServer_requestProcess(p_udsServerData);

    /* 处理响应 */
    udsClient_responseProcess(p_udsServerData);

    free(p_udsServerData);
    return 0;
}