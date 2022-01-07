#include "mem.h"


void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	//variable to store the old permissions
	DWORD oldprotect;
	//get permission to read and write the memory
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	//write into the memory
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	//recover the old permissions
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);

}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	// 0x90 = nop
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;

}