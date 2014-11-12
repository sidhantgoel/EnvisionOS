///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------Names.h-----------------------------------//
//                                      v 1.0                                    //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
// Created by: Sidhant Goel                                                      //
// Created on: 19 march 2007                                                     //
//                                                                               //
//--------------------------------------Notes------------------------------------//
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
//                                                                               //
///////////////////////////////////////////////////////////////////////////////////
#ifndef NAMES_H_INCLUDED
#define NAMES_H_INCLUDED

/* Method names - these methods can appear anywhere in the namespace */

#define METHOD_NAME__HID        "_HID"
#define METHOD_NAME__CID        "_CID"
#define METHOD_NAME__UID        "_UID"
#define METHOD_NAME__ADR        "_ADR"
#define METHOD_NAME__INI        "_INI"
#define METHOD_NAME__STA        "_STA"
#define METHOD_NAME__REG        "_REG"
#define METHOD_NAME__SEG        "_SEG"
#define METHOD_NAME__BBN        "_BBN"
#define METHOD_NAME__PRT        "_PRT"
#define METHOD_NAME__CRS        "_CRS"
#define METHOD_NAME__PRS        "_PRS"
#define METHOD_NAME__PRW        "_PRW"
#define METHOD_NAME__SRS        "_SRS"

/* Method names - these methods must appear at the namespace root */

#define METHOD_NAME__BFS        "\\_BFS"
#define METHOD_NAME__GTS        "\\_GTS"
#define METHOD_NAME__PTS        "\\_PTS"
#define METHOD_NAME__SST        "\\_SI._SST"
#define METHOD_NAME__WAK        "\\_WAK"

/* Definitions of the predefined namespace names  */

#define ACPI_UNKNOWN_NAME       (UINT32) 0x3F3F3F3F     /* Unknown name is "????" */
#define ACPI_ROOT_NAME          (UINT32) 0x5F5F5F5C     /* Root name is    "\___" */

#define ACPI_PREFIX_MIXED       (UINT32) 0x69706341     /* "Acpi" */
#define ACPI_PREFIX_LOWER       (UINT32) 0x69706361     /* "acpi" */

#define ACPI_NS_ROOT_PATH       "\\"
#define ACPI_NS_SYSTEM_BUS      "_SB_"

#endif // NAMES_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------Names.h-----------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
