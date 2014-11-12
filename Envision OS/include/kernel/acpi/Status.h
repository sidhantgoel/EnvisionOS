///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Status.h-----------------------------------//
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
#ifndef STATUS_H_INCLUDED
#define STATUS_H_INCLUDED

/*
#define AE_CODE_ENVIRONMENTAL           0xC0000000|FACILITY_ACPI|0x0000
#define AE_CODE_PROGRAMMER              0xC0000000|FACILITY_ACPI|0x1000
#define AE_CODE_ACPI_TABLES             0xC0000000|FACILITY_ACPI|0x2000
#define AE_CODE_AML                     0xC0000000|FACILITY_ACPI|0x3000
#define AE_CODE_CONTROL                 0xC0000000|FACILITY_ACPI|0x4000
#define AE_CODE_MASK                    0xC0000000|FACILITY_ACPI|0xF000

#define AE_OK                           (ACPI_STATUS) STATUS_SUCCESS
*/

/*
 * Environmental exceptions
 */

/*
#define AE_ERROR                        (KESTATUS) (0x0001 | AE_CODE_ENVIRONMENTAL)
#define AE_NO_ACPI_TABLES               (KESTATUS) (0x0002 | AE_CODE_ENVIRONMENTAL)
#define AE_NO_NAMESPACE                 (KESTATUS) (0x0003 | AE_CODE_ENVIRONMENTAL)
#define AE_NO_MEMORY                    (KESTATUS) (0x0004 | AE_CODE_ENVIRONMENTAL)
#define AE_NOT_FOUND                    (KESTATUS) (0x0005 | AE_CODE_ENVIRONMENTAL)
#define AE_NOT_EXIST                    (KESTATUS) (0x0006 | AE_CODE_ENVIRONMENTAL)
#define AE_ALREADY_EXISTS               (KESTATUS) (0x0007 | AE_CODE_ENVIRONMENTAL)
#define AE_TYPE                         (KESTATUS) (0x0008 | AE_CODE_ENVIRONMENTAL)
#define AE_NULL_OBJECT                  (KESTATUS) (0x0009 | AE_CODE_ENVIRONMENTAL)
#define AE_NULL_ENTRY                   (KESTATUS) (0x000A | AE_CODE_ENVIRONMENTAL)
#define AE_BUFFER_OVERFLOW              (KESTATUS) (0x000B | AE_CODE_ENVIRONMENTAL)
#define AE_STACK_OVERFLOW               (KESTATUS) (0x000C | AE_CODE_ENVIRONMENTAL)
#define AE_STACK_UNDERFLOW              (KESTATUS) (0x000D | AE_CODE_ENVIRONMENTAL)
#define AE_NOT_IMPLEMENTED              (KESTATUS) (0x000E | AE_CODE_ENVIRONMENTAL)
#define AE_VERSION_MISMATCH             (KESTATUS) (0x000F | AE_CODE_ENVIRONMENTAL)
#define AE_SUPPORT                      (KESTATUS) (0x0010 | AE_CODE_ENVIRONMENTAL)
#define AE_SHARE                        (KESTATUS) (0x0011 | AE_CODE_ENVIRONMENTAL)
#define AE_LIMIT                        (KESTATUS) (0x0012 | AE_CODE_ENVIRONMENTAL)
#define AE_TIME                         (KESTATUS) (0x0013 | AE_CODE_ENVIRONMENTAL)
#define AE_UNKNOWN_STATUS               (KESTATUS) (0x0014 | AE_CODE_ENVIRONMENTAL)
#define AE_ACQUIRE_DEADLOCK             (KESTATUS) (0x0015 | AE_CODE_ENVIRONMENTAL)
#define AE_RELEASE_DEADLOCK             (KESTATUS) (0x0016 | AE_CODE_ENVIRONMENTAL)
#define AE_NOT_ACQUIRED                 (KESTATUS) (0x0017 | AE_CODE_ENVIRONMENTAL)
#define AE_ALREADY_ACQUIRED             (KESTATUS) (0x0018 | AE_CODE_ENVIRONMENTAL)
#define AE_NO_HARDWARE_RESPONSE         (KESTATUS) (0x0019 | AE_CODE_ENVIRONMENTAL)
#define AE_NO_GLOBAL_LOCK               (KESTATUS) (0x001A | AE_CODE_ENVIRONMENTAL)
#define AE_LOGICAL_ADDRESS              (KESTATUS) (0x001B | AE_CODE_ENVIRONMENTAL)
#define AE_ABORT_METHOD                 (KESTATUS) (0x001C | AE_CODE_ENVIRONMENTAL)
#define AE_SAME_HANDLER                 (KESTATUS) (0x001D | AE_CODE_ENVIRONMENTAL)
#define AE_WAKE_ONLY_GPE                (KESTATUS) (0x001E | AE_CODE_ENVIRONMENTAL)
#define AE_OWNER_ID_LIMIT               (KESTATUS) (0x001F | AE_CODE_ENVIRONMENTAL)

#define AE_CODE_ENV_MAX                 0x001F
*/

