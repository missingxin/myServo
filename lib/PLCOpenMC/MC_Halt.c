/*******************************************************************************
* File Name          : PLCOpen.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/8/2018
* Description        : 建立可物件化的PLCOpen Middleware
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCOpen Middleware
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_Halt.h"
// ############################### Object Way ###############################

//######### 需要額外實作的功能 ####################
//呼叫一個可以持續被呼叫的函式，
//目的：當doing為0時表示為一個新的hoem指令
//             TRUE時表示是一個正在跑的指令，需要查詢它的狀態
//回傳0時表示完成且無錯誤
//回傳1時表示進行中
//回傳2時表示有錯誤且已停止
extern unsigned char DEV_Halt(
  AXIS_REF * axis, 
  BOOL doing,
  REAL accelerateion,
  REAL jerk
);




//######### 輸入輸出及狀態管理器 ####################
void MC_Halt_updater(MC_Halt_T *obj){
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
    unsigned char res = DEV_Halt(obj->Axis, obj->Busy, *(obj->Deceleration),*(obj->Jerk)  );
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

