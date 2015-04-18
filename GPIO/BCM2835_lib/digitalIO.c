#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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
	volatile unsigned int* address;
};

int configGPIO(struct raspberry_peripheral* rp) {
	//Sets GPIO physical offset
	rp->physicalAddress = GPIO_BASE;
	//Opens file with physical memory mapping
	rp->memFileDescriptor = open("/dev/mem", O_RDWR | O_DSYNC);

	if(rp->memFileDescriptor < 0) {
		printf("Can't open /dev/mem file\n");
		return -1;
	} else {
		printf("/dev/mem openning successful!\n");
	}

	//Maps virtual memory of size of BLOCK_SIZE with reading and writing protection
	rp->virtualMem = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, rp->memFileDescriptor, rp->physicalAddress);

	if(rp->virtualMem == MAP_FAILED) {
		printf("Error:\n");
		perror(NULL);
		return -1;
	} else {
		printf("Virtual memory mapping successful!\n");
	}
	rp->address = (volatile unsigned int *) rp->virtualMem;

	return 0;
}

void releaseMapping(struct raspberry_peripheral* rp) {
	if(rp->virtualMem != NULL && rp->virtualMem != MAP_FAILED)
		munmap(rp->virtualMem, BLOCK_SIZE);
	close(rp->memFileDescriptor);
	return;
}

void inputPin(int pin, struct raspberry_peripheral* peripheral) {
	//Point to GPFSEL# register (address 0x7E200000 - 0x7E200014) 
	volatile unsigned int *pinRegister = peripheral->address + pin/10;
	//Set group of 3 bits to 000 in order to enable pin as input
	*pinRegister &= ~(0x07 << (pin%10 * 3));
	return;
}

void outputPin(int pin, struct raspberry_peripheral* peripheral) {
	//Point to GPFSEL# register (address 0x7E200000 - 0x7E200014) 
	volatile unsigned int *pinRegister = peripheral->address + pin/10;
	//Set group of 3 bits to 001 in order to enable pin as output
	*pinRegister |= 0x01 << (pin%10 * 3);
	return;
}

void setPin(int pin, struct raspberry_peripheral* peripheral) {
	//Point to GPSET0 register (address 0x7E20001C) 
	volatile unsigned int *pinRegister = peripheral->address + 7;
	//Set bit in order to put pin as HIGH
	*pinRegister = 0x01 << pin;
	return;
}

void clearPin(int pin, struct raspberry_peripheral* peripheral) {
	//Point to GPCLR0 register (address 0x7E200028) 
	volatile unsigned int *pinRegister = peripheral->address + 10;
	//Set bit in order to put pin as LOW
	*pinRegister = 0x01 << pin;
	return;
}

int readPin(int pin, struct raspberry_peripheral* peripheral) {
	//Point to GPLEV0 register (address 0x7E200034)
	volatile unsigned int *pinRegister = peripheral->address + 13;
	//Get pin value
	*pinRegister &= 0x01 << pin;
	printf("PIN REGISTER %u\n", *pinRegister);
	return *pinRegister;
}
