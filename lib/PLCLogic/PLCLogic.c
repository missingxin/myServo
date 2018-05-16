/*******************************************************************************
* File Name          : PLCLogic.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/15/2018
* Description        : 建立可物件化的PLCLogic Function Block Diagram (FBD)
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCLogic Function Block Diagram (FBD)
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "PLCLogic.h"
#include <stdio.h>
#include <string.h>
// ############################### Object Way ###############################


//######### 指令傳入參數 ####################
void INPUT_SOURCE_BOOL_assign(void* iobj, BOOL val){
  INPUT_SOURCE_BOOL_T* obj; obj = iobj;
  obj = iobj;
  (*obj).IN = val;
  printf("(%d)INPUT_SOURCE_BOOL_assign, assigning %d to IN, now IN = %d\r\n",(int)obj,val, (*obj).IN);
}

//######### 輸入輸出及狀態管理器 ####################
void INPUT_SOURCE_BOOL_updater(void* iobj){
  INPUT_SOURCE_BOOL_T* obj; obj = iobj;
  (*obj).OUT = obj->IN;
  printf("(%d)INPUT_SOURCE_BOOL_updater, move IN(%d) to OUT(%d,%d)\r\n",(int)obj,obj->IN, (*obj).OUT, (int)&((*obj).OUT));
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NOT_updater(void *iobj){
  FBD_IO_T* obj; obj = iobj;
  (*obj).OUT = !*(obj->IN);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_AND_updater(void *iobj){
  FBD_IIO_T* obj; obj = iobj;
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) &&
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
  printf("(%d)LOGIC_AND_updater, IN1(%d,%d) and IN2(%d,%d) = OUT(%d)\r\n",(int)obj,*(obj->IN1),(int)(obj->IN1),*(obj->IN2),(int)(obj->IN2),(*obj).OUT);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_OR_updater(void *iobj){
  FBD_IIO_T* obj; obj = iobj;
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) ||
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NAND_updater(void *iobj){
  FBD_IIO_T* obj; obj = iobj;
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) &&
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NOR_updater(void *iobj){
  FBD_IIO_T* obj; obj = iobj;
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) ||
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_XOR_updater(void *iobj){
  FBD_IIO_T* obj; obj = iobj;
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) !=
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_XNOR_updater(void *iobj){
  FBD_IIO_T* obj; obj = iobj;
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) !=
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}
