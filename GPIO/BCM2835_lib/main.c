#include <stdio.h>
#include "digitalIO.c"


int main() {
	struct raspberry_peripheral peripheral;
	setupGPIO(&peripheral);
	printf("Hola\n");
}