/*! @file : K32L2B31A_Project.c
 * @author  Ernesto Andres Rincon Cruz
 * @version 0.0.000
 * @date    23/08/2021
 * @brief   Funcion principal main
 * @details
 *			v0.0.000	Proyecto base creado usando MCUXpresso
 *
 *
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
#include <limits.h>
#include <float.h>

#include <iot_sdk_peripherals_leds.h>
#include <iot_sdk_peripherals_light.h>
#include "iot_sdk_peripherals_buttons.h"
#include "iot_sdk_peripheral_temperature.h"

#include "iot_sdk_ irq_lptimer0.h"
#include "iot_sdk_irq_lpuart0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
enum _ec25_lista_comandos_at {
	kAT = 0,
	kATI,
	kAT_CPIN,
	kAT_QCSQ,
	kAT_CREG,
	kAT_CGREG,
	kAT_CEREG,
	kAT_CFUN_0,
	kAT_CFUN_1,
	kAT_CSQ,
	kesperando_respuesta,
};

enum lista_comandos_LEDS {
	kLED_VERDE_ON = 0,
	kLED_VERDE_OFF,
	kLED_ROJO_ON,
	kLED_ROJO_OFF,
	kSENSOR_LUX,
	kSENSOR_TEMP,
};

#define COMANDOS_DISPONIBLES_LEDS 2
#define COMANDOS_DISPONIBLES_AT 9
#define BYTES_EN_BUFFER	100


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/
const char *comandos_at[] = {
    		    "AT",			//comprueba disponibilidad de dispositivo
    			"ATI",			//consulta información del modem
    			"AT+CPIN?",		//consulta estado de la simcard
    			"AT+QCSQ",
    			"AT+CREG?", //consulta estado de la red celular y tecnología usada en red celular
    			"AT+CGREG?",//consulta estado de la red celular
    			"AT+CEREG?",
    			"AT+CFUN=0",
    			"AT+CFUN=1",
    			"AT+CSQ", //consulta calidad de la señal RSSI
    	};

    //Lista de respuestas a cada comando AT
    const char  *repuestas_at[]={
    		        "OK",		//AT
    				"EC25",		//ATI
    				"READY",	//AT+CPIN?
    				"\"LTE\"",
    				"0,1",		//AT+CREG? = GSM,REGISTERED
    				"0,1",
    				"0,1",
    				"pdpdeact",
    				"OK",
    				"+CSQ:",		//AT+CSQ
    };

    const char  *repuestas_LEDS[]={
       		        "LED VERDE ON",		//AT
       				"LED VERDE OFF",		//ATI
       				"LED ROJO ON",	//AT+CPIN?
       				"LED ROJO OFF",

       };


    uint8_t buffer_rx[BYTES_EN_BUFFER];		//almacena las datos que provienen del modem
    uint8_t index_buffer_rx = 0;				//apuntador de buffer de datos


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/

    void BorrarBufferRX(void){
        	uint8_t i;

        	//LLenar de ceros buffer que va a recibir datos provenientes del modem
        	for(i=0;i<BYTES_EN_BUFFER;i++){
        		buffer_rx[i]=0;
        	}

        	//borra apuntador de posicion donde se va a almacenar el proximo dato
        	//Reset al apuntador
        	index_buffer_rx=0;
        }


 int main(void) {
	float light_value;
	float temperature_value;
	bool estado1;
	bool estado2;
	int j=0;
	uint8_t nuevo_byte_lpuart0;


    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* Activa LPTMR0 para que iniciar contador y posterior IRQ cada 1 segundo*/
    LPTMR_StartTimer(LPTMR0);


    while(1) {


    		switch(j){

    		    case kAT:
    		    	printf("%s\r\n",comandos_at[kAT]);
    		    	j=kesperando_respuesta;

    		    break;

    		    case kATI:
    		        printf("%s\r\n",comandos_at[kATI]);
    		        j=kesperando_respuesta;

    		    break;

    		    case kAT_CPIN:
    		        printf("%s\r\n",comandos_at[kAT_CPIN]);
    		        j=kesperando_respuesta;

    		    break;

    		    case kAT_QCSQ:
    		         printf("%s\r\n",comandos_at[kAT_QCSQ]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kAT_CREG:
    		         printf("%s\r\n",comandos_at[kAT_CREG]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kAT_CGREG:
    		         printf("%s\r\n",comandos_at[kAT_CGREG]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kAT_CEREG:
    		         printf("%s\r\n",comandos_at[kAT_CEREG]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kAT_CFUN_0:
    		         printf("%s\r\n",comandos_at[kAT_CFUN_0]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kAT_CFUN_1:
    		         printf("%s\r\n",comandos_at[kAT_CFUN_1]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kAT_CSQ:
    		         printf("%s\r\n",comandos_at[kAT_CSQ]);
    		         j=kesperando_respuesta;

    		    break;

    		    case kesperando_respuesta:

    		    	if (uart0CuantosDatosHayEnBuffer() !=0) {
    		    	    		nuevo_byte_lpuart0 = LeerByteDesdeBuffer();

    		    	    					//almacena dato en buffer rx
    		    	    					buffer_rx[index_buffer_rx] = nuevo_byte_lpuart0;
    		    	    					//incrementa apuntador de datos en buffer
    		    	    					index_buffer_rx++;


    		    	    					char *puntero_ok;
    		    	    					char *puntero_ok2;

    		    	    					if(nuevo_byte_lpuart0 == '\n'){

    		    	    						for(int i=0; i<COMANDOS_DISPONIBLES_AT; i++){

    		    	    						puntero_ok = (char*) (strstr((char*) (&buffer_rx[0]),(char*) (repuestas_at[i])));
    		    	    						puntero_ok2 = (char*) (strstr((char*) (&buffer_rx[0]),(char*) (repuestas_LEDS[i])));

    		    	    						   if(puntero_ok!=0){
    		    	    						    	switch(i){


    		    	    								   case kAT:

    		    	    									   printf("RESPUESTA ENCONTRADA\r\n");

    		    	    									   j=kATI;

    		    	    						    	   break;

    		    	    								   case kATI:

    		    	    								        printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								        j=kAT_CPIN;

    		    	    								   break;

    		    	    								   case kAT_CPIN:

    		    	    								        printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								        j=kAT_QCSQ;

    		    	    								   break;

    		    	    								   case kAT_QCSQ:

    		    	    								         printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								       	j=kAT_CREG;

    		    	    								   break;

    		    	    								   case kAT_CREG:

    		    	    								         printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								         j=kAT_CGREG;

    		    	    								    break;

    		    	    								   case kAT_CGREG:

    		    	    								         printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								        j=kAT_CEREG;

    		    	    								   break;

    		    	    								   case kAT_CEREG:

    		    	    								         printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								        j=kAT_CFUN_0;

    		    	    								   break;

    		    	    								   case kAT_CFUN_0:

    		    	    								         printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								        j=kAT_CFUN_1;

    		    	    								   break;

    		    	    								   case kAT_CFUN_1:

    		    	    								       	 printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								         j=kAT_CFUN_1;

    		    	    								    break;

    		    	    								   case kAT_CSQ:

    		    	    								       	printf("RESPUESTA ENCONTRADA\r\n");

    		    	    								        j=kAT_CFUN_1;

    		    	    								   break;



    		    	    						   }
    		    	    						}

    		    	    						   if(puntero_ok2!=0){

    		    	    							  switch(i){


    		    	    						       	case kLED_VERDE_ON:

    		    	    						       		GPIO_PinWrite(GPIOD,5,0);

    		    	    						        break;

    		    	    						        case kLED_VERDE_OFF:

    		    	    						        	GPIO_PinWrite(GPIOD,5,1);

    		    	    						        break;

    		    	    						        case kLED_ROJO_ON:

    		    	    						        	GPIO_PinWrite(GPIOE,31U,0);

    		    	    						       	break;

    		    	    						        case kLED_ROJO_OFF:

    		    	    						        	GPIO_PinWrite(GPIOE,31U,1);

    		    	    						        break;

    		    	    						       case kSENSOR_LUX:

    		    	    						    	   light_value=getLightADC();
    		    	    						    	   printf("Light (LUX): %d\r\n", light_value);
    		    	    						    	   printf("\r\n");

    		    	    						       break;

    		    	    						       case kSENSOR_TEMP:

    		    	    						    	   temperature_value=getTemperatureValue();
    		    	    						    	   printf("Temperatura: %f\r\n", temperature_value);
    		    	    						    	   printf("\r\n");


    		    	    						       break;



    		    	    						     }
    		    	    						    }

    		    	    						}

    		    	    		BorrarBufferRX();

    		    	    		}
    		    	           }


    		    break;

    		 }








    	if(lptmr0_ticks!=0){
    		lptmr0_ticks=0;
    		 estado1 = leerBoton1();
    		 estado2 = leerBoton2();
    		 if (estado1!=true){

    			 printf("boton1\r\n");
    			  printf("\r\n");
    		 }

    		 if (estado2!=true){

    			 printf("boton2\r\n");
    			  printf("\r\n");
    		 }

    	}

    }
    return 0 ;
}
