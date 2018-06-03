/*******************************************************************************
* File Name          : MC_Power.c
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : MC_Power FB
********************************************************************************
* 說明：MC_Power FB
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "MC_Power.h"
// ############################### Object Way ###############################

//物件更新器
void MC_Power_updater(void *iobj){
  MC_Power_t* obj;
  obj = (MC_Power_t*) iobj;
  //handle input
  PowerControlResult_t pcResult = obj->Axis->callPower(obj->Axis, *(obj->Enable), *(obj->EnablePositive), *(obj->EnableNegative));
  switch (pcResult){
    case(PowerControl_Done):{
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
    case(PowerControl_Doing):{
      //進行中
      obj->Status = FALSE;
      obj->Valid = FALSE;
      obj->Error = FALSE;
      obj->ErrorID = WORD_NO_ERROR;
      break;
    }
    case(PowerControl_Error):{
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

void FB_ADD_MC_POWER_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount)
{
  MC_Power_t *fbobj =  malloc(sizeof(MC_Power_t));
  *fbobj = (MC_Power_t){MC_Power_updater, 
  malloc(sizeof(void ***)*4),  //C inList
  malloc(sizeof(void **)*5),   //C outList
  4, //C  inNumber
  5, //C  outNumber
  0, //IO axis 
  0, //I  enable
  0, //I  enable positive
  0, //I  enable negativek 
  0, //O  status
  0, //O  valid
  0, //O  error
  0, //O  errorID
  0  //C  prevEnable, internel use
  };
  ((*fbobj).inList)[0] = (void **)&(fbobj->Axis);
  ((*fbobj).inList)[1] = (void **)&(fbobj->Enable);
  ((*fbobj).inList)[2] = (void **)&(fbobj->EnablePositive);
  ((*fbobj).inList)[3] = (void **)&(fbobj->EnableNegative);
  ((*fbobj).outList)[0] = (fbobj->Axis);
  ((*fbobj).outList)[1] = &(fbobj->Status);
  ((*fbobj).outList)[2] = &(fbobj->Valid);
  ((*fbobj).outList)[3] = &(fbobj->Error);
  ((*fbobj).outList)[4] = &(fbobj->ErrorID);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_MC_Power);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}

