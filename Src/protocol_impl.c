/******************************************************************************
* File Name          : Protocol_impl.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 06/3/2018
* Description        : 傳輸封裝協定與下一層之間的接合，在此的下層為UART
********************************************************************************
* 說明：此處應包含送出及送出所需要Ring Buffer，本次實驗來不及完成。
*******************************************************************************/
#include "FBDprotocol.h"
#include "PLCLogic.h"
#include "PLCOpen.h"
#include "usart.h"

unsigned char fbd_start = 0;

FUNCTION_BLOCK_POOL_t **currentPool;

void protocol_rx_byte_in(unsigned char ch)
{
  byte_in(ch);
}

void protocol_send_byte(unsigned char ch)
{
  HAL_UART_Transmit_IT(&huart1, &ch, 1);
  return;
}

void protocol_send_bytes(unsigned char *data, unsigned int length)
{
  HAL_UART_Transmit_IT(&huart1, data, length);
  return;
}

void cb_push_input(packet_t *packet)
{
  printf("cb_push_input in\r\n");
  if (packet->length == 0)
    return;

  unsigned char *indata;
  indata = packet->payload;


  // dummy data
  // float tmp = 9.12345;
  // unsigned char *tmpbyte;
  // tmpbyte = (unsigned char*)(void*)&tmp;
  // unsigned char indata[10] = {
  //     2,
  //     0, 0, 1,
  //     1, 1};
  // indata[6] = *(tmpbyte);
  // indata[7] = *(tmpbyte+1);
  // indata[8] = *(tmpbyte+2);
  // indata[9] = *(tmpbyte+3);

  unsigned char inputCnt = indata[0];
  unsigned int i;
  unsigned int offset = 1;
  unsigned int offset_check = offset;
  for (i = 0; i < inputCnt; i++)
  {
    unsigned char FBT = indata[offset_check + 1];
    switch (FBT)
    {
    case (bool_type_id):
    {
      if (indata[offset_check + 2] > 1)
      { //檢查不過，失敗
        return;
      }
      //input->data.value_bool = indata[offset + 2];
      offset_check += 3;
      break;
    }
    case (real_type_id):
    {
      offset_check += 6;
      break;
    }
    default:
      //檢查不過，失敗
      return;
    }
  }

  //apply
  for (i = 0; i < inputCnt; i++){
    unsigned char fb_idx = indata[offset];
    unsigned char fb_type = indata[offset + 1];
    //printf("fb_idx = %d, fb_type = %d\r\n",fb_idx,fb_type);
    switch (fb_type)
    {
      case (bool_type_id): {
        //printf("setInputBool((*currentPool)->ipool, %d, %d)\r\n", fb_idx, indata[offset + 2]);
        setInputBool((*currentPool)->ipool, fb_idx, indata[offset + 2]);
        offset += 3;
        break;
      }
      case (real_type_id): {
        union_value_t ivalue;
        memcpy(&(ivalue.value_bytes),&(indata[offset + 2]),4);
        //printf("setInputReal((*currentPool)->ipool, %d, %f)\r\n", fb_idx, ivalue.value_real);
        setInputReal((*currentPool)->ipool, fb_idx, ivalue.value_real);
        offset += 6;
        break;
      } 
      default:
        break;
    }
  }

//  printf("cb_push_input done\r\n");
}

void cb_pull_output(packet_t *packet)
{
  printf("cb_pull_output done\r\n");
}

void cb_set_graph(packet_t *packet){
  printf("cb_set_graph in\r\n");
  FUNCTION_BLOCK_POOL_t *POOL = setLinkTable2(packet->payload);
  *currentPool = POOL;
  printf("cb_set_graph done\r\n");
}

void cb_get_input_list(packet_t *packet)
{
  printf("cb_get_input_list done\r\n");
}

void cb_get_output_list(packet_t *packet)
{
  printf("cb_get_output_list done\r\n");
}

void cb_get_fb_list(packet_t *packet)
{
  printf("cb_get_fb_list done\r\n");
}
void cb_start_fbd(packet_t *packet)
{
  fbd_start = 1;
  printf("cb_start_fbd done\r\n");
}

void cb_stop_fbd(packet_t *packet)
{
  fbd_start = 0;
  printf("cb_stop_fbd done\r\n");
}
void protocol_init(FUNCTION_BLOCK_POOL_t **pool)
{
  currentPool = pool;
  comm_handle_packet_cb_t callbacks[cmd_type_end];
  callbacks[cmd_push_input] = cb_push_input;
  callbacks[cmd_pull_output] = cb_pull_output;
  callbacks[cmd_set_graph] = cb_set_graph;
  callbacks[cmd_get_input_list] = cb_get_input_list;
  callbacks[cmd_get_output_list] = cb_get_output_list;
  callbacks[cmd_get_fb_list] = cb_get_fb_list;
  callbacks[cmd_start_fbd] = cb_start_fbd;
  callbacks[cmd_stop_fbd] = cb_stop_fbd;
  protocol_set_callbacks(callbacks, protocol_send_bytes, protocol_send_byte);
}
 