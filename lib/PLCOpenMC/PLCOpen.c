/******************************************************************************
* File Name          : PLCOpen.c
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : 建立可物件化的PLCOpen Middleware
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCOpen Middleware
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "PLCOpen.h"

// ############################### Object Way ###############################


//######### 指令傳入參數 ####################
void INPUT_REAL_assignor(void* iobj, REAL val)
{
  INPUT_REAL_t* obj;
  obj = (INPUT_REAL_t*) iobj;
  (*obj).IN = val;
}

//######### 輸入輸出及狀態管理器 ####################
void INPUT_REAL_updater(void* iobj){
  INPUT_REAL_t* obj;
  obj = (INPUT_REAL_t*)iobj;
  (*obj).OUT = obj->IN;
}

//######### 輸入輸出及狀態管理器 ####################
void OUTPUT_REAL_updater(void *iobj)
{
  OUTPUT_REAL_t* obj; 
  obj = (OUTPUT_REAL_t*)iobj;
  (*obj).OUT = *(obj->IN);
}
// #################### 建立頁面共用的工作 ####################
void setInputReal(FUNCTION_BLOCK_PAGE_t ** inputPool, unsigned char idx, REAL val)
{
  INPUT_REAL_t * thisInputFB = (INPUT_REAL_t *)fbPagePtr(inputPool,idx)->obj;
  thisInputFB->assign(thisInputFB,val);
}

void FB_ADD_INPUT_REAL_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount,
  FUNCTION_BLOCK_PAGE_t ** ipool,
  unsigned char *ipoolCount)
{
  INPUT_REAL_t *fbobj =  malloc(sizeof(INPUT_REAL_t));
  *fbobj = (INPUT_REAL_t){INPUT_REAL_updater, 0,malloc(sizeof(void **)*1),0,1,INPUT_REAL_assignor, 0, 0};
  ((*fbobj).outList)[0] = &(fbobj->OUT);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_Input_Real);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
  *(ipool+*ipoolCount) = newPage;
  *ipoolCount = *ipoolCount+1;
}

void FB_ADD_OUTPUT_REAL_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount,
  FUNCTION_BLOCK_PAGE_t ** opool,
  unsigned char *opoolCount)
{
  OUTPUT_REAL_t *fbobj =  malloc(sizeof(OUTPUT_REAL_t));
  *fbobj = (OUTPUT_REAL_t){OUTPUT_REAL_updater, malloc(sizeof(void ***)*1),malloc(sizeof(void **)*1),1,1,0, 0};
  ((*fbobj).inList)[0] = (void **)&(fbobj->IN);
  ((*fbobj).outList)[0] = &(fbobj->OUT);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_Output_Bool);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
  *(opool+*opoolCount) = newPage;
  *opoolCount = *opoolCount +1;
}
