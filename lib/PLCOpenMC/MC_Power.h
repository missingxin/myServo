#ifndef __MC_POWER_H__
#define __MC_POWER_H__
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
typedef struct MC_Power_T MC_Power_T;
void MC_Power_updater(MC_Power_T *obj);
struct MC_Power_T{
  AXIS_REF *Axis;        //I/O B axis
  BOOL *Enable;          //IN  B As long as ‘Enable’ is true, power is being enabled.
  BOOL *EnablePositive;  //IN  E As long as ‘Enable’ is true, this permits motion in positive direction
  BOOL *EnableNegative;   //IN  E As long as ‘Enable’ is true, this permits motion in negative direction
  BOOL Status;           //OUT B Effective state of the power stage
  BOOL Valid;            //OUT E If true, a valid set of outputs is available at the FB
  BOOL Error;            //OUT B Signals that an error has occurred within the Function Block
  WORD ErrorID;          //OUT E Error identification
  void (*updater)(MC_Power_T* obj);
};


#endif //__MC_POWER_H__