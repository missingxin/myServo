/*******************************************************************************
* File Name          : myMain.c
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
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "myMain.h"
#include "myServo.h"

#include "PLCOpen.h"
#include "PLCLogic.h"
#include "FBDprotocol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

// ** 把printf的輸出指向uart2, 另外還需要把syscalls.c放進來編譯 **
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1,0xFFFF);
  return ch;
}
asm(".global _printf_float");  //或 linker flag 加入 “-u _printf_float”

FUNCTION_BLOCK_POOL_t *GLOBALPOOL = 0;

/*******************************************************************************
* Function Name  : myMain
* Description    : real main function.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/

unsigned char rx;
extern unsigned char fbd_start;

    void
    ur1_callback()
{
  unsigned char ch = rx;
  protocol_rx_byte_in(ch);
  HAL_UART_Receive_IT(&huart1, &rx, 1);
  //HAL_UART_Transmit(&huart2,data,length,500);
}
void tim2_callback(){
  device_timer_callback();
  if (GLOBALPOOL && fbd_start)
  {
    updateFBS(GLOBALPOOL->fbpool, GLOBALPOOL->fbpoolCnt);
    dump_IO(GLOBALPOOL->ipool,
          GLOBALPOOL->ipoolCnt,
          GLOBALPOOL->opool,
          GLOBALPOOL->opoolCnt);
  }
}
void
myMain()
{
  HAL_UART_Receive_IT(&huart1, &rx, 1);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
  int ledi;
  for(ledi=0;ledi<6;ledi++){
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  }
  printf("myMain in\r\n");
  device_init();
  protocol_init(&GLOBALPOOL);
  //since we don't always have debugger, here are some dump functions for our convenience.
  // printf("dump fbpool:\r\n");
  // dump_FBPool(GLOBALPOOL->fbpool, GLOBALPOOL->fbpoolCnt);
  // printf("dump ipool:\r\n");
  // dump_FBPool(GLOBALPOOL->ipool, GLOBALPOOL->ipoolCnt);
  // printf("dump opool:\r\n");
  // dump_FBPool(GLOBALPOOL->opool, GLOBALPOOL->opoolCnt);
  // while (1)
  //   ;

  // int i;
  // printf("GLOBALPOOL = %lx\r\n", (unsigned long)GLOBALPOOL);
  // for (i = 0; i < 100; i++)
  // {
  //   printf("%3d ",i);
  //   updateFBS(GLOBALPOOL->fbpool, GLOBALPOOL->fbpoolCnt);
  //   dump_IO(GLOBALPOOL->ipool,
  //           GLOBALPOOL->ipoolCnt,
  //           GLOBALPOOL->opool,
  //           GLOBALPOOL->opoolCnt);
  //   HAL_Delay(20);
  // }
  printf("ready\r\n");
  while (1);
}







  // //********************************
  // // formed method, so we can pass the whole graph through various protocol.
  // //********************************
  // unsigned char createGraphCommand[] = {
  //     3, 6, 7,
  //     //pure input
  //     bt_Input_Axis, 0,
  //     bt_Input_Bool, 0,
  //     bt_Input_Real, 0,
  //     //operent fb
  //     bt_MC_Power, 2, 0, 0, 0, 1, 1, 0,
  //     bt_Logic_And, 2, 0, 3, 1, 1, 3, 2,
  //     bt_MC_Home, 2, 0, 0, 0, 1, 4, 0,
  //     bt_MC_MoveAbsolute, 3, 0, 0, 0, 1, 5, 1, 3, 2, 0,
  //     bt_MC_Halt, 2, 0, 0, 0, 1, 6, 1,
  //     bt_MC_Stop, 2, 0, 0, 0, 1, 7, 1,
  //     //pure output
  //     bt_Output_Bool, 1, 0, 3, 1,
  //     bt_Output_Bool, 1, 0, 3, 2,
  //     bt_Output_Bool, 1, 0, 4, 0,
  //     bt_Output_Bool, 1, 0, 5, 1,
  //     bt_Output_Bool, 1, 0, 6, 1,
  //     bt_Output_Bool, 1, 0, 7, 1,
  //     bt_Output_Bool, 1, 0, 8, 1};

  // //匯入並建立對應graph
  // FUNCTION_BLOCK_POOL_t *POOL = setLinkTable2(createGraphCommand);

    // //試跑一小段，將來應該要把這段放在 super loop 或 timer callback
    // int i;
    // for (i = 0; i < 30; i++)
    // {
    //   updateFBS(GLOBALPOOL->fbpool, GLOBALPOOL->fbpoolCnt);
    //   dump_IO(GLOBALPOOL->ipool,
    //           GLOBALPOOL->ipoolCnt,
    //           GLOBALPOOL->opool,
    //           GLOBALPOOL->opoolCnt);
    //   HAL_Delay(20);
    // }