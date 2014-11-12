////////////////////////////////////////////////////////////////////////////////
//------------------------------------iso.h-----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 27 Jan 2009                                                  //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef __iso_h__
#define __iso_h__

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

UINT32 STDCALL IsoProbe(VIRTUAL_PTR Vol);
UINT32 STDCALL IsoReadFile(VIRTUAL_PTR Vol, char *Path, VIRTUAL_PTR Buffer);
UINT32 STDCALL IsoFileSize(VIRTUAL_PTR Vol, char *Path, UINT32 *FileSize);

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif //__iso_h__
////////////////////////////////////////////////////////////////////////////////
//------------------------------------iso.h-----------------------------------//
//-----------------------Copyright � Sidhant Goel, 2009-----------------------//
////////////////////////////////////////////////////////////////////////////////