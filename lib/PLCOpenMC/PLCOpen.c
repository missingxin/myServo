/*******************************************************************************
* File Name          : PLCOpen.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/8/2018
* Description        : 建立可物件化的PLCOpen Middleware
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCOpen Middleware
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "PLCOpen.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


// ############################### function 形式 ###############################
void MC_Power(  //I/O
              AXIS_REF Asix, 
                //O
              bool *Status, bool *Valid, bool *Error, WORD ErrorID, 
                //I
              bool Enable,bool EnablePositive,bool EnableNegative)
{
  //初始化輸出
  *Status = false;
  *Valid  = false;
  *Error  = false;
  ErrorID = WORD_NO_ERROR;
  //讀取輸入
  Asix.positive = EnablePositive;
  Asix.negative = EnableNegative;
  //按FA及輸入決定處理方式
  if(Enable == true){
    switch(Asix.stat){
      case(FA_Disabled):{
        *Status = AXIS_POWER(Enable);
        if(*Status == true){
          Asix.power=true;
          Asix.stat = FA_Standstill;
        }else{
          Asix.power=false;
          Asix.stat = FA_Errorstop;
        }
        *Valid = *Status;
        break;
      }
      case(FA_Errorstop):{
        Asix.power=false;
        break;
      }
      default:{
        Asix.power=true;
        *Valid = true;
        break;
      }
    }
  }else{
    AXIS_POWER(Enable);
    Asix.power=false;
    *Valid = true;
  }
}



// ############################### Object Way ###############################

//建立物件
void MC_Power_updater(MC_Power_St *obj){
  
}
MC_Power_St test1 = {NULL,NULL,NULL,0,0,0,0,MC_Power_updater};










/*
//abstract functions
//這裡的東西必須要由底層來實做，與PLCOpen分離
extern void blockingForwardTurn(unsigned int speed,unsigned int timeout);     //阻擋式正轉,   timeout時間單位為ms, 若設為0表示永不timeout
extern void blockingBackwardTurn(unsigned int speed,unsigned int timeout);     //阻擋式反轉,   timeout時間單位為ms, 若設為0表示永不timeout
extern void nonBlockingForwardTurn(unsigned int speed,unsigned int timeout);   //非中斷式正轉,  timeout時間單位為ms, 若設為0表示永不timeout
extern void nonBlockingBackwardTurn(unsigned int speed,unsigned int timeout); //非中斷式反轉,  timeout時間單位為ms, 若設為0表示永不timeout
extern void nonBlockingBackwardTurn(); //非中斷式停止

//還不確定這幹嘛的
void enableTIM2(){
  ;//TIM_Cmd(TIM2, ENABLE);
}
//還不確定這幹嘛的
void enableTIM3(){
  ;//TIM_Cmd(TIM3, ENABLE);
}
//還不確定這幹嘛的
void disableTIM2(){
  ;//TIM_Cmd(TIM2, DISABLE);
}
//還不確定這幹嘛的
void disableTIM3(){
  ;//TIM_Cmd(TIM3, DISABLE);
}

//內部使用變數

int power=0;
float holdvelocity=0;
float position = 0.0;
int positive;
int negative;
int step=0;
int stop;
float deceleration;
float jerk;
int duringdeceleration=0;
int duringacceleration=0;
int duringbalance=0;
float Reachposition;
float velocity;
float acceleration;
float predictposition;
float distance;
int superimposed=0;
float rightlimit=100000;
float leftlimit=-100000;
float torque;
int Probe=0;
int axiserror=0;



void MC_Power(
                //I/O
              AXIS_REF Asix, 
                //O
              bool *Status, bool *Valid, bool *Error, WORD ErrorID, 
                //I
              bool Enable,bool EnablePositive,bool EnableNegative)
{
  power=Enable;
  //printf("----------------: %d\n\r",power);
  if(EnablePositive==1&&EnableNegative==1)
  {
    axis.Error=1;
    axis.ErrorID=2;
    ErrorIDTable(2);
    axis.Status=0;
  }
  else
  {
    positive=EnablePositive;
    negative=EnableNegative;
    axis.Error=0;
    axis.ErrorID=0;
    axis.Status=1;
  }
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=1;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}

//Position BufferMode:
void MC_Home(int Execute ,float Position ,int BufferMode)
{
//  scanf("%f",&holdvelocity);
//  printf("%f\n\r",holdvelocity);
//  returnPeriod();
  if(power==1)
  {
    if(Position>position)
    {
      positive=1;
      negative=0;
      transfer(Position-position);
    }
    else if(Position<position)
    {
      positive=0;
      negative=1;
      transfer(position-Position);
    }
    enableTIM2();
//  printf("Home\n\r");
//  printf("----------------: %f\n\r",Reachposition);
//  printf("----------------: %d\n\r",power);
//  printf("----------------: %f\n\r",position);
  }
  //??Execute
  //??BufferMode
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=1;
  axis.Standstill=0;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}

//Deceleration Jerk :
void MC_Stop(int Execute, float Deceleration, float Jerk)
{ //減速到零
  int a;
  int remainder;
  float stephold=0.5;
//  scanf("%f",&holdvelocity);
//  printf("%f\n\r",holdvelocity);
//  velocity=holdvelocity;
//  printf("MC_Stop: %d\n\r",step);
//  printf("holdvelocity: %f\n\r",holdvelocity);
//  returnPeriod();

  remainder=(int)holdvelocity%10;
  for(a=0;a<=(holdvelocity/10)-1;a++)
  {
    step=step+(10/Deceleration)*stephold*4;
    stephold=stephold*2;
  }
  step=step+((remainder/Deceleration)+1)*stephold*4;
  //step等於再走幾步就會停止(配存tm32f10x_it.c的Xlevel=1時才會走一步)
  stop=1;
  //??Execute
  deceleration=Deceleration;
  jerk=Jerk;
  duringdeceleration=1;
  enableTIM2(); // 會執行 TIM2_IRQHandler(); 定義在 stm32f10x_it.c (轉動)
  enableTIM3(); // 會執行 TIM3_IRQHandler(); 定義在 stm32f10x_it.c (減速(deceleration)到零或某個數值及加速(acceleration)到某個數值)
  axis.Disabled=0;
  axis.Stopping=1;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}

//Deceleration Jerk BufferMode:
void MC_Halt(int Execute,float Deceleration, float Jerk,int BufferMode)
{
  //也是stop但不同點是,Halt為可被中斷的停止,改執行其他指令
  int a;
  int remainder;
  float stephold=0.5;
//  printf("MC_Halt: %d\n\r",step);
//  scanf("%f",&holdvelocity);
//  printf("%f\n\r",holdvelocity);
//  returnPeriod();

  remainder=(int)holdvelocity%10;
  for(a=0;a<=(holdvelocity/10)-1;a++)
  {
    step=step+(10/Deceleration)*stephold*4;
    stephold=stephold*2;
  }
  step=step+((remainder/Deceleration)+1)*stephold*4;
  stop=0;
  //??Execute
  deceleration=Deceleration;
  jerk=Jerk;
  duringdeceleration=1;
  enableTIM2();
  enableTIM3();
  //??BufferMode
  axis.Disabled=0;
  axis.Stopping=1;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}


void MC_MoveAbsolute(
  int Execute ,
  int ContinuousUpdate ,
  float Position ,
  float Velocity, 
  float Acceleration, 
  float Deceleration, 
  float Jerk,
  int Direction ,
  int BufferMode)
{
  //移動到絕對位置
  //Position為將要到達的位置,Velocity從現在的速度加減速到指定的速度,Acceleration加速度,Deceleration減速度
  int a;
  int remainder;
  float stephold=0.5;
  int hold;
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  Reachposition=Position;
  velocity=Velocity;
  acceleration=Acceleration;//加速度(TIM3_IRQHandler有用到acceleration變數)
  deceleration=Deceleration;//減速度(TIM3_IRQHandler有用到deceleration變數)
  jerk=Jerk;

  remainder=(int)Velocity%10;
  for(a=0;a<=(Velocity/10)-1;a++)
  {
    hold=hold+(10/Deceleration)*stephold*4;
    stephold=stephold*2;
  }
  hold=hold+((remainder/Deceleration)+1)*stephold*4;

  if(Position>position)//position為現在位置,Position為將要到達的位置
  {
    positive=1;
    negative=0;
    transfer(Position-position);
    predictposition=step-hold;
  }
  else if(Position<position)
  {
    positive=0;
    negative=1;
    transfer(position-Position);//和馬達有關的換算
    predictposition=step-hold;
  }
//  if(predictposition>rightlimit)
//  {
//    predictposition=rightlimit;
//  }
//  else if(predictposition<leftlimit)
//  {
//    predictposition=leftlimit;
//  }

  enableTIM2();
  enableTIM3();
  //??Direction
  //??BufferMode
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=1;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}

//ContinuousUpdate Distance Velocity Acceleration Deceleration Jerk BufferMode :
void MC_MoveRelative(int Execute ,int ContinuousUpdate ,float Distance, float Velocity, float Acceleration, float Deceleration, float Jerk ,int BufferMode)
{
  //移動到相對位置
  //Distance只能為正值,由MC_Power決定為正轉或逆轉再做加減
  int a;
  int remainder;
  float stephold=0.5;
  int hold=0;
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  distance=Distance;
  velocity=Velocity;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;

  remainder=(int)Velocity%10;
  for(a=0;a<=(Velocity/10)-1;a++)
  {
    hold=hold+(10/Deceleration)*stephold*4;
    stephold=stephold*2;
  }
  hold=hold+((remainder/Deceleration)+1)*stephold*4;
  transfer(Distance);//和馬達有關的換算

  if(positive==1)
  {
    predictposition=step-hold;
  }
  else if(negative==1)
  {
    predictposition=step-hold;
  }
//  if(predictposition>rightlimit)
//  {
//    predictposition=rightlimit;
//  }
//  else if(predictposition<leftlimit)
//  {
//    predictposition=leftlimit;
//  }

  enableTIM2();
  enableTIM3();
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=1;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}

//ContinuousUpdate Distance Velocity Acceleration Deceleration Jerk BufferMode :
void MC_MoveAdditive(int Execute ,int ContinuousUpdate ,float Distance, float Velocity, float Acceleration, float Deceleration, float Jerk ,int BufferMode)
{
  //雷同[移動到相對位置],不同的是比較像是微調動作,通常接在[移動到絕對位置]及[移動到相對位置]之後
  int a;
  int remainder;
  float stephold=0.5;
  int hold;
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  distance=Distance;
  velocity=Velocity;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;

  remainder=(int)Velocity%10;
  for(a=0;a<=(Velocity/10)-1;a++)
  {
    hold=hold+(10/Deceleration)*stephold*4;
    stephold=stephold*2;
  }
  hold=hold+((remainder/Deceleration)+1)*stephold*4;
  transfer(Distance);

  if(positive==1)
  {
    predictposition=step-hold;
  }
  else if(negative==1)
  {
    predictposition=step-hold;
  }
//  if(predictposition>rightlimit)
//  {
//    predictposition=rightlimit;
//  }
//  else if(predictposition<leftlimit)
//  {
//    predictposition=leftlimit;
//  }

  enableTIM2();
  enableTIM3();
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=1;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}

//ContinuousUpdate Distance VelocityDiff Acceleration Deceleration :
void MC_MoveSuperimposed(int Execute ,int ContinuousUpdate ,float Distance, float VelocityDiff, float Acceleration, float Deceleration, float Jerk )
{
  //不常用,要配合前面的某個離散運動(MC_MoveAbsolute,MC_MoveRelative)
  int a;
  int remainder;
  float stephold=0.5;
  int hold;
  int regist;
  superimposed=1;
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  distance=distance+Distance;
  velocity=holdvelocity+VelocityDiff;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;

  remainder=(int)velocity%10;
  for(a=0;a<=(velocity/10)-1;a++)
  {
    hold=hold+(10/Deceleration)*stephold*4;
    stephold=stephold*2;
  }
  regist=step;
  hold=hold+((remainder/Deceleration)+1)*stephold*4;
  transfer(Distance);
  regist=regist+step;
  if(positive==1)
  {
    predictposition=step-hold;
  }
  else if(negative==1)
  {
    predictposition=step-hold;
  }
//  if(predictposition>rightlimit)
//  {
//    predictposition=rightlimit;
//  }
//  else if(predictposition<leftlimit)
//  {
//    predictposition=leftlimit;
//  }

  enableTIM2();
  enableTIM3();
  axis.CoveredDistance=0;
  if(axis.ContinuousMotion==1)
  {
    axis.ContinuousMotion=1;
    axis.DiscreteMotion=0;
  axis.SynchronizedMotion=0;
  }
  else if(axis.DiscreteMotion==1)
  {
    axis.ContinuousMotion=0;
    axis.DiscreteMotion=1;
  axis.SynchronizedMotion=0;
  }
  else if(axis.SynchronizedMotion==1)
  {
    axis.ContinuousMotion=0;
    axis.DiscreteMotion=0;
    axis.SynchronizedMotion=1;
  }
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
}

//Deceleration Jerk:
void MC_HaltSuperimposed(int Execute , float Deceleration, float Jerk )//??
{
  //取消MC_MoveSuperimposed動作
  //??decelaration=Deceleration;
  //??jerk=Jerk;
  superimposed=0;
}

//ContinuousUpdate Velocity Acceleration Deceleration Jerk Direction BufferMode:
void MC_MoveVelocity(int Execute ,int ContinuousUpdate ,float Velocity, float Acceleration, float Deceleration, float Jerk ,int Direction ,int BufferMode)
{
  //連續運動,維持在某個速度動作(不會自動停止),要使用MC_Stop停止運動
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  velocity=Velocity;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;

  if(positive==1)
  {
    transfer(rightlimit-position);
  }
  else if(negative==1)
  {
    transfer(position-leftlimit);
  }
  predictposition=step;

  enableTIM2();
//  enableTIM3();

  axis.InVelocity=1;
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=1;
  axis.SynchronizedMotion=0;
}

//ContinuousUpdate Position EndVelocity Velocity Acceleration Deceleration Jerk Direction BufferMode :
void MC_MoveContinuousAbsolute(int Execute ,int ContinuousUpdate ,float Position, float EndVelocity,float Velocity,float Acceleration, float Deceleration,float Jerk ,int Direction ,int BufferMode)
{
  //當成MC_MoveAbsolute+MC_MoveVelocity
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  Reachposition=Position;
  velocity=Velocity;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;

  if(Position>position)
  {
    positive=1;
    negative=0;
    transfer(Position-position);
  }
  else if(Position<position)
  {
    positive=0;
    negative=1;
    transfer(position-Position);
  }
  predictposition=step;

  enableTIM2();
  enableTIM3();
  axis.InEndVelocity=1;
}
//ContinuousUpdate Distance EndVelocity Velocity Acceleration Deceleration Jerk BufferMode :
void MC_MoveContinuousRelative(int Execute ,int ContinuousUpdate ,float Distance, float EndVelocity,float Velocity, float Acceleration, float Deceleration,float Jerk,int BufferMode)
{
  //當成MC_MoveRelative+MC_MoveVelocity
  //??ContinuousUpdate
  duringacceleration=1;
  duringbalance=0;
  duringdeceleration=0;
  distance=Distance;
  velocity=Velocity;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;

  transfer(Distance);
  predictposition=step;

  enableTIM2();
  enableTIM3();
  axis.InEndVelocity=1;
}
void MC_TorqueControl(int Execute ,int ContinuousUpdate ,float Torque, float TorqueRamp,float Velocity, float Acceleration, float Deceleration,float Jerk,int Direction ,int BufferMode)//??
{
  //沒在用(馬達做不到這個功能)
  //??ContinuousUpdate
  //??Torque
  //??TorqueRamp
  velocity=Velocity;
  acceleration=Acceleration;
  deceleration=Deceleration;
  jerk=Jerk;
  //??Direction
  //??BufferMode
  axis.InTorque=1;
}
void MC_PositionProfile(int Execute ,int ContinuousUpdate ,float TimeScale, float PositionScale,float Offset,int BufferMode)//??
{
  //??ContinuousUpdate
  //??TimeScale
  //??PositionScale
  //??Offset
  //??BufferMode
//  if(absposition[order]>position)
//  {
//    positive=1;
//    negative=0;
//    holdvelocity=(absposition[order]-position)/deltatime[order];
//}
//  else if(absposition[order]<position)
//  {
//    positive=0;
//    negative=1;
//    holdvelocity=(position-absposition[order])/deltatime[order];
//  }
//  order++;
//  enableTIM2();
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=1;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
}
void MC_VelocityProfile(int Execute ,int ContinuousUpdate ,float TimeScale, float VelocityScale,float Offset,int BufferMode)//??
{
  //??ContinuousUpdate
  //??TimeScale
  //??VelocityScale
  //??Offset
  //??BufferMode
//  if(vel[order]>=0)
//  {
//    positive=1;
//    negative=0;
//    holdvelocity=vel[order];
//  }
//  else
//  {
//    positive=0;
//    negative=1;
//    holdvelocity=vel[order]*-1;
//  }
//  order++;
//  enableTIM2();
//  VelocityProfilenode.ProfileCompleted=1;
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=1;
  axis.SynchronizedMotion=0;
}
void MC_AccelerationProfile(int TimeAcceleration, int Execute ,int ContinuousUpdate ,float TimeScale, float AccelerationScale,float Offset,int BufferMode)//??
{
  //??ContinuousUpdate
  //??TimeScale
  //??AccelerationScale
  //??Offset
  //??BufferMode
//  enableTIM2();
//  AccelerationProfilenode.ProfileCompleted=1;
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=0;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=1;
  axis.SynchronizedMotion=0;
}
void MC_SetPosition(int Execute ,float Position, int Relative,int ExecutionMode)
{
  //將目前位置強制設定為Position(新的開始位置)
  //??Position
  //??Relative
  //??ExecutionMode
  position=Position;
}
void MC_SetOverride(bool Enable,float VelFactor, float AccFactor,int JerkFactor)
{
  //未定義
  //??Enable
  //??VelFactor
  //??AccFactor
  //??JerkFactor
  axis.Enabled=1;
}
void MC_ReadParameter(bool Enable,int ParameterNumber)
{
  //讀MC_Parameter值
  ReadParameterNumberTable(ParameterNumber);
  axis.Value=1;
}
void MC_ReadboolParameter(bool Enable,int ParameterNumber)
{
  //讀MC_Parameter值
  ReadboolParameterNumberTable(ParameterNumber);
  axis.Value=1;
}
void MC_WriteParameter(int Execute ,int ParameterNumber,float Value,int ExecutionMode)
{
  //寫MC_Parameter值
  //??ExecutionMode
  WriteRealParameterTable(ParameterNumber,Value);
}
void MC_WriteboolParameter(int Execute ,int ParameterNumber,int Value,int ExecutionMode)
{
  //寫MC_Parameter值
  //??ExecutionMode
  WriteboolParameterTable(ParameterNumber,Value);
}
void MC_ReadDigitalInput(bool Enable,int InputNumber)
{
  //讀取一個外訊號
  //Enable
  //InputNumber
  axis.Value=0;
}
void MC_ReadDigitalOutput(bool Enable,int OutputNumber)
{
  //未定義
  //Enable
  //OutputNumber
  axis.Value=0;
}
void MC_WriteDigitalOutput(bool Enable,int OutputNumber,int Value,int ExecutionMode)
{
  //輸出訊息給外部的機械
  //Enable
  //OutputNumber
  //Value
  //??ExecutionMode
}
void MC_ReadActualPosition(int Enable)
{
  printf("Position: %f\n\r",position);
}
void MC_ReadActualVelocity(int Enable)
{
  printf("Velocity: %f\n\r",holdvelocity);
}
void MC_ReadActualTorque(int Enable)
{
  printf("Torque: %f\n\r",torque);
}
void MC_ReadStatus(int Enable)
{
  if(axis.Disabled==1)
  {
    printf("Status: Disabled\n\r");
  }
  else if(axis.Stopping==1)
  {
    printf("Status: Stopping\n\r");
  }
  else if(axis.Homing==1)
  {
    printf("Status: Homing\n\r");
  }
  else if(axis.Standstill==1)
  {
    printf("Status: Standstill\n\r");
  }
  else if(axis.DiscreteMotion==1)
  {
    printf("Status: DiscreteMotion\n\r");
  }
  else if(axis.ContinuousMotion==1)
  {
    printf("Status: ContinuousMotion\n\r");
  }
  else
  {
    printf("Status: SynchronizedMotion\n\r");
  }
}
void MC_ReadMotionState(bool Enable,int Source)
{
  if(axis.ConstantVelocity==1)
  {
    printf("MotionState: ConstantVelocity\n\r");
  }
  else if(axis.Accelerating==1)
  {
    printf("MotionState: Accelerating\n\r");
  }
  else if(axis.Decelerating==1)
  {
    printf("MotionState: Decelerating\n\r");
  }
  if(axis.DirectionPositive==1)
  {
    printf("MotionState: DirectionPositive\n\r");
  }
  else if(axis.DirectionNegative==1)
  {
    printf("MotionState: DirectionNegative\n\r");
  }
  //??Source
}
void MC_ReadAxisInfo(int Enable)
{
  if(axis.HomeAbsSwitch==1)
  {
    printf("AxisInfo: HomeAbsSwitch\n\r");
  }
  if(axis.LimitSwitchPos==1)
  {
    printf("AxisInfo: LimitSwitchPos\n\r");
  }
  if(axis.LimitSwitchNeg==1)
  {
    printf("AxisInfo: LimitSwitchNeg\n\r");
  }
  if(axis.Simulation==1)
  {
    printf("AxisInfo: Simulation\n\r");
  }
  if(axis.CommunicationReady==1)
  {
    printf("AxisInfo: CommunicationReady\n\r");
  }
  if(axis.ReadyForPowerOn==1)
  {
    printf("AxisInfo: ReadyForPowerOn\n\r");
  }
  if(axis.PowerOn==1)
  {
    printf("AxisInfo: PowerOn\n\r");
  }
  if(axis.IsHomed==1)
  {
    printf("AxisInfo: IsHomed\n\r");
  }
  if(axis.AxisWarning==1)
  {
    printf("AxisInfo: AxisWarning\n\r");
  }
}
void MC_ReadAxisError(int Enable)
{
  AxisError();
}
void MC_Reset(int Enable)
{
  //回到靜止前的動作
  axis.ErrorStop=0;
  axis.Standstill=1;
  axiserror=0;
}
void MC_DigitalCamSwitch(bool Enable,float EnableMask,int ValueSource)
{
  //未定義
  //??EnableMask
  //??ValueSource
  axis.InOperation=1;
}
void MC_TouchProbe(int Execute,int WindowOnly,float FirstPosition,float LastPosition)
{
  //記錄一個觸發的位置
  //??WindowOnly
  //??FirstPosition
  //??LastPosition
  Probe=1;
  axis.RecordedPosition=position;
}
void MC_AbortTrigger(int Execute)
{
  Probe=0;
}

void MC_CamTableSelect(int Execute,int Periodic,int MasterAbsolute,int SlaveAbsolute,int ExecutionMode)
{
  multiAxis.Done=0;
}
void MC_CamIn(int Execute,int ContiunousUpdate,float MasterOffset,float SlaveOffset,float MasterScaling,float SlaveScaling,float MasterStartDistance,float MasterSyncPosition,int StartMode,int MasterValueSource,int CamTableId,int BufferMode)
{
}
void MC_CamOut(int Execute)
{
}
void MC_GearIn(int Execute,int ContinuousUpdate,int RatioNumberator,int RatioDenominator,int MasterValueSource,float Acceleration,float Deceleration,float Jerk,int BufferMode)
{
}
void MC_GearOut(int Execute)
{
}
void MC_GearInPos(int Execute,int RatioNumberator,int RatioDenominator,int MasterValueSource,float MasterSyncPosition,float SlaveSyncPosition,int SyncMode,float MasterStartDistance,float Velocity,float Acceleration,float Deceleration,float Jerk,int BufferMode)
{
}
void MC_PhasingAbsolute(int Execute,float PhaseShift,float Velocity,float Acceleration,float Deceleration,float Jerk,int BufferMode)
{
}
void MC_PhasingRelative(int Execute,float PhaseShift,float Velocity,float Acceleration,float Deceleration,float Jerk,int BufferMode)
{
}
void MC_CombineAxes(int Execute,int ContinuousUpdate,int CombineMode,int GearRatioNumeratorM1,int GearRatioDenominatorM1,int GearRatioNumeratorM2,int GearRatioDenominatorM2,int mcMasterValueSourceM1,int mcMasterValueSourceM2,int BufferMode)
{
}




void initial()
{
  axis.Valid=0;
  axis.Done=0;
  axis.Active=0;
  axis.Busy=0;
  axis.CommandAborted=0;
  axis.Error=0;
  axis.ErrorID=0;
  axis.ErrorStop=0;
  axis.Status=0;
  axis.Disabled=0;
  axis.Stopping=0;
  axis.Homing=0;
  axis.Standstill=1;
  axis.DiscreteMotion=0;
  axis.ContinuousMotion=0;
  axis.SynchronizedMotion=0;
  axis.ConstantVelocity=1;
  axis.Accelerating=0;
  axis.Decelerating=0;
  axis.DirectionPositive=0;
  axis.DirectionNegative=0;
  axis.HomeAbsSwitch=1;
  axis.LimitSwitchPos=1;
  axis.LimitSwitchNeg=1;
  axis.Simulation=0;
  axis.CommunicationReady=0;
  axis.ReadyForPowerOn=1;
  axis.PowerOn=0;
  axis.IsHomed=0;
  axis.AxisWarning=0;
  axis.InVelocity=0;
  axis.InEndVelocity=0;
  axis.InTorque=0;
  axis.ProfileCompleted=0;
  axis.AxisErrorID=0;
  axis.InOperation=0;
  axis.Enabled=0;
  axis.Position=0;
  axis.Velocity=0;
  axis.Torque=0;
  axis.CoveredDistance=0;
  axis.Value=0;
  axis.RecordedPosition=0;

  mc_parameter.CommandedPosition=0;
  mc_parameter.SWLimitPos=10000;
  mc_parameter.SWLimitNeg=-10000;
  mc_parameter.EnableLimitPos=1;
  mc_parameter.EnableLimitNeg=1;
  mc_parameter.EnablePosLagMonitoring=1;
  mc_parameter.MaxPositionLag=0;
  mc_parameter.MaxVelocitySystem=60;
  mc_parameter.MaxVelocityAppl=60;
  mc_parameter.ActualVelocity=holdvelocity;
  mc_parameter.CommandedVelocity=0;
  mc_parameter.MaxAccelerationSystem=1;
  mc_parameter.MaxAccelerationAppl=1;
  mc_parameter.MaxDecelerationSystem=1;
  mc_parameter.MaxDecelerationAppl=1;
  mc_parameter.MaxJerkSystem=1;
  mc_parameter.MaxJerkAppl=1;

  setMultiAxisinfo();
}




void setMultiAxisinfo(void)
{
  multiAxis.Done=0;           //int
  multiAxis.Busy=0;           //int
  multiAxis.Error=0;           //int
  multiAxis.ErrorID=0;         //int
  multiAxis.CamTableID=0;       //int
  multiAxis.InSync=0;         //int
  multiAxis.Active=0;         //int
  multiAxis.CommandAborted=0; //int
  multiAxis.EndOfProfile=0;     //int
  multiAxis.InGear=0;         //int
  multiAxis.StartSync=0;       //int
  multiAxis.AbsolutePhaseShift=0.0;//float
  multiAxis.ConveredPhaseShift=0.0;//float

}

void returnPeriod()
{
  if(holdvelocity>=0&&holdvelocity<10)
  {
    
    //TIM_TimeBaseStructure.TIM_Period = 128;
  }
  else if(holdvelocity>=10&&holdvelocity<20)
  {
    //TIM_TimeBaseStructure.TIM_Period = 64;
  }
  else if(holdvelocity>=20&&holdvelocity<30)
  {
    //TIM_TimeBaseStructure.TIM_Period = 32;
  }
  else if(holdvelocity>=30&&holdvelocity<40)
  {
    //TIM_TimeBaseStructure.TIM_Period = 16;
  }
  else if(holdvelocity>=40&&holdvelocity<50)
  {
    //TIM_TimeBaseStructure.TIM_Period = 8;
  }
  else if(holdvelocity>=50&&holdvelocity<=60)
  {
    //TIM_TimeBaseStructure.TIM_Period = 4;
  }
  //TIM_TimeBaseStructure.TIM_Period 本來就會影響TIM2,TIM3
  //TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);    //***只重新設定Timer2的週期,就不會影響TIM3
}







void ReadParameterNumberTable(int number)
{
  if(number==1)
  {
    //CommandedPosition
    printf("\n\r");
  }
  else if(number==2)
  {
    //SWLimitPos
    printf("%f\n\r",mc_parameter.SWLimitPos);
  }
  else if(number==3)
  {
    //SWLimitNeg
    printf("%f\n\r",mc_parameter.SWLimitNeg);
  }
  else if(number==7)
  {
    //MaxPositionLag
    printf("\n\r");
  }
  else if(number==8)
  {
    //MaxVelocitySystem
    printf("%f\n\r",mc_parameter.MaxVelocitySystem);
  }
  else if(number==9)
  {
    //MaxVelocityAppl
    printf("%f\n\r",velocity);
  }
  else if(number==10)
  {
    //ActualVelocity
    printf("%f\n\r",holdvelocity);
  }
  else if(number==11)
  {
    //CommandedVelocity
    printf("\n\r");
  }
  else if(number==12)
  {
    //MaxAccelerationSystem
    printf("%f\n\r",mc_parameter.MaxAccelerationSystem);
  }
  else if(number==13)
  {
    //MaxAccelerationAppl
    printf("%f\n\r",acceleration);
  }
  else if(number==14)
  {
    //MaxDecelerationSystem
    printf("%f\n\r",mc_parameter.MaxDecelerationSystem);
  }
  else if(number==15)
  {
    //MaxDecelerationAppl
    printf("%f\n\r",deceleration);
  }
  else if(number==16)
  {
    //MaxJerkSystem
    printf("%f\n\r",mc_parameter.MaxJerkSystem);
  }
  else if(number==17)
  {
    //MaxJerkAppl
    printf("%f\n\r",jerk);
  }
}
void ReadboolParameterNumberTable(int number)
{
  if(number==4)
  {
    //EnableLimitPos
    printf("%d\n\r",mc_parameter.EnableLimitPos);
  }
  else if(number==5)
  {
    //EnableLimitNeg
    printf("%d\n\r",mc_parameter.EnableLimitNeg);
  }
  else if(number==6)
  {
    //EnablePosLagMonitoring
    printf("%d\n\r",mc_parameter.EnablePosLagMonitoring);
  }
}
void WriteRealParameterTable(int number,float realvalue)
{
  if(number==2)
  {
    mc_parameter.SWLimitPos=realvalue;
  }
  else if(number==3)
  {
    mc_parameter.SWLimitNeg=realvalue;
  }
  else if(number==7)
  {
    mc_parameter.MaxPositionLag=realvalue;
  }
  else if(number==9)
  {
    mc_parameter.MaxVelocityAppl=realvalue;
  }
  else if(number==13)
  {
    mc_parameter.MaxAccelerationAppl=realvalue;
  }
  else if(number==15)
  {
    mc_parameter.MaxDecelerationAppl=realvalue;
  }
  else if(number==17)
  {
    mc_parameter.MaxJerkAppl=realvalue;
  }
}
void WriteboolParameterTable(int number,int boolvalue)
{
  if(number==4)
  {
    mc_parameter.EnableLimitPos=boolvalue;
  }
  else if(number==5)
  {
    mc_parameter.EnableLimitNeg=boolvalue;
  }
  else if(number==6)
  {
    mc_parameter.EnablePosLagMonitoring=boolvalue;
  }
}



void ErrorIDTable(int number)
{
  if(number==0)
  {
    printf("reach the limit of right\n\r");
  }
  else if(number==1)
  {
    printf("reach the limit of left\n\r");
  }
  else if(number==2)
  {
    printf("不能分辨正轉還是反轉\n\r");
  }
}
void AxisError(void)
{
  if(axiserror==1)
  {
    printf("AxisError: axis errors\n\r");
  }
  else if(axiserror==2)
  {
    printf("AxisError: drive errors\n\r");
  }
  else if(axiserror==3)
  {
    printf("AxisError: communication errors\n\r");
  }
}


*/