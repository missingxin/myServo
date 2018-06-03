/******************************************************************************
* File Name          : Axis.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 06/3/2018
* Description        : PLCOpen MC 的驅動器物件，將由此處連接各種驅動器
********************************************************************************
* 說明：PLCOpen MC 的驅動器物件，將由此處連接各種驅動器
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/

#ifndef __AXIS_H__
#define __AXIS_H__
#include "PLCLogic.h"
typedef enum{
  PowerControl_Done = 0,
  PowerControl_Doing,
  PowerControl_Error
}PowerControlResult_t;


typedef enum {
  FA_DISABLED,  //0
  FA_STANDSTILL,
  FA_HOMING,
  FA_STOPPING,
  FA_DISCRETEMOTION,
  FA_CONTINUOUSMOTION,
  FA_SYNCHRONIZEDMOTION,
  FA_ERRORSTOP
}FA_STAT_NUM_t;

typedef struct{
  BOOL FA_DISABLED;
  BOOL FA_STANDSTILL;
  BOOL FA_HOMING;
  BOOL FA_STOPPING;
  BOOL FA_DISCRETEMOTION;
  BOOL FA_CONTINUOUSMOTION;
  BOOL FA_SYNCHRONIZEDMOTION;
  BOOL FA_ERRORSTOP;
}FA_STAT_t;


typedef enum{
  mcPositiveDirection,
  mcShortestWay,
  mcNegativeDirection,
  mcCurrentDirection
}MC_DIRECTION_t;

//typedef PowerControlResult_t (*AxisPowerControl_t)( AXIS_t * axis, BOOL enable,  BOOL enablePositive,  BOOL enableNegative );

typedef struct AXIS_t AXIS_t;
struct AXIS_t{
  FA_STAT_t stat;     // FA_Disabled;
  BOOL    power;    // FALSE;
  BOOL    positive; // FALSE;
  BOOL    negative; // FALSE;
  void *  DEVICE;
  void *  CurrentFB;
  void (*setStat)(AXIS_t *axis, FA_STAT_NUM_t stat); 
//######### 需要額外實作的功能 ####################
  PowerControlResult_t (*callPower)(  AXIS_t * axis, 
                                        BOOL enable,  
                                        BOOL enablePositive,  
                                        BOOL enableNegative );
//######### 需要額外實作的功能 ####################
  unsigned char (*callStop)(  AXIS_t * axis, 
                          BOOL doing,
                          REAL acceleration,
                          REAL jerk);

//######### 需要額外實作的功能 ####################
  unsigned char (*callHome)(  AXIS_t * axis, 
                            BOOL doing,
                            REAL position);
//######### 需要額外實作的功能 ####################
  unsigned char (*callHalt)(  AXIS_t * axis, 
                          BOOL doing,
                          REAL accelerateion,
                          REAL jerk );

//######### 需要額外實作的功能 ####################
  unsigned char (*callMoveAbs)( AXIS_t * axis, 
                            BOOL doing,
                            BOOL continuousUpdate,
                            REAL position,
                            REAL velocity,
                            REAL Acceleration,
                            REAL Deceleration,
                            REAL Jerk,
                            MC_DIRECTION_t direction);


};

typedef struct{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  void (*assign)(void* obj, unsigned char idx);
  AXIS_t Axis;
}INPUT_AXIS_t;


void setStat(AXIS_t *axis, FA_STAT_NUM_t stat);

void FB_ADD_AXIS_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount);


#endif //__AXIS_H__