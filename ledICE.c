#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int RedLED = 21;
volatile sig_atomic_t signal_recieved = 0;

void sigint_handler(int_signal) {
    signal_recieved = signal;
}

int main(){
    if (gpioInitialise() == PI_INIT_FAILED) {
	printf("ERROR: Failed to initialize the GPIO interface. \n");
	return 1;
    }
   
    gpioSetMode(RedLED, PI_OUTPUT);
    signal (SIGINT, sigint_handler);
    printf("Press CTRL-C to exit.\n");

    while(!signal_recieved) {
	gpioWrite(RedLED, PI_HIGH);
	time_sleep(1);
    }

    gpioSetMode(RedLED, PI_INPUT);
    gpioTerminate();
    printf("\n");
    return 0;
}









}
