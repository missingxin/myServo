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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PWM_Period 60000
#define argA 33.33333333333333f   // ((33.33333333333333f)*PWM_Period/60000)
#define argB 4500                 // ((4500)*PWM_Period/60000)
// y=ax+b;  a,b 由 servo spec 取得 (y=dutycycle, x=angle)
// uint16_t UpdateAngle(float angle){ return (uint16_t)(angle*33.33333333333333f + 4500); } //previous code

/* Private macro -------------------------------------------------------------*/

// ** 把printf的輸出指向uart2, 另外還需要把syscalls.c放進來編譯 **
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
 
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
asm(".global _printf_float");  //或 linker flag 加入 “-u _printf_float” 

/* Private function prototypes -----------------------------------------------*/
// 1. #########  更新 servo 角度所需要的 callback 實體 #########
//    由於不確定 SFR ptr 的存取是否可以跨平台，所以目前 servo 選擇TIM channel /控制 PWM 的動作都包在這裡
void servoDoAngle0(float angle){ htim3.Instance->CCR1 = (uint16_t)(angle * argA + argB); }
void servoDoAngle1(float angle){ htim3.Instance->CCR2 = (uint16_t)(angle * argA + argB); }
void servoDoAngle2(float angle){ htim3.Instance->CCR3 = (uint16_t)(angle * argA + argB); }
void servoDoAngle3(float angle){ htim3.Instance->CCR4 = (uint16_t)(angle * argA + argB); }
void servoDoAngle4(float angle){ htim4.Instance->CCR1 = (uint16_t)(angle * argA + argB); }
void servoDoAngle5(float angle){ htim4.Instance->CCR2 = (uint16_t)(angle * argA + argB); }



/* Private variables ---------------------------------------------------------*/
// 2. #########  初始化 servo object, 包括相關狀態，設定以及功能  #########
SERVO servo0 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle0 };
SERVO servo1 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle1 };
SERVO servo2 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle2 };
SERVO servo3 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle3 };
SERVO servo4 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle4 };
SERVO servo5 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle5 };


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : servoUpdate
* Description    : callback function callback from regular timer.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************
* 3.把 callback fn 掛進timer, 此timer的period必須符合我們的設定, 每顆servo都會
*   按時被拿出來更新一下，因為servo pwm 的 period 有 20ms, 基本上大部份時間系統都
*   在發呆. 另一個可以掛 hook 的地方是PWM it 本身，每次發出一個pulse就更新一下
*******************************************************************************/
void servoUpdate(){
  ServoTimerCallback(&servo0);
  ServoTimerCallback(&servo1);
  ServoTimerCallback(&servo2);
  ServoTimerCallback(&servo3);
  ServoTimerCallback(&servo4);
  ServoTimerCallback(&servo5);
}



void manualInit(){
  // 初始化 servo 使用的 TIM channel 和 callback timer.
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  htim3.Instance->ARR = PWM_Period;
  htim4.Instance->ARR = PWM_Period;
}

