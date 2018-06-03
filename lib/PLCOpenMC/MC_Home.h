/*******************************************************************************
* File Name          : MC_Home.h
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_Home FB
********************************************************************************
* 說明：MC_Home FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __MC_HOME_H__
#define __MC_HOME_H__
#include "PLCOpen.h"
/* Private typedef -----------------------------------------------------------*/

void MC_Home_updater(void *obj);
typedef struct{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  AXIS_t *Axis;             //I/O B axis
  BOOL *Execute;              //IN  B Start the motion at rising edge
  REAL *Position;             //IN  E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
  MC_BUFFER_MODE *BufferMode;  //IN  E As long as ‘Enable’ is true, this permits motion in negative direction
  BOOL Done;                  //OUT B Reference known and set sucessfully
  BOOL Busy;                  //OUT E The FB is not finished and new output values are to be expected
  BOOL Active;                //OUT E Indicates that the FB has control on the axis
  BOOL CommandAborted;        //OUT E ‘Command’ is aborted by another command
  BOOL Error;                 //OUT E Signals that an error has occurred within the Function Block
  WORD ErrorID;               //OUT E Error identification
  BOOL prevExecute;           //Internal use, to cache previous Execute value, so we can capture the rising edge.
}MC_Home_t;
void FB_ADD_MC_HOME_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount);
#endif //__MC_HOME_H__