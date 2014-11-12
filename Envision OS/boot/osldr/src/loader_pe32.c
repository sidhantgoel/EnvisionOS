#include <loader_pe32.h>
#include <debug.h>
#include <exe\pecoff.h>

#pragma warning(disable : 4311 4312 4313 4018 4013)
#define printf DbgPrintf

int ldrpe32_MakeTables(unsigned long Addr)
{
    unsigned int i,j;
    unsigned int DirAddr = Addr + 511*4096;
    unsigned int Pg0Addr = Addr + 512*4096;

    for(i=0;i<513*1024;i++) ((unsigned int*)Addr)[i] = 0;

    *(unsigned int*)DirAddr = Pg0Addr|3;

    j = Addr;
    for(i=2048;i<4096;i+=4)
    {
        *(unsigned int*)(DirAddr+i) = j|3;
        j+=4096;
    }

    j = 0;
    for(i=0;i<4096;i+=4)
    {
        *(unsigned int*)(Pg0Addr+i) = j|3;
        j+=4096;
    }
    __asm
    {
		//jmp $
        mov eax,DirAddr
        mov cr3,eax
        mov eax,cr0
        or eax,0x80000000
        mov cr0,eax
    }
    return 1;
}

int ldrpe32_MapPhysical(unsigned long vAddr,unsigned long pAddr,unsigned int Count)
{
    unsigned int StartPage;
    unsigned int *PageEntries;
    unsigned int i,j;

    if((unsigned int)vAddr<0x80000000)
    {
        printf("\nUnnable to load pages below 0x80000000");
        return 0;
    }
    if((unsigned int)vAddr%4096 || (unsigned int)pAddr%4096)
    {
        printf("\nAddresses must be aligned to Page Boundary");
        return 0;
    }

    StartPage = vAddr/4096;
    PageEntries = (unsigned int*)0xFFC00000;

    j = pAddr;
    for(i=0;i<Count;i++)
    {
        PageEntries[StartPage+i] = j|3;
        j += 4096;
    }
    return 1;
}

int ldrpe32_FixAlign(unsigned long Start, unsigned long End)
{
	IMAGE_DOS_HEADER *DosHeader;
    IMAGE_HEADERS32 *ImgHeader;
    IMAGE_SECTION_HEADER *ScnHeaders;
	unsigned int i, delta = 0, size;

    DosHeader = (IMAGE_DOS_HEADER*) Start;
    ImgHeader = (IMAGE_HEADERS32*) (Start + DosHeader->e_lfanew);
    ScnHeaders = (IMAGE_SECTION_HEADER*)(Start + DosHeader->e_lfanew + sizeof(IMAGE_HEADERS32));
	ImgHeader->OptionalHeader.FileAlignment = IMAGE_ALIGN;
    for(i=0;i<ImgHeader->FileHeader.NumberOfSections;i++)
    {
		ScnHeaders[i].PointerToRawData += delta;
		if(ScnHeaders[i].PointerToRawData & (IMAGE_ALIGN-1))
		{
			memmove(Start + ((delta + ScnHeaders[i].PointerToRawData) | (IMAGE_ALIGN-1)) + 1, Start + delta + ScnHeaders[i].PointerToRawData, (End) - (Start + ScnHeaders[i].PointerToRawData));
			delta += (ScnHeaders[i].PointerToRawData | (IMAGE_ALIGN-1)) + 1 - ScnHeaders[i].PointerToRawData;
			End += (ScnHeaders[i].PointerToRawData | (IMAGE_ALIGN-1)) + 1 - ScnHeaders[i].PointerToRawData;
			ScnHeaders[i].PointerToRawData |= (IMAGE_ALIGN-1);
			ScnHeaders[i].PointerToRawData++;
		}
		if((ScnHeaders[i].VirtualSize > ScnHeaders[i].SizeOfRawData) || (ScnHeaders[i].VirtualSize & (IMAGE_ALIGN-1)))
		{
			size = ScnHeaders[i].VirtualSize;
			if(size & (IMAGE_ALIGN-1)) size = (size | (IMAGE_ALIGN-1)) + 1;
			memmove(Start + ScnHeaders[i].PointerToRawData + size, Start + ScnHeaders[i].PointerToRawData + ScnHeaders[i].SizeOfRawData, (End) - (Start + ScnHeaders[i].PointerToRawData + ScnHeaders[i].SizeOfRawData));
			delta += size - ScnHeaders[i].SizeOfRawData;
			End += size - ScnHeaders[i].SizeOfRawData;
			ScnHeaders[i].VirtualSize = ScnHeaders[i].SizeOfRawData = size;
		}
    }
    return End-Start;
}

