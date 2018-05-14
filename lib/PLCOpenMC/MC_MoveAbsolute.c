/*******************************************************************************
* File Name          : MC_MoveAbsolute.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/13/2018
* Description        : MC_MoveAbsolute FB
********************************************************************************
* 說明：MC_MoveAbsolute FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_MoveAbsolute.h"
// ############################### Object Way ###############################

//######### 需要額外實作的功能 ####################
//呼叫一個可以持續被呼叫的函式，
//目的：當doing為0時表示為一個新的hoem指令
//             TRUE時表示是一個正在跑的指令，需要查詢它的狀態
//回傳0時表示完成且無錯誤
//回傳1時表示進行中
//回傳2時表示有錯誤且已停止
extern unsigned char DEV_MoveAbsolute(
  AXIS_REF * axis, 
  BOOL doing,
  BOOL continuousUpdate,
  REAL position,
  REAL velocity,
  REAL Acceleration,
  REAL Deceleration,
  REAL Jerk,
  MC_DIRECTION direction
);



//######### 輸入輸出及狀態管理器 ####################
void MC_Home_updater(MC_MoveAbsolute_T *obj){
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
    obj->Busy = FALSE; //保持FALSE來讓DEV_func判斷是否為第一次進入
  }
  obj->prevExecute = *(obj->Execute);


  if (obj->Active){
    unsigned char res = DEV_MoveAbsolute(
      obj->Axis, 
      obj->Busy, 
      *(obj->ContinuousUpdate),
      *(obj->Position),
      *(obj->Velocity),
      *(obj->Acceleration),
      *(obj->Deceleration),
      *(obj->Jerk),
      *(obj->Direction)
    );
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
