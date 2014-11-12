////////////////////////////////////////////////////////////////////////////////
//----------------------------------status.h----------------------------------//
//                                   v 1.0                                    //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
// Author: Sidhant Goel                                                       //
// Date Created: 11 July 2008                                                 //
//                                                                            //
//----------------------------------Notes-------------------------------------//
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#define ESUCCESS				0x00000000
#define EDETECTED               0x00000001
#define EATA                    0x00000002
#define EATAPI                  0x00000003
#define EPRESENT                0x00000004
#define ENOTPRESENT             0x00000005
#define ETIMEOUT				0x80000001
#define EDRQTIMEOUT				0x80000002
#define EWRONGDEVICE            0x80000003
#define EWRONGPARAMETER         0x80000004
#define ENOTDETECTED            0x80000005
#define EFAILED                 0x80000006
#define ECMDERROR               0x80000007
#define EFILENOTFOUND           0x80000008
#define EDIRNOTFOUND            0x80000009
#define EINVALIDPATH            0x8000000A
#define EDUPLICATEPATH          0x8000000B
#define ENOTSUPPORTED           0x8000000C
#define EABORTED                0x8000000D
#define ENOMEDIA                0x8000000E
#define EUNKNOWN				0xFFFFFFFF

#define EOK(e)					((signed)e >= 0)
#define EFAIL(e)				((signed)e < 0)

////////////////////////////////////////////////////////////////////////////////
//----------------------------------status.h----------------------------------//
//-----------------------Copyright © Sidhant Goel, 2008-----------------------//
////////////////////////////////////////////////////////////////////////////////
