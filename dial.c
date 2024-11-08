#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>
/*
        gcc -pthread -o mcp3008 mcp3008.c -lpigpio -lrt
        mcp3008 [num_of_loops] [speed_of_transmission]
                defaults to 1000000 in both cases
*/

const int RedLED = 21;
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal){
    signal_received = 1;
}

int main(int argc, char *argv[]){
    if (gpioInitialise()==PI_INIT_FAILED){
        printf("ERROR: Failed to initialize GPIO interface...\n");
        return 1;
    }
    int i;
    int h;
    int v;
    int loops;
    int speed;
    int light;
    double start, diff, sps;
    unsigned char buf[3];
    
    gpioSetMode(RedLED, PI_OUTPUT);
    signal(SIGINT, sigint_handler);
    gpioSetPWMrange(RedLED, 255);
    //gpioWrite(RedLED, PI_HIGH);
    
    //while(!signal_received){ 
        if (argc > 1) loops = atoi(argv[1]);
        else loops = 1000000;
        if (argc > 2) speed = atoi(argv[2]);
        else speed = 1000000;
        //if (gpioInitialise() < 0) return 1;
        h = spiOpen(0, speed, 0);
        if (h < 0) return 2;
        start = time_time();
        for (i=0; i<loops; i++){
            buf[0] = 1;
            buf[1] = 128;
            buf[2] = 0;

        spiXfer(h, buf, buf, 3);
        v = ((buf[1]&3)<<8) | buf[2];
        light = v/4;
        gpioPWM(RedLED, light);
        printf("%d\n", v);
        //if (!signal_received) { break;}
    
        }
        diff = time_time() - start;
    
        fprintf(stderr, "sps=%.1f @ %d bps (%d/%.1f)\n", 
            (double)loops / diff, speed, loops, diff);
        spiClose(h);
        
    //}
    gpioWrite(RedLED, PI_LOW);
    gpioSetMode(RedLED, PI_INPUT);
    gpioTerminate();
    return 0; 
}
