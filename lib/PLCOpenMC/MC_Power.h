/*******************************************************************************
* File Name          : MC_Power.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/11/2018
* Description        : MC_Power FB
********************************************************************************
* 說明：MC_Power FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_POWER_H__
#define __MC_POWER_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/

typedef struct MC_Power_T MC_Power_T;
void MC_Power_updater(void *obj);
struct MC_Power_T{
  void (*updater)(void* obj);
  AXIS_REF *Axis;        //I/O B axis
  BOOL *Enable;          //IN  B As long as ‘Enable’ is true, power is being enabled.
  BOOL *EnablePositive;  //IN  E As long as ‘Enable’ is true, this permits motion in positive direction
  BOOL *EnableNegative;   //IN  E As long as ‘Enable’ is true, this permits motion in negative direction
  BOOL Status;           //OUT B Effective state of the power stage
  BOOL Valid;            //OUT E If true, a valid set of outputs is available at the FB
  BOOL Error;            //OUT B Signals that an error has occurred within the Function Block
  WORD ErrorID;          //OUT E Error identification
  BOOL prevEnable;
};


#endif //__MC_POWER_H__