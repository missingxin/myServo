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

//輸入FB, 接受外部值, 更新時bypass, BOOL
#if !defined(INPUT_SOURCE_BOOL_T)
#define INPUT_SOURCE_BOOL_T INPUT_SOURCE_BOOL_T
typedef struct INPUT_SOURCE_BOOL_T INPUT_SOURCE_BOOL_T;
struct INPUT_SOURCE_BOOL_T{
  BOOL IN;
  BOOL OUT;
  void (*updater)(INPUT_SOURCE_BOOL_T* obj);
  void (*assign)(INPUT_SOURCE_BOOL_T* obj, BOOL val);
};
#endif

//1進1出的FB結構
typedef struct FBD_IO_T FBD_IO_T;
struct FBD_IO_T{
  BOOL *IN;
  BOOL OUT;
  void (*updater)(FBD_IO_T* obj);
};

//2進1出的FB結構
typedef struct FBD_IIO_T FBD_IIO_T;
struct FBD_IIO_T{
  BOOL *IN1;
  BOOL *IN2;
  BOOL OUT;
  void (*updater)(FBD_IIO_T* obj);
};


void INPUT_SOURCE_BOOL_assign(INPUT_SOURCE_BOOL_T* obj, BOOL val);
void INPUT_SOURCE_BOOL_updater(INPUT_SOURCE_BOOL_T* obj);
void LOGIC_NOT_updater(FBD_IO_T *obj);
void LOGIC_AND_updater(FBD_IIO_T *obj);
void LOGIC_OR_updater(FBD_IIO_T *obj);
void LOGIC_NAND_updater(FBD_IIO_T *obj);
void LOGIC_NOR_updater(FBD_IIO_T *obj);
void LOGIC_XOR_updater(FBD_IIO_T *obj);

#endif //__PLCLOGIC_H__