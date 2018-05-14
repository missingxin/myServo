/*******************************************************************************
* File Name          : MC_Power.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/11/2018
* Description        : MC_Power FB
********************************************************************************
* 說明：MC_Power FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_Power.h"
// ############################### Object Way ###############################

//######### 需要額外實作的功能 ####################
//呼叫一個可以持續被呼叫的函式
//目的：檢查兩種方向是否都可以工作
//回傳0時表示完成且無錯誤
//回傳1時表示進行中
//回傳2時表示有錯誤且已停止
extern unsigned char DEV_Power(
  AXIS_REF * axis, 
  BOOL enable, 
  BOOL enablePositive, 
  BOOL enableNegative
);


//物件更新器
void MC_Power_updater(MC_Power_T *obj){
  //handle input
  unsigned char res = DEV_Power(obj->Axis, *(obj->Enable), *(obj->EnablePositive), *(obj->EnableNegative));
  switch (res){
    case(0):{
      //完成
      obj->Status = TRUE;
      obj->Valid = TRUE;
      obj->Error = FALSE;
      obj->ErrorID = WORD_NO_ERROR;
      //由Disabled 轉移到Standstill   >>> Note5 <<<
      if(obj->Axis->stat.FA_DISABLED == TRUE){
        obj->Axis->setStat(obj->Axis,FA_STANDSTILL);
      }
      break;
    }
    case(1):{
      //進行中
      obj->Status = FALSE;
      obj->Valid = FALSE;
      obj->Error = FALSE;
      obj->ErrorID = WORD_NO_ERROR;
      break;
    }
    case(2):{
      //錯誤
      obj->Status = TRUE;
      obj->Valid = FALSE;
      obj->Error = TRUE;
      obj->ErrorID = WORD_UNDEFINED;
      break;
    }
  }
  //Power enabled 由高轉低時，除了ErrorStop外的其他狀態全都轉移到Disabled
  //正在跑的FB要變成 CommandAborted                     >>> Note2 <<<
  if ( *(obj->Enable) == FALSE && obj->prevEnable == TRUE){
    if(obj->Axis->stat.FA_ERRORSTOP == FALSE){
      obj->Axis->setStat(obj->Axis,FA_DISABLED);
      obj->Axis->CurrentFB = (void *)0;
    }
  }
  (*obj).prevEnable = *(obj->Enable);
  //handle output
}
