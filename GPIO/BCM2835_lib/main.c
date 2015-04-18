#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "digitalIO.h"

char getch(){
    char buf=0;
    struct termios old={0};
    fflush(stdout);
    if(tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag&=~ICANON;
    old.c_lflag&=~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)
        perror("read()");
    old.c_lflag|=ICANON;
    old.c_lflag|=ECHO;
    if(tcsetattr(0, TCSADRAIN, &old)<0)
        perror ("tcsetattr ~ICANON");
    printf("%c\n",buf);
    return buf;
 }

int main() {
	int i;
	raspberry_peripheral peripheral;
	configGPIO(&peripheral);

	inputPin(23, &peripheral);
	outputPin(24, &peripheral);
	while(getch() != 27) {
		setPin(24, &peripheral);
		sleep(2);
		clearPin(24, &peripheral);
		sleep(2);
		printf("Pin read lecture: %u\n", readPin(23, &peripheral));
	}
	releaseMapping(&peripheral);
	return 0;
}