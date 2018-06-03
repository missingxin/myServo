/*******************************************************************************
* File Name          : MC_Stop.h
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_Stop FB
********************************************************************************
* 說明：MC_Stop FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_STOP_H__
#define __MC_STOP_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/

void MC_Stop_updater(void *obj);
typedef struct{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  AXIS_t *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  REAL *Deceleration;         //IN  E Value of the ‘Deceleration’ [u/s2]
  REAL *Jerk;                 //IN  E Value of the ‘Jerk’ [u/s3]
  BOOL Done;                  //OUT B Zero velocity reached
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
}MC_Stop_t;
void FB_ADD_MC_STOP_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount);
#endif //__MC_STOP_H__