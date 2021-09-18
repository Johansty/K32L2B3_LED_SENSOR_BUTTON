/*
 * iot_sdk_irq_lpuart0.h
 *
 *  Created on: 18/09/2021
 *      Author: Johan T
 */

#ifndef IRQ_IOT_SDK_IRQ_LPUART0_H_
#define IRQ_IOT_SDK_IRQ_LPUART0_H_


#include "peripherals.h"



void PushByteDesdeBuffer(uint8_t nuevo_byte);
uint8_t LeerByteDesdeBuffer();

uint16_t uart0CuantosDatosHayEnBuffer(void);


#endif /* IRQ_IOT_SDK_IRQ_LPUART0_H_ */
