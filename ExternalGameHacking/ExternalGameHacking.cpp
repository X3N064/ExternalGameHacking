#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include "proc.h"

//examples
unsigned int ex1, ex2, ex3 = 0x0;

int main()
{
	//get process id from target program
	DWORD procId = GetProcId(L"ex");

	//get modulebase addr
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ex");

	//Get Handle to process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//resolve base addr of the pointer chain
	uintptr_t dynamicPtrBaseAddr = moduleBase + ex1;

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

	//resolve the pointer chain
	std::vector<unsigned int> offsets = { ex1, ex2, ex3 };

	uintptr_t addr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, offsets);

	//read the value
	int value = 0;

	ReadProcessMemory(hProcess, (byte*)addr, &value, sizeof(value), nullptr);
	std::cout << "Original value = " << std::dec << value << std::endl;

	//write the value
	int newValue = 999999999; //ex;
	WriteProcessMemory(hProcess, (byte*)addr, &newValue, sizeof(newValue), nullptr);

	//assert
	ReadProcessMemory(hProcess, (byte*)addr, &value, sizeof(value), nullptr);
	std::cout << "New value = " << std::dec << value << std::endl;

	//pause

	getchar();
	return 0;
}
