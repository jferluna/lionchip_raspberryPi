#include <stdio.h>
#include <unistd.h>
#include "digitalIO.c"

#define DEV 1


int main() {
	int i;
	struct raspberry_peripheral peripheral;
	configGPIO(&peripheral);

#if DEV
	printf("Pheripheral address: %d\n", peripheral.address);
#endif

	inputPin(23, &peripheral);
	outputPin(24, &peripheral);
	for(i = 0; i < 10; i++) {
		setPin(24, &peripheral);
		sleep(5);
		clearPin(24, &peripheral);
		sleep(5);
	}
	releaseMapping(&peripheral);
	return 0;
}