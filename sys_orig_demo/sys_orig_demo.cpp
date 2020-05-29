// Sys_fix_demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <stdio.h>
#include "syscall.h"
void* intAlloc(DWORD len, DWORD type)
{
    LPVOID newmem = NULL;
    if (FAILED(NtAllocateVirtualMemory((HANDLE)-1, &newmem, 0, &len, MEM_COMMIT | MEM_RESERVE, type))) { return NULL; }
    return newmem;
}
NTSTATUS intFree(PVOID addr)
{
    DWORD size = 0;
    return NtFreeVirtualMemory((HANDLE)-1, &addr, &size, 0x8000);
}
//#define intFree(addr) NtFreeVirtualMemory((HANDLE)-1, addr, 512, 0x8000)

int main()
{
    BYTE* buffer = NULL;
    int i = 0;
    NTSTATUS stat = 0;
    buffer = (BYTE*)intAlloc(512, PAGE_READWRITE);
    if (buffer == NULL)
    {
        printf("Failed to allocate buffer\n");
        return 0;
    }
    for (i = 0; i < 512; i++)
    {
        buffer[i] = i;
    }
    DWORD bufsize = 512;
    DWORD old = 0;
    stat = NtProtectVirtualMemory((HANDLE)-1, &buffer, &bufsize, PAGE_READWRITE, &old);
    if (FAILED(stat))
    {
        printf("Failed to change memory permissions: %x\n", stat);
    }
    stat = intFree(buffer);
    if (FAILED(stat))
    {
        printf("Failed to free buffer: %x\n", stat);
    }
    printf("done\n");

}

