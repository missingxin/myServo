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
// ############################### Object Way ###############################


//######### 指令傳入參數 ####################
void INPUT_SOURCE_BOOL_assign(INPUT_SOURCE_BOOL_T* obj, BOOL val){
  (*obj).IN = val;
}

//######### 輸入輸出及狀態管理器 ####################
void INPUT_SOURCE_BOOL_updater(INPUT_SOURCE_BOOL_T* obj){
  (*obj).OUT = obj->IN;
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NOT_updater(FBD_IO_T *obj){
  (*obj).OUT = !*(obj->IN);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_AND_updater(FBD_IIO_T *obj){
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) &&
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_OR_updater(FBD_IIO_T *obj){
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) ||
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}


//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NAND_updater(FBD_IIO_T *obj){
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) &&
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}


//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NOR_updater(FBD_IIO_T *obj){
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) ||
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_XOR_updater(FBD_IIO_T *obj){
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) !=
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}
