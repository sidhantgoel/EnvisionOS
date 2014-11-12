#ifndef __multiboot_h__
#define __multiboot_h__

typedef struct _multiboot_header
{
    unsigned long magic;
    unsigned long flags;
    unsigned long checksum;
    unsigned long header_addr;
    unsigned long load_addr;
    unsigned long load_end_addr;
    unsigned long bss_end_addr;
    unsigned long entry_addr;
    unsigned long mode_type;
    unsigned long width;
    unsigned long height;
    unsigned long depth;
}multiboot_header;

typedef struct _aout_symbol_table
{
    unsigned long tabsize;
    unsigned long strsize;
    unsigned long addr;
    unsigned long reserved;
}aout_symbol_table;


typedef struct _elf_section_header_table
{
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long shndx;
}elf_section_header_table;

typedef struct _module
{
    void *mod_start;
    void *mod_end;
    char *string;
    unsigned long reserved;
}module;

typedef struct _multiboot_info
{
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    unsigned long cmdline;
    unsigned long mods_count;
    module *mods_addr;
    union
    {
        aout_symbol_table aout_sym;
        elf_section_header_table elf_sec;
    }u;
    unsigned long mmap_length;
    unsigned long mmap_addr;
}multiboot_info;

typedef struct _memory_map
{
    unsigned long size;
    unsigned long base_addr_low;
    unsigned long base_addr_high;
    unsigned long length_low;
    unsigned long length_high;
    unsigned long type;
}memory_map;

#define MULTIBOOT_MAGIC     0x1BADB002
#define MULTIBOOT_PAGEALIGN 0x00000001
#define MULTIBOOT_MEMINFO   0x00000002
#define MULTIBOOT_VALID     0x00010000

#endif //__multiboot_h__
