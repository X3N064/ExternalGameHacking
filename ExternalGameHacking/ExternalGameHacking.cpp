
#include "proc.h"
#include "mem.h"

//examples
unsigned int ex1, ex2, ex3 = 0x0;

int main()
{
	//Get Handle to process
	HANDLE hProcess = 0;
	
	//variables
	uintptr_t moduleBase = 0, localPlayerPtr = 0, healthAddr = 0;
	bool bHealth = false, bAmmo = false, bRecoil = false;
	const int newValue = 99999;

	//get process id from target program
	DWORD procId = GetProcId(L"ex");

	if (procId)
	{

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);


		//get modulebase addr
		moduleBase = GetModuleBaseAddress(procId, L"ex");

		localPlayerPtr = moduleBase + 0x0; //ex

		healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0x0 }); //ex

	}

	else
	{
		std::cout << "Process not found, press enter to exit\n";

		//pause
		getchar();
		return 0;
	}

	DWORD dwExit = 0;
	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHealth = !bHealth;

		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;
			if (bAmmo)
			{
				//	\xFF\x06 = inc [esi]
				mem::PatchEx((BYTE*)(moduleBase + 0x0), (BYTE*)"\xFF\x06", 2, hProcess); //ex

			}
			else
			{
				//	\xFF\x0E = dec [esi]
				mem::PatchEx((BYTE*)(moduleBase + 0x0), (BYTE*)"\xFF\x0E", 2, hProcess); //ex
			}
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				mem::NopEx((BYTE*)(moduleBase + 0x0), 10, hProcess); //ex
			}
			else
			{
				//	\x tells the compiler to use the literal bytes we're defining in the char array
				mem::PatchEx((BYTE*)(moduleBase + 0x0), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess); //ex
			}
		}
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return 0;
		}

		//continuous write or freeze
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&newValue, sizeof(newValue), hProcess);
		}

		Sleep(10); 
	}

	//exit
	std::cout << "Process not found, press enter to exit\n";
	
	//pause
	getchar();
	return 0;

	


}
