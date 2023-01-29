#include "lib/include.h"
 uint16_t Result[3];
 uint16_t duty[3];


int main(void)
{

//Experimento 2
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz con tres potenciometros variar el ciclo de trabajo
 * para controlar la posicion de tres servos sg90 o otros.
 *
 */
     //printChar('Y');
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    //Configurar_UART0();//Yo FCLK 20MHZ Baudrate 9600
    Configura_Reg_ADC0();
    Configura_Reg_PWM0(8);

   /* while (1)
    {
        ADC0_InSeq2(Result, duty); //llamada a la conversion por procesador
        PWM0->_0_CMPB = duty[0];
        PWM0->_1_CMPA = duty[1];
        PWM0->_2_CMPA = duty[2];  
      
      printChar('M');
    }*/

    while (1)
    {
        //Se llama a la función para leer y convertir valores del ADC
        ADC0_InSeq2(Result,duty);
      
        //Modificar ciclo de trabajo 
        //Canal ADC 1: PE2, PWM1: PF1
        PWM0->_0_CMPB = duty[0];

        //Canal ADC 2: PE1, PWM2: PF2
        PWM0->_1_CMPA = duty[1];

        //Canal ADC 8: PE5, PWM4: PG0
        PWM0->_2_CMPA = duty[2];
    }
    
}

