/*******************************************************************************
* File Name          : MC_Halt.c
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_Halt FB
********************************************************************************
* 說明：MC_Halt FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_Halt.h"
// ############################### Object Way ###############################

//######### 輸入輸出及狀態管理器 ####################
void MC_Halt_updater(void *iobj){
  MC_Halt_t* obj = (MC_Halt_t*) iobj;
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
  if( obj->Axis->stat.FA_STANDSTILL &&
      obj->prevExecute == FALSE &&
      *(obj->Execute) == TRUE
      ){
    //開始跑FB
    obj->prevExecute = *(obj->Execute);
    obj->Done = FALSE;
    obj->Axis->CurrentFB = (void *)obj; obj->Active = 1;
    obj->Busy = FALSE; //保持FALSE來讓DEV_func判斷是否為第一次進入
  }
  obj->prevExecute = *(obj->Execute);
  
  if (obj->Active){
    unsigned char res = obj->Axis->callHalt(obj->Axis, obj->Busy, *(obj->Deceleration),*(obj->Jerk)  );
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

void FB_ADD_MC_HALT_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount)
{
  printf("FB_ADD_MC_HALT_PAGE\r\n");
  MC_Halt_t *fbobj =  malloc(sizeof(MC_Halt_t));
  *fbobj = (MC_Halt_t){MC_Halt_updater, 
  malloc(sizeof(void ***)*5),  //C inList
  malloc(sizeof(void **)*7),   //C outList
  5, //C  inNumber
  7, //C  outNumber
  0, //IO axis 
  0, //I  Execute
  0, //I  Deceleration
  0, //I  Jerk
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
  ((*fbobj).inList)[2] = (void **)&(fbobj->Deceleration);
  ((*fbobj).inList)[3] = (void **)&(fbobj->Jerk);
  ((*fbobj).inList)[4] = (void **)&(fbobj->BufferMode);
  ((*fbobj).outList)[0] = (fbobj->Axis);
  ((*fbobj).outList)[1] = &(fbobj->Done);
  ((*fbobj).outList)[2] = &(fbobj->Busy);
  ((*fbobj).outList)[3] = &(fbobj->Active);
  ((*fbobj).outList)[4] = &(fbobj->CommandAborted);
  ((*fbobj).outList)[5] = &(fbobj->Error);
  ((*fbobj).outList)[6] = &(fbobj->ErrorID);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_MC_Halt);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}