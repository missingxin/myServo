/******************************************************************************
* File Name          : Axis.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 06/3/2018
* Description        : PLCOpen MC 的驅動器物件，將由此處連接各種驅動器
********************************************************************************
* 說明：PLCOpen MC 的驅動器物件，將由此處連接各種驅動器
*******************************************************************************/
#include "Axis.h"
#include "myServo.h"
#include "myMain.h"
#include "tim.h"
// y=ax+b;  a,b 由 servo spec 取得 (y=dutycycle, x=angle)
// uint16_t UpdateAngle(float angle){ return (uint16_t)(angle*33.33333333333333f + 4500); } //previous code

#define argA 33.33333333333333f   // ((33.33333333333333f)*PWM_Period/60000)
#define argB 4500                 // ((4500)*PWM_Period/60000)


/* Private function prototypes -----------------------------------------------*/
// 1. #########  更新 servo 角度所需要的 callback 實體 #########
//    由於不確定 SFR ptr 的存取是否可以跨平台，所以目前 servo 選擇TIM channel /控制 PWM 的動作都包在這裡
void servoDoAngle0(float angle){ htim3.Instance->CCR1 = (uint16_t)(angle * argA + argB); }



/* Private variables ---------------------------------------------------------*/
// 2. #########  初始化 servo object, 包括相關狀態，設定以及功能  #########
SERVO servo0 = { 1, 0, 0, {}, 1, {}, 90.0f, -90.0f, 0, 20, servoDoAngle0 };


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : device_timer_callback
* Description    : callback function callback from regular timer.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************
* 3.把 callback fn 掛進timer, 此timer的period必須符合我們的設定, 每顆servo都會
*   按時被拿出來更新一下，因為servo pwm 的 period 有 20ms, 基本上大部份時間系統都
*   在發呆. 另一個可以掛 hook 的地方是PWM it 本身，每次發出一個pulse就更新一下
*******************************************************************************/
void device_timer_callback(){
  ServoTimerCallback(&servo0); //更新servo物件
}



/*******************************************************************************
* Function Name  : device_init
* Description    : light up the pwm needed by servo.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************
* 3.light up the pwm needed by servo
*******************************************************************************/
void device_init(){
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  htim3.Instance->ARR = PWM_Period;
}


PowerControlResult_t axisDoPower( AXIS_t * axis, BOOL enable, BOOL enablePositive, BOOL enableNegative )
{
  if (axis->power != enable){
    axis->power = enable;
    if(enable == TRUE){
      printf("  ======== simulator :: AXIS DONE POWER (ON) ======== \r\n");
      axis->setStat(axis,FA_STANDSTILL);
    }else{
      axis->setStat(axis,FA_DISABLED);
      printf("  ======== simulator :: AXIS DONE POWER (OFF)======== \r\n");
    }
    return PowerControl_Done;
  }
  axis->power = TRUE;
  axis->setStat(axis,FA_STANDSTILL);

  return PowerControl_Done;
}
PowerControlResult_t axisDoStop(AXIS_t * axis,  BOOL busy, REAL acceleration, REAL jerk)
{ 
  if (busy == FALSE){
    axis->setStat(axis,FA_DISABLED);
    printf("  ======== simulator :: AXIS DONE STOP ======== \r\n");
  }
  return PowerControl_Done;
}
PowerControlResult_t axisDoHome(AXIS_t * axis,  BOOL busy, REAL position)
{
  if (busy == FALSE){
    axis->setStat(axis,FA_STANDSTILL);
    //假裝有做homeing的動作
    printf("  ======== simulator :: AXIS DOING HOME ======== \r\n");
  }
  return PowerControl_Done;
}
PowerControlResult_t axisDoHalt(AXIS_t * axis,  BOOL busy, REAL accelerateion, REAL jerk)
{
  if (busy == FALSE){
    axis->setStat(axis,FA_STANDSTILL);
    //假裝有做Halting的動作
    printf("  ======== simulator :: AXIS DOING HOME ======== \r\n");
  }
  return PowerControl_Done;
}
PowerControlResult_t axisDoMoveAbs(AXIS_t * axis, 
                            BOOL busy,
                            BOOL continuousUpdate,
                            REAL position,
                            REAL velocity,
                            REAL Acceleration,
                            REAL Deceleration,
                            REAL Jerk,
                            MC_DIRECTION_t direction)
{
  if(busy == FALSE){
    GotoAngleInMs(&servo0,position,500);
    printf("  ======== simulator :: AXIS DOING MOVE ABS (to %f)======== \r\n", position);
    return PowerControl_Doing;
  }else{
    if(servo0.angle == position){
      printf("  ======== simulator :: AXIS DONE MOVE ABS (to %f)!!!!!!!!!!! ======== \r\n",position);
      return PowerControl_Done;    
    }else{
      //printf("  ======== simulator :: AXIS DOING MOVE ABS ======== \r\n");
      return PowerControl_Doing;
    }
  }
  printf("  ======== simulator :: AXIS DONE MOVE ABS !!!!!!!!!!! ======== \r\n");
  return PowerControl_Done;
}
