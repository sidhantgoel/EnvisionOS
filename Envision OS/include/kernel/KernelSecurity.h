///////////////////////////////////////////////////////////////////////////////////
//---------------------------------KernelSecurity.h------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 22 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef KERNELSECURITY_H_INCLUDED
#define KERNELSECURITY_H_INCLUDED

/*      Access Right Masks          */

#define ACR_MASK_OBJECT             0x0000FFFF
#define ACR_MASK_STANDARD           0x00FF0000
#define ACR_MASK_MISC               0x0F000000
#define ACR_MASK_GENERIC            0xF0000000

/*      Standard Access Rights      */

#define ACR_STANDARD_DELETE         0x00010000
#define ACR_STANDARD_READ_CONTROL   0x00020000
#define ACR_STAMDARD_WRITE_DAC      0x00040000
#define ACR_STANDARD_WRITE_OWNER    0x00080000
#define ACR_STANDARD_SYNCHRONIZE    0x00100000
//                                  0x00200000  //Reserved
//                                  0x00400000  //Reserved
//                                  0x00800000  //Reserved

/*      Misc Access Rights          */

//                                  0x01000000  //Reserved
#define ACR_MAXIMUM_ALLOWED         0x02000000
//                                  0x04000000  //Reserved
//                                  0x08000000  //Reserved

/*      Generic Access Rights       */

#define ACR_GENERIC_ALL             0x10000000
#define ACR_GENERIC_EXECUTE         0x20000000
#define ACR_GENERIC_WRITE           0x40000000
#define ACR_GENERIC_READ            0x80000000

#endif // KERNELSECURITY_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//---------------------------------KernelSecurity.h------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
