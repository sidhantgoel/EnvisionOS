////////////////////////////////////////////////////////////////////////////////
//------------------------------------iso.c-----------------------------------//
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
#include <iso.h>
#include <fs\iso.h>

UINT32 STDCALL IsoProbe(VPTR Vol)
{
    return -1;
}

UINT32 STDCALL IsoReadFile(VPTR Vol, char *Path, VPTR Buffer)
{
    return -1;
}

UINT32 STDCALL IsoFileSize(VPTR Vol, char *Path, UINT32 *FileSize)
{
    return -1;
}


////////////////////////////////////////////////////////////////////////////////
//------------------------------------iso.c-----------------------------------//
//-----------------------Copyright � Sidhant Goel, 2009-----------------------//
////////////////////////////////////////////////////////////////////////////////