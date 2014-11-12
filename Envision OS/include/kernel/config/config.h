///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Config.h-----------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 13 april 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

//32Bit Kernel Configuration
#define KERNEL32_BASE               0xC0000000
#define KERNEL32_HEAP_BASE          0xD0000000
#define KERNEL32_HEAP_SIZE          0x00600000
#define KERNEL32_IO_BASE		    0xE0000000
#define KERNEL32_IO_LIMIT			0xFF800000

//64Bit Kernel Configuration
#define KERNEL64_BASE               0xC000000000000000
#define KERNEL64_HEAP_BASE          0xD000000000000000
#define KERNEL64_HEAP_SIZE          0x0010000000000000
#define KERNEL64_IO_BASE		    0xE000000000000000
#define KERNEL64_IO_LIMIT			0xFF80000000000000

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//////////////////    Do not edit below this line    //////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

#ifndef MACHINE_WIDTH
#error MACHINE_WIDTH not defined
#endif

#if MACHINE_WIDTH==32

#define KERNEL_BASE                 KERNEL32_BASE
#define KERNEL_HEAP_BASE            KERNEL32_HEAP_BASE
#define KERNEL_HEAP_SIZE            KERNEL32_HEAP_SIZE
#define KERNEL_IO_BASE		        KERNEL32_IO_BASE
#define KERNEL_IO_LIMIT			    KERNEL32_IO_LIMIT

#elif MACHINE_WIDTH==64

#define KERNEL_BASE                 KERNEL64_BASE
#define KERNEL_HEAP_BASE            KERNEL64_HEAP_BASE
#define KERNEL_HEAP_SIZE            KERNEL64_HEAP_SIZE
#define KERNEL_IO_BASE		        KERNEL64_IO_BASE
#define KERNEL_IO_LIMIT			    KERNEL64_IO_LIMIT

#else
#error MACHINE_WIDTH invalid
#endif

#endif // CONFIG_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//------------------------------KernelMemoryManager.h----------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
