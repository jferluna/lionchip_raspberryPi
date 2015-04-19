#ifndef DIGITALIO_H_
#define DIGITALIO_H_

typedef struct _raspberry_peripheral {
	unsigned int physicalAddress;
	int memFileDescriptor;
	void* virtualMem;
	volatile unsigned int* address;
} raspberry_peripheral;

int configGPIO(raspberry_peripheral* rp);
void releaseMapping(raspberry_peripheral* rp);
void inputPin(int pin, raspberry_peripheral* peripheral);
void outputPin(int pin, raspberry_peripheral* peripheral);
void setPin(int pin, raspberry_peripheral* peripheral);
void clearPin(int pin, raspberry_peripheral* peripheral);
int readPin(int pin, raspberry_peripheral* peripheral);

#endif