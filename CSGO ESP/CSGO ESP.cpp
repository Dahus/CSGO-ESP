#include "Offsets.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

/* SexOffenderSally helped code a lot/most of this with me, a long time friend! Give him a <3 on discord
Make sure character set is 'Multi-Byte' in project settings! And game must be windowed fullscreen.
Updated offsets: https://github.com/frk1/hazedumper/blob/master/csgo.cs     */

#define EnemyPen 0x000000FF
HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t moduleBase = GetModuleBaseAddress(GetProcId(L"csgo.exe"), L"client.dll");
HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetProcId(L"csgo.exe"));
HDC hdc = GetDC(FindWindowA(NULL, "Counter-Strike: Global Offensive"));

template<typename T> T RPM(SIZE_T address) {
	//The buffer for data that is going to be read from memory
	T buffer;

	//The actual RPM
	ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), NULL);

	//Return our buffer
	return buffer;
}

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

struct Vector3
{
	float x, y, z;
};

Vector3 WorldToScreen(const Vector3 pos, view_matrix_t matrix) {
	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenX * .5f;
	float y = screenY * .5f;

	x += 0.5f * _x * screenX + 0.5f;
	y -= 0.5f * _y * screenY + 0.5f;

	return { x,y,w };
}

void DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, EnemyBrush);
}

void DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	DrawFilledRect(x, y, w, thickness); //Top horiz line
	DrawFilledRect(x, y, thickness, h); //Left vertical line
	DrawFilledRect((x + w), y, thickness, h); //right vertical line
	DrawFilledRect(x, y + h, w + thickness, thickness); //bottom horiz line
}

void DrawLine(float StartX, float StartY, float EndX, float EndY)
{
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 2, EnemyPen);// penstyle, width, color
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL); //start
	a = LineTo(hdc, EndX, EndY); //end
	DeleteObject(SelectObject(hdc, hOPen));
}

int main()
{
	while (true)
	{
		view_matrix_t vm = RPM<view_matrix_t>(moduleBase + dwViewMatrix);
		int localteam = RPM<int>(RPM<DWORD>(moduleBase + dwEntityList) + m_iTeamNum);

		for (int i = 1; i < 64; i++)
		{
			uintptr_t pEnt = RPM<DWORD>(moduleBase + dwEntityList + (i * 0x10));

			int health = RPM<int>(pEnt + m_iHealth);
			int team = RPM<int>(pEnt + m_iTeamNum);

			Vector3 pos = RPM<Vector3>(pEnt + m_vecOrigin);
			Vector3 head;
			head.x = pos.x;
			head.y = pos.y;
			head.z = pos.z + 75.f;
			Vector3 screenpos = WorldToScreen(pos, vm);
			Vector3 screenhead = WorldToScreen(head, vm);
			float height = screenhead.y - screenpos.y;
			float width = height / 2.4f;

			if (screenpos.z >= 0.01f && team != localteam && health > 0 && health < 101) {
				DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 1);
				DrawLine(screenX / 2, screenY, screenpos.x, screenpos.y);
			}
		}
	}
}