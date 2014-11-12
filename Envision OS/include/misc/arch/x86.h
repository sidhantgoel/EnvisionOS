#ifndef __x86_h__
#define __x86_h__

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define PDE_PRESENT     0x001 //set if PDE is valid
#define PDE_READ_WRITE  0x002 //set for RW clear for R
#define PDE_USER        0x004 //set for user clear for supervisor
#define PDE_PWT         0x008 //set write-through clear write-back caching
#define PDE_PCD         0x010 //set chache enabled clear cashe disabled
#define PDE_ACCESSED    0x020 //set on access
#define PDE_RESERVED    0x040 //reserved by intel
#define PDE_PAGE_SIZE   0x080 //clear 4 KB set 4 MB
#define PDE_GLOBAL      0x100 //global page
#define PDE_MSK_AVL     0xE00 //available to system programmer

#define PTE_PRESENT     0x001 //set if PDE is valid
#define PTE_READ_WRITE  0x002 //set for RW clear for R
#define PTE_USER        0x004 //set for user clear for supervisor
#define PTE_PWT         0x008 //set write-through clear write-back caching
#define PTE_PCD         0x010 //set chache enabled clear cashe disabled
#define PTE_ACCESSED    0x020 //set on access
#define PTE_DIRTY       0x040 //set on page write
#define PTE_PAT         0x080 //select PAT
#define PTE_GLOBAL      0x100 //global page
#define PTE_MSK_AVL     0xE00 //available to system programmer

#define PDE4M_PAT       0x1000 //select PAT index for 4 mb page

#define SELECTOR_RPL0   0x00 //Requested privilege level 0
#define SELECTOR_RPL1   0x01 //Requested privilege level 1
#define SELECTOR_RPL2   0x02 //Requested privilege level 2
#define SELECTOR_RPL3   0x03 //Requested privilege level 3
#define SELECTOR_GDT    0x00 //in GDT
#define SELECTOR_LDT    0x01 //in LDT
#define MAKE_SELECTOR(index,table,rpl)  (SELECTOR)(((SELECTOR)index<<3)|((SELECTOR)table<<2)|(SELECTOR)rpl)

#define DESC_DPL0           0x00
#define DESC_DPL1           0x01
#define DESC_DPL2           0x02
#define DESC_DPL3           0x03
#define DESC_S_SYS          0x00
#define DESC_S_CODE_DATA    0x01
#define DESC_G_B            0x00
#define DESC_G_4K           0x01
#define DESC_DB_16          0x00
#define DESC_DB_32          0x01

#define SEG_DATA                0x0
#define SEG_DATA_EXPAND_DOWN    0x4 //data segments
#define SEG_DATA_WRITE          0x2 //data segments
#define SEG_DATA_ACCESSED       0x1
#define SEG_CODE                0x8
#define SEG_CODE_CONFORMING     0xC //code segments
#define SEG_CODE_READ           0xA //code segments
#define SEG_CODE_ACCESSED       0x9

#define DESC_SYS_TSS16A     0x1
#define DESC_SYS_LDT        0x2
#define DESC_SYS_TSS16B     0x3
#define DESC_SYS_CALLGATE16 0x4
#define DESC_SYS_TASKGATE   0x5
#define DESC_SYS_INTGATE16  0x6
#define DESC_SYS_TRAPGATE16 0x7
#define DESC_SYS_TSS32A     0x9
#define DESC_SYS_TSS32B     0xB
#define DESC_SYS_CALLGATE32 0xC
#define DESC_SYS_INTGATE32  0xE
#define DESC_SYS_TRAPGATE32 0xF

#define MAKE_DESC_LOW(base,limit)           \
    (UINT32)((((UINT32)base & 0xFFFF)<<16)  \
    |((UINT32)limit & 0xFFFF))

#define MAKE_DESC_HIGH(base,limit,g,db,l,avl,p,dpl,s,type) \
    (UINT32)(((UINT32)base & 0xFF000000)    \
    |((UINT32)g << 23)                      \
    |((UINT32)db << 22)                     \
    |((UINT32)l << 21)                     \
    |((UINT32)avl << 20)                     \
    |((UINT32)limit & 0xF0000)              \
    |((UINT32)p << 15)                      \
    |((UINT32)dpl << 13)                    \
    |((UINT32)s << 12)                      \
    |((UINT32)type << 8)                    \
    |(((UINT32)base & 0xFF0000) >> 16))

