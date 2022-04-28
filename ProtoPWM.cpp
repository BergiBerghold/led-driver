
#include "FGLibraries/CommonIncludes.h"
#include "FGLibraries/Error.h"

#include "FGLibraries/StringUtils.h"
#include "FGLibraries/Bridges/FGUSBBulk.h"

#include "FGLibraries/PicProto/PIC24FJ32B002_Proto.h"

FGPIC24FJ32B002_Proto Pic;

void SetupTimerA() {				//125ns-8ms  scale Timer
	Pic.T1CON = 0x0000;  			//Stops the Timer1 and reset control reg.
	Pic.TMR1  = 0x0000;  			//Clear contents of the timer register
	Pic.PR1 = 0xFFFF;			//Load the Period register with the value 							0xFFFF
	
	Pic.IEC0bits.T1IE = 0;			//Interrupts disabled
	
	Pic.T1CONbits.TCKPS = 0b00;			//Timer1 prescaler 1:1
	Pic.T1CONbits.TON = 1; 			//Start Timer1 
						//clock source set to the internal instruction cycle
}

void SetupTimerB() {				//1s-1h scale Timer
	Pic.T2CON = 0x0000;  			//Stops the Timer2 and reset control reg.
	Pic.T3CON = 0x0000;
	Pic.TMR2  = 0x0000;  			//Clear contents of the timer register
	Pic.TMR3  = 0x0000;
	Pic.PR3 = 0xFFFF;
	Pic.PR2 = 0xFFFF;				//Load the Period register with the value 0xFFFF
	
	//IPC2bits.T3IP=0x01;			//Timer 3 Interrupt Priority 1
	
	Pic.IFS0bits.T3IF=0;			//Timer 3 Interrupt Flag cleared
	Pic.IEC0bits.T3IE=0;			//Timer 3 Interrups disabled
	Pic.IEC0bits.T3IE=0;
	
	Pic.T2CONbits.TCKPS = 0b01;			//Timer2 prescaler 1:8
	Pic.T2CONbits.T32 = 1;			//32-bit timer
	Pic.T2CONbits.TON = 1; 			//Start Timer2
						//clock source set to the internal instruction cycle
}

void SetupMCU(){
	Pic.TRISB=0;				//All Pins Output and Digital
	Pic.TRISA=0;
	Pic.AD1PCFG = 0xFFFF;
	Pic.LATA=0;
	Pic.LATB=0;
	Pic.LATAbits.LATA0=1;
	Pic.LATAbits.LATA1=1;
	Pic.LATBbits.LATB3=1;
	Pic.TRISBbits.TRISB13=1;		
}

void SetupADC() {
        Pic.AD1CON1bits.ADON = 0; //ADC on/off
        Pic.AD1CON1bits.ADSIDL = 0; //idle on /off
        Pic.AD1CON1bits.FORM = 0b00; //output format 00=int
        Pic.AD1CON1bits.SSRC = 0b111; //trig source
        Pic.AD1CON1bits.ASAM = 1; //auto start
        Pic.AD1CON1bits.SAMP = 0; //amp hold

        Pic.AD1CON2bits.VCFG = 0b000; //Vref config
        Pic.AD1CON2bits.SMPI = 0b1111; //Sample/Convert Sequences Per Interrupt Selection bits
        Pic.AD1CON2bits.BUFM = 0; //8bit/16bit mode
        Pic.AD1CON2bits.ALTS = 0; // mux mode

        //adc clk = ADCS * Tcy = ADCS * Tosc*2
        Pic.AD1CON3bits.ADRC = 0; //clk source
        Pic.AD1CON3bits.SAMC = 31; //Tad
        Pic.AD1CON3bits.ADCS = 5; //Tcy

        Pic.AD1CHSbits.CH0NA = 0;//neg input AN1/Vr-

        Pic.AD1PCFG = 0xFFFF;
        Pic.AD1PCFGbits.PCFG11 = 0; //digital/analog
        //Pic.AD1PCFGbits.PCFG14 = 0; //digital/analog

        Pic.AD1CON1 = 0x00E0;   // auto convert after end of sampling
        Pic.AD1CSSL = 0x0000;   // no scanning required
        Pic.AD1CON2 = 0x0000;   // use MUXA, AVss and AVdd are used as Vref+/-
}

uint16_t GetSample(int Channel) {
        Pic.AD1CHS = Channel; // select analog input channel

        Pic.AD1CON1bits.ADON = 1;
        Pic.AD1CON1bits.SAMP = 1;      // start sampling, automatic conversion will follow
        return Pic.ADC1BUF0;           // read the conversion result
}

void SetupPWM(){
	Pic.RPOR7bits.RP15R	= 18;			//OC1 to RP15
	
	Pic.OC1R		= 255;			//OC1 on-time 100%
	Pic.OC1RS		= 255;			//OC1 Period 256us
	Pic.OC1CON2bits.SYNCSEL	= 0x1F;			//OC1 Sync Source
	Pic.OC1CON2bits.OCTRIG	= 0;			//Sync OC1 with SYNCSEL
	Pic.OC1CON1bits.OCTSEL	= 0b100;		//Timer 1 as Clock Source
	Pic.OC1CON1bits.OCM	= 0b110;		//Edge Aligned PWM	
}

int main(int argc, char **argv) {
	SetupMCU();
	SetupTimerA();
	//SetupTimerB();
	SetupPWM();
	SetupADC();
	
	while (1){
		cout << '\n';
		int V=0;
		cin >> V;
		Pic.OC1R=V;
		
	}
}
