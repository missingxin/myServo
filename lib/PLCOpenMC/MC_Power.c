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
  //handle output
}
