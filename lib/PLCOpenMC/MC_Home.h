/*******************************************************************************
* File Name          : MC_Home.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/11/2018
* Description        : MC_Home FB
********************************************************************************
* 說明：MC_Home FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_HOME_H__
#define __MC_HOME_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/


typedef struct MC_Home_T MC_Home_T;
void MC_Home_updater(MC_Home_T *obj);
struct MC_Home_T{
  AXIS_REF *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  REAL *Position;             //IN  E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
  MC_BUFFER_MODE *BufferMode;  //IN  E As long as ‘Enable’ is true, this permits motion in negative direction
  BOOL Done;                  //OUT B Reference known and set sucessfully
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL Active;                //OUT E Indicates that the FB has control on the axis
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  void (*updater)(MC_Home_T* obj);
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
};


#endif //__MC_HOME_H__