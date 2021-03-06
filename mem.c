#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mem.h"
#include "ppu.h"
#include "mmc.h"
#include "rom.h"

//this is extern'd out to EmulationManager.c
unsigned char *znes_mem;
static int banksize;
static int swapbank;

unsigned char *mem_getaddr(unsigned int b)
{
	return &znes_mem[b];
}
unsigned int get_short_at(unsigned int addr)
{
	return znes_mem[addr+1] << 8 | znes_mem[addr];
}

static unsigned char readb(unsigned int addr)
{
	switch(addr)
	{
		case 0x2002:
			return 0x80; /* Always vblank, for now */
		break;
	}

	return znes_mem[addr];
}
unsigned char get_byte_at(unsigned int addr)
{
	return readb(addr);
}

void init_mem(void)
{
	unsigned char *b = rom_getbytes();

	znes_mem = calloc(0x10000, 1); /* 0 - FFFF */
	memcpy(&znes_mem[0x8000], b+0x38000, 16384);
	memcpy(&znes_mem[0xC000], b+0x3C000, 16384);
//  memcpy(&mem[0x6000], sram from file);
}

void writeb(unsigned int addr, unsigned char val)
{
	/* If mapper == 01 */
	if(addr >= 0x8000 && (val & 0x80))
		mmc_shift_reset();
	else if(addr >= 0x8000 && addr < 0xA000)
		mmc_reg0_sendbit(val);
	else if(addr >= 0xA000 && addr < 0xC000)
		mmc_reg1_sendbit(val);
	else if(addr >= 0xC000 && addr < 0xE000)
		mmc_reg2_sendbit(val);
	else if(addr >= 0xE000 && addr <= 0xFFFF)
		mmc_reg3_sendbit(val);

	switch(addr)
	{
		case 0x2000:
			ppu_write_reg1(val);
		break;
		case 0x2001:
			ppu_write_reg2(val);
		break;
		case 0x2006:
			ppu_write_addr(val);
		break;
		case 0x2007:
			ppu_write_data(val);
		break;
		default:
			znes_mem[addr] = val;
		break;
	}
}
