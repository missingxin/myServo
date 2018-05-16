/*******************************************************************************
* File Name          : PLCLogic.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/15/2018
* Description        : 建立可物件化的PLCLogic Function Block Diagram (FBD) 
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCLogic Function Block Diagram (FBD) 
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __PLCLOGIC_H__
#define __PLCLOGIC_H__
/* Private typedef -----------------------------------------------------------*/
#if !defined(BOOL)
#define BOOL unsigned char
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

#if !defined(REAL)
#define REAL float
#endif

//所有元件的父元件...結構上
typedef struct {
  void (*updater)(void* obj);
}FUNCTION_BLOCK_T;


//  input FB 不會有這張表，它是直接Assign的
typedef struct{
  char ourInPinIDX;
  char theirFBIDX;
  char theirOutPinIDX;
}FB_INPUT_LINK_T;

typedef struct {
  FUNCTION_BLOCK_T* obj;
  unsigned char type;
  unsigned char priority;
  char tag[16]; //for debug
  unsigned char numOfInLinks;
  FB_INPUT_LINK_T* inLinkList;
}FUNCTION_BLOCK_PAGE_T;






//輸入FB, 接受外部值, 更新時bypass, BOOL
typedef struct INPUT_SOURCE_BOOL_T INPUT_SOURCE_BOOL_T;
struct INPUT_SOURCE_BOOL_T{
  void (*updater)(void* obj);
  void (*assign)(void* obj, BOOL val);
  BOOL IN;
  BOOL OUT;
};

//1進1出的FB結構
typedef struct FBD_IO_T FBD_IO_T;
struct FBD_IO_T{
  void (*updater)(void* obj);
  BOOL *IN;
  BOOL OUT;
};

//2進1出的FB結構
typedef struct FBD_IIO_T FBD_IIO_T;
struct FBD_IIO_T{
  void (*updater)(void* obj);
  BOOL *IN1;
  BOOL *IN2;
  BOOL OUT;
};


void INPUT_SOURCE_BOOL_assign(void* obj, BOOL val);
void INPUT_SOURCE_BOOL_updater(void* obj);
void LOGIC_NOT_updater(void *obj);
void LOGIC_AND_updater(void *obj);
void LOGIC_OR_updater(void *obj);
void LOGIC_NAND_updater(void *obj);
void LOGIC_NOR_updater(void *obj);
void LOGIC_XOR_updater(void *obj);
void LOGIC_XNOR_updater(void *obj);

#endif //__PLCLOGIC_H__