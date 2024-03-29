// SHBsc.h: interface for the SHBsc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SHBSC_H_
#define _SHBSC_H_

#include "swirly.h"
#include "SHCpu.h"

// offsets of registers, from 0xff800000
#define PCTRA 0x2c
#define PDTRA 0x30
#define PCTRB 0x40
#define PDTRB 0x44

class SHBsc  
{
public:
	Dword hook(int event, Dword addr, Dword data);
	SHBsc(class SHCpu *cpu);
	virtual ~SHBsc();
private:
	SHBsc() {};
	void update();
	Dword regs[256];
	class SHCpu *cpu;
};

#endif
