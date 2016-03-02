/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : drive_usart.h
  Version       : Initial Draft
  Author        : Hui.Liu
  Created       : 2015/2/24
  Last Modified :
  Description   : drive_usart.c header file
  Function List :
  History       :
  1.Date        : 2015/2/24
    Author      : Hui.Liu
    Modification: Created file

******************************************************************************/

#ifndef __DRIVE_USART_H__
#define __DRIVE_USART_H__

#include "typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#define USART1_RxBuff_SIZE			50
#define USART1_TxBuff_SIZE			50

    typedef struct
    {
        BOOL flag;
        UINT8 size;
        UINT8 buff[USART1_RxBuff_SIZE];
    } USART_RX_STRUCT;

    extern USART_RX_STRUCT st_USART1_RX;

    extern void drv_usart1Init(UINT32 boud);
    extern void drv_usart1RxHandler(void);
    extern void drv_usart1TxHandler(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HWA_USART_H__ */