int ldrpe32_MapPE(unsigned long Base,unsigned long Start,unsigned long End)
{
    IMAGE_DOS_HEADER *DosHeader;
    IMAGE_HEADERS32 *ImgHeader;
    IMAGE_SECTION_HEADER *ScnHeaders;
    unsigned int i;
    if(!ldrpe32_MapPhysical(Base,Start,1))
    {
        return 0;
    }
    DosHeader = (IMAGE_DOS_HEADER*) Base;
    ImgHeader = (IMAGE_HEADERS32*) (Base + DosHeader->e_lfanew);
    ScnHeaders = (IMAGE_SECTION_HEADER*)(Base + DosHeader->e_lfanew + sizeof(IMAGE_HEADERS32));
    for(i=0;i<ImgHeader->FileHeader.NumberOfSections;i++)
    {
        //printf("\nMapping Section");
        //printf("\nVirtual Address = %p",Base + ScnHeaders[i].VirtualAddress);
        //printf("\nPhysical Address = %p",Start + ScnHeaders[i].PointerToRawData);
        //printf("\nSize of Data = %p",ScnHeaders[i].SizeOfRawData);
        if(!ldrpe32_MapPhysical(Base + ScnHeaders[i].VirtualAddress,Start + ScnHeaders[i].PointerToRawData,ScnHeaders[i].SizeOfRawData/4096))
        {
            return 0;
        }
        if(ScnHeaders[i].VirtualSize > ScnHeaders[i].SizeOfRawData)
        {
            printf("\nVirtualSize %p > SizeOfRawData %p", ScnHeaders[i].VirtualSize, ScnHeaders[i].SizeOfRawData);
			return 0;
        }
    }
    return 1;
}

int ldrpe32_FixPE(unsigned long Addr)
{
    IMAGE_DOS_HEADER *DosHeader;
    IMAGE_HEADERS32 *ImgHeader;
    IMAGE_BASE_RELOCATION *ImgReloc;
    unsigned int Base,RelocDirSize,RelocDirAddr,RelocCount,RelocType;
    void *RelocBase;
    signed int Delta;
    unsigned short *RelocArray;
    DWORD* RelocPtr;
    int i;

    DosHeader = (IMAGE_DOS_HEADER*) Addr;
    ImgHeader = (IMAGE_HEADERS32*) (Addr + DosHeader->e_lfanew);
    Base = ImgHeader->OptionalHeader.ImageBase;
    Delta = Addr - Base;

    if(Delta)
    {
        RelocDirSize = ImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;
        RelocDirAddr = (Addr + ImgHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
        ImgReloc = (IMAGE_BASE_RELOCATION*)RelocDirAddr;

        //printf("\nRelocating Image");
        //printf("\nRelocating Delta = %p",Delta);
        //printf("\nRelocating Dir VA = %p",RelocDirAddr);
        //printf("\nRelocating Size = %p",RelocDirSize);

        //__asm jmp $

        while((unsigned long)ImgReloc < RelocDirAddr+RelocDirSize)
        {
            RelocCount = (ImgReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
            RelocBase = (void*)(Addr + ImgReloc->VirtualAddress);
            RelocArray = (void*)((unsigned int)ImgReloc + sizeof(IMAGE_BASE_RELOCATION));

            //printf("\nRelocating Count = %p",RelocCount);
            //printf("\nRelocating Base = %p",RelocBase);
            //printf("\nRelocating Array = %p",RelocArray);

            //__asm jmp $

            for(i=0; i<RelocCount; i++)
            {
                RelocType = RelocArray[i] >> 12;
                RelocPtr = (DWORD*)((unsigned long)RelocBase + (RelocArray[i] & 0x0fff));

                switch(RelocType)
                {
                    case 0:
                        break;
                    case 3:
                        //printf("\nRelocating Address = %X",reloc_ptr);
                        *RelocPtr += Delta;
                        break;
                    default:
                        printf("\nRelocation type Not Supported");
                        return 0;
                }
            }
            ImgReloc = (IMAGE_BASE_RELOCATION*)((unsigned long)ImgReloc + ImgReloc->SizeOfBlock);
        }
    }
    return 1;
}

int ldrpe32_LoadKernel(unsigned long Start,unsigned long End)
{
    if(!ldrpe32_MapPE(KERNEL_BASE,Start,End)) return 0;
    if(!ldrpe32_FixPE(KERNEL_BASE)) return 0;
    return 1;
}

int ldrpe32_SetupStack(unsigned long Start,unsigned long End)
{
    unsigned int Size = End - Start;
    if(!ldrpe32_MapPhysical(KERNEL_STACK,Start,Size/4096)) return 0;
    return 1;
}

int ldrpe32_JumpToKernel(kboot_info *binf)
{
    IMAGE_DOS_HEADER *DosHeader;
    IMAGE_HEADERS32 *ImgHeader;
    register void *Entry;

    DosHeader = (IMAGE_DOS_HEADER*) KERNEL_BASE;
    ImgHeader = (IMAGE_HEADERS32*) (KERNEL_BASE + DosHeader->e_lfanew);

    printf("\nKernel Entry = %p",KERNEL_BASE + ImgHeader->OptionalHeader.AddressOfEntryPoint);
	
    Entry = (void*)(KERNEL_BASE + ImgHeader->OptionalHeader.AddressOfEntryPoint);
    __asm
    {
        mov esp,KERNEL_STACK
        add esp,0x4000
        push binf
        //jmp $
	}
	//DbgStepEnable();
	__asm
	{
		//clts
        call Entry
    }
    return 1;
}