/*
 * Programmer exceptions
 */

/*
#define AE_BAD_PARAMETER                (KESTATUS) (0x0001 | AE_CODE_PROGRAMMER)
#define AE_BAD_CHARACTER                (KESTATUS) (0x0002 | AE_CODE_PROGRAMMER)
#define AE_BAD_PATHNAME                 (KESTATUS) (0x0003 | AE_CODE_PROGRAMMER)
#define AE_BAD_DATA                     (KESTATUS) (0x0004 | AE_CODE_PROGRAMMER)
#define AE_BAD_ADDRESS                  (KESTATUS) (0x0005 | AE_CODE_PROGRAMMER)
#define AE_ALIGNMENT                    (KESTATUS) (0x0006 | AE_CODE_PROGRAMMER)
#define AE_BAD_HEX_CONSTANT             (KESTATUS) (0x0007 | AE_CODE_PROGRAMMER)
#define AE_BAD_OCTAL_CONSTANT           (KESTATUS) (0x0008 | AE_CODE_PROGRAMMER)
#define AE_BAD_DECIMAL_CONSTANT         (KESTATUS) (0x0009 | AE_CODE_PROGRAMMER)

#define AE_CODE_PGM_MAX                 0x0009
*/

/*
 * Acpi table exceptions
 */

/*
#define AE_BAD_SIGNATURE                (KESTATUS) (0x0001 | AE_CODE_ACPI_TABLES)
#define AE_BAD_HEADER                   (KESTATUS) (0x0002 | AE_CODE_ACPI_TABLES)
#define AE_BAD_CHECKSUM                 (KESTATUS) (0x0003 | AE_CODE_ACPI_TABLES)
#define AE_BAD_VALUE                    (KESTATUS) (0x0004 | AE_CODE_ACPI_TABLES)
#define AE_TABLE_NOT_SUPPORTED          (KESTATUS) (0x0005 | AE_CODE_ACPI_TABLES)
#define AE_INVALID_TABLE_LENGTH         (KESTATUS) (0x0006 | AE_CODE_ACPI_TABLES)

#define AE_CODE_TBL_MAX                 0x0006
*/

/*
 * AML exceptions.  These are caused by problems with
 * the actual AML byte stream
 */

