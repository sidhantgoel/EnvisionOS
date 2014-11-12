////////////////////////////////////////////////////////////////////////////////
//----------------------------------volume.h----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 15 July 2008                                                 //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef __volume_h__
#define __volume_h__

#include <kernel.h>

UINT32 STDCALL VolInit();
UINT32 STDCALL VolAddStorage(UINT32 Type, VIRTUAL_PTR Device);
UINT32 STDCALL VolGetStorage(VIRTUAL_PTR Vol, VIRTUAL_PTR *Storage);
UINT32 STDCALL VolReadSectors(VIRTUAL_PTR Vol, VIRTUAL_PTR Buffer, SIZE_T nSect, SIZE_T SectCnt);
UINT32 STDCALL VolReadFile(char *Path, VIRTUAL_PTR Buffer);
UINT32 STDCALL VolFileSize(char *Path, UINT32 *FileSize);
UINT32 STDCALL VolList();

#endif //__volume_h__
////////////////////////////////////////////////////////////////////////////////
//----------------------------------volume.h----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////