/*
 * ADC.c
 *
 *  Created on: 02/01/2023
 *      Author: AdrianaMtzR
 */
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{   
//Experimento 2
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz con tres potenciometros variar el ciclo de trabajo
 * para controlar la posicion de tres servos sg90 o otros.
 *
 */

// HABILITO CANAL 1 (PE2) - SECUENCIADOR 0
// HABILITO CANAL 2 (PE1) - SECUENCIADOR 1
// HABILITO CANAL 8 (PE5) - SECUENCIADOR 2 

     //Pag 396 para inicializar el modulo 0 de reloj del adc RCGCADC
    SYSCTL->RCGCADC = (1<<0); 

    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<1);

    //Pag 760 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOE_AHB->DIR = (0<<1) | (0<<2) | (0<<5); //PE5 PE1 y PE2* 

    //(GPIOAFSEL) pag.770 Enable alternate función para que el modulo analógico tenga control de esos pines
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<2) | (1<<5); 

    //(GPIODEN) pag.781 desabilita el modo digital
    GPIOE_AHB-> DEN = (0<<1) | (0<<2)| (0<<5);

    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808 - establecer funcion alternativa con la mascara
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FF00F);

    //(GPIOAMSEL) pag.786 habilitar analogico
    GPIOE_AHB->AMSEL = (1<<1) | (1<<2) | (1<<5);

    //Pag 1159 El registro (ADCPC) establece la velocidad de reloj de conversión por segundo
    //Se pone un 1 en binario para indicar que los 2 millos de muestrs por s se divide /8 para obtener 250ksps
    ADC0->PC = (0<<2)|(1<<1)|(1<<0);//velocidad 250ksps 

    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x3012; //usa secuenciador 2 que recibe 4 canales 
     
    //Pag 1077 (ADCACTSS) Este registro controla la desactivacion de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);

    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0000); //por procesador 

    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    //ADC0->SSMUX2 = 0x0821; 
    ADC0->SSMUX0 = (1<<0); // el secuenciador 0 se asigna el canal 1
    ADC0->SSMUX1 = (2<<0); // el secuenciador 1 se asigna el canal 2
    ADC0->SSMUX2 = (8<<0); // el secuenciador 2 se asigna el canal 8

    //pag 1130 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción 
    //ADC0->SSCTL2 = (1<<1) | (1<<2) | (1<<5) | (1<<6) | (1<<10) | (1<<9); // son dos por cada canal y tenemos 3 canales 
    ADC0->SSCTL0 = (1<<2) | (1<<1);
    ADC0->SSCTL1 = (1<<2) | (1<<1);
    ADC0->SSCTL2 = (1<<2) | (1<<1);

    // indicar cuando manda la señal, mandar 1 al secuenciador que se ocupa (2)
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<0) | (1<<1) | (1<<2); //Unmask ADC0 sequence 0, 1 y 2 interrupt pag 1082
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;

    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (1<<0);
    
    // Configuracion por procesador 
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2); 
}
extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty)
{
    //se habilita el modo de configuración - por procesador 
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2); 
      
    // espera al convertidor
    while((ADC0->RIS&0x04)==0){};  
       
    //Secuenciador 0 - Canal 1 - PE2
    Result[0] = ADC0->SSFIFO0&0xFFF; // En el FIFO0 se almacenan las muestras del secuenciador 0, por lo que se lee, pag 860 
    duty[0] = (Result[0]*50000)/4096; //Cuentas para 20ms 
    
    //Secuenciador 1 - Canal 2 - PE1
    Result[1] = ADC0->SSFIFO1&0xFFF; // En el FIFO0 se almacenan las muestras del secuenciador 1
    duty[1] = (Result[1]*50000)/4096; //Cuentas para 20ms
     
    //Secuenciador 2 - Canal 8 - PE5
    Result[2] = ADC0->SSFIFO2&0xFFF;// En el FIFO0 se almacenan las muestras del secuenciador 2
    duty[2] = (Result[2]*50000)/4096;//Cuentas para 20ms
    
    // Conversion finalizada
    ADC0->ISC = (1<<0) | (1<<1) | (1<<2);  

}





