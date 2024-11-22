#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <stdlib.h>
#include <unistd.h>

const int switchPin = 18;
const int RedLED = 21;

const int APIN = 4;
const int BPIN = 17;
const int CPIN = 22;
const int DPIN = 5;
//const int EPIN = 6;
//const int FPIN = 19;
//const int GPIN = 13; 
const int EPIN = 23;
const int FPIN = 24;
const int GPIN = 25;

//int segment_pins[7] = {4, 17, 22, 5, 6, 19, 13};
int segment_pins[7] = {4, 17, 22, 5, 23, 24, 25};


const int PPIN = 27;

const int digits[10][7] = {
	{1,1,1,1,1,1,0}, //0
	{0,1,1,0,0,0,0}, //1
	{1,1,0,1,1,0,1}, //2
	{1,1,1,1,0,0,1}, //3
	{0,1,1,0,0,1,1}, //4
	{1,0,1,1,0,1,1}, //5
	{1,0,1,1,1,1,1}, //6
	{1,1,1,0,0,0,0}, //7
	{1,1,1,1,1,1,1}, //8
	{1,1,1,1,0,1,1}  //9
};

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
	signal_received = signal;
}

int button = 0;
int buttonState = 0;


void displayDigit( int digit) {
	for (int i = 0; i < 7; i++) {
		printf("digit is %d\n", digit);
		if (digits[digit][i] == 1) {
			gpioWrite(segment_pins[i], PI_HIGH);
			printf("making light on %d (segment %d)\n", segment_pins[i] ,i);
		} 
		else {
			printf("not making light on %d(segment %d)\n", segment_pins[i] ,i);
			gpioWrite(segment_pins[i], PI_LOW);
			
		}
	}
	
}
			


	

int main () {

	
	
	int count = 0;
	if(gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initiliaze the GPIO interface.\n");
		return 1;
	}
	
		// set pioutput for our 8-display pins
	for (int i = 0; i < 7; i++) {
		gpioSetMode(segment_pins[i], PI_OUTPUT);
		printf("Pin %d set as output.\n", segment_pins[i]);
		sleep(1);
	}

	//gpioWrite(APIN, PI_OUTPUT);
	//gpioWrite(BPIN, PI_OUTPUT);
	//gpioWrite(CPIN, PI_OUTPUT);
	//gpioWrite(DPIN, PI_OUTPUT);
	//gpioWrite(EPIN, PI_OUTPUT);
	//gpioWrite(FPIN, PI_OUTPUT);
	//gpioWrite(GPIN, PI_OUTPUT);
	//printf("GOT THRU THAT CRAP!");
	//sleep(20);
	//exit(1);
	
	gpioSetMode(RedLED, PI_OUTPUT);
	gpioSetMode(switchPin, PI_INPUT);
	signal(SIGINT, sigint_handler);
	printf("Press CTRL-C to exit.\n");
	gpioWrite(RedLED, PI_HIGH);
	while(!signal_received) {
		displayDigit(count);
		button = gpioRead(switchPin);
	//	printf("%d\n", button);
		if(button == 1 && buttonState == 0){
			buttonState = 1;

			count ++; 
			count = count % 10;
			

			time_sleep(.33);
			printf("Digit = %d\n",count);
		}
		//time_sleep(1);
		else if (button == 1 && buttonState == 1){
			buttonState = 0;
			
			count ++; 
			count = count % 10;
			
			
			time_sleep(.33);
			printf("Digit = %d\n", count);
		}
		if(buttonState) {gpioWrite(RedLED, PI_HIGH);}
		//else {gpioWrite(APIN, PI_LOW);}
		//time_sleep(1);
	}


	// CLEAN UP
	gpioWrite(RedLED, PI_LOW);
	gpioSetMode(RedLED, PI_INPUT);
	gpioSetMode(switchPin, PI_INPUT);
	
	for (int i = 0; i < 7; i++) {
		gpioSetMode(segment_pins[i], PI_INPUT);
	}
	
	
	gpioTerminate();
	printf("\n");
	
	return 0;
}
