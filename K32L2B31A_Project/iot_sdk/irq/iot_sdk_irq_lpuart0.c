/*
 * iot_sdk_irq_lpuart0.c
 *
 *  Created on: 18/09/2021
 *      Author: Johan T
 */

#include "iot_sdk_irq_lpuart0.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPUART            LPUART0
#define DEMO_LPUART_CLKSRC     BOARD_DEBUG_UART_CLKSRC
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetFreq(BOARD_DEBUG_UART_CLKSRC)
#define DEMO_LPUART_IRQn       LPUART0_IRQn
#define DEMO_LPUART_IRQHandler LPUART0_IRQHandler

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PushByteDesdeBuffer(uint8_t nuevo_byte);
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "Lpuart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
volatile uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */
volatile uint32_t tmprxIndex = 0;
volatile uint32_t tmptxIndex = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_LPUART_IRQHandler(void)
{
	uint32_t intStatus;
	uint8_t nuevo_dato_uart;

	intStatus = LPUART_GetStatusFlags(LPUART0);

    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&intStatus)
    {
    	nuevo_dato_uart = LPUART_ReadByte(LPUART0);
    	LeerByteDesdeBuffer(nuevo_dato_uart);

    }
#if defined_CORTEX_M && (_CORTEX_M ==4U)
    _DSB();
#endif
}



void PushByteDesdeBuffer(uint8_t nuevo_byte){

if (((tmprxIndex + 1) % DEMO_RING_BUFFER_SIZE) != tmptxIndex)
        {
            demoRingBuffer[rxIndex] = nuevo_byte;
            rxIndex++;
            rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
  }


uint8_t LeerByteDesdeBuffer(){

	uint8_t byte_a_retornar;
	            if (tmprxIndex != tmptxIndex)
	            {
	               byte_a_retornar=demoRingBuffer[tmptxIndex];
	                txIndex++;
	                txIndex %= DEMO_RING_BUFFER_SIZE;
	            }

	            return(byte_a_retornar);
  }





uint16_t uart0CuantosDatosHayEnBuffer(void) {
	return ((uint16_t) (rxIndex - txIndex));
}
