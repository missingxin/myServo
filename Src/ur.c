#include "ur.h"
void print132(uint32_t val){
  uint8_t s[4];
  s[0]=val>>24;
  s[1]=val>>16;
  s[2]=val>>8;
  s[3]=val;
  HAL_UART_Transmit(&huart1,s,4,100);
}
void print116(uint16_t val){
  uint8_t s[2];
  s[0]=val>>8;
  s[1]=val;
  HAL_UART_Transmit(&huart1,s,2,100);
}
void print18(uint8_t val){
  HAL_UART_Transmit(&huart1,&val,1,100);
}

void print232(uint32_t val){
  uint8_t s[4];
  s[0]=val>>24;
  s[1]=val>>16;
  s[2]=val>>8;
  s[3]=val;
  HAL_UART_Transmit(&huart2,s,4,100);
}
void print216(uint16_t val){
  uint8_t s[2];
  s[0]=val>>8;
  s[1]=val;
  HAL_UART_Transmit(&huart2,s,2,100);
}
void print28(uint8_t val){
  HAL_UART_Transmit(&huart2,&val,1,100);
}