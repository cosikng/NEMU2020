#include "common.h"
#include "cpu/reg.h"
#include <stdlib.h>

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

uint32_t read_cache(hwaddr_t addr, size_t len, bool *flag);
bool write_cahce(hwaddr_t addr, size_t len, uint32_t data);
uint32_t read_cache2(hwaddr_t addr, size_t len, bool *flag);
bool write_cahce2(hwaddr_t addr, size_t len, uint32_t data);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{
	bool flag;
	uint32_t d1 = 0, d2 = 0, dat = 0;
	uint32_t up = (addr & ~((1 << cpu.cache1.b) - 1)) + (1 << cpu.cache1.b);
	//printf("Read\n");
	//printf("addr:0x%x,up:0x%x\n",addr,up);
	if (addr + len > up)
	{
		d1 = read_cache(addr, up - addr, &flag);
		d2 = read_cache(up, addr + len - up, &flag);
		dat = d1 + (d2 << (up - addr) * 8);
		//if (dat != (dram_read(addr, len) & (~0u >> ((4 - len) << 3))))
		//	panic("2read 0x%x,expected 0x%x\naddr:0x%x,up:0x%x,len:%d\nd1:0x%x,d2:0x%x\n", dat, (dram_read(addr, len) & (~0u >> ((4 - len) << 3))), addr, up, (int)len, d1, d2);
		return dat;
	}
	return read_cache(addr, len, &flag);
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
	bool f;
	uint32_t up = (addr & ~((1 << cpu.cache1.b) - 1)) + (1 << cpu.cache1.b);
	if (addr + len > up)
	{
		f = write_cahce(addr, up - addr, data);
		f = write_cahce(up, addr + len - up, data >> (up - addr) * 8);
	}
	f = write_cahce(addr, len, data);
	//dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
	uint32_t dir, page, diritem, pageitem, off, paddr, data1, data2, sublen;
	dir = (addr >> 22) & 0x3ff;
	page = (addr >> 12) & 0x3ff;
	off = addr & 0xfff;
	paddr = addr;
	if (cpu.CR0.paging == 1)
	{

		//printf("Read0x%x\n",addr);

		diritem = hwaddr_read(cpu.CR3.val + dir * 4, 4);
		assert((diritem & 1) == 1);
		pageitem = hwaddr_read((diritem & 0xfffff000) + page * 4, 4);
		assert((pageitem & 1) == 1);
		paddr = (pageitem & 0xfffff000) + off;
		//printf("Read:0x%x\n",paddr);
		goto next;
		if (addr + len >= (addr & 0xfffff000) + 0x1000)
		{
			sublen = (addr & 0xfffff000) + 0x1000 - addr;
			data1 = hwaddr_read(paddr, sublen);
			pageitem = hwaddr_read((diritem & 0xfffff000) + page * 4 + 4, 4); //读取下一页
			assert((pageitem & 1) == 1);
			paddr = (pageitem & 0xfffff000);
			data2 = hwaddr_read(paddr, len - sublen);
			return (data2 << sublen * 8) + data1;
		}
	}
	next:
	return hwaddr_read(paddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	uint32_t dir, page, diritem, pageitem, off, paddr, sublen;
	dir = (addr >> 22) & 0x3ff;
	page = (addr >> 12) & 0x3ff;
	off = addr & 0xfff;
	paddr = addr;
	if (cpu.CR0.paging == 1)
	{

		diritem = hwaddr_read(cpu.CR3.val + dir * 4, 4);
		assert((diritem & 1) == 1);
		pageitem = hwaddr_read((diritem & 0xfffff000) + page * 4, 4);
		assert((pageitem & 1) == 1);
		paddr = (pageitem & 0xfffff000) + off;
		goto next;
		if (addr + len >= (addr & 0xfffff000) + 0x1000)
		{
			sublen = (addr & 0xfffff000) + 0x1000 - addr;
			hwaddr_write(paddr, sublen, data & ((1 << sublen) - 1));
			pageitem = hwaddr_read((diritem & 0xfffff000) + page * 4 + 4, 4); //读取下一页
			assert((pageitem & 1) == 1);
			paddr = (pageitem & 0xfffff000);
			hwaddr_write(paddr, len - sublen, data >> sublen * 8);
			return;
		}
	}
	next:
	hwaddr_write(paddr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg)
{
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if ((cpu.CR0.val & 1) == 1)
	{
		return lnaddr_read(addr + cpu.Sregcache[sreg].base, len);
	}
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg)
{
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	if ((cpu.CR0.val & 1) == 1)
	{
		lnaddr_write(addr + cpu.Sregcache[sreg].base, len, data);
		return;
	}
	lnaddr_write(addr, len, data);
}

uint32_t read_cache(hwaddr_t addr, size_t len, bool *flag)
{
	uint32_t index, tag, off, E;
	struct set *set;
	int i, j;
	bool flag2;
	index = (addr >> cpu.cache1.b) & ((1 << cpu.cache1.s) - 1);
	tag = addr >> (cpu.cache1.s + cpu.cache1.b);
	off = addr & ((1 << cpu.cache1.b) - 1);
	E = cpu.cache1.E;
	set = &cpu.cache1.sets[index];

	for (i = 0; i < E && set->blocks[i].valid == true; i++)
	{
		if (set->blocks[i].tag == tag)
		{
			*flag = true;
			cpu.cache1.hit++;
			if (len == 4)
				return *((uint32_t *)(set->blocks[i].buf + off));
			if (len == 3)
				return *((uint32_t *)(set->blocks[i].buf + off - 1)) >> 8;
			if (len == 2)
				return *((uint16_t *)(set->blocks[i].buf + off));
			if (len == 1)
				return *((uint8_t *)(set->blocks[i].buf + off));
		}
	}
	*flag = false;
	cpu.cache1.miss++;
	if (i < E)
	{
		set->blocks[i].valid = true;
		set->blocks[i].tag = tag;
		for (j = 0; j < (1 << cpu.cache1.b); j++)
		{
			//set->blocks[i].buf[j] = dram_read(addr - off + j, 1) & 255;
			set->blocks[i].buf[j] = read_cache2(addr - off + j, 1, &flag2) & 255;
		}
	}
	else
	{
		i = rand() % E;
		set->blocks[i].tag = tag;
		for (j = 0; j < (1 << cpu.cache1.b); j++)
		{
			//set->blocks[i].buf[j] = dram_read(addr - off + j, 1) & 255;
			set->blocks[i].buf[j] = read_cache2(addr - off + j, 1, &flag2) & 255;
		}
	}
	if (len == 4)
		return *((uint32_t *)(set->blocks[i].buf + off));
	if (len == 3)
		return *((uint32_t *)(set->blocks[i].buf + off - 1)) >> 8;
	if (len == 2)
		return *((uint16_t *)(set->blocks[i].buf + off));
	if (len == 1)
		return *((uint8_t *)(set->blocks[i].buf + off));
	return 0;
}
bool write_cahce(hwaddr_t addr, size_t len, uint32_t data)
{
	uint32_t index, tag, off, E;
	struct set *set;
	int i;
	index = (addr >> cpu.cache1.b) & ((1 << cpu.cache1.s) - 1);
	tag = addr >> (cpu.cache1.s + cpu.cache1.b);
	off = addr & ((1 << cpu.cache1.b) - 1);
	E = cpu.cache1.E;
	set = &cpu.cache1.sets[index];

	for (i = 0; i < E && set->blocks[i].valid == true; i++)
	{
		if (set->blocks[i].tag == tag)
		{
			if (len == 4)
				*((uint32_t *)(set->blocks[i].buf + off)) = data;
			if (len == 3)
			{
				*((uint8_t *)(set->blocks[i].buf + off)) = data & 0xff;
				*((uint8_t *)(set->blocks[i].buf + off + 1)) = (data >> 8) & 0xff;
				*((uint8_t *)(set->blocks[i].buf + off + 2)) = (data >> 16) & 0xff;
			}
			if (len == 2)
				*((uint16_t *)(set->blocks[i].buf + off)) = data;
			if (len == 1)
				*((uint8_t *)(set->blocks[i].buf + off)) = data;
			//dram_write(addr, len, data);
			write_cahce2(addr, len, data);
			cpu.cache1.hit++;
			return true;
		}
	}
	cpu.cache1.miss++;
	//dram_write(addr, len, data);
	write_cahce2(addr, len, data);
	return false;
}

uint32_t read_cache2(hwaddr_t addr, size_t len, bool *flag)
{
	uint32_t index, tag, off, E;
	struct set *set;
	int i, j;
	index = (addr >> cpu.cache2.b) & ((1 << cpu.cache2.s) - 1);
	tag = addr >> (cpu.cache2.s + cpu.cache2.b);
	off = addr & ((1 << cpu.cache2.b) - 1);
	E = cpu.cache2.E;
	set = &cpu.cache2.sets[index];

	for (i = 0; i < E && set->blocks[i].valid == true; i++)
	{
		if (set->blocks[i].tag == tag)
		{
			*flag = true;
			cpu.cache2.hit++;
			if (len == 4)
				return *((uint32_t *)(set->blocks[i].buf + off));
			if (len == 3)
				return *((uint32_t *)(set->blocks[i].buf + off - 1)) >> 8;
			if (len == 2)
				return *((uint16_t *)(set->blocks[i].buf + off));
			if (len == 1)
				return *((uint8_t *)(set->blocks[i].buf + off));
		}
	}
	*flag = false;
	cpu.cache2.miss++;
	if (i < E)
	{
		set->blocks[i].valid = true;
		set->blocks[i].tag = tag;
		for (j = 0; j < (1 << cpu.cache2.b); j++)
		{
			set->blocks[i].buf[j] = dram_read(addr - off + j, 1) & 255;
		}
	}
	else
	{
		i = rand() % E;
		if (set->blocks[i].dirty)
		{
			for (j = 0; j < (1 << cpu.cache2.b); j++)
			{
				dram_write((tag << (cpu.cache2.s + cpu.cache2.b)) + (index << cpu.cache2.b) + j, 1, set->blocks[i].buf[j]);
			}
			set->blocks[i].dirty = false;
		}
		set->blocks[i].tag = tag;
		for (j = 0; j < (1 << cpu.cache2.b); j++)
		{
			set->blocks[i].buf[j] = dram_read(addr - off + j, 1) & 255;
		}
	}
	if (len == 4)
		return *((uint32_t *)(set->blocks[i].buf + off));
	if (len == 3)
		return *((uint32_t *)(set->blocks[i].buf + off - 1)) >> 8;
	if (len == 2)
		return *((uint16_t *)(set->blocks[i].buf + off));
	if (len == 1)
		return *((uint8_t *)(set->blocks[i].buf + off));
	return 0;
}
bool write_cahce2(hwaddr_t addr, size_t len, uint32_t data)
{
	uint32_t index, tag, off, E;
	struct set *set;
	int i, j;
	index = (addr >> cpu.cache2.b) & ((1 << cpu.cache2.s) - 1);
	tag = addr >> (cpu.cache2.s + cpu.cache2.b);
	off = addr & ((1 << cpu.cache2.b) - 1);
	E = cpu.cache2.E;
	set = &cpu.cache2.sets[index];

	for (i = 0; i < E && set->blocks[i].valid == true; i++)
	{
		if (set->blocks[i].tag == tag)
		{
			if (len == 4)
				*((uint32_t *)(set->blocks[i].buf + off)) = data;
			if (len == 3)
			{
				*((uint8_t *)(set->blocks[i].buf + off)) = data & 0xff;
				*((uint8_t *)(set->blocks[i].buf + off + 1)) = (data >> 8) & 0xff;
				*((uint8_t *)(set->blocks[i].buf + off + 2)) = (data >> 16) & 0xff;
			}
			if (len == 2)
				*((uint16_t *)(set->blocks[i].buf + off)) = data;
			if (len == 1)
				*((uint8_t *)(set->blocks[i].buf + off)) = data;
			cpu.cache2.hit++;
			return true;
		}
	}
	cpu.cache2.miss++;
	if (i < E)
	{
		set->blocks[i].valid = true;
		set->blocks[i].tag = tag;
		set->blocks[i].dirty = true;
		for (j = 0; j < (1 << cpu.cache2.b); j++)
		{
			//printf("%d\n",j);
			set->blocks[i].buf[j] = dram_read(addr - off + j, 1) & 255;
		}
	}
	else
	{
		i = rand() % E;
		if (set->blocks[i].dirty)
		{
			for (j = 0; j < (1 << cpu.cache2.b); j++)
			{
				dram_write((tag << (cpu.cache2.s + cpu.cache2.b)) + (index << cpu.cache2.b) + j, 1, set->blocks[i].buf[j]);
			}
		}
		set->blocks[i].tag = tag;
		for (j = 0; j < (1 << cpu.cache2.b); j++)
		{
			set->blocks[i].buf[j] = dram_read(addr - off + j, 1) & 255;
		}
	}
	if (len == 4)
		*((uint32_t *)(set->blocks[i].buf + off)) = data;
	if (len == 3)
	{
		*((uint8_t *)(set->blocks[i].buf + off)) = data & 0xff;
		*((uint8_t *)(set->blocks[i].buf + off + 1)) = (data >> 8) & 0xff;
		*((uint8_t *)(set->blocks[i].buf + off + 2)) = (data >> 16) & 0xff;
	}
	if (len == 2)
		*((uint16_t *)(set->blocks[i].buf + off)) = data;
	if (len == 1)
		*((uint8_t *)(set->blocks[i].buf + off)) = data;
	return false;
}