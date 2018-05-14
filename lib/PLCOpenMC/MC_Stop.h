/*******************************************************************************
* File Name          : MC_Stop.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/11/2018
* Description        : MC_Stop FB
********************************************************************************
* 說明：MC_Stop FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_STOP_H__
#define __MC_STOP_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/


typedef struct MC_Stop_T MC_Stop_T;
void MC_Stop_updater(MC_Stop_T *obj);
struct MC_Stop_T{
  AXIS_REF *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  REAL *Deceleration;         //IN  E Value of the ‘Deceleration’ [u/s2]
  REAL *Jerk;                 //IN  E Value of the ‘Jerk’ [u/s3]
  BOOL Done;                  //OUT B Zero velocity reached
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  void (*updater)(MC_Stop_T* obj);
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
};


#endif //__MC_STOP_H__