#define MAKE_CALLGATE_LOW(sel,offset)       \
    (UINT32)((((UINT32)sel & 0xFFFF)<<16)   \
    |((UINT32)offset & 0xFFFF))

#define MAKE_CALLGATE_HIGH(offset,p,dpl,pc) \
    (UINT32)(((UINT32)0xC00)                \
    |((UINT32)offset & 0xFFFF0000)          \
    |((UINT32)p << 15)                      \
    |((UINT32)dpl << 13)                    \
    |((UINT32)pc))

#define MAKE_INTGATE_LOW(sel,offset)        \
    (UINT32)((((UINT32)sel & 0xFFFF)<<16)   \
    |((UINT32)offset & 0xFFFF))

#define MAKE_INTGATE_HIGH(offset,p,dpl,d)   \
    (UINT32)(((UINT32)0x600)                \
    |((UINT32)offset & 0xFFFF0000)          \
    |((UINT32)p << 15)                      \
    |((UINT32)dpl << 13)                    \
    |((UINT32)d << 11))

#define MAKE_TRAPGATE_LOW(sel,offset)       \
    (UINT32)((((UINT32)sel & 0xFFFF)<<16)   \
    |((UINT32)offset & 0xFFFF))

#define MAKE_TRAPGATE_HIGH(offset,p,dpl,d)  \
    (UINT32)(((UINT32)0x700)                \
    |((UINT32)offset & 0xFFFF0000)          \
    |((UINT32)p << 15)                      \
    |((UINT32)dpl << 13)                    \
    |((UINT32)d << 11))

#define MAKE_TASKGATE_LOW(sel)              \
    (UINT32)(((UINT32)sel & 0xFFFF)<<16)

#define MAKE_TASKGATE_HIGH(p,dpl)           \
    (UINT32)(((UINT32)0x500)                \
    |((UINT32)p << 15)                      \
    |((UINT32)dpl << 13))

#define MAKE_TSS_DESC_LOW(base,limit)       \
    (UINT32)((((UINT32)base & 0xFFFF)<<16)  \
    |((UINT32)limit & 0xFFFF))

#define MAKE_TSS_DESC_HIGH(base,limit,g,avl,p,dpl,b) \
    (UINT32)(((UINT32)base & 0xFF000000)    \
    |((UINT32)g << 23)                      \
    |((UINT32)avl << 20)                    \
    |((UINT32)limit & 0xF0000)              \
    |((UINT32)p << 15)                      \
    |((UINT32)dpl << 13)                    \
    |((UINT32)b << 9)                       \
    |((UINT32)0x9 << 8)                     \
    |(((UINT32)base & 0xFF0000) >> 16))

typedef USHORT  SELECTOR;

__declspec(align(8)) typedef struct _DESCRIPTOR
{
    UINT32 low;
    UINT32 high;
}DESCRIPTOR,GDT_ENTRY,LDT_ENTRY,IDT_ENTRY;

#pragma pack(push,2)
typedef struct _DESC_PTR
{
    USHORT limit;
    DESCRIPTOR  *desc;
}DESC_PTR;
#pragma pack(pop)

#define PFEC_P          0x01 //
#define PFEC_WRITE      0x02 //
#define PFEC_USER       0x04 //
#define PFEC_RSVD       0x08 //
#define PFEC_IF         0x10 //

typedef struct _TSS
{
    USHORT	backlink, __blh;
    UINT32	esp0;
    USHORT	ss0, __ss0h;
    UINT32	esp1;
    USHORT	ss1, __ss1h;
    UINT32	esp2;
    USHORT	ss2, __ss2h;
    UINT32	cr3;
    UINT32	eip;
    UINT32	eflags;
    UINT32	eax, ecx, edx, ebx;
    UINT32	esp, ebp, esi, edi;
    USHORT	es, __esh;
    USHORT	cs, __csh;
    USHORT	ss, __ssh;
    USHORT	ds, __dsh;
    USHORT	fs, __fsh;
    USHORT	gs, __gsh;
    USHORT	ldt, __ldth;
    USHORT	trace, bitmap;
}TSS;

