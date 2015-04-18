#include <stdio.h>
#include <unistd.h>
#include "digitalIO.c"

#define DEV 0


int main() {
	int i;
	struct raspberry_peripheral peripheral;
	configGPIO(&peripheral);

	inputPin(23, &peripheral);
	outputPin(24, &peripheral);
	for(i = 0; i < 10; i++) {
		setPin(24, &peripheral);
		sleep(2);
		clearPin(24, &peripheral);
		sleep(2);
		printf("Pin read lecture: %u\n", readPin(23, &peripheral));
	}
	releaseMapping(&peripheral);
	return 0;
}