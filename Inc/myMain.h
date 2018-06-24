/*******************************************************************************
* File Name          : myMain.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/8/2018
* Description        : Demonstration of myServo Middleware.
********************************************************************************
* 說明：在STM32F03C8T6(Bluepill board)上使用 myServo Middleware 作為展示範例
*  使用的 servo 參數為：
*     period：20ms
*     duty cycle: 0.5ms ~ 2ms
*     角度：正負90度
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#ifndef __MYMAIN_H__
#define __MYMAIN_H__
#include "PLCLogic.h"
#include "FBDprotocol.h"
/* Private typedef -----------------------------------------------------------*/
#define PWM_Period 60000
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void myMain();
void device_init();
void protocol_init(FUNCTION_BLOCK_POOL_t **pool);
void device_timer_callback();
void cb_set_graph(packet_t *packet);
void cb_push_input(packet_t *packet);
void tim2_callback();
void ur1_callback();
void protocol_rx_byte_in(unsigned char);
/* Private functions ---------------------------------------------------------*/
#endif