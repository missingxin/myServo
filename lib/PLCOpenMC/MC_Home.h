#ifndef __MC_HOME_H__
#define __MC_HOME_H__
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