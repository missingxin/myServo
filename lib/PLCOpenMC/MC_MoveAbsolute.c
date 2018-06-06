/*******************************************************************************
* File Name          : MC_MoveAbsolute.c
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_MoveAbsolute FB
********************************************************************************
* 說明：MC_MoveAbsolute FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_MoveAbsolute.h"
// ############################### Object Way ###############################





//######### 輸入輸出及狀態管理器 ####################
void MC_MoveAbs_updater(void *iobj){
  MC_MoveAbsolute_t* obj = (MC_MoveAbsolute_t*) iobj;
  if(obj->Axis->power == FALSE){
      //忽然停電
      obj->Done = FALSE;
      obj->Busy = FALSE;
      obj->Active = FALSE;
      obj->CommandAborted = FALSE;
      obj->Error = FALSE;
      obj->ErrorID = WORD_NO_ERROR;
      obj->prevExecute = *(obj->Execute);
      return;
  }
  
  obj->Active = obj->Axis->CurrentFB == (void *)obj;
  if( obj->prevExecute == FALSE &&
      *(obj->Execute) == TRUE
      ){
    //開始跑FB
    obj->prevExecute = *(obj->Execute);
    obj->Done = FALSE;
    obj->Axis->CurrentFB = (void *)obj; obj->Active = 1;
    obj->Busy = FALSE; //保持FALSE來讓device判斷是否為第一次進入
  }
  obj->prevExecute = *(obj->Execute);

  if (obj->Active){
    unsigned char res = obj->Axis->callMoveAbs(
        obj->Axis,
        obj->Busy,
        *(obj->ContinuousUpdate),
        *(obj->Position),
        *(obj->Velocity),
        *(obj->Acceleration),
        *(obj->Deceleration),
        *(obj->Jerk),
        *(obj->Direction));
    switch (res){
      case(PowerControl_Done):{
        //完成
        obj->Done = TRUE;
        obj->Busy = FALSE;
        obj->Axis->CurrentFB = (void *)0;
        obj->CommandAborted = FALSE;
        obj->Error = FALSE;
        obj->ErrorID = WORD_NO_ERROR;
        break;
      }
      case(PowerControl_Doing):{
        //進行中
        obj->Done = FALSE;
        obj->Busy = TRUE;
        obj->CommandAborted = FALSE;
        obj->Error = FALSE;
        obj->ErrorID = WORD_NO_ERROR;
        break;
      }
      case(PowerControl_Error):{
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
  }else{
    if(obj->Busy){
      //被其他FB取得執行權，放棄執行
        obj->Done = FALSE;
        obj->Busy = FALSE;
        obj->CommandAborted = TRUE;
        obj->Error = FALSE;
        obj->ErrorID = WORD_NO_ERROR;
    }
  }
}


void FB_ADD_MC_MOVEABSOLUTE_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount)
{
  printf("FB_ADD_MC_MOVEABSOLUTE_PAGE\r\n");
  MC_MoveAbsolute_t *fbobj =  malloc(sizeof(MC_MoveAbsolute_t));
  *fbobj = (MC_MoveAbsolute_t){MC_MoveAbs_updater, 
  malloc(sizeof(void ***)*10),  //C inList
  malloc(sizeof(void **)*7),   //C outList
  10, //C  inNumber
  7, //C  outNumber
  0, //IO axis 
  0, //I  Execute
  0, //I  ContinuousUpdate
  0, //I  Position
  0, //I  Velocity
  0, //I  Acceleration
  0, //I  Deceleration
  0, //I  Jerk
  0, //I  Direction
  0, //I  BufferMode
  0, //O  Done
  0, //O  Busy
  0, //O  Active
  0, //O  CommandAborted
  0, //O  Error
  0, //O  ErrorID
  0  //C  prevEnable, internel use
  };
  ((*fbobj).inList)[0] = (void **)&(fbobj->Axis);
  ((*fbobj).inList)[1] = (void **)&(fbobj->Execute);
  ((*fbobj).inList)[2] = (void **)&(fbobj->ContinuousUpdate);
  ((*fbobj).inList)[3] = (void **)&(fbobj->Position);
  ((*fbobj).inList)[4] = (void **)&(fbobj->Velocity);
  ((*fbobj).inList)[5] = (void **)&(fbobj->Acceleration);
  ((*fbobj).inList)[6] = (void **)&(fbobj->Deceleration);
  ((*fbobj).inList)[7] = (void **)&(fbobj->Jerk);
  ((*fbobj).inList)[8] = (void **)&(fbobj->Direction);
  ((*fbobj).inList)[9] = (void **)&(fbobj->BufferMode);
  ((*fbobj).outList)[0] = (fbobj->Axis);
  ((*fbobj).outList)[1] = &(fbobj->Done);
  ((*fbobj).outList)[2] = &(fbobj->Busy);
  ((*fbobj).outList)[3] = &(fbobj->Active);
  ((*fbobj).outList)[4] = &(fbobj->CommandAborted);
  ((*fbobj).outList)[5] = &(fbobj->Error);
  ((*fbobj).outList)[6] = &(fbobj->ErrorID);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_MC_MoveAbsolute);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}