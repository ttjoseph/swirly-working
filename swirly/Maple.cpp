#include "Maple.h"
#include "Overlord.h"
#include <stdio.h>
#include <string.h>

Maple::Maple(class SHCpu *cpu)
{
	this->cpu = cpu;
	// initialize a DeviceInfo for a single gamepad
	strcpy(gamepad.productName, "Swirly Fake Gamepad");
	strcpy(gamepad.productLicense, "I like cheese");
	gamepad.suppFuncs = Overlord::switchEndian(MAPLE_CONTROLLER);
	gamepad.standbyPower = 1; // this is completely made up
	gamepad.maxPower = 2; // so is this
}

Maple::~Maple()
{

}

Dword Maple::hook(int eventType, Dword addr, Dword data)
{
	switch(eventType)
	{
	case MMU_READ_DWORD:
		switch(addr)
		{
			// XXX: is this Right?
		case 0xa05f6900: // TA's VBL reg
			return 0x88; // hack

		case 0xa05f688c: // XXX: some SPU-related reg?
			return 1; // hack

		case 0xa05f6c18: // Maple DMA
			printf("Maple: check DMA status, PC=%08X\n", cpu->PC);
			return 0; // hack, pretend DMA is done

		default:
			return 0;
		}
	case MMU_WRITE_DWORD:
		switch(addr)
		{
			case 0xa05f6c04: // DMA address set
				// we are given a physical address here, NOT a virtual address
				dmaAddr = data;
				return 0;
			case 0xa05f6c14: // Maple enable
				if(data & 1)
					printf("Maple: enabled\n");
				else
					printf("Maple: disabled\n");
				return 0;
			case 0xa05f6c18: // DMA start
				if(data & 1) // if bit 0 is set, begin transfer
				{
					int cmd, recipient, sender, addtlwords, last, port, length;
					// XXX: finish me!
					// now we need to get the byte there
					Dword transferDesc1 = cpu->mmu->access(dmaAddr, MMU_READ_DWORD);
					Dword transferDesc2 = cpu->mmu->access(dmaAddr+4, MMU_READ_DWORD);
					Dword frameHeader = cpu->mmu->access(dmaAddr+8, MMU_READ_DWORD);
					frameHeader = Overlord::switchEndian(frameHeader);
					last = Overlord::bits(transferDesc1, 31, 31);
					port = Overlord::bits(transferDesc1, 17, 16);
					length = Overlord::bits(transferDesc1, 7, 0);
					cmd = Overlord::bits(frameHeader, 31, 24);
					recipient = Overlord::bits(frameHeader, 23, 16);
					sender = Overlord::bits(frameHeader, 15, 8);
					addtlwords = Overlord::bits(frameHeader, 7, 0);
					Dword resultAddr = Overlord::bits(transferDesc2, 28, 5) << 5;

					/*
					printf("Maple: DMA transfer from %08X: ", dmaAddr);
					printf("last %d, port %d, length %d ", last, port, length);
					printf("Result addr: %08X\n", transferDesc2);
					printf("Maple: command %08X: ", frameHeader);
					printf("cmd: %d ", cmd);
					printf("recipient: %d ", recipient);
					printf("sender: %d ", sender);
					printf("addtl words: %d\n", addtlwords);
					*/

					switch(cmd)
					{
					case MAPLE_REQ_DEVICE_INFO:
						printf("Maple: request device info of %02x (on port %d)\n", recipient, Overlord::bits(recipient, 7, 6));
						// XXX: we're going to pretend that our gamepad is connected to every
						// address on the maple bus with the exception of where the DC itself would be
						if((recipient & 0x1f) != 0) // if it isn't the DC itself...
						{ // ...then say there's a gamepad connected
							for(int i=0; i<(sizeof(DeviceInfo) / 4); i++)
								cpu->mmu->writeDwordToExternal(resultAddr+i*4, ((Dword*)&gamepad)[i]);
						}
						break;

					case MAPLE_GET_CONDITION:
						printf("Maple: get condition of %02x (on port %d)\n", recipient, Overlord::bits(recipient, 7, 6));
						break;
					}
				}
				return 0;
			case 0xa05f6c80: // timeout / speed
				printf("Maple: timeout set to %d\n", data >> 16);
				return 0;
			case 0xa05f6c8c: // unknown - a magic value is usually written to this
				printf("Maple: register 8c written with %08x\n", data);
				return 0;
		}
	}
	return 0xbeefbeef;

}
