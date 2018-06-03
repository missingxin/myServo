/******************************************************************************
* File Name          : Axis.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 06/3/2018
* Description        : PLCOpen MC 的驅動器物件，將由此處連接各種驅動器
********************************************************************************
* 說明：PLCOpen MC 的驅動器物件，將由此處連接各種驅動器
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Axis.h"


////######### 立起指定stat, 倒下其他所有stat //#########
void axisSetStat(AXIS_t *axis, FA_STAT_NUM_t stIdx){
  unsigned char i;
  BOOL *status;
  status = (BOOL *)&(axis->stat);
  for(i=0;i<8;i++){ *(status + i) = 0; }
  *(status + stIdx) = 1;
}
PowerControlResult_t axisDoPower( AXIS_t * axis,
                                  BOOL enable,
                                  BOOL enablePositive,
                                  BOOL enableNegative )
{
  printf("  ======== simulator :: AXIS DOING POWER ======== \r\n");
  return PowerControl_Done;
}
unsigned char axisDoStop(AXIS_t * axis, 
                          BOOL doing,
                          REAL acceleration,
                          REAL jerk)
{ 
  printf("  ======== simulator :: AXIS DOING STOP ======== \r\n");
  return 0;
}
unsigned char axisDoHome(AXIS_t * axis, 
                            BOOL doing,
                            REAL position)
{
  printf("  ======== simulator :: AXIS DOING HOME ======== \r\n");
  return 0;
}
unsigned char axisDoHalt(AXIS_t * axis, 
                          BOOL doing,
                          REAL accelerateion,
                          REAL jerk)
{
  printf("  ======== simulator :: AXIS DOING HALT ======== \r\n");
  return 0;
}
unsigned char axisDoMoveAbs(AXIS_t * axis, 
                            BOOL doing,
                            BOOL continuousUpdate,
                            REAL position,
                            REAL velocity,
                            REAL Acceleration,
                            REAL Deceleration,
                            REAL Jerk,
                            MC_DIRECTION_t direction)
{
  printf("  ======== simulator :: AXIS DOING MOVE ABS ======== \r\n");
  return 0;
}


void FB_ADD_AXIS_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount)
{
  INPUT_AXIS_t *fbobj =  malloc(sizeof(INPUT_AXIS_t));

  //實體化AXIS
  *fbobj = (INPUT_AXIS_t){0, 0,malloc(sizeof(void **)*1),0,1,0};
  //todo ： 初始化 axis

  (*fbobj).Axis = (AXIS_t){{1,0,0,0,0,0,0,0},(BOOL)0,(BOOL)0,(BOOL)0,(void *)0,(void *)0,
  axisSetStat,
  axisDoPower,
  axisDoStop,
  axisDoHome,
  axisDoHalt,
  axisDoMoveAbs,
  };

  ((*fbobj).outList)[0] = &(fbobj->Axis);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_Input_Axis);
  //加入fbpool以取得連結之id
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}
