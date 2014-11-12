#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include "multiboot.h"
#include "pecoff.h"
#include <memory.h>

char *filename;
FILE *f;
char *data;
int filesize;
multiboot_header mbhdr;
unsigned long entry;
IMAGE_DOS_HEADER *doshdr;
IMAGE_HEADERS32 *imghdr;
unsigned long base;

#define HEADER_OFFSET   0x00
#define STACK_ADDRESS   0x00103fff

unsigned char bootstrap[16] = {
	0xBC, 0x00, 0x00, 0x00, 0x00,   //mov esp,0x00000000
	0x31, 0xC9,                     //xor ecx,ecx
	0x51,                           //push ecx
	0x9D,                           //popf
	0x50,                           //push eax
	0x53,                           //push ebx
	0xE8, 0x00, 0x00, 0x00, 0x00    //call 0x00000000
};

int main(int nArg,char **args)
{
    filename = args[1];
    f=fopen(filename,"r+");
    if(f)
    {
        printf("%s%s\n","successfully opened ",filename);

        fseek(f,0,SEEK_END);
        filesize = ftell(f);
        printf("%s%d\n","file size : ",filesize);

        printf("%s\n","reading file");
        data = (char*)malloc(filesize);
        //fsetpos(f,0);
        fseek(f,0,SEEK_SET);
        fread(data,filesize,1,f);

        doshdr = (IMAGE_DOS_HEADER*)data;
        imghdr = (IMAGE_HEADERS32*)(data+doshdr->e_lfanew);

        printf("%s%x\n","magic : ",imghdr->OptionalHeader.Magic);
        base = imghdr->OptionalHeader.ImageBase;
        printf("%s%x\n","base : ",base);
        entry = imghdr->OptionalHeader.AddressOfEntryPoint;
        printf("%s%d\n","entry : ",entry);

        mbhdr.magic = MULTIBOOT_MAGIC;
        mbhdr.flags = MULTIBOOT_PAGEALIGN|MULTIBOOT_MEMINFO|MULTIBOOT_VALID;
        mbhdr.header_addr = base + HEADER_OFFSET;
        mbhdr.load_addr = base;// + HEADER_OFFSET;
        mbhdr.load_end_addr = base + filesize;
        mbhdr.bss_end_addr = base + filesize;
        mbhdr.entry_addr = base + HEADER_OFFSET + sizeof(multiboot_header);
        mbhdr.mode_type = 0;
        mbhdr.width = 0;
        mbhdr.height = 0;
        mbhdr.depth = 0;

        mbhdr.checksum = ((mbhdr.magic+mbhdr.flags)^0xFFFFFFFF)+1;

        *(unsigned long*)(bootstrap+1) = STACK_ADDRESS;
        *(unsigned long*)(bootstrap+12) = entry-(HEADER_OFFSET+sizeof(mbhdr)+sizeof(bootstrap));

        //memcpy(data+HEADER_OFFSET+sizeof(multiboot_header),bootstrap,16);

        printf("%s\n","writing back");
        fseek(f,HEADER_OFFSET,SEEK_SET);
        fwrite(&mbhdr,sizeof(mbhdr),1,f);
        fwrite(bootstrap,sizeof(bootstrap),1,f);
        printf("%s\n","closing file");
        fclose(f);
        printf("%s\n","done...");
    }
    else
    {
        printf("%s\n","error opening file");
    }
    printf("%s\n","press any key to continue");
    //getch();
    return 0;
}