#define EFLAGS_CF       0x00000001	//Carry
#define EFLAGS_1        0x00000002	//Always Set to one
#define EFLAGS_PF       0x00000004	//Parity
#define EFLAGS_AF       0x00000010	//Auxiliary Carry
#define EFLAGS_ZF       0x00000040	//Zero
#define EFLAGS_SF       0x00000080	//Sign
#define EFLAGS_TF       0x00000100	//Trap
#define EFLAGS_IF       0x00000200	//Interrupt Enable
#define EFLAGS_DF       0x00000400	//Direction
#define EFLAGS_OF       0x00000800	//Overflow
#define EFLAGS_IOPL     0x00003000	//I/O Privilege Level
#define EFLAGS_NT       0x00004000	//Nested Task
#define EFLAGS_RF       0x00010000	//Resume
#define EFLAGS_VM       0x00020000	//Virtual-8086 Mode
#define EFLAGS_AC       0x00040000	//Alignment Check
#define EFLAGS_VIF      0x00080000	//Virtual Interrupt
#define EFLAGS_VIP      0x00100000	//Virtual Interrupt Pending
#define EFLAGS_ID       0x00200000	//CPUID

#define EFLAGS_IOPL0    0x000000000 //I/O Privilege Level 0
#define EFLAGS_IOPL1    0x000001000 //I/O Privilege Level 0
#define EFLAGS_IOPL2    0x000002000 //I/O Privilege Level 0
#define EFLAGS_IOPL3    0x000003000 //I/O Privilege Level 0

#define CR0_PE          0x000000001 //Protection Enable
#define CR0_MP          00x00000002 //Monitor Coprocessor
#define CR0_EM          0x000000004 //mulation
#define CR0_TS          0x000000008 //Task Switched
#define CR0_ET          0x000000010 //Extension Type
#define CR0_NE          0x000000020 //Numeric Error
#define CR0_WP          0x000010000 //Write Protected
#define CR0_AM          0x000040000 //Alignment Mask
#define CR0_NW          0x020000000 //Not Write-Through
#define CR0_CD          0x040000000 //Cache Disable
#define CR0_PG          0x080000000 //Paging

#define CR3_PWT         0x000000008 //Page Writes Transparent
#define CR3_PCD         0x000000010 //Page Cache Disable

#define CR4_VME         0x000000001 //Virtual-8086 Mode Ext.
#define CR4_PVI         0x000000002 //Protected Virtual Int.
#define CR4_TSD         0x000000004 //Time Stamp Disable
#define CR4_DE          0x000000008 //Debugging Extensions
#define CR4_PSE         0x000000010 //Page Size Extension
#define CR4_PAE         0x000000020 //Physical Address Ext.
#define CR4_MCE         0x000000040 //Machine Check Enable
#define CR4_PGE         0x000000080 //Global Page Enable
#define CR4_PCE         0x000000100 //RDPMC Enabled
#define CR4_OSFXSR      0x000000200 //FXSAVE, FXRSTOR
#define CF4_OSXMMEXCPT  0x000000400 //Unmasked SIMD FP Exception

typedef struct _REGS
{
	UINT32 edi, esi, ebp, esp, ebx, edx, ecx, eax; /* PUSHA/POP */
	UINT32 ds, es, fs, gs;
	UINT32 eip, cs, eflags, user_esp, user_ss; /* INT nn/IRET */
	UINT32 v_es, v_ds, v_fs, v_gs; /* V86 mode only */
} REGS;

typedef struct _REGS_E
{
	UINT32 edi, esi, ebp, esp, ebx, edx, ecx, eax; /* PUSHA/POP */
	UINT32 ds, es, fs, gs;
	UINT32 err_code;
	UINT32 eip, cs, eflags, user_esp, user_ss; /* INT nn/IRET */
	UINT32 v_es, v_ds, v_fs, v_gs; /* V86 mode only */
} REGS_E;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif //__x86_h__
