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

#include <arch\arch.h>

UINT32 STDCALL VolAddStorage(UINT32 Type, VPTR Device);
UINT32 STDCALL VolGetStorage(VPTR Vol, VPTR *Storage);
UINT32 STDCALL VolReadSectors(VPTR Vol, VPTR Buffer, SIZE_T nSect, SIZE_T SectCnt);
UINT32 STDCALL VolReadFile(char *Path, VPTR Buffer);
UINT32 STDCALL VolFileSize(char *Path, UINT32 *FileSize);
UINT32 STDCALL VolList();

#endif //__volume_h__
////////////////////////////////////////////////////////////////////////////////
//----------------------------------volume.h----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////