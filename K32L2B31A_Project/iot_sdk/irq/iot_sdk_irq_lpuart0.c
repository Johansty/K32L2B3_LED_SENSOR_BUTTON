/*
 * iot_sdk_irq_lpuart0.c
 *
 *  Created on: 18/09/2021
 *      Author: Johan T
 */

#include "iot_sdk_irq_lpuart0.h"

//#include "pin_mux.h"
//#include "clock_config.h"
//#include "board.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PushByteDesdeBuffer(uint8_t nuevo_byte);
/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint8_t rx_ring_buffer[RX_RING_BUFFER_SIZE];
volatile uint32_t rxIndex_productor = 0;
volatile uint32_t rx_index_consumidor = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/



void LPUART0_SERIAL_RX_TX_IRQHANDLER(void) {
  uint32_t intStatus;
  uint8_t nuevo_dato_uart;
  intStatus = LPUART_GetStatusFlags(LPUART0_PERIPHERAL);

  if ((kLPUART_RxDataRegFullFlag)&intStatus)
     {

	  nuevo_dato_uart = LPUART_ReadByte(LPUART0);
	  PushByteDesdeBuffer(nuevo_dato_uart);


     }


  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


void PushByteDesdeBuffer(uint8_t nuevo_byte){

if (((rxIndex_productor + 1) % RX_RING_BUFFER_SIZE) != rx_index_consumidor)
        {
	        rx_ring_buffer[rxIndex_productor] = nuevo_byte;
            rxIndex_productor++;
            rxIndex_productor %= RX_RING_BUFFER_SIZE;
        }
  }


uint8_t LeerByteDesdeBuffer(){

	uint8_t byte_a_retornar;
	            if (rxIndex_productor != rx_index_consumidor)
	            {
	               byte_a_retornar=rx_ring_buffer[rx_index_consumidor];
	               rx_index_consumidor++;
	               rx_index_consumidor %= RX_RING_BUFFER_SIZE;
	            }

	            return(byte_a_retornar);
  }





int32_t uart0CuantosDatosHayEnBuffer(void) {
	return ((int32_t) (rxIndex_productor - rx_index_consumidor));
}
