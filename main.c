/**     Flasher With Timer0
 *     By : Rehab Samy
 */

#define REG volatile unsigned long int *

REG RCGCTIMER = 0x400FE604;
REG GPTMCTL   = 0x4003000C;
REG GPTMCFG   = 0x40030000;
REG GPTMTAMR  = 0x40030004;
REG GPTMTAILR = 0x40030028;
REG GPTMRIS   = 0x4003001C;
REG GPTMICR   = 0x40030024;

REG GPIOHBCTL = 0x400FE06C;
REG RCGCGPIO =  0x400FE608;
REG GPIOAFSEL_F = 0x4005D420;
REG GPIODIRF =  0x4005D400;
REG GPIODEN_F =  0x4005D51C;
REG GPIODATA_F1 = 0x4005D008;


int main(void)
{


    /**   Configure GPIO F red led (PF1) **/
    *GPIOHBCTL |= (1<<5);  //ENABLE AHP FOR PORTF
    *RCGCGPIO |= (1<<5); //Enable clock in PortF

    *GPIOAFSEL_F &= ~(1<<1);  //Disable the alternate function on PF1
    *GPIODIRF |= (1<<1);      //make PF1 OUTPUT

    *GPIODEN_F  |= (1<<1);   //Enable digital I/O for PF1
    *GPIODATA_F1 |= (1<<1);  //LED ON


    /**     11.4 Initialization and Configuration of GPTM  **/
    //To use a GPTM, the appropriate TIMERn bit must be set in the RCGCTIMER
   *RCGCTIMER |= (1<<0); //Enable and provide a clock to 16/32-bit general-purpose timer0 in Run mode

    /**     11.4.1 One-Shot/Periodic Timer Mode  **/

   //1. Ensure the timer is disabled(the TnEN bit in the GPTMCTL register is cleared) before making any changes.
    *GPTMCTL &= ~(1<<0);

   //2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.
    *GPTMCFG = 0x00000000;

   //3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
    *GPTMTAMR |= (0x2<<0);

   //4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
   //to select whether to capture the value of the free-running timer at time-out, use an external
   //trigger to start counting, configure an additional trigger or interrupt, and count up or down
    *GPTMTAMR &= ~(1<<4);   //Count-Down Timer

    //5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR)
    *GPTMTAILR = 0x00F42400;  //16MHz

   //6.If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register(GPTMIMR)
    //No need for it now....

    //7. Set the TnEN bit in the GPTMCTL register to enable the timer and start counting
    *GPTMCTL |= (1<<0);

    while(1){
        //8. Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
        //the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
        //Register (GPTMICR).
     if( (*GPTMRIS & 0x00000001) == 1){
         *GPTMICR |= (1<<0);
         //Writing a 1 to this bit clears the TATORIS bit in the GPTMRIS register
         //and the TATOMIS bit in the GPTMMIS register.
      *GPIODATA_F1 ^= (1<<1); //Toggle the LED

     }

    }

	return 0;
}
