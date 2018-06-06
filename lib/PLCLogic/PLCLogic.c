/*******************************************************************************
* File Name          : PLCLogic.c
* Author             : Joseph Lin
* Version            : V0.0.2
* Date               : 06/3/2018
* Description        : 建立可物件化的PLCLogic Function Block Diagram (FBD)
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCLogic Function Block Diagram (FBD)
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "PLCLogic.h"
#include "PLCOpen.h"
// ############################### Object Way ###############################


//######### 指令傳入參數 ####################
void INPUT_BOOL_assignor(void* iobj, BOOL val)
{
  INPUT_BOOL_t* obj;
  obj = (INPUT_BOOL_t* )iobj;
  (*obj).IN = val;
}

//######### 輸入輸出及狀態管理器 ####################
void INPUT_BOOL_updater(void* iobj)
{
  INPUT_BOOL_t* obj;
  obj = (INPUT_BOOL_t*)iobj;
  (*obj).OUT = obj->IN;
}

//######### 輸入輸出及狀態管理器 ####################
void OUTPUT_BOOL_updater(void *iobj)
{
  FBD_IO_t* obj; obj = iobj;
  (*obj).OUT = *(obj->IN);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NOT_updater(void *iobj)
{
  FBD_IO_t* obj; obj = iobj;
  (*obj).OUT = !*(obj->IN);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_AND_updater(void *iobj)
{
  FBD_IIO_t* obj; obj = iobj;
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) &&
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_OR_updater(void *iobj)
{
  FBD_IIO_t* obj; obj = iobj;
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) ||
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NAND_updater(void *iobj)
{
  FBD_IIO_t* obj; obj = iobj;
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) &&
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_NOR_updater(void *iobj)
{
  FBD_IIO_t* obj; obj = iobj;
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) ||
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_XOR_updater(void *iobj)
{
  FBD_IIO_t* obj; obj = iobj;
  (*obj).OUT = (obj->IN1 != 0 && *(obj->IN1) == TRUE) !=
               (obj->IN2 != 0 && *(obj->IN2) == TRUE);
}

//######### 輸入輸出及狀態管理器 ####################
void LOGIC_XNOR_updater(void *iobj)
{
  FBD_IIO_t* obj; obj = iobj;
  (*obj).OUT = !((obj->IN1 != 0 && *(obj->IN1) == TRUE) !=
               (obj->IN2 != 0 && *(obj->IN2) == TRUE));
}

//######### 按登記表將連結實際連接 ####################
void linkLinkTable(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char poolSize)
{
  int fbid;
  for(fbid=0;fbid<poolSize;fbid++){
    FUNCTION_BLOCK_PAGE_t **thisPage = &(*(pool + fbid));
    int linkid;
    int cnt = (**thisPage).inLinkTableLength;
    for(linkid=0;linkid<cnt;linkid++){
      FB_INPUT_LINK_t *thisLink = ((**thisPage).inLinkTable + linkid);
      unsigned char ourIn    = thisLink->ourInPinIDX;
      unsigned char theirFB  = thisLink->theirFBIDX;
      unsigned char theirOut = thisLink->theirOutPinIDX;
      FUNCTION_BLOCK_PAGE_t **thatPage = &(*(pool + theirFB));
      printf("linking %d %d %d %d\r\n", fbid, ourIn, theirFB, theirOut);
      *((**thisPage).obj->inList[ourIn]) = (**thatPage).obj->outList[theirOut];
      printf("0x%lx, ", (unsigned long)&((**thisPage).obj->inList[ourIn]));
      printf("0x%lx, ", (unsigned long) ((**thisPage).obj->inList[ourIn]));
      printf("0x%lx, ", (unsigned long)*((**thisPage).obj->inList[ourIn]));
      printf("0x%lx, ", (unsigned long)&((**thatPage).obj->outList[theirOut]));
      printf("0x%lx,\r\n", (unsigned long)((**thatPage).obj->outList[theirOut]));
    }
  }
}

//######### 登記FB之間連結的狀態到登記表中 ####################
void setLinkTable(
  FUNCTION_BLOCK_PAGE_t ** pool,
  unsigned char targetFBID,
  FB_INPUT_LINK_t* linkTable,
  unsigned char linkTableLength)
{
  fbPage(pool,targetFBID).inLinkTableLength = linkTableLength;
  if (linkTableLength ==0){
    fbPage(pool,targetFBID).inLinkTable = (void*)0;
  }else{
    fbPage(pool,targetFBID).inLinkTable = malloc(sizeof(FB_INPUT_LINK_t)*linkTableLength);
    memcpy(fbPage(pool,targetFBID).inLinkTable,linkTable,sizeof(FB_INPUT_LINK_t)*linkTableLength);
  }
}

//######### 登記FB之間連結的狀態到登記表中 ####################
// todo:改為吃byte array
FUNCTION_BLOCK_POOL_t* setLinkTable2(unsigned char *rule){
  //1.準備好容器
  printf("Create pools . v2\r\n");
  FUNCTION_BLOCK_POOL_t* pool;
  pool = malloc(sizeof(FUNCTION_BLOCK_POOL_t));
  (*pool).ipoolCnt = *(rule+0);
  (*pool).fbpoolCnt = *(rule + 0) + *(rule + 1) + *(rule + 2);
  (*pool).opoolCnt = *(rule+2);
  (*pool).ipool = malloc(sizeof(FUNCTION_BLOCK_PAGE_t) * pool->ipoolCnt);
  (*pool).fbpool = malloc(sizeof(FUNCTION_BLOCK_PAGE_t) * pool->fbpoolCnt);
  (*pool).opool = malloc(sizeof(FUNCTION_BLOCK_PAGE_t) * pool->opoolCnt);

  //2.建立所有對應FB
  printf("Create FBs . v2\r\n");
  unsigned int cnt;
  unsigned char FBP_cnt = 0;
  unsigned char INPUT_cnt = 0;
  unsigned char OUTPUT_cnt = 0;
  unsigned int offset = 3;
  for (cnt = 0; cnt < pool->fbpoolCnt; cnt++)
  {
    switch(rule[offset]){
      case(bt_Input_Bool):{
        FB_ADD_INPUT_BOOL_PAGE(pool->fbpool,&FBP_cnt,pool->ipool,&INPUT_cnt);
        break;
      }
      case(bt_Output_Bool):{
        FB_ADD_OUTPUT_BOOL_PAGE(pool->fbpool,&FBP_cnt,pool->opool,&OUTPUT_cnt);
        break;
      }
      case(bt_Logic_Not):{
        FB_ADD_NOT_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Logic_And):{
        FB_ADD_AND_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Logic_Or):{
        FB_ADD_OR_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Logic_Nand):{
        FB_ADD_NAND_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Logic_Nor):{
        FB_ADD_NOR_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Logic_Xor):{
        FB_ADD_XOR_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Logic_Xnor):{
        FB_ADD_XNOR_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_Input_Real):{
        FB_ADD_INPUT_REAL_PAGE(pool->fbpool,&FBP_cnt,pool->ipool,&INPUT_cnt); 
        break;
      }
      case(bt_Output_Real):{
        FB_ADD_OUTPUT_REAL_PAGE(pool->fbpool,&FBP_cnt,pool->opool,&OUTPUT_cnt);
        break;
      }
      case(bt_Input_Axis):{
        FB_ADD_AXIS_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_MC_Power):{
        FB_ADD_MC_POWER_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_MC_Home):{
        FB_ADD_MC_HOME_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_MC_Stop):{
        FB_ADD_MC_STOP_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_MC_Halt):{
        FB_ADD_MC_HALT_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
      case(bt_MC_MoveAbsolute):{
        FB_ADD_MC_MOVEABSOLUTE_PAGE(pool->fbpool,&FBP_cnt);
        break;
      }
    }
    offset = offset + 2 + rule[offset+1]*3;
  }
  
  //3.建立連結表
  printf("Create LinkTable . v2\r\n");
  offset = 3;
  for (cnt = 0; cnt < pool->fbpoolCnt; cnt++)
  {
    unsigned char linkTableLength = rule[offset + 1];
    fbPage(pool->fbpool, cnt).inLinkTableLength = linkTableLength;
    if (linkTableLength == 0)
    {
      fbPage(pool->fbpool, cnt).inLinkTable = (void *)0;
    }
    else
    {
      fbPage(pool->fbpool, cnt).inLinkTable = malloc(sizeof(FB_INPUT_LINK_t) * linkTableLength);
      memcpy(fbPage(pool->fbpool, cnt).inLinkTable, (FB_INPUT_LINK_t *)&(rule[offset + 2]), sizeof(FB_INPUT_LINK_t) * linkTableLength);
      int i;
      printf(" FB(%d) : ", cnt);
      for (i = 0; i < linkTableLength; i++)
      {
        printf("{ %d, %d, %d } , ",
               fbPage(pool->fbpool, cnt).inLinkTable[i].ourInPinIDX,
               fbPage(pool->fbpool, cnt).inLinkTable[i].theirFBIDX,
               fbPage(pool->fbpool, cnt).inLinkTable[i].theirOutPinIDX);
      }
      printf("(cnt = %d)\r\n", linkTableLength);
    }
    offset = offset + 2 + rule[offset+1]*3;

    if (cnt == 3)
    {
      printf("FB(3).obj.outlist(1) = %lx\r\n",
             (unsigned long)fbPage(pool->fbpool, cnt).obj->outList[1]);
      printf("FB(3).obj.outlist(2) = %lx\r\n",
             (unsigned long)fbPage(pool->fbpool, cnt).obj->outList[2]);
    }
    if (cnt == 4)
    {
      printf("FB(4).obj.inlist(0) = %lx , %lx\r\n",
             (unsigned long)fbPage(pool->fbpool, cnt).obj->inList[0],
             (unsigned long)*(fbPage(pool->fbpool, cnt).obj->inList[0]));
      printf("FB(4).obj.inlist(1) = %lx , %lx\r\n",
             (unsigned long)fbPage(pool->fbpool, cnt).obj->inList[1],
             (unsigned long)*(fbPage(pool->fbpool, cnt).obj->inList[1]));
      printf("FB(4).obj.outlist(0) = %lx\r\n",
             (unsigned long)fbPage(pool->fbpool, cnt).obj->outList[0]);
    }
    if (cnt == 4)
    {
      printf("FB(5).obj.inlist(1) = %lx , %lx\r\n",
             (unsigned long)fbPage(pool->fbpool, cnt).obj->inList[1],
             (unsigned long)*(fbPage(pool->fbpool, cnt).obj->inList[1]));
    }
  }

  //4.連結
  printf("Link FBs according to linkTable . v2\r\n");
  linkLinkTable(pool->fbpool,pool->fbpoolCnt);
  return pool;
}

// #################### 更新池子裡的FB ####################
void updateFBS(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char poolSize)
{
  int fbid;
  for(fbid=0;fbid<poolSize;fbid++){
    if(*fbPagePtr(pool,fbid)->obj->updater){
      fbPagePtr(pool,fbid)->obj->updater(fbPagePtr(pool,fbid)->obj);
    }
  }
}

// #################### 建立頁面共用的工作 ####################
void setInputBool(FUNCTION_BLOCK_PAGE_t ** inputPool, unsigned char idx, BOOL val)
{
  INPUT_BOOL_t * thisInputFB = (INPUT_BOOL_t *)fbPagePtr(inputPool,idx)->obj;
  thisInputFB->assign(thisInputFB,val);
}

// #################### 建立頁面共用的工作 ####################
FUNCTION_BLOCK_PAGE_t * createPage(FUNCTION_BLOCK_t* fbobj,Block_Type_t type)
{
  FUNCTION_BLOCK_PAGE_t *page = malloc(sizeof(FUNCTION_BLOCK_PAGE_t));
  *page = (FUNCTION_BLOCK_PAGE_t){(FUNCTION_BLOCK_t*)fbobj,type,(FB_INPUT_LINK_t * )0,0};
  return page;
}

// #################### 建立包含FB及額外資訊的分頁 ####################
void FB_ADD_INPUT_BOOL_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount,
  FUNCTION_BLOCK_PAGE_t ** ipool,
  unsigned char *ipoolCount)
{
  printf("FB_ADD_INPUT_BOOL_PAGE\r\n");
  INPUT_BOOL_t *fbobj =  malloc(sizeof(INPUT_BOOL_t));
  *fbobj = (INPUT_BOOL_t){INPUT_BOOL_updater, 0,malloc(sizeof(void **)*1),0,1,INPUT_BOOL_assignor, 0, 0};
  ((*fbobj).outList)[0] = &(fbobj->OUT);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_Input_Bool);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
  *(ipool+*ipoolCount) = newPage;
  *ipoolCount = *ipoolCount+1;
}

// #################### 建立包含FB及額外資訊的分頁 ####################
void FB_ADD_OUTPUT_BOOL_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount,
  FUNCTION_BLOCK_PAGE_t ** opool,
  unsigned char *opoolCount)
{
  printf("FB_ADD_OUTPUT_BOOL_PAGE\r\n");
  OUTPUT_BOOL_t *fbobj =  malloc(sizeof(OUTPUT_BOOL_t));
  *fbobj = (OUTPUT_BOOL_t){OUTPUT_BOOL_updater, malloc(sizeof(void ***)*1),malloc(sizeof(void **)*1),1,1,0, 0};
  ((*fbobj).inList)[0] = (void **)&(fbobj->IN);
  ((*fbobj).outList)[0] = &(fbobj->OUT);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_Output_Bool);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
  *(opool+*opoolCount) = newPage;
  *opoolCount = *opoolCount +1;
}

// #################### 建立包含FB及額外資訊的分頁 ####################
void FB_ADD_NOT_PAGE(
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char *fpoolCount)
{
  FBD_IO_t *fbobj = malloc(sizeof(FBD_IO_t));
  *fbobj = (FBD_IO_t){LOGIC_NOT_updater, malloc(sizeof(void ***)*1),malloc(sizeof(void **)*1),1,1,0, 0};
  ((*fbobj).inList)[0] = (void **)&(fbobj->IN);
  ((*fbobj).outList)[0] = &(fbobj->OUT);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,bt_Logic_Not);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}

// #################### 建立包含FB及額外資訊的分頁 ####################
void _FB_ADD_IIO_PAGE( 
  FUNCTION_BLOCK_PAGE_t ** fpool,
  unsigned char * fpoolCount,
  void (*updater)(void *iobj),
  unsigned char type)
{
  printf("_FB_ADD_IIO_PAGE\r\n");
  FBD_IIO_t *fbobj = malloc(sizeof(FBD_IIO_t));
  *fbobj = (FBD_IIO_t){updater,malloc(sizeof(void ***)*2),malloc(sizeof(void **)*1),2,1,0,0,0};
  ((*fbobj).inList)[0] = (void **)&(fbobj->IN1);
  ((*fbobj).inList)[1] = (void **)&(fbobj->IN2);
  ((*fbobj).outList)[0] = &(fbobj->OUT);
  FUNCTION_BLOCK_PAGE_t* newPage = createPage((FUNCTION_BLOCK_t*)fbobj,type);
  *(fpool+*fpoolCount) = newPage;
  *fpoolCount = *fpoolCount +1;
}

void dump_UPDATER()
{
  printf("(0x%lx)INPUT_BOOL_assignor\r\n",(unsigned long)INPUT_BOOL_assignor);
  printf("(0x%lx)INPUT_BOOL_updater\r\n",(unsigned long)INPUT_BOOL_updater);
  printf("(0x%lx)OUTPUT_BOOL_updater\r\n",(unsigned long)OUTPUT_BOOL_updater);
  printf("(0x%lx)LOGIC_NOT_updater\r\n",(unsigned long)LOGIC_NOT_updater);
  printf("(0x%lx)LOGIC_AND_updater\r\n",(unsigned long)LOGIC_AND_updater);
  printf("(0x%lx)LOGIC_OR_updater\r\n",(unsigned long)LOGIC_OR_updater);
  printf("(0x%lx)LOGIC_NAND_updater\r\n",(unsigned long)LOGIC_NAND_updater);
  printf("(0x%lx)LOGIC_NOR_updater\r\n",(unsigned long)LOGIC_NOR_updater);
  printf("(0x%lx)LOGIC_XOR_updater\r\n",(unsigned long)LOGIC_XOR_updater);
  printf("(0x%lx)LOGIC_XNOR_updater\r\n",(unsigned long)LOGIC_XNOR_updater);
}


void dump_IO(FUNCTION_BLOCK_PAGE_t ** ipool, unsigned char icount, FUNCTION_BLOCK_PAGE_t ** opool, unsigned char ocount)
{
  unsigned char i;
  printf("\tIN : ");
  for(i=0;i<icount;i++){
    if(fbPage(ipool,i).type == bt_Input_Bool){
      INPUT_BOOL_t** objptr = (INPUT_BOOL_t**)&(fbPage(ipool,i).obj);
      printf("%d  ",(**objptr).IN);
    }
    if(fbPage(ipool,i).type == bt_Input_Real){
      INPUT_REAL_t** objptr = (INPUT_REAL_t**)&(fbPage(ipool,i).obj);
      printf("%f  ",(**objptr).IN);
    }
  }

  printf("  OUT : ");
  for(i=0;i<ocount;i++){
    if(fbPage(opool,i).type == bt_Output_Bool){
      OUTPUT_BOOL_t** objptr = (OUTPUT_BOOL_t**)&(fbPage(opool,i).obj);
      printf("%d  ",(**objptr).OUT);
    }
    if(fbPage(opool,i).type == bt_Output_Real){
      OUTPUT_REAL_t** objptr = (OUTPUT_REAL_t**)&(fbPage(opool,i).obj);
      printf("%f  ",(**objptr).OUT);
    }
  }
  printf("\r\n");
}


void dump_INPUT(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char count)
{
  printf("Dumping output:\r\n");
  unsigned char i;
  for(i=0;i<count;i++){
    if(fbPage(pool,i).type == bt_Input_Bool){
      INPUT_BOOL_t** objptr = (INPUT_BOOL_t**)&(fbPage(pool,i).obj);
      printf("\tIN[%d] : (BOOL)%d \r\n",i,(**objptr).IN);
    }
    if(fbPage(pool,i).type == bt_Input_Real){
      INPUT_REAL_t** objptr = (INPUT_REAL_t**)&(fbPage(pool,i).obj);
      printf("\tIN[%d] : (REAL)%f \r\n",i,(**objptr).IN);
    }
  }
  printf("\r\n");
}
void dump_OUTPUT(FUNCTION_BLOCK_PAGE_t ** pool, unsigned char count)
{
  printf("Dumping output:\r\n");
  unsigned char i;
  for(i=0;i<count;i++){
    if(fbPage(pool,i).type == bt_Output_Bool){
      OUTPUT_BOOL_t** objptr = (OUTPUT_BOOL_t**)&(fbPage(pool,i).obj);
      printf("\tOUT[%d] : (BOOL)%d \r\n",i,(**objptr).OUT);
    }
    if(fbPage(pool,i).type == bt_Output_Real){
      OUTPUT_REAL_t** objptr = (OUTPUT_REAL_t**)&(fbPage(pool,i).obj);
      printf("\tOUT[%d] : (REAL)%f \r\n",i,(**objptr).OUT);
    }
  }
  printf("\r\n");
}

void dump_PAGE(FUNCTION_BLOCK_PAGE_t ** fpool, unsigned char count)
{
  printf("Pool : 0x%lx\r\n",(unsigned long)fpool);
  unsigned char i;
  for(i=0;i<count;i++){
    FUNCTION_BLOCK_PAGE_t** thisPage = &(*(fpool+i));
    // printf("(0x%lx)Pool[%d] : 0x%lx\r\n\t(0x%lx)obj : \r\n\t\t(0x%lx)updater\r\n",(unsigned long)thisPage,i, (unsigned long)*thisPage, (unsigned long) (**thisPage).obj, (unsigned long) (**thisPage).obj->updater );
    unsigned char inidx;
    
    for(inidx = 0;inidx<(**thisPage).obj->outNumber;inidx++){
      void ** thisOut = &(*((**thisPage).obj->outList+inidx));
      unsigned char *a;
      a = *thisOut;
      printf("\t\t\t(0x%lx)outList[%d] : 0x%lx = %d\r\n",(unsigned long)thisOut,inidx,(unsigned long)*thisOut, (unsigned char)*a);
    }
    printf("\t\t(0x%lx)type : %d\r\n",(unsigned long)&((**thisPage).type),(**thisPage).type);
    if((**thisPage).type == bt_Input_Bool){
      //input
      INPUT_BOOL_t **objptr = (INPUT_BOOL_t **)&((**thisPage).obj);
      printf("\t\t(0x%lx)IN : %d\r\n",(unsigned long)&((**objptr).IN),(**objptr).IN);
      printf("\t\t(0x%lx)OUT : %d\r\n",(unsigned long)&((**objptr).OUT),(**objptr).OUT);
    }else if((**thisPage).type == bt_Output_Bool){
      //input
      OUTPUT_BOOL_t **objptr = (OUTPUT_BOOL_t **)&((**thisPage).obj);
      printf("\t\t(0x%lx)IN1 : 0x%lx = %d\r\n",(unsigned long)&((**objptr).IN),(unsigned long)(**objptr).IN, *((**objptr).IN));
      printf("\t\t(0x%lx)OUT : %d\r\n",(unsigned long)&((**objptr).OUT),(**objptr).OUT);
    }else if((**thisPage).type == bt_Logic_Not){
      //input
      FBD_NOT_t **objptr = (FBD_NOT_t **)&((**thisPage).obj);
      printf("\t\t(0x%lx)IN1 : 0x%lx = %d\r\n",(unsigned long)&((**objptr).IN),(unsigned long)(**objptr).IN, *((**objptr).IN));
      printf("\t\t(0x%lx)OUT : %d\r\n",(unsigned long)&((**objptr).OUT),(**objptr).OUT);
    }else if((**thisPage).type >= bt_Logic_And && (**thisPage).type <= bt_Logic_Xnor){
      //and
      FBD_AND_t **objptr = (FBD_AND_t **)&((**thisPage).obj);
      printf("\t\t(0x%lx)IN1 : 0x%lx = %d\r\n",(unsigned long)&((**objptr).IN1),(unsigned long)(**objptr).IN1, *((**objptr).IN1));
      printf("\t\t(0x%lx)IN2 : 0x%lx = %d\r\n",(unsigned long)&((**objptr).IN2),(unsigned long)(**objptr).IN2, *((**objptr).IN2));
      printf("\t\t(0x%lx)OUT : %d\r\n",(unsigned long)&((**objptr).OUT),(**objptr).OUT);
    }
  }
}
