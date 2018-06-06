/*******************************************************************************
* File Name          : MC_Stop.c
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_Stop FB
********************************************************************************
* 說明：MC_Stop FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_Stop.h"
// ############################### Object Way ###############################


void MC_Stop_updater(void *iobj){
  MC_Stop_t* obj = (MC_Stop_t*) iobj;
  if(obj->Axis->power == FALSE){
    //斷電，放棄所有動作而且指出CommandAborted
      obj->Done = FALSE;
      obj->Busy = FALSE;
      obj->CommandAborted = TRUE;
      obj->Error = FALSE;
      obj->ErrorID = WORD_NO_ERROR;
      return;
  }

  if( obj->prevExecute == FALSE &&
      *(obj->Execute) == TRUE
      ){
    //登記開始一個Stopping FB
    obj->prevExecute = *(obj->Execute);
    obj->Done = FALSE;
    obj->Axis->CurrentFB = (void *)obj;
    obj->Busy = FALSE; //保持FALSE來讓DEV_func判斷是否為第一次進入
    obj->Axis->setStat(obj->Axis,FA_STOPPING);
    unsigned char res = obj->Axis->callStop(obj->Axis, obj->Busy, *(obj->Deceleration), *(obj->Jerk));
    switch (res){
      case(0):{
        //完成
        obj->Done = TRUE;
        obj->Busy = FALSE;
        obj->Axis->CurrentFB = (void *)0;
        obj->CommandAborted = FALSE;
        obj->Error = FALSE;
        obj->ErrorID = WORD_NO_ERROR;
        break;
      }
      case(1):{
        //進行中
        obj->Done = FALSE;
        obj->Busy = TRUE;
        obj->CommandAborted = FALSE;
        obj->Error = FALSE;
        obj->ErrorID = WORD_NO_ERROR;
        break;
      }
      case(2):{
        //錯誤
        obj->Done = FALSE;
        obj->Busy = FALSE;
        obj->Axis->CurrentFB = (void *)0;
        obj->CommandAborted = FALSE;
        obj->Error = FALSE;
        obj->ErrorID = WORD_NO_ERROR;
        break;
      }
    }
  }else if(*(obj->Execute) == FALSE && obj->Done == TRUE){
    obj->Axis->setStat(obj->Axis, FA_STANDSTILL);
  }
  obj->prevExecute = *(obj->Execute);
}

void FB_ADD_MC_STOP_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount)
{
  printf("FB_ADD_MC_STOP_PAGE\r\n");
  MC_Stop_t *fbobj =  malloc(sizeof(MC_Stop_t));
  *fbobj = (MC_Stop_t){MC_Stop_updater, 
  malloc(sizeof(void ***)*4),  //C inList
  malloc(sizeof(void **)*6),   //C outList
  4, //C  inNumber
  6, //C  outNumber
  0, //IO axis 
  0, //I  Execute
  0, //I  Deceleration
  0, //I  Jerk
  0, //O  Done
  0, //O  Busy
  0, //O  CommandAborted
  0, //O  Error
  0, //O  ErrorID
  0  //C  prevEnable, internel use
  };
  ((*fbobj).inList)[0] = (void **)&(fbobj->Axis);
  ((*fbobj).inList)[1] = (void **)&(fbobj->Execute);
  ((*fbobj).inList)[2] = (void **)&(fbobj->Deceleration);
  ((*fbobj).inList)[3] = (void **)&(fbobj->Jerk);
  ((*fbobj).outList)[0] = (fbobj->Axis);
  ((*fbobj).outList)[1] = &(fbobj->Done);
  ((*fbobj).outList)[2] = &(fbobj->Busy);
  ((*fbobj).outList)[3] = &(fbobj->CommandAborted);
  ((*fbobj).outList)[4] = &(fbobj->Error);
  ((*fbobj).outList)[5] = &(fbobj->ErrorID);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_MC_Stop);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}


