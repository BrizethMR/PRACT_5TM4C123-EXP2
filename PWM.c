
#include "lib/include.h"

extern void Configura_Reg_PWM0(uint16_t freq)
{
    //Experimento 2
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz con tres potenciometros variar el ciclo de trabajo
 * para controlar la posicion de tres servos sg90 o otros
*/
    // PARA ESTA PRACTICA SE CONECTA EL SERVO 

    //SE HABILITA MODULO PWM0 P. 354
    SYSCTL->RCGCPWM |= (1<<0); 

    //HABILITA GPIO PUERTO G Y F PORQUE SE TRABAJA CON PG0, PF1 Y PF2, P. 383
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<6); //Enable reloj de GPIO Puerto F pag 340 pin 5

   // HABILLITAR LA FUNCION ALTERNATIVA P. 770
   GPIOG_AHB->AFSEL |= (1<<0); //PG0 
   GPIOF_AHB->AFSEL |= (1<<1); //PF1 
   GPIOF_AHB->AFSEL |= (1<<2); //PF2

   // HABILITAR O DESABILITAR EL DIVISOR E INDICAR CUAL ES EL VALOR DEL DIVISOR P. 1747
   // SE COLOCA 1 AL BIT 8 PARA ACTIVAR DIVIDOR Y EN EL BIT 0,1,2 SE PONE UN 2 EN BINARIO PARA DIVIDIR ENTRE 8
   //  frec, tarjeta / frec PWM = cuentas
   PWM0->CC = (1<<8) | (0<<2) | (1<<1) | (0<<0);
   //Se Habilita porque salen 50 mil cuentas que no caben en los 16 bits del contador del PWM, 20 millinoes/ 50 = 400 mil cuentas

    // SE HABILITAN COMO SALIDA LOS PINES DEL PWM 
    GPIOG_AHB->DIR = (1<<0); // PG0
    GPIOF_AHB->DIR = (1<<1); // PF1
    GPIOF_AHB->DIR = (1<<2); // PF2

    // INDICAR FUNCION ALTERNATIVA
    //TABLA DE P. 1808 Y REGISTRO PCTL, P. 788 PARA INDICAR QUE ES PWM
    // SE PONE EL VALOR DE 6 EN EL PIN 
    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006; //PG0
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFF0FF) | 0x00000600; //PF2
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFFF0F) | 0x00000060; //PF1

    //HABILITAR LA FUNCIONN LA ALTERNATIVA - para decirle si es digital o no 
    GPIOG_AHB->DEN |= (1<<0); //PG0
    GPIOF_AHB->DEN |= (1<<1); //PF1
    GPIOF_AHB->DEN |= (1<<2); //PF2

    /*P. 11672 el bit 0 indica el modo habilitado o deshabilitado */
    /*EL NUMERO DE AQUI INDICA GENERADOR*/
    PWM0->_1_CTL = (0<<0);/*Bloqueo y desbloqueo, SE DESACTIVA EL GENERADOR 1, LO DESABILITA PARA CONFIGURARLO */ 
    PWM0->_0_CTL = (0<<0);
    PWM0->_2_CTL = (0<<0);

    /*MODO DE TRABAJO - CUENTA REGRSIVA P. 1727, 1678 Y 1687*/
    //Config. : (0X2<<2) para enviar pwmB a bajo cuando Counter=LOAD, (0X3<<10) para enviar pwmB a alto cuando contador coincide con comparador B, 
   //Config. : (0x0<<0) para No hace nada cuando Counter=0, (0X3<<6) para enviar pwmA a alto cuando contador coincide con comparador A
    PWM0->_0_GENB |= (0X2<<2)|(0X3<<10)|(0x0<<0);   
    PWM0->_1_GENA |= (0X2<<2)|(0X3<<6)|(0x0<<0);  
    PWM0->_2_GENA |= (0X2<<2)|(0X3<<6)|(0x0<<0);

    PWM0->_0_LOAD = 50000; /*cuentas = fclk/fpwm  para 1khz cuentas = (20,000,000/50)*/
    PWM0->_1_LOAD = 50000;
    PWM0->_2_LOAD = 50000;

    // Establecer el valor del ciclo de trabajo
    PWM0->_0_CMPB = 5000;//Cuentas para 2 ms
    PWM0->_1_CMPA = 5000;           
    PWM0->_2_CMPA = 5000;

    PWM0->_0_CTL = (1<<0); //se activa el generador 0
    PWM0->_1_CTL = (1<<0);// Se activa el generador 1 
    PWM0->_2_CTL = (1<<0); // se activa el generador 2

    PWM0->ENABLE = (1<<1)|(1<<2)|(1<<4); /*habilitar el bloque pa que pase Pag 1247*/
}