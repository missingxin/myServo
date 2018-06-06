/*******************************************************************************
* File Name          : PLCLogic.h
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : 建立可物件化的PLCLogic Function Block Diagram (FBD) 
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCLogic Function Block Diagram (FBD) 
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#ifndef __PLCLOGIC_H__
#define __PLCLOGIC_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

#define fbPagePtr(pool,id) (*(pool+id))
#define fbPage(pool,id) (*(*(pool+id)))

#define FB_ADD_AND_PAGE(a,b)  _FB_ADD_IIO_PAGE(a,b,LOGIC_AND_updater,bt_Logic_And)
#define FB_ADD_OR_PAGE(a,b)   _FB_ADD_IIO_PAGE(a,b,LOGIC_OR_updater,bt_Logic_Or)
#define FB_ADD_NAND_PAGE(a,b) _FB_ADD_IIO_PAGE(a,b,LOGIC_NAND_updater,bt_Logic_Nand)
#define FB_ADD_NOR_PAGE(a,b)  _FB_ADD_IIO_PAGE(a,b,LOGIC_NOR_updater,bt_Logic_Nor)
#define FB_ADD_XOR_PAGE(a,b)  _FB_ADD_IIO_PAGE(a,b,LOGIC_XOR_updater,bt_Logic_Xor)
#define FB_ADD_XNOR_PAGE(a,b) _FB_ADD_IIO_PAGE(a,b,LOGIC_XNOR_updater,bt_Logic_Xnor)

typedef enum {
  bt_Input_Bool = 0,
  bt_Output_Bool,
  bt_Logic_Not,
  bt_Logic_And,
  bt_Logic_Or,
  bt_Logic_Nand,
  bt_Logic_Nor,
  bt_Logic_Xor,
  bt_Logic_Xnor,
  bt_Input_Real,
  bt_Output_Real,
  bt_Input_Axis,  //取得實體Axis
  bt_MC_Power,
  bt_MC_Home,
  bt_MC_Stop,
  bt_MC_Halt,
  bt_MC_MoveAbsolute,
}Block_Type_t;


//所有元件的父元件...結構上
typedef struct {
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
}FUNCTION_BLOCK_t;


//  input FB 不會有這張表，它是直接Assign的
typedef struct{
  char ourInPinIDX;
  char theirFBIDX;
  char theirOutPinIDX;
}FB_INPUT_LINK_t;

typedef struct {
  FUNCTION_BLOCK_t* obj;
  Block_Type_t type;
  FB_INPUT_LINK_t *inLinkTable;
  char inLinkTableLength;
}FUNCTION_BLOCK_PAGE_t;

typedef struct {
  FUNCTION_BLOCK_PAGE_t** ipool;
  FUNCTION_BLOCK_PAGE_t** fbpool;
  FUNCTION_BLOCK_PAGE_t** opool;
  unsigned char ipoolCnt;
  unsigned char fbpoolCnt;
  unsigned char opoolCnt;
}FUNCTION_BLOCK_POOL_t;

//輸入FB, 接受外部值, 更新時bypass, BOOL
typedef struct {
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  void (*assign)(void* obj, BOOL val);
  BOOL IN;
  BOOL OUT;
}INPUT_BOOL_t;

//1進1出的FB結構
typedef struct FBD_IO_t{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  BOOL *IN;
  BOOL OUT;
}FBD_IO_t;
typedef FBD_IO_t FBD_NOT_t;
typedef FBD_IO_t OUTPUT_BOOL_t;

//2進1出的FB結構
typedef struct FBD_IIO_t{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  BOOL *IN1;
  BOOL *IN2;
  BOOL OUT;
}FBD_IIO_t;

typedef FBD_IIO_t FBD_AND_t;
typedef FBD_IIO_t FBD_OR_t;
typedef FBD_IIO_t FBD_NAND_t;
typedef FBD_IIO_t FBD_NOR_t;
typedef FBD_IIO_t FBD_XOR_t;
typedef FBD_IIO_t FBD_XNOR_t;


//輸入FB, 接受外部值, 更新時bypass, REAL, 因目前只會在這裡使用，所以做在這裡
typedef struct{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  void (*assign)(void* obj, REAL val);
  REAL IN;
  REAL OUT;
}INPUT_REAL_t;


//1進1出real的FB結構
typedef struct{
  void (*updater)(void* obj);
  void ***inList;
  void **outList;
  unsigned char inNumber;
  unsigned char outNumber;
  REAL *IN;
  REAL OUT;
}OUTPUT_REAL_t;


void INPUT_BOOL_assignor(void* obj, BOOL val);
void INPUT_BOOL_updater(void* obj);
void OUTPUT_BOOL_updater(void* obj);
void LOGIC_NOT_updater(void *obj);
void LOGIC_AND_updater(void *obj);
void LOGIC_OR_updater(void *obj);
void LOGIC_NAND_updater(void *obj);
void LOGIC_NOR_updater(void *obj);
void LOGIC_XOR_updater(void *obj);
void LOGIC_XNOR_updater(void *obj);

void FB_ADD_INPUT_BOOL_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount,
  FUNCTION_BLOCK_PAGE_t ** ipool, unsigned char *ipoolCount);
void FB_ADD_OUTPUT_BOOL_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount,
  FUNCTION_BLOCK_PAGE_t ** opool, unsigned char *opoolCount);
void FB_ADD_NOT_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);
// void FB_ADD_AND_PAGE( FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);
// void FB_ADD_OR_PAGE( FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);
// void FB_ADD_NAND_PAGE( FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);
// void FB_ADD_NOR_PAGE( FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);
// void FB_ADD_XOR_PAGE( FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);
// void FB_ADD_XNOR_PAGE( FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char *fpoolCount);

void setLinkTable(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char targetFBID, FB_INPUT_LINK_t* linkTable, unsigned char linkTableLength);
FUNCTION_BLOCK_POOL_t* setLinkTable2(unsigned char *rule);
void linkLinkTable(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char poolSize);
void updateFBS(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char poolSize);
void setInputBool(FUNCTION_BLOCK_PAGE_t ** inputPool, unsigned char idx, BOOL val);

void dump_PAGE(FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char count);
void dump_UPDATER();

void dump_INPUT(FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char count);
void dump_OUTPUT(FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char count);
void dump_IO(FUNCTION_BLOCK_PAGE_t ** ipool, unsigned char icount, FUNCTION_BLOCK_PAGE_t ** opool, unsigned char ocount);
FUNCTION_BLOCK_PAGE_t * createPage(FUNCTION_BLOCK_t* fbobj,Block_Type_t type);

void _FB_ADD_IIO_PAGE( 
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char * fpoolCount,
  void (*updater)(void *iobj),
  unsigned char type);


#endif //__PLCLOGIC_H__