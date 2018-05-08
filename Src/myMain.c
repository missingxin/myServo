/**
  ******************************************************************************
  * File Name          : myMain.c
  * Description        : Demonstration of myServo Middleware.
  ******************************************************************************
  ** 說明：在STM32F03C8T6(Bluepill board)上使用 myServo Middleware 作為展示範例
  *  使用的 servo 參數為：
  *     period：20ms
  *     duty cycle: 0.5ms ~ 2ms
  *     角度：正負90度
  ******************************************************************************
*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "myMain.h"
#include "myServo.h"
#include "ur.h"



#define PWM_Period 60000
#define argA 33.33333333333333f   // ((33.33333333333333f)*PWM_Period/60000)
#define argB 4500                 // ((4500)*PWM_Period/60000)
// y=ax+b;  a,b 由 servo spec 取得 (y=dutycycle, x=angle)
// uint16_t UpdateAngle(float angle){ return (uint16_t)(angle*33.33333333333333f + 4500); } //previous code


// 1. #########  更新 servo 角度所需要的 callback 實體 #########
//    由於不確定 SFR ptr 的存取是否可以跨平台，所以目前 servo 選擇TIM channel /控制 PWM 的動作都包在這裡
void servoDoAngle0(float angle){ htim3.Instance->CCR1 = (uint16_t)(angle * argA + argB); }
void servoDoAngle1(float angle){ htim3.Instance->CCR2 = (uint16_t)(angle * argA + argB); }
void servoDoAngle2(float angle){ htim3.Instance->CCR3 = (uint16_t)(angle * argA + argB); }
void servoDoAngle3(float angle){ htim3.Instance->CCR4 = (uint16_t)(angle * argA + argB); }
void servoDoAngle4(float angle){ htim4.Instance->CCR1 = (uint16_t)(angle * argA + argB); }
void servoDoAngle5(float angle){ htim4.Instance->CCR2 = (uint16_t)(angle * argA + argB); }




// 2. #########  初始化 servo object, 包括相關狀態，設定以及功能  #########
SERVO servo0 = { 0, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle0 };
SERVO servo1 = { 0, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle1 };
SERVO servo2 = { 0, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle2 };
SERVO servo3 = { 0, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle3 };
SERVO servo4 = { 0, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle4 };
SERVO servo5 = { 0, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle5 };


// 3. #########  把 callback fn 掛進timer, 此timer的period必須符合我們的設定 #########
//    每顆servo都會按時被拿出來更新一下，因為servo pwm 的 period 有 20ms, 基本上大部份時間系統都在發呆
//    另一個可以掛 hook 的地方是PWM it 本身，每次發出一個pulse就更新一下
void servoUpdate(){
  ServoTimerCallback(&servo0);
  ServoTimerCallback(&servo1);
  ServoTimerCallback(&servo2);
  ServoTimerCallback(&servo3);
  ServoTimerCallback(&servo4);
  ServoTimerCallback(&servo5);
}

void myMain(){
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




  //開始使用各個servo. 先來個很醜的
  while(1){

    GotoAngleInMs(&servo0,-30, 500); GotoAngleInMs(&servo1,-60, 500); GotoAngleInMs(&servo2,-50, 500); GotoAngleInMs(&servo3,-40, 500); GotoAngleInMs(&servo4,-30, 500); GotoAngleInMs(&servo5,-20, 500); HAL_Delay(500);
    GotoAngleInMs(&servo0, 30,1000); GotoAngleInMs(&servo1, 60,1000); GotoAngleInMs(&servo2, 50,1000); GotoAngleInMs(&servo3, 40,1000); GotoAngleInMs(&servo4, 30,1000); GotoAngleInMs(&servo5, 20,1000); HAL_Delay(1000);
    GotoAngleInMs(&servo0,  0, 500); GotoAngleInMs(&servo1,  0, 500); GotoAngleInMs(&servo2,  0, 500); GotoAngleInMs(&servo3,  0, 500); GotoAngleInMs(&servo4,  0, 500); GotoAngleInMs(&servo5,  0, 500); HAL_Delay(500);
    GotoAngleInMs(&servo0,-30, 500); GotoAngleInMs(&servo1,-60, 500); GotoAngleInMs(&servo2,-50, 500); GotoAngleInMs(&servo3,-40, 500); GotoAngleInMs(&servo4,-30, 500); GotoAngleInMs(&servo5,-20, 500); HAL_Delay(500);
    GotoAngleInMs(&servo0, 30,1000); GotoAngleInMs(&servo1, 60,1000); GotoAngleInMs(&servo2, 50,1000); GotoAngleInMs(&servo3, 40,1000); GotoAngleInMs(&servo4, 30,1000); GotoAngleInMs(&servo5, 20,1000); HAL_Delay(1000);
    GotoAngleInMs(&servo0,  0, 500); GotoAngleInMs(&servo1,  0, 500); GotoAngleInMs(&servo2,  0, 500); GotoAngleInMs(&servo3,  0, 500); GotoAngleInMs(&servo4,  0, 500); GotoAngleInMs(&servo5,  0, 500); HAL_Delay(500);
 
    GotoAngleInMs(&servo1,-80,1000); GotoAngleInMs(&servo2,-60,1000); GotoAngleInMs(&servo5,-50,1000); HAL_Delay(1000);
    GotoAngleInMs(&servo1, 80,2000); GotoAngleInMs(&servo2, 60,2000); GotoAngleInMs(&servo5, 50,2000); HAL_Delay(2000);
    GotoAngleInMs(&servo1,  0,1000); GotoAngleInMs(&servo2,  0,1000); GotoAngleInMs(&servo5,  0,1000); HAL_Delay(1000);
    GotoAngleInMs(&servo1,-80,1000); GotoAngleInMs(&servo2,-60,1000); GotoAngleInMs(&servo5,-50,1000); HAL_Delay(1000);
    GotoAngleInMs(&servo1, 80,2000); GotoAngleInMs(&servo2, 60,2000); GotoAngleInMs(&servo5, 50,2000); HAL_Delay(2000);
    GotoAngleInMs(&servo1,  0,1000); GotoAngleInMs(&servo2,  0,1000); GotoAngleInMs(&servo5,  0,1000); HAL_Delay(1000);
 
    GotoAngleInMs(&servo2,-80,2000); HAL_Delay(2000);
    GotoAngleInMs(&servo2, 80,4000); HAL_Delay(4000);
    GotoAngleInMs(&servo2,  0,2000); HAL_Delay(2000);
    GotoAngleInMs(&servo2,-80,2000); HAL_Delay(2000);
    GotoAngleInMs(&servo2, 80,4000); HAL_Delay(4000);
    GotoAngleInMs(&servo2,  0,2000); HAL_Delay(2000);
 
  }
}

