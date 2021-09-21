
/*
 * iot_sdk_irq_lpuart0.h
 *
 *  Created on: 18/09/2021
 *      Author: Johan T
 */

#ifndef IRQ_IOT_SDK_IRQ_LPUART0_H_
#define IRQ_IOT_SDK_IRQ_LPUART0_H_


#include "peripherals.h"



/*! @brief Ring buffer size (Unit: Byte). */
#define RX_RING_BUFFER_SIZE 100


void PushByteDesdeBuffer(uint8_t nuevo_byte);
uint8_t LeerByteDesdeBuffer();

int32_t uart0CuantosDatosHayEnBuffer(void);


#endif /* IRQ_IOT_SDK_IRQ_LPUART0_H_ */
