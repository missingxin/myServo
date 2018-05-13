#ifndef __MC_MOVEABSOLUTE_H__
#define __MC_MOVEABSOLUTE_H__
/*******************************************************************************
* File Name          : PLCOpen.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/8/2018
* Description        : 建立可物件化的PLCOpen Middleware
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCOpen Middleware
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#include "PLCOpen.h"


typedef struct MC_MoveAbsolute_T MC_MoveAbsolute_T;
void MC_MoveAbsolute_updater(MC_MoveAbsolute_T *obj);
struct MC_MoveAbsolute_T{
  AXIS_REF *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  BOOL *ContinuousUpdate;     //IN  E See 2.4.6 The input ‘ContinuousUpdate’
  REAL *Position;             //IN  E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
  REAL *Velocity;             //IN  E Value of the maximum ‘Velocity’ (not necessarily reached) [u/s].
  REAL *Acceleration;         //IN  E Value of the ‘Acceleration’ (always positive) (increasing energy of the motor) [u/s2]
  REAL *Deceleration;         //IN  E Value of the ‘Deceleration’ (always positive) (decreasing energy of the motor) [u/s2]
  REAL *Jerk;                 //IN  E Value of the ‘Jerk’ [u/s3]
  MC_DIRECTION *Direction;    //IN  B Enum type (1-of-4 values: mcPositiveDirection, mcShortestWay, mcNegativeDirection, mcCurrentDirection)
  MC_BUFFER_MODE *BufferMode; //IN  E Defines the chronological sequence of the FB. See 2.4.2 ‘Aborting versus Buffered modes’

  BOOL Done;                  //OUT B Commanded position finally reached
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL Active;                //OUT E Indicates that the FB has control on the axis
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  void (*updater)(MC_MoveAbsolute_T* obj);
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
};


#endif //__MC_MOVEABSOLUTE_H__