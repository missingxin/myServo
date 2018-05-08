/*******************************************************************************
* File Name          : myServo.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/8/2018
* Description        : Here we create a low dependency servo Middleware here,
*                      which we can stake PLCOpen layer on top of.
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 Servo Middleware.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#ifndef MYSERVO_H
#define MYSERVO_H
/* Private typedef -----------------------------------------------------------*/
#define bool unsigned char

typedef struct{
  unsigned char workmode;
  float targetAngle;
  float velocity;
  float accelerate;
  float jerk;
  long  countdownInMs;
  long  totalTimeInMs;
  long  (*exAngleFunc)       (float angle, long ms, long ttlms);
  long  (*exVelocityFunc)    (float angle, float velocity, long ms, long ttlms);
  long  (*exAccelerateFunc)  (float angle, float velocity, float accelerate, long ms, long ttlms);
  long  (*exJerkFunc)        (float angle, float velocity, float accelerate, float jerk, long ms, long ttlms);
}JOB;

//typedef struct SERVO SERVO;
typedef struct{
  bool  power;
  bool  halt;
  bool  hasPendingJob;
  JOB   pendingJob;
  bool  currentJobDne;
  JOB   currentJob;
  float angleMax;
  float angleMin;
  float angle;
  int   timerPeriod; //ns
  void  (*DoAngle)(float angle);
}SERVO;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/*******************************************************************************
* Function Name  : GotoAngleInMs
* Description    : 在特定時間ms以內，調整自身至指定位置
* Input          : @SERVO *servo
*                  @float angle
*                  @long ms
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void GotoAngleInMs(SERVO *servo,float angle,long ms);


/*******************************************************************************
* Function Name  : GotoAngleInMs
* Description    : 在特定時間ms以內，用velocity,accelerate,jerk 決定接下來的位置，超過最
*                  大角度時會用最停在最大角度並暫停  velocity ＝ angle/s，accelerate = angle/s^3
* Input          : @SERVO *servo
*                  @float velocity
*                  @float accelerate
*                  @float jerk
*                  @long ms
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void DoJerkInMs(SERVO *servo,float velocity, float accelerate, float jerk, long ms);


/*******************************************************************************
* Function Name  : DoAccelerateInMs
* Description    : 在特定時間ms以內，用velocity,accelerate 決定接下來的位置，超過最大角度時會用最停在最大角度並暫停  velocity ＝ angle/s，accelerate = angle/s^2
* Input          : @SERVO *servo
*                  @float velocity
*                  @float accelerate
*                  @long ms
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void DoAccelerateInMs(SERVO *servo,float velocity, float accelerate, long ms);


/*******************************************************************************
* Function Name  : GotoAngleInMs
* Description    : 在特定時間ms以內，用velocity,決定接下來的位置，超過最大角度時會用最停在最大角度並暫停 velocity ＝ angle/s
* Input          : @SERVO *servo
*                  @float velocity
*                  @long ms
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void DoVelocityInMs(SERVO *servo, float velocity, long ms);

//自帶系數變化規則
/*******************************************************************************
* Function Name  : ExGotoAngleInMs
* Description    : 在特定時間ms以內，調整自身至指定位置
* Input          : @SERVO *servo
*                  @long ms
*                  @long (*exAngleFunc)(float angle, long ms, long ttlms)
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void ExGotoAngleInMs(SERVO *servo, long ms, 
long (*exAngleFunc)(float angle, long ms, long ttlms));


/*******************************************************************************
* Function Name  : ExGotoAngleInMs
* Description    : 在特定時間ms以內，用 exVelocityFunc 決定下一刻的 velocity，之後用一般計算
* Input          : @SERVO *servo
*                  @float velocity
*                  @long ms
*                  @long (*exVelocityFunc)(float angle, float velocity, long ms, long ttlms)
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void ExDoVelocityInMs(SERVO *servo, float velocity, long ms, 
long (*exVelocityFunc)(float angle, float velocity, long ms, long ttlms));


/*******************************************************************************
* Function Name  : ExGotoAngleInMs
* Description    : 在特定時間ms以內，用velocity,accelerate 決定接下來的位置，超過最大角度時會用最停在最大角度並暫停  velocity ＝ angle/s，accelerate = angle/s^2
* Input          : @SERVO *servo,
*                  @float velocity, 
*                  @float accelerate, 
*                  @long ms, 
*                  @long (*exAccelerateFunc)(float angle, float velocity, float accelerate, long ms, long ttlms)
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void ExDoAccelerateInMs(SERVO *servo,float velocity, float accelerate, long ms, 
long (*exAccelerateFunc)(float angle, float velocity, float accelerate, long ms, long ttlms));


/*******************************************************************************
* Function Name  : ExDoJerkInMs
* Description    : 在特定時間ms以內，用velocity,accelerate,jerk 決定接下來的位置，超過最大角度時會用最停在最大角度並暫停  velocity ＝ angle/s，accelerate = angle/s^3
* Input          : @SERVO *servo,
*                  @float velocity, 
*                  @float accelerate, 
*                  @float jerk, 
*                  @long ms, 
*                  @long (*exJerkFunc)(float angle, float velocity, float accelerate, float jerk, long ms, long ttlms)
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void ExDoJerkInMs(SERVO *servo,float velocity, float accelerate, float jerk, long ms,
long (*exJerkFunc)(float angle, float velocity, float accelerate, float jerk, long ms, long ttlms));

/*******************************************************************************
* Function Name  : ExGotoAngleInMs
* Description    : Update servo data
* Input          : @SERVO *servo
* Output         : None
* Return         : None
*******************************************************************************
*******************************************************************************/
void  ServoTimerCallback(SERVO *servo); //

//構想中，讓 Homing 的動作溫柔一點
//void ServoSoftHoming(SERVO servo);


/* Private functions ---------------------------------------------------------*/
#endif
