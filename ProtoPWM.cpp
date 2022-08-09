
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

void SetupPWM(){
	Pic.RPOR7bits.RP15R	= 18;			//OC1 to RP15
	
	Pic.OC1R		= 0;			//OC1 on-time 100%
	Pic.OC1RS		= 65535;			//OC1 Period 2^16 f= 122Hz
	Pic.OC1CON2bits.SYNCSEL	= 0x1F;			//OC1 Sync Source
	Pic.OC1CON2bits.OCTRIG	= 0;			//Sync OC1 with SYNCSEL
	Pic.OC1CON1bits.OCTSEL	= 0b100;		//Timer 1 as Clock Source
	Pic.OC1CON1bits.OCM	= 0b110;		//Edge Aligned PWM	
}

int main(int argc, char **argv) {
	SetupMCU();
	SetupTimerA();
	SetupPWM();
	
	int V = atoi(argv[1]);
	Pic.OC1R = V;


//	while (1){
//		cout << '\n';
//		int V=0;
//		cin >> V;
//		cout << to_string(V) << " / 65535" << '\n';
//		Pic.OC1R=V;
//
//	}
}