/*
#define AE_AML_ERROR                    (KESTATUS) (0x0001 | AE_CODE_AML)
#define AE_AML_PARSE                    (KESTATUS) (0x0002 | AE_CODE_AML)
#define AE_AML_BAD_OPCODE               (KESTATUS) (0x0003 | AE_CODE_AML)
#define AE_AML_NO_OPERAND               (KESTATUS) (0x0004 | AE_CODE_AML)
#define AE_AML_OPERAND_TYPE             (KESTATUS) (0x0005 | AE_CODE_AML)
#define AE_AML_OPERAND_VALUE            (KESTATUS) (0x0006 | AE_CODE_AML)
#define AE_AML_UNINITIALIZED_LOCAL      (KESTATUS) (0x0007 | AE_CODE_AML)
#define AE_AML_UNINITIALIZED_ARG        (KESTATUS) (0x0008 | AE_CODE_AML)
#define AE_AML_UNINITIALIZED_ELEMENT    (KESTATUS) (0x0009 | AE_CODE_AML)
#define AE_AML_NUMERIC_OVERFLOW         (KESTATUS) (0x000A | AE_CODE_AML)
#define AE_AML_REGION_LIMIT             (KESTATUS) (0x000B | AE_CODE_AML)
#define AE_AML_BUFFER_LIMIT             (KESTATUS) (0x000C | AE_CODE_AML)
#define AE_AML_PACKAGE_LIMIT            (KESTATUS) (0x000D | AE_CODE_AML)
#define AE_AML_DIVIDE_BY_ZERO           (KESTATUS) (0x000E | AE_CODE_AML)
#define AE_AML_BAD_NAME                 (KESTATUS) (0x000F | AE_CODE_AML)
#define AE_AML_NAME_NOT_FOUND           (KESTATUS) (0x0010 | AE_CODE_AML)
#define AE_AML_INTERNAL                 (KESTATUS) (0x0011 | AE_CODE_AML)
#define AE_AML_INVALID_SPACE_ID         (KESTATUS) (0x0012 | AE_CODE_AML)
#define AE_AML_STRING_LIMIT             (KESTATUS) (0x0013 | AE_CODE_AML)
#define AE_AML_NO_RETURN_VALUE          (KESTATUS) (0x0014 | AE_CODE_AML)
#define AE_AML_METHOD_LIMIT             (KESTATUS) (0x0015 | AE_CODE_AML)
#define AE_AML_NOT_OWNER                (KESTATUS) (0x0016 | AE_CODE_AML)
#define AE_AML_MUTEX_ORDER              (KESTATUS) (0x0017 | AE_CODE_AML)
#define AE_AML_MUTEX_NOT_ACQUIRED       (KESTATUS) (0x0018 | AE_CODE_AML)
#define AE_AML_INVALID_RESOURCE_TYPE    (KESTATUS) (0x0019 | AE_CODE_AML)
#define AE_AML_INVALID_INDEX            (KESTATUS) (0x001A | AE_CODE_AML)
#define AE_AML_REGISTER_LIMIT           (KESTATUS) (0x001B | AE_CODE_AML)
#define AE_AML_NO_WHILE                 (KESTATUS) (0x001C | AE_CODE_AML)
#define AE_AML_ALIGNMENT                (KESTATUS) (0x001D | AE_CODE_AML)
#define AE_AML_NO_RESOURCE_END_TAG      (KESTATUS) (0x001E | AE_CODE_AML)
#define AE_AML_BAD_RESOURCE_VALUE       (KESTATUS) (0x001F | AE_CODE_AML)
#define AE_AML_CIRCULAR_REFERENCE       (KESTATUS) (0x0020 | AE_CODE_AML)
#define AE_AML_BAD_RESOURCE_LENGTH      (KESTATUS) (0x0021 | AE_CODE_AML)
#define AE_AML_ILLEGAL_ADDRESS          (KESTATUS) (0x0022 | AE_CODE_AML)

#define AE_CODE_AML_MAX                 0x0022

*/
/*
 * Internal exceptions used for control
 */
/*
#define AE_CTRL_RETURN_VALUE            (KESTATUS) (0x0001 | AE_CODE_CONTROL)
#define AE_CTRL_PENDING                 (KESTATUS) (0x0002 | AE_CODE_CONTROL)
#define AE_CTRL_TERMINATE               (KESTATUS) (0x0003 | AE_CODE_CONTROL)
#define AE_CTRL_TRUE                    (KESTATUS) (0x0004 | AE_CODE_CONTROL)
#define AE_CTRL_FALSE                   (KESTATUS) (0x0005 | AE_CODE_CONTROL)
#define AE_CTRL_DEPTH                   (KESTATUS) (0x0006 | AE_CODE_CONTROL)
#define AE_CTRL_END                     (KESTATUS) (0x0007 | AE_CODE_CONTROL)
#define AE_CTRL_TRANSFER                (KESTATUS) (0x0008 | AE_CODE_CONTROL)
#define AE_CTRL_BREAK                   (KESTATUS) (0x0009 | AE_CODE_CONTROL)
#define AE_CTRL_CONTINUE                (KESTATUS) (0x000A | AE_CODE_CONTROL)
#define AE_CTRL_SKIP                    (KESTATUS) (0x000B | AE_CODE_CONTROL)
#define AE_CTRL_PARSE_CONTINUE          (KESTATUS) (0x000C | AE_CODE_CONTROL)
#define AE_CTRL_PARSE_PENDING           (KESTATUS) (0x000D | AE_CODE_CONTROL)

#define AE_CODE_CTRL_MAX                0x000D
*/
#endif // STATUS_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Status.h-----------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
