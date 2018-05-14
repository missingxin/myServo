/*******************************************************************************
* File Name          : MC_Halt.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/12/2018
* Description        : MC_Halt FB
********************************************************************************
* 說明：MC_Halt FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_HALT_H__
#define __MC_HALT_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/


typedef struct MC_Halt_T MC_Halt_T;
void MC_Halt_updater(MC_Halt_T *obj);
struct MC_Halt_T{
  AXIS_REF *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  REAL *Deceleration;         //IN  E Value of the ‘Deceleration’ [u/s2]
  REAL *Jerk;                 //IN  E Value of the ‘Jerk’ [u/s3]
  MC_BUFFER_MODE *BufferMode; //IN  E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
  BOOL Done;                  //OUT B Zero velocity reached
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL Active;                //OUT E Indicates that the FB has control on the axis
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  void (*updater)(MC_Halt_T* obj);
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
};


#endif //__MC_HALT_H__