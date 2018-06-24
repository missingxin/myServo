/*******************************************************************************
* File Name          : FBDprotocol.c
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 6/17/2018
* Description        : 暫用的 FBD protocol.
********************************************************************************
* 說明：此檔應該專注在解封包或包封包，不管其他事
* 1.指定各種封包處理的cb, 以及資料送出(TX)的cb
* 2.將讀入資料(RX)傳入，bytes_in 或 byte_in.
*******************************************************************************/

#include "FBDprotocol.h"
#include "gpio.h"
#include "usart.h"
unsigned char my_id = 2;

#define protocol_header_char 0x55
#define protocol_footer_char 0xAA

enum
{
  header_byte = 0,
  target_id,
  packet_type,
  sender_id,
  length, //0-127 byte時只佔1byte, 128～65535 byte時佔2byte
  payload,
  checksum,
  footer_byte
};

comm_handle_packet_cb_t cbPool[cmd_type_end] = {0};
uart_bytes_write_cb_t urWriteBytes = 0;
uart_byte_write_cb_t urWriteByte = 0;

unsigned char fbdp_timeout;

unsigned char addsum_8bit = 0;
void addsum_8bit_clean()
{
  addsum_8bit = 0;
}
void addsum_8bit_push(unsigned char ch)
{
  addsum_8bit+=ch;
}
unsigned char addsum_8bit_calc()
{
  return addsum_8bit;
}

void fbdp_tick_1ms_callback(void)
{
  if (fbdp_timeout)
  {
    fbdp_timeout--;
  }
}
void defaultByteWriter(unsigned char data)
{
  if (urWriteBytes == 0) return;
  urWriteBytes(&data,1);
}
void defaultBytesWriter(unsigned char *data, unsigned int length){
  if(urWriteByte == 0) return;
  unsigned int i;
  for(i = 0;i<length;i++){
    urWriteByte(data[i]);
  }
}

void protocol_set_callbacks(comm_handle_packet_cb_t callbacks[cmd_type_end], uart_bytes_write_cb_t bytesWriter, uart_byte_write_cb_t byteWriter)
{
  //封包處理用callback
  memcpy(cbPool,callbacks,sizeof(cbPool));

  //ur回傳 cb, 至少指定其中一個才能用
  urWriteByte = byteWriter;
  urWriteBytes = bytesWriter;
  if (urWriteBytes == 0 && urWriteByte != 0)
  {
    urWriteBytes = defaultBytesWriter;
  }else if (urWriteByte == 0 && urWriteBytes != 0)
  {
    urWriteByte = defaultByteWriter;
  }
}

// API 02
void bytes_in(unsigned char *ch, unsigned int length)
{
  unsigned int i;
  for(i=0;i<length;i++){
    byte_in(ch[i]);
  }
}

// API 03
void byte_in(unsigned char ch)
{
  static unsigned char stage_idx;
  static unsigned int payload_idx;
  static unsigned char length_idx;
  static packet_t packet;
  if (fbdp_timeout == 0)
  {
    stage_idx = 0;
  }
  fbdp_timeout = 10; //重置逾時倒數，10ms 沒有income則視為逾時
  switch (stage_idx)
  {
  case (header_byte):
  {
    if (ch == protocol_header_char){
      stage_idx++;
      addsum_8bit_clean();
    }
    break;
  }
  case (target_id):
  {
    if (ch == my_id){
      stage_idx++;
      addsum_8bit_push(ch);
    }else{
      fbdp_timeout = 0;
    }
    break;
  }
  case (packet_type):
  {
    if(ch >= cmd_type_end){
      fbdp_timeout = 0;
      break;
    }
    packet.type = ch;
    addsum_8bit_push(ch);
    stage_idx++;
    break;
  }
  case (sender_id):
  {
    packet.sender = ch;
    addsum_8bit_push(ch);
    stage_idx++;
    packet.length = 0;
    length_idx = 0;
    break;
  }
  case (length): //0-127 byte時只佔1byte, 128～65535 byte時佔2byte
  {
    packet.length = packet.length | ( ((unsigned int)ch) << (7 * length_idx++));
    addsum_8bit_push(ch);
    if (ch <= 0x80)
    {
      unsigned char len = packet.length;
      HAL_UART_Transmit(&huart2, &len, 1, 100);
      stage_idx++;
      if(packet.length==0){
        stage_idx++;
      }
      payload_idx = 0;
      break;
    }
    if (packet.length > payloadBufferSize ){ //長度超過buffer大小, 重新接收
      fbdp_timeout = 0;
    }
    if (length_idx >= 2)
    { //接收了最大byte數2bytes還不夠， 14 bit能顯示最多16383 byte;
      fbdp_timeout = 0;
      break;
    }
    break;
  } 
  case (payload):{
    packet.payload[payload_idx++] = ch;
    addsum_8bit_push(ch);
    if ( payload_idx == packet.length ) {
      stage_idx++;
    }
    break;
  }
  case (checksum):
  {
    unsigned char sum = addsum_8bit_calc();
    if (ch == sum)
    {
      stage_idx++;
    }else{
      HAL_UART_Transmit(&huart2, &sum, 1, 100);
      fbdp_timeout = 0;
    }
    break;
  }
  case(footer_byte):{
    fbdp_timeout = 0;
    if (ch == protocol_footer_char)
    {
      packet.locking = 1;
      if (cbPool[packet.type]){
        cbPool[packet.type](&packet);
      }
      packet.locking = 0;
    }
    break;
  }
  }
}
