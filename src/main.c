/*
 * Copyright(C) 2022-2023
 *        file: main.c
 *        date: 2022年10月24日
 *      Author: wzw
 *      useage: 控制台模拟发送客户端服务请求
 */
#include <malloc.h>
#include "inc/uds.h"

int main(int argc, char const *argv[]) {
    UdsDataType *p_udsServerData = (UdsDataType*)malloc(sizeof(UdsDataType));  // NOLINT
    memset(p_udsServerData, 0, sizeof(UdsDataType));
    for (int i = 0; i < argc-1; i ++) {
        /* 模拟服务端接收客户端发来的服务请求 */
        p_udsServerData->rxMsgData[i] = (uint8_t)argv[i + 1];
    }
    p_udsServerData->rxMsgLength = argc - 1;

    /* 等待服务端响应 */
    udsServer_requestProcess(p_udsServerData);

    /* 处理响应 */
    udsClient_responseProcess(p_udsServerData);

    free(p_udsServerData);
    return 0;
}
