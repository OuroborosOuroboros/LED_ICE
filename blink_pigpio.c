#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int switchPin = 18;
const int RedLED = 21;
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
	signal_received = signal;
}

int button = 0;
int buttonState = 0;

int main () {
	if(gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initiliaze the GPIO interface.\n");
		return 1;
	}
	gpioSetMode(RedLED, PI_OUTPUT);
	gpioSetMode(switchPin, PI_INPUT);
	signal(SIGINT, sigint_handler);
	printf("Press CTRL-C to exit.\n");
	while(!signal_received) {
		button = gpioRead(switchPin);
	//	printf("%d\n", button);
		if(button == 1 && buttonState == 0){
			buttonState = 1;
			gpioWrite(RedLED, PI_HIGH);
			time_sleep(.33);
			printf("Line 31\nBUTTON: %d, BUTTON STATE: %d\n", button, buttonState);
			//gpioWrite(RedLED, PI_HIGH);
		}
		//time_sleep(1);
		else if (button == 1 && buttonState == 1){
			buttonState = 0;
			gpioWrite(RedLED, PI_LOW);
			time_sleep(.33);
			printf("Line 38\nBUTTON: %d, BUTTON STATE: %d\n", button, buttonState);
			//gpioWrite(RedLED, PI_LOW);
		}
		if(buttonState) {gpioWrite(RedLED, PI_HIGH);}
		else {gpioWrite(RedLED, PI_LOW);}
		//time_sleep(1);
	}
	gpioSetMode(RedLED, PI_INPUT);
	gpioSetMode(switchPin, PI_INPUT);
	gpioTerminate();
	printf("\n");
	
	return 0;
}
