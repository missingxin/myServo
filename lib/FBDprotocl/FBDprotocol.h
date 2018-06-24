/*******************************************************************************
* File Name          : FBDprotocol.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 6/17/2018
* Description        : 暫用的 FBD protocol.
********************************************************************************
* 說明：此檔應該專注在解封包或包封包，不管其他事
* 1.指定各種封包處理的cb, 以及資料送出(TX)的cb
* 2.將讀入資料(RX)傳入，bytes_in 或 byte_in.
*******************************************************************************/
#ifndef __FBDPROTOCOL_H__
#define __FBDPROTOCOL_H__

#include "PLCLogic.h"

#define payloadBufferSize ((uint16_t)2048)
typedef enum
{
  bool_type_id = 0,
  real_type_id,
  variable_type_end
} variable_type_t;

typedef enum
{
  cmd_push_input = 0,
  cmd_pull_output,
  cmd_set_graph,
  cmd_get_input_list,
  cmd_get_output_list,
  cmd_get_fb_list,
  cmd_start_fbd,
  cmd_stop_fbd,
  cmd_type_end
} cmd_type_t;

typedef union{ //最長應限制在4byte
  BOOL value_bool;
  REAL value_real;
  unsigned char value_bytes[4];
}union_value_t;

typedef struct{
  unsigned char fb_idx;
  variable_type_t data_type;
  union_value_t data;
}fb_data_t;

typedef struct{
  unsigned char num;
  fb_data_t *data;
} fb_data_list_t;

typedef struct
{
  unsigned char sender;
  cmd_type_t type;
  unsigned int length;
  unsigned char locking;
  unsigned char payload[ payloadBufferSize ];
} packet_t;

typedef void (*comm_handle_packet_cb_t)(packet_t *packet); //cause they need sender id to send stuff back.
typedef void (*uart_bytes_write_cb_t)(unsigned char *data, unsigned int length); //cause they need sender id to send stuff back.
typedef void (*uart_byte_write_cb_t)(unsigned char data); //cause they need sender id to send stuff back.

// callback function for system to call it every 1ms.
void fbdp_tick_1ms_callback(void);

// API 01, set callback function， 中斷層級會和觸發時的 bytes_in 或 byte_in 相同，
// 所以當使用中斷（含DMA）執行 byte(s)_in 時，回呼裡必須保持極度簡短，防止佔用中斷。
void protocol_set_callbacks(comm_handle_packet_cb_t callbacks[cmd_type_end], uart_bytes_write_cb_t bytesWriter, uart_byte_write_cb_t byteWriter);

// API 02
void bytes_in(unsigned char *ch, unsigned int length);

// API 03
void byte_in(unsigned char ch);

void send_packet(unsigned char sender, cmd_type_t type, unsigned char *ch, unsigned int length);
#endif //__FBDPROTOCOL_H__