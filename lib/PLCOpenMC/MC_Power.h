/*******************************************************************************
* File Name          : MC_Power.h
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_Power FB
********************************************************************************
* 說明：MC_Power FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_POWER_H__
#define __MC_POWER_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/

void MC_Power_updater(void *obj);
typedef struct {
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  AXIS_t *Axis;        //I/O B axis
  BOOL *Enable;          //IN  B As long as ‘Enable’ is true, power is being enabled.
  BOOL *EnablePositive;  //IN  E As long as ‘Enable’ is true, this permits motion in positive direction
  BOOL *EnableNegative;   //IN  E As long as ‘Enable’ is true, this permits motion in negative direction
  BOOL Status;           //OUT B Effective state of the power stage
  BOOL Valid;            //OUT E If true, a valid set of outputs is available at the FB
  BOOL Error;            //OUT B Signals that an error has occurred within the Function Block
  WORD ErrorID;          //OUT E Error identification
  BOOL prevEnable;
}MC_Power_t;
void FB_ADD_MC_POWER_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount);
#endif //__MC_POWER_H__