/*******************************************************************************
* Function Name  : myMain
* Description    : real main function.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void myMain(){
  manualInit();


  unsigned char createGraphCommand[] = {
    2,6,6,
        //pure input
            bt_Input_Bool, 0,
            bt_Input_Bool, 0,
        //operent fb
            bt_Logic_And,   2, 0,0,0, 1,1,0,
            bt_Logic_Or,    2, 0,0,0, 1,1,0,
            bt_Logic_Nand,  2, 0,0,0, 1,1,0,
            bt_Logic_Nor,   2, 0,0,0, 1,1,0,
            bt_Logic_Xor,   2, 0,0,0, 1,1,0,
            bt_Logic_Xnor,  2, 0,0,0, 1,1,0,
        //pure output
            bt_Output_Bool, 1, 0,2,0,
            bt_Output_Bool, 1, 0,3,0,
            bt_Output_Bool, 1, 0,4,0,
            bt_Output_Bool, 1, 0,5,0,
            bt_Output_Bool, 1, 0,6,0,
            bt_Output_Bool, 1, 0,7,0
  };
  unsigned char a  = createGraphCommand[0]; //get rud of warning
  createGraphCommand[0] = a; 

//目前先手動打造表格，完成驗證後再將表格格式化
  FUNCTION_BLOCK_PAGE_t *FBP_Pool[50];
  FUNCTION_BLOCK_PAGE_t *INPUT_Pool[50];
  FUNCTION_BLOCK_PAGE_t *OUTPUT_Pool[50];
  unsigned char FBP_cnt = 0;
  unsigned char INPUT_cnt = 0;
  unsigned char OUTPUT_cnt = 0;
//1 建立一張儲存所有FB的表
//  INPUT_BOOL_t *input1 = FB_Create_INPUT_BOOL(0, 0);
//  INPUT_BOOL_t *input2 = FB_Create_INPUT_BOOL(0, 0);
//  FBD_AND_t *and1 = FB_Create_AND(0,0,0);
//  OUTPUT_BOOL_t *out = FB_Create_OUTPUT_BOOL(0,0);

// 建立指定FB
  printf("Create FBs\r\n");
  FB_ADD_AXIS_PAGE(FBP_Pool,&FBP_cnt);
  FB_ADD_INPUT_BOOL_PAGE(FBP_Pool,&FBP_cnt,INPUT_Pool,&INPUT_cnt);
  FB_ADD_INPUT_BOOL_PAGE(FBP_Pool,&FBP_cnt,INPUT_Pool,&INPUT_cnt);
  FB_ADD_MC_POWER_PAGE(FBP_Pool,&FBP_cnt);
  FB_ADD_AND_PAGE (FBP_Pool,&FBP_cnt);
  FB_ADD_OR_PAGE  (FBP_Pool,&FBP_cnt);
  FB_ADD_XOR_PAGE (FBP_Pool,&FBP_cnt);
  FB_ADD_OUTPUT_BOOL_PAGE(FBP_Pool,&FBP_cnt,OUTPUT_Pool,&OUTPUT_cnt);
  FB_ADD_OUTPUT_BOOL_PAGE(FBP_Pool,&FBP_cnt,OUTPUT_Pool,&OUTPUT_cnt);
  FB_ADD_OUTPUT_BOOL_PAGE(FBP_Pool,&FBP_cnt,OUTPUT_Pool,&OUTPUT_cnt);
  FB_ADD_OUTPUT_BOOL_PAGE(FBP_Pool,&FBP_cnt,OUTPUT_Pool,&OUTPUT_cnt);
  

//2 指定所有連結方法
  printf("Create LinkTable\r\n");
  FB_INPUT_LINK_t ffaxis[4] = {{0,0,0},{1,1,0},{2,2,0},{3,2,0}};
  FB_INPUT_LINK_t ff[2] = {{0,1,0},{1,2,0}};
  FB_INPUT_LINK_t ff2[1] = {{0,3,1}};
  FB_INPUT_LINK_t ff3[1] = {{0,4,0}};
  FB_INPUT_LINK_t ff4[1] = {{0,5,0}};
  FB_INPUT_LINK_t ff5[1] = {{0,6,0}};
  setLinkTable(FBP_Pool,0,0,0); //in
  setLinkTable(FBP_Pool,1,0,0); //in
  setLinkTable(FBP_Pool,2,0,0); //in
  setLinkTable(FBP_Pool,3,ffaxis,4); //fb
  setLinkTable(FBP_Pool,4,ff,2); //fb
  setLinkTable(FBP_Pool,5,ff,2); //fb
  setLinkTable(FBP_Pool,6,ff,2); //fb
  setLinkTable(FBP_Pool,7,ff2,1); //out
  setLinkTable(FBP_Pool,8,ff3,1); //out
  setLinkTable(FBP_Pool,9,ff4,1); //out
  setLinkTable(FBP_Pool,10,ff5,1); //out
  
//3 連接
  printf("Link FBs according to linkTable\r\n");
  linkLinkTable(FBP_Pool,FBP_cnt);



  //4. 更新共驗證，目前暫定放在 super loop 的 execution phace 裡
  printf("Update 1\r\n");
  setInputBool(INPUT_Pool,0,0);
  setInputBool(INPUT_Pool,1,0);
  updateFBS(FBP_Pool,FBP_cnt);

  dump_INPUT(INPUT_Pool,INPUT_cnt);
  dump_OUTPUT(OUTPUT_Pool,OUTPUT_cnt);
  

  printf("Update 2\r\n");
  setInputBool(INPUT_Pool,0,0);
  setInputBool(INPUT_Pool,1,1);
  updateFBS(FBP_Pool,FBP_cnt);

  dump_INPUT(INPUT_Pool,INPUT_cnt);
  dump_OUTPUT(OUTPUT_Pool,OUTPUT_cnt);
  

  printf("Update 3\r\n");
  setInputBool(INPUT_Pool,0,1);
  setInputBool(INPUT_Pool,1,0);
  updateFBS(FBP_Pool,FBP_cnt);

  dump_INPUT(INPUT_Pool,INPUT_cnt);
  dump_OUTPUT(OUTPUT_Pool,OUTPUT_cnt);
  

  printf("Update 4\r\n");
  setInputBool(INPUT_Pool,0,1);
  setInputBool(INPUT_Pool,1,1);
  updateFBS(FBP_Pool,FBP_cnt);

  dump_INPUT(INPUT_Pool,INPUT_cnt);
  dump_OUTPUT(OUTPUT_Pool,OUTPUT_cnt);
  
  //printf("dump_PAGE\r\n");
  //dump_PAGE(FBP_Pool,FBP_cnt);
  while(1);

  while(1);
  //開始使用各個servo. 先來個很醜的
  while(1){
    
    // GotoAngleInMs(&servo0,-30, 500); GotoAngleInMs(&servo1,-60, 500); GotoAngleInMs(&servo2,-50, 500); GotoAngleInMs(&servo3,-40, 500); GotoAngleInMs(&servo4,-30, 500); GotoAngleInMs(&servo5,-20, 500); HAL_Delay(500);
    // GotoAngleInMs(&servo0, 30,1000); GotoAngleInMs(&servo1, 60,1000); GotoAngleInMs(&servo2, 50,1000); GotoAngleInMs(&servo3, 40,1000); GotoAngleInMs(&servo4, 30,1000); GotoAngleInMs(&servo5, 20,1000); HAL_Delay(1000);
    // GotoAngleInMs(&servo0,  0, 500); GotoAngleInMs(&servo1,  0, 500); GotoAngleInMs(&servo2,  0, 500); GotoAngleInMs(&servo3,  0, 500); GotoAngleInMs(&servo4,  0, 500); GotoAngleInMs(&servo5,  0, 500); HAL_Delay(500);
    // GotoAngleInMs(&servo0,-30, 500); GotoAngleInMs(&servo1,-60, 500); GotoAngleInMs(&servo2,-50, 500); GotoAngleInMs(&servo3,-40, 500); GotoAngleInMs(&servo4,-30, 500); GotoAngleInMs(&servo5,-20, 500); HAL_Delay(500);
    // GotoAngleInMs(&servo0, 30,1000); GotoAngleInMs(&servo1, 60,1000); GotoAngleInMs(&servo2, 50,1000); GotoAngleInMs(&servo3, 40,1000); GotoAngleInMs(&servo4, 30,1000); GotoAngleInMs(&servo5, 20,1000); HAL_Delay(1000);
    // GotoAngleInMs(&servo0,  0, 500); GotoAngleInMs(&servo1,  0, 500); GotoAngleInMs(&servo2,  0, 500); GotoAngleInMs(&servo3,  0, 500); GotoAngleInMs(&servo4,  0, 500); GotoAngleInMs(&servo5,  0, 500); HAL_Delay(500);
 
    // GotoAngleInMs(&servo1,-80,1000); GotoAngleInMs(&servo2,-60,1000); GotoAngleInMs(&servo5,-50,1000); HAL_Delay(1000);
    // GotoAngleInMs(&servo1, 80,2000); GotoAngleInMs(&servo2, 60,2000); GotoAngleInMs(&servo5, 50,2000); HAL_Delay(2000);
    // GotoAngleInMs(&servo1,  0,1000); GotoAngleInMs(&servo2,  0,1000); GotoAngleInMs(&servo5,  0,1000); HAL_Delay(1000);
    // GotoAngleInMs(&servo1,-80,1000); GotoAngleInMs(&servo2,-60,1000); GotoAngleInMs(&servo5,-50,1000); HAL_Delay(1000);
    // GotoAngleInMs(&servo1, 80,2000); GotoAngleInMs(&servo2, 60,2000); GotoAngleInMs(&servo5, 50,2000); HAL_Delay(2000);
    // GotoAngleInMs(&servo1,  0,1000); GotoAngleInMs(&servo2,  0,1000); GotoAngleInMs(&servo5,  0,1000); HAL_Delay(1000);
 
    // GotoAngleInMs(&servo2,-80,2000); HAL_Delay(2000);
    // GotoAngleInMs(&servo2, 80,4000); HAL_Delay(4000);
    // GotoAngleInMs(&servo2,  0,2000); HAL_Delay(2000);
    // GotoAngleInMs(&servo2,-80,2000); HAL_Delay(2000);
    // GotoAngleInMs(&servo2, 80,4000); HAL_Delay(4000);
    // GotoAngleInMs(&servo2,  0,2000); HAL_Delay(2000);
 
  }
}

