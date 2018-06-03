/*******************************************************************************
* File Name          : MC_MoveAbsolute.h
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_MoveAbsolute FB
********************************************************************************
* 說明：MC_MoveAbsolute FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_MOVEABSOLUTE_H__
#define __MC_MOVEABSOLUTE_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/

void MC_MoveAbsolute_updater(void *obj);
typedef struct{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  AXIS_t *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  BOOL *ContinuousUpdate;     //IN  E See 2.4.6 The input ‘ContinuousUpdate’
  REAL *Position;             //IN  E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
  REAL *Velocity;             //IN  E Value of the maximum ‘Velocity’ (not necessarily reached) [u/s].
  REAL *Acceleration;         //IN  E Value of the ‘Acceleration’ (always positive) (increasing energy of the motor) [u/s2]
  REAL *Deceleration;         //IN  E Value of the ‘Deceleration’ (always positive) (decreasing energy of the motor) [u/s2]
  REAL *Jerk;                 //IN  E Value of the ‘Jerk’ [u/s3]
  MC_DIRECTION_t *Direction;  //IN  B Enum type (1-of-4 values: mcPositiveDirection, mcShortestWay, mcNegativeDirection, mcCurrentDirection)
  MC_BUFFER_MODE *BufferMode; //IN  E Defines the chronological sequence of the FB. See 2.4.2 ‘Aborting versus Buffered modes’

  BOOL Done;                  //OUT B Commanded position finally reached
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL Active;                //OUT E Indicates that the FB has control on the axis
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
}MC_MoveAbsolute_t;
void FB_ADD_MC_MOVEABSOLUTE_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount);
#endif //__MC_MOVEABSOLUTE_H__