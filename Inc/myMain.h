
/**
  ******************************************************************************
  * File Name          : myMain.h
  * Description        : Demonstration of myServo Middleware.
  ******************************************************************************
  ** 說明：在STM32F03C8T6(Bluepill board)上使用 myServo Middleware 作為展示範例
  *  使用的 servo 參數為：
  *     period：20ms
  *     duty cycle: 0.5ms ~ 2ms
  *     角度：正負90度
  ******************************************************************************
*/
#ifndef __MYMAIN_H__
#define __MYMAIN_H__
void myMain();
void servoUpdate();
#endif