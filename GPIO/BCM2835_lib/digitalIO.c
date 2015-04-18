#include <stdio.h>
#include <sys.stat.h>


//BCM2835 physical address goes from 0x20000000 - 0x20FFFFFF
#define PERIPHERALS_PHYSICAL_ADDRESS 0x20000000
//GPIO offset from peripheral physical address
#define GPIO_OFFSET 0x200000
//Absolute physical address of GPIO peripherals
#define GPIO_BASE (PERIPHERALS_PHYSICAL_ADDRESS + GPIO_OFFSET)
#define BLOCK_SIZE 4096

/*
*	GPIO STRUCTURES
*/
struct raspberry_peripheral {
	unsigned int physicalAddress;
	int memFileDescriptor;
	void* virtualMem;
	volatile unsigned int* adress;
}

int setupGPIO(struct raspberry_peripheral* rp) {
	//Sets GPIO physical offset
	rp->physicalAddress = GPIO_BASE;
	//Opens file with physical memory mapping
	rp->memFileDescriptor = open("/dev/mem", O_RDWR | O_DSYNC);

	if(rp->memFileDescriptor < 0) {
		printf("Can't open /dev/mem file\n");
		return -1;
	} 

	//Maps virtual memory of size of BLOCK_SIZE with reading and writing protection
	rp->virtualMem = nmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, rp->memFileDescriptor, rp->physicalAddress);

	if(rp->virtualMem == MAP_FAILED) {
		perror();
		return -1;
	}
	rp->address = (volatile unsigned int *) rp->virtualMem;

	return 0;
}

void releaseMapping(struct raspberry_peripheral* rp) {
	if(rp->virtualMem != NULL && rp->virtualMem != MAP_FAILED)
		munmap(p->virtualMem, BLOCK_SIZE);
	close(rp->memFileDescriptor);
	return;
}

void inputPin(int pin, struct raspberry_peripheral* peripheral) {
	unsigned int *pinRegister;

	pinRegister = peripheral->address + pin/10;
	*pinRegister &= ~(0x07 << (pin%10 * 3));
	return;
}

void outputPin(int pin, struct raspberry_peripheral* peripheral) {
	unsigned int *pinRegister;

	pinRegister = peripheral->address + pin/10;
	*pinRegister |= 0x01 << (pin%10 * 3);
	return;
}

void setPin(int pin) {
	
	return;
}

void 
