///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Tables.h-----------------------------------//
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
#ifndef TABLES_H_INCLUDED
#define TABLES_H_INCLUDED

#pragma pack(push,1)

/*********************************************************************************************
 *
 * Values for description table header signatures. Useful because they make
 * it more difficult to inadvertently type in the wrong signature.
 *
 ********************************************************************************************/
#define ACPI_SIG_APIC           "APIC"      /* Multiple APIC Description Table              */
#define ACPI_SIG_DSDT           "DSDT"      /* Differentiated System Description Table      */
#define ACPI_SIG_ECDT           "ECDT"      /* Embedded Controller Boot Resources Table     */
#define ACPI_SIG_FACP           "FACP"      /* Fixed ACPI Description Table (FADT)          */
#define ACPI_SIG_FACS           "FACS"      /* Firmware ACPI Control Structure              */
#define ACPI_SIG_PSDT           "PSDT"      /* Persistent System Description Table          */
#define ACPI_SIG_RSDT           "RSDT"      /* Root System Description Table                */
#define ACPI_SIG_SBST           "SBST"      /* Smart Battery Specification Table            */
#define ACPI_SIG_SSDT           "SSDT"      /* Secondary System Description Table           */
#define ACPI_SIG_XSDT           "XSDT"      /* Extended System Description Table            */
#define ACPI_SIG_BOOT           "BOOT"      /* Simple Boot Flag Table                       */
#define ACPI_SIG_CPEP           "CPEP"      /* Corrected Platform Error Polling Table       */
#define ACPI_SIG_DBGP           "DBGP"      /* Debug Port Table                             */
#define ACPI_SIG_ETDT           "ETDT"      /* Event Timer Description Table                */
#define ACPI_SIG_HPET           "HPET"      /* IA-PC High Precision Event Timer Table       */
#define ACPI_SIG_SLIT           "SLIT"      /* System Locality Information Table            */
#define ACPI_SIG_SPCR           "SPCR"      /* Serial Port Console Redirection Table        */
#define ACPI_SIG_SRAT           "SRAT"      /* Static Resource Affinity Table               */
#define ACPI_SIG_SPMI           "SPMI"      /* Server Platform Management Interface Table   */
#define ACPI_SIG_TCPA           "TCPA"      /* Trusted Computing Platform Alliance          */
                                            /* Capabilities Table                           */
#define ACPI_SIG_RSDP           "RSD PTR "  /* Root System Description Pointer              */
#define ACPI_RSDP_NAME          "RSDP"      /* Short name for RSDP, not signature           */

#define ACPI_SIG_ASF            "ASF!"      /* Alert Standard Format table */
#define ACPI_SIG_DMAR           "DMAR"      /* DMA Remapping table */
#define ACPI_SIG_WDRT           "WDRT"      /* Watchdog Resource Table */


/*********************************************************************************************
 *
 * GAS - Generic Address Structure (ACPI 2.0+)
 *
 * Note: Since this structure is used in the ACPI tables, it is byte aligned.
 * If misalignment is not supported, access to the Address field must be
 * performed with care.
 *
 ********************************************************************************************/
typedef struct _ACPI_GENERIC_ADDRESS
{
    UINT8                   SpaceId;                /* Address space where struct or register exists    */
    UINT8                   BitWidth;               /* Size in bits of given register                   */
    UINT8                   BitOffset;              /* Bit offset within the register                   */
    UINT8                   AccessWidth;            /* Minimum Access size (ACPI 3.0)                   */
    UINT64                  Address;                /* 64-bit address of struct or register             */
} ACPI_GENERIC_ADDRESS;

#define ACPI_SPACE_ID_SYSTEM_MEMORY         0x00    /* System Memory                */
#define ACPI_SPACE_ID_SYSTEM_IO             0x01    /* System I/O                   */
#define ACPI_SPACE_ID_PCI_CONFIG            0x02    /* PCI Configuration Space      */
#define ACPI_SPACE_ID_EMBEDDED_CONTROLER    0x03    /* Embedded Controller          */
#define ACPI_SPACE_ID_SMBUS                 0x04    /* SMBus                        */
#define ACPI_SPACE_ID_FIXED                 0x7F    /* Functional Fixed Hardware    */


/*******************************************************************************
 *
 * ACPI Table Header. This common header is used by all tables except the
 * RSDP and FACS. The define is used for direct inclusion of header into
 * other ACPI tables
 *
 ******************************************************************************/
#define ACPI_NAME_SIZE              4
#define ACPI_OEM_ID_SIZE            6
#define ACPI_OEM_TABLE_ID_SIZE      8
typedef struct _ACPI_TABLE_HEADER
{
    CHAR                    Signature[ACPI_NAME_SIZE];          /* ASCII table signature */
    UINT32                  Length;                             /* Length of table in bytes, including this header */
    UINT8                   Revision;                           /* ACPI Specification minor version # */
    UINT8                   Checksum;                           /* To make sum of entire table == 0 */
    CHAR                    OemId[ACPI_OEM_ID_SIZE];            /* ASCII OEM identification */
    CHAR                    OemTableId[ACPI_OEM_TABLE_ID_SIZE]; /* ASCII OEM table identification */
    UINT32                  OemRevision;                        /* OEM revision number */
    CHAR                    AslCompilerId[ACPI_NAME_SIZE];      /* ASCII ASL compiler vendor ID */
    UINT32                  AslCompilerRevision;                /* ASL compiler version */

} ACPI_TABLE_HEADER;

/*******************************************************************************
 *
 * RSDP - Root System Description Pointer (Signature is "RSD PTR ")
 *
 ******************************************************************************/
typedef struct _ACPI_TABLE_RSDP
{
    CHAR                    Signature[8];               /* ACPI signature, contains "RSD PTR " */
    UINT8                   Checksum;                   /* ACPI 1.0 checksum */
    CHAR                    OemId[ACPI_OEM_ID_SIZE];    /* OEM identification */
    UINT8                   Revision;                   /* Must be (0) for ACPI 1.0 or (2) for ACPI 2.0+ */
    UINT32                  RsdtPhysicalAddress;        /* 32-bit physical address of the RSDT */
    UINT32                  Length;                     /* Table length in bytes, including header (ACPI 2.0+) */
    UINT64                  XsdtPhysicalAddress;        /* 64-bit physical address of the XSDT (ACPI 2.0+) */
    UINT8                   ExtendedChecksum;           /* Checksum of entire table (ACPI 2.0+) */
    UINT8                   Reserved[3];                /* Reserved, must be zero */

} ACPI_TABLE_RSDP;

#define ACPI_RSDP_REV0_SIZE     20                  /* Size of original ACPI 1.0 RSDP */


/*******************************************************************************
 *
 * RSDT/XSDT - Root System Description Tables
 *
 ******************************************************************************/
typedef struct _ACPI_TABLE_RSDT
{
    ACPI_TABLE_HEADER       Header;                 /* Common ACPI table header */
    UINT32                  TableOffsetEntry[1];    /* Array of pointers to ACPI tables */

} ACPI_TABLE_RSDT;

typedef struct _ACPI_TABLE_XSDT
{
    ACPI_TABLE_HEADER       Header;                 /* Common ACPI table header */
    UINT64                  TableOffsetEntry[1];    /* Array of pointers to ACPI tables */

} ACPI_TABLE_XSDT;


/*******************************************************************************
 *
 * FACS - Firmware ACPI Control Structure (FACS)
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_FACS
{
    CHAR                    Signature[4];           /* ASCII table signature */
    UINT32                  Length;                 /* Length of structure, in bytes */
    UINT32                  HardwareSignature;      /* Hardware configuration signature */
    UINT32                  FirmwareWakingVector;   /* 32-bit physical address of the Firmware Waking Vector */
    UINT32                  GlobalLock;             /* Global Lock for shared hardware resources */
    UINT32                  Flags;
    UINT64                  XFirmwareWakingVector;  /* 64-bit version of the Firmware Waking Vector (ACPI 2.0+) */
    UINT8                   Version;                /* Version of this table (ACPI 2.0+) */
    UINT8                   Reserved[31];           /* Reserved, must be zero */

} ACPI_TABLE_FACS;

/* Flag macros */

#define ACPI_FACS_S4_BIOS_PRESENT (1)               /* 00: S4BIOS support is present */

/* Global lock flags */

#define ACPI_GLOCK_PENDING      0x01                /* 00: Pending global lock ownership */
#define ACPI_GLOCK_OWNED        0x02                /* 01: Global lock is owned */


/*******************************************************************************
 *
 * FADT - Fixed ACPI Description Table (Signature "FACP")
 *
 ******************************************************************************/

/* Fields common to all versions of the FADT */

typedef struct acpi_table_fadt
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT32                  Facs;               /* 32-bit physical address of FACS */
    UINT32                  Dsdt;               /* 32-bit physical address of DSDT */
    UINT8                   Model;              /* System Interrupt Model (ACPI 1.0) - not used in ACPI 2.0+ */
    UINT8                   PreferredProfile;   /* Conveys preferred power management profile to OSPM. */
    UINT16                  SciInterrupt;       /* System vector of SCI interrupt */
    UINT32                  SmiCommand;         /* 32-bit Port address of SMI command port */
    UINT8                   AcpiEnable;         /* Value to write to smi_cmd to enable ACPI */
    UINT8                   AcpiDisable;        /* Value to write to smi_cmd to disable ACPI */
    UINT8                   S4BiosRequest;      /* Value to write to SMI CMD to enter S4BIOS state */
    UINT8                   PstateControl;      /* Processor performance state control*/
    UINT32                  Pm1aEventBlock;     /* 32-bit Port address of Power Mgt 1a Event Reg Blk */
    UINT32                  Pm1bEventBlock;     /* 32-bit Port address of Power Mgt 1b Event Reg Blk */
    UINT32                  Pm1aControlBlock;   /* 32-bit Port address of Power Mgt 1a Control Reg Blk */
    UINT32                  Pm1bControlBlock;   /* 32-bit Port address of Power Mgt 1b Control Reg Blk */
    UINT32                  Pm2ControlBlock;    /* 32-bit Port address of Power Mgt 2 Control Reg Blk */
    UINT32                  PmTimerBlock;       /* 32-bit Port address of Power Mgt Timer Ctrl Reg Blk */
    UINT32                  Gpe0Block;          /* 32-bit Port address of General Purpose Event 0 Reg Blk */
    UINT32                  Gpe1Block;          /* 32-bit Port address of General Purpose Event 1 Reg Blk */
    UINT8                   Pm1EventLength;     /* Byte Length of ports at Pm1xEventBlock */
    UINT8                   Pm1ControlLength;   /* Byte Length of ports at Pm1xControlBlock */
    UINT8                   Pm2ControlLength;   /* Byte Length of ports at Pm2ControlBlock */
    UINT8                   PmTimerLength;      /* Byte Length of ports at PmTimerBlock */
    UINT8                   Gpe0BlockLength;    /* Byte Length of ports at Gpe0Block */
    UINT8                   Gpe1BlockLength;    /* Byte Length of ports at Gpe1Block */
    UINT8                   Gpe1Base;           /* Offset in GPE number space where GPE1 events start */
    UINT8                   CstControl;         /* Support for the _CST object and C States change notification */
    UINT16                  C2Latency;          /* Worst case HW latency to enter/exit C2 state */
    UINT16                  C3Latency;          /* Worst case HW latency to enter/exit C3 state */
    UINT16                  FlushSize;          /* Processor's memory cache line width, in bytes */
    UINT16                  FlushStride;        /* Number of flush strides that need to be read */
    UINT8                   DutyOffset;         /* Processor duty cycle index in processor's P_CNT reg*/
    UINT8                   DutyWidth;          /* Processor duty cycle value bit width in P_CNT register.*/
    UINT8                   DayAlarm;           /* Index to day-of-month alarm in RTC CMOS RAM */
    UINT8                   MonthAlarm;         /* Index to month-of-year alarm in RTC CMOS RAM */
    UINT8                   Century;            /* Index to century in RTC CMOS RAM */
    UINT16                  BootFlags;          /* IA-PC Boot Architecture Flags. See Table 5-10 for description */
    UINT8                   Reserved;           /* Reserved, must be zero */
    UINT32                  Flags;              /* Miscellaneous flag bits (see below for individual flags) */
    ACPI_GENERIC_ADDRESS    ResetRegister;      /* 64-bit address of the Reset register */
    UINT8                   ResetValue;         /* Value to write to the ResetRegister port to reset the system */
    UINT8                   Reserved4[3];       /* Reserved, must be zero */
    UINT64                  XFacs;              /* 64-bit physical address of FACS */
    UINT64                  XDsdt;              /* 64-bit physical address of DSDT */
    ACPI_GENERIC_ADDRESS    XPm1aEventBlock;    /* 64-bit Extended Power Mgt 1a Event Reg Blk address */
    ACPI_GENERIC_ADDRESS    XPm1bEventBlock;    /* 64-bit Extended Power Mgt 1b Event Reg Blk address */
    ACPI_GENERIC_ADDRESS    XPm1aControlBlock;  /* 64-bit Extended Power Mgt 1a Control Reg Blk address */
    ACPI_GENERIC_ADDRESS    XPm1bControlBlock;  /* 64-bit Extended Power Mgt 1b Control Reg Blk address */
    ACPI_GENERIC_ADDRESS    XPm2ControlBlock;   /* 64-bit Extended Power Mgt 2 Control Reg Blk address */
    ACPI_GENERIC_ADDRESS    XPmTimerBlock;      /* 64-bit Extended Power Mgt Timer Ctrl Reg Blk address */
    ACPI_GENERIC_ADDRESS    XGpe0Block;         /* 64-bit Extended General Purpose Event 0 Reg Blk address */
    ACPI_GENERIC_ADDRESS    XGpe1Block;         /* 64-bit Extended General Purpose Event 1 Reg Blk address */

} ACPI_TABLE_FADT;


/* FADT flags */

#define ACPI_FADT_WBINVD            (1)         /* 00: The wbinvd instruction works properly */
#define ACPI_FADT_WBINVD_FLUSH      (1<<1)      /* 01: The wbinvd flushes but does not invalidate */
#define ACPI_FADT_C1_SUPPORTED      (1<<2)      /* 02: All processors support C1 state */
#define ACPI_FADT_C2_MP_SUPPORTED   (1<<3)      /* 03: C2 state works on MP system */
#define ACPI_FADT_POWER_BUTTON      (1<<4)      /* 04: Power button is handled as a generic feature */
#define ACPI_FADT_SLEEP_BUTTON      (1<<5)      /* 05: Sleep button is handled as a generic feature, or  not present */
#define ACPI_FADT_FIXED_RTC         (1<<6)      /* 06: RTC wakeup stat not in fixed register space */
#define ACPI_FADT_S4_RTC_WAKE       (1<<7)      /* 07: RTC wakeup stat not possible from S4 */
#define ACPI_FADT_32BIT_TIMER       (1<<8)      /* 08: tmr_val is 32 bits 0=24-bits */
#define ACPI_FADT_DOCKING_SUPPORTED (1<<9)      /* 09: Docking supported */
#define ACPI_FADT_RESET_REGISTER    (1<<10)     /* 10: System reset via the FADT RESET_REG supported */
#define ACPI_FADT_SEALED_CASE       (1<<11)     /* 11: No internal expansion capabilities and case is sealed */
#define ACPI_FADT_HEADLESS          (1<<12)     /* 12: No local video capabilities or local input devices */
#define ACPI_FADT_SLEEP_TYPE        (1<<13)     /* 13: Must execute native instruction after writing  SLP_TYPx register */
#define ACPI_FADT_PCI_EXPRESS_WAKE  (1<<14)     /* 14: System supports PCIEXP_WAKE (STS/EN) bits (ACPI 3.0) */
#define ACPI_FADT_PLATFORM_CLOCK    (1<<15)     /* 15: OSPM should use platform-provided timer (ACPI 3.0) */
#define ACPI_FADT_S4_RTC_VALID      (1<<16)     /* 16: Contents of RTC_STS valid after S4 wake (ACPI 3.0) */
#define ACPI_FADT_REMOTE_POWER_ON   (1<<17)     /* 17: System is compatible with remote power on (ACPI 3.0) */
#define ACPI_FADT_APIC_CLUSTER      (1<<18)     /* 18: All local APICs must use cluster model (ACPI 3.0) */
#define ACPI_FADT_APIC_PHYSICAL     (1<<19)     /* 19: All local xAPICs must use physical dest mode (ACPI 3.0) */


/*
 * FADT Prefered Power Management Profiles
 */
enum AcpiPreferedPmProfiles
{
    PM_UNSPECIFIED          = 0,
    PM_DESKTOP              = 1,
    PM_MOBILE               = 2,
    PM_WORKSTATION          = 3,
    PM_ENTERPRISE_SERVER    = 4,
    PM_SOHO_SERVER          = 5,
    PM_APPLIANCE_PC         = 6
};


/* FADT Boot Arch Flags */

#define BAF_LEGACY_DEVICES              0x0001
#define BAF_8042_KEYBOARD_CONTROLLER    0x0002

#define FADT2_REVISION_ID               3
#define FADT2_MINUS_REVISION_ID         2


/*******************************************************************************
 *
 * Additional ACPI Tables
 *
 * These tables are not consumed directly by the ACPICA subsystem, but are
 * included here to support device drivers and the AML disassembler.
 *
 ******************************************************************************/

/*******************************************************************************
 *
 * Common Sub-table header (used in MADT, SRAT, etc.)
 *
 ******************************************************************************/
typedef struct _ACPI_SUBTABLE_HEADER
{
    UINT8                   Type;
    UINT8                   Length;

} ACPI_SUBTABLE_HEADER;


/*******************************************************************************
 *
 * ASF - Alert Standard Format table (Signature "ASF!")
 *
 * Conforms to the Alert Standard Format Specification V2.0, 23 April 2003
 *
 ******************************************************************************/
typedef struct _ACPI_TABLE_ASF
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */

} ACPI_TABLE_ASF;


/* ASF subtable header */

typedef struct _ACPI_ASF_HEADER
{
    UINT8                   Type;
    UINT8                   Reserved;
    UINT16                  Length;

} ACPI_ASF_HEADER;


/* Values for Type field above */

enum AcpiAsfType
{
    ACPI_ASF_TYPE_INFO          = 0,
    ACPI_ASF_TYPE_ALERT         = 1,
    ACPI_ASF_TYPE_CONTROL       = 2,
    ACPI_ASF_TYPE_BOOT          = 3,
    ACPI_ASF_TYPE_ADDRESS       = 4,
    ACPI_ASF_TYPE_RESERVED      = 5
};

/*
 * ASF subtables
 */

/* 0: ASF Information */

typedef struct _ACPI_ASF_INFO
{
    ACPI_ASF_HEADER         Header;
    UINT8                   MinResetValue;
    UINT8                   MinPollInterval;
    UINT16                  SystemId;
    UINT32                  MfgId;
    UINT8                   Flags;
    UINT8                   Reserved2[3];

} ACPI_ASF_INFO;

/* 1: ASF Alerts */

typedef struct _ACPI_ASF_ALERT
{
    ACPI_ASF_HEADER         Header;
    UINT8                   AssertMask;
    UINT8                   DeassertMask;
    UINT8                   Alerts;
    UINT8                   DataLength;

} ACPI_ASF_ALERT;

typedef struct _ACPI_ASF_ALERT_DATA
{
    UINT8                   Address;
    UINT8                   Command;
    UINT8                   Mask;
    UINT8                   Value;
    UINT8                   SensorType;
    UINT8                   Type;
    UINT8                   Offset;
    UINT8                   SourceType;
    UINT8                   Severity;
    UINT8                   SensorNumber;
    UINT8                   Entity;
    UINT8                   Instance;

} ACPI_ASF_ALERT_DATA;

/* 2: ASF Remote Control */

typedef struct _ACPI_ASF_REMOTE
{
    ACPI_ASF_HEADER         Header;
    UINT8                   Controls;
    UINT8                   DataLength;
    UINT16                  Reserved2;

} ACPI_ASF_REMOTE;

typedef struct _ACPI_ASF_CONTROL_DATA
{
    UINT8                   Function;
    UINT8                   Address;
    UINT8                   Command;
    UINT8                   Value;

} ACPI_ASF_CONTROL_DATA;

/* 3: ASF RMCP Boot Options */

typedef struct _ACPI_ASF_RMCP
{
    ACPI_ASF_HEADER         Header;
    UINT8                   Capabilities[7];
    UINT8                   CompletionCode;
    UINT32                  EnterpriseId;
    UINT8                   Command;
    UINT16                  Parameter;
    UINT16                  BootOptions;
    UINT16                  OemParameters;

} ACPI_ASF_RMCP;

/* 4: ASF Address */

typedef struct _ACPI_ASF_ADDRESS
{
    ACPI_ASF_HEADER         Header;
    UINT8                   EpromAddress;
    UINT8                   Devices;

} ACPI_ASF_ADDRESS;


/*******************************************************************************
 *
 * BOOT - Simple Boot Flag Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_BOOT
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT8                   CmosIndex;          /* Index in CMOS RAM for the boot register */
    UINT8                   Reserved[3];

} ACPI_TABLE_BOOT;


/*******************************************************************************
 *
 * CPEP - Corrected Platform Error Polling table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_CPEP
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT64                  Reserved;

} ACPI_TABLE_CPEP;


/* Subtable */

typedef struct _ACPI_CPEP_POLLING
{
    UINT8                   Type;
    UINT8                   Length;
    UINT8                   Id;                 /* Processor ID */
    UINT8                   Eid;                /* Processor EID */
    UINT32                  Interval;           /* Polling interval (msec) */

} ACPI_CPEP_POLLING;


/*******************************************************************************
 *
 * DBGP - Debug Port table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_DBGP
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT8                   Type;               /* 0=full 16550, 1=subset of 16550 */
    UINT8                   Reserved[3];
    ACPI_GENERIC_ADDRESS    DebugPort;

} ACPI_TABLE_DBGP;


/*******************************************************************************
 *
 * DMAR - DMA Remapping table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_DMAR
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT8                   Width;              /* Host Address Width */
    UINT8                   Reserved[11];

} ACPI_TABLE_DMAR;

/* DMAR subtable header */

typedef struct _ACPI_DMAR_HEADER
{
    UINT16                  Type;
    UINT16                  Length;
    UINT8                   Flags;
    UINT8                   Reserved[3];

} ACPI_DMAR_HEADER;

/* Values for subtable type in ACPI_DMAR_HEADER */

enum AcpiDmarType
{
    ACPI_DMAR_TYPE_HARDWARE_UNIT        = 0,
    ACPI_DMAR_TYPE_RESERVED_MEMORY      = 1,
    ACPI_DMAR_TYPE_RESERVED             = 2     /* 2 and greater are reserved */
};

typedef struct _ACPI_DMAR_DEVICE_SCOPE
{
    UINT8                   EntryType;
    UINT8                   Length;
    UINT8                   Segment;
    UINT8                   Bus;

} ACPI_DMAR_DEVICE_SCOPE;

/* Values for EntryType in ACPI_DMAR_DEVICE_SCOPE */

enum AcpiDmarScopeType
{
    ACPI_DMAR_SCOPE_TYPE_NOT_USED       = 0,
    ACPI_DMAR_SCOPE_TYPE_ENDPOINT       = 1,
    ACPI_DMAR_SCOPE_TYPE_BRIDGE         = 2,
    ACPI_DMAR_SCOPE_TYPE_RESERVED       = 3     /* 3 and greater are reserved */
};


/*
 * DMAR Sub-tables, correspond to Type in ACPI_DMAR_HEADER
 */

/* 0: Hardware Unit Definition */

typedef struct _ACPI_DMAR_HARDWARE_UNIT
{
    ACPI_DMAR_HEADER        Header;
    UINT64                  Address;            /* Register Base Address */

} ACPI_DMAR_HARDWARE_UNIT;

/* Flags */

#define ACPI_DMAR_INCLUDE_ALL       (1)

/* 1: Reserved Memory Defininition */

typedef struct _ACPI_DMAR_RESERVED_MEMORY
{
    ACPI_DMAR_HEADER        Header;
    UINT64                  Address;            /* 4K aligned base address */
    UINT64                  EndAddress;         /* 4K aligned limit address */

} ACPI_DMAR_RESERVED_MEMORY;

/* Flags */

#define ACPI_DMAR_ALLOW_ALL         (1)


/*******************************************************************************
 *
 * ECDT - Embedded Controller Boot Resources Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_ECDT
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    ACPI_GENERIC_ADDRESS    Control;            /* Address of EC command/status register */
    ACPI_GENERIC_ADDRESS    Data;               /* Address of EC data register */
    UINT32                  Uid;                /* Unique ID - must be same as the EC _UID method */
    UINT8                   Gpe;                /* The GPE for the EC */
    UINT8                   Id[1];              /* Full namepath of the EC in the ACPI namespace */

} ACPI_TABLE_ECDT;


/*******************************************************************************
 *
 * HPET - High Precision Event Timer table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_HPET
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT32                  Id;                 /* Hardware ID of event timer block */
    ACPI_GENERIC_ADDRESS    Address;            /* Address of event timer block */
    UINT8                   Sequence;           /* HPET sequence number */
    UINT16                  MinimumTick;        /* Main counter min tick, periodic mode */
    UINT8                   Flags;

} ACPI_TABLE_HPET;

/*! Flags */

#define ACPI_HPET_PAGE_PROTECT      (1)         /* 00: No page protection */
#define ACPI_HPET_PAGE_PROTECT_4    (1<<1)      /* 01: 4KB page protected */
#define ACPI_HPET_PAGE_PROTECT_64   (1<<2)      /* 02: 64KB page protected */

/*! [End] no source code translation !*/


/*******************************************************************************
 *
 * MADT - Multiple APIC Description Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_MADT
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT32                  Address;            /* Physical address of local APIC */
    UINT32                  Flags;

} ACPI_TABLE_MADT;

/* Flags */

#define ACPI_MADT_PCAT_COMPAT       (1)         /* 00:    System also has dual 8259s */

/* Values for PCATCompat flag */

#define ACPI_MADT_DUAL_PIC          0
#define ACPI_MADT_MULTIPLE_APIC     1


/* Values for subtable type in ACPI_SUBTABLE_HEADER */

enum AcpiMadtType
{
    ACPI_MADT_TYPE_LOCAL_APIC           = 0,
    ACPI_MADT_TYPE_IO_APIC              = 1,
    ACPI_MADT_TYPE_INTERRUPT_OVERRIDE   = 2,
    ACPI_MADT_TYPE_NMI_SOURCE           = 3,
    ACPI_MADT_TYPE_LOCAL_APIC_NMI       = 4,
    ACPI_MADT_TYPE_LOCAL_APIC_OVERRIDE  = 5,
    ACPI_MADT_TYPE_IO_SAPIC             = 6,
    ACPI_MADT_TYPE_LOCAL_SAPIC          = 7,
    ACPI_MADT_TYPE_INTERRUPT_SOURCE     = 8,
    ACPI_MADT_TYPE_RESERVED             = 9     /* 9 and greater are reserved */
};


/*
 * MADT Sub-tables, correspond to Type in ACPI_SUBTABLE_HEADER
 */

/* 0: Processor Local APIC */

typedef struct _ACPI_MADT_LOCAL_APIC
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   ProcessorId;        /* ACPI processor id */
    UINT8                   Id;                 /* Processor's local APIC id */
    UINT32                  LapicFlags;

} ACPI_MADT_LOCAL_APIC;

/* 1: IO APIC */

typedef struct _ACPI_MADT_IO_APIC
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   Id;                 /* I/O APIC ID */
    UINT8                   Reserved;           /* Reserved - must be zero */
    UINT32                  Address;            /* APIC physical address */
    UINT32                  GlobalIrqBase;      /* Global system interrupt where INTI lines start */

} ACPI_MADT_IO_APIC;

/* 2: Interrupt Override */

typedef struct _ACPI_MADT_INTERRUPT_OVERRIDE
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   Bus;                /* 0 - ISA */
    UINT8                   SourceIrq;          /* Interrupt source (IRQ) */
    UINT32                  GlobalIrq;          /* Global system interrupt */
    UINT16                  IntiFlags;

} ACPI_MADT_INTERRUPT_OVERRIDE;

/* 3: NMI Source */

typedef struct _ACPI_MADT_NMI_SOURCE
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT16                  IntiFlags;
    UINT32                  GlobalIrq;          /* Global system interrupt */

} ACPI_MADT_NMI_SOURCE;

/* 4: Local APIC NMI */

typedef struct _ACPI_MADT_LOCAL_APIC_NMI
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   ProcessorId;        /* ACPI processor id */
    UINT16                  IntiFlags;
    UINT8                   Lint;               /* LINTn to which NMI is connected */

} ACPI_MADT_LOCAL_APIC_NMI;

/* 5: Address Override */

typedef struct _ACPI_MADT_LOCAL_APIC_OVERRIDE
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT16                  Reserved;           /* Reserved, must be zero */
    UINT64                  Address;            /* APIC physical address */

} ACPI_MADT_LOCAL_APIC_OVERRIDE;

/* 6: I/O Sapic */

typedef struct _ACPI_MADT_IO_SAPIC
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   Id;                 /* I/O SAPIC ID */
    UINT8                   Reserved;           /* Reserved, must be zero */
    UINT32                  GlobalIrqBase;      /* Global interrupt for SAPIC start */
    UINT64                  Address;            /* SAPIC physical address */

} ACPI_MADT_IO_SAPIC;

/* 7: Local Sapic */

typedef struct _ACPI_MADT_LOCAL_SAPIC
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   ProcessorId;        /* ACPI processor id */
    UINT8                   Id;                 /* SAPIC ID */
    UINT8                   Eid;                /* SAPIC EID */
    UINT8                   Reserved[3];        /* Reserved, must be zero */
    UINT32                  LapicFlags;
    UINT32                  Uid;                /* Numeric UID - ACPI 3.0 */
    CHAR                    UidString[1];       /* String UID  - ACPI 3.0 */

} ACPI_MADT_LOCAL_SAPIC;

/* 8: Platform Interrupt Source */

typedef struct _ACPI_MADT_INTERRUPT_SOURCE
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT16                  IntiFlags;
    UINT8                   Type;               /* 1=PMI, 2=INIT, 3=corrected */
    UINT8                   Id;                 /* Processor ID */
    UINT8                   Eid;                /* Processor EID */
    UINT8                   IoSapicVector;      /* Vector value for PMI interrupts */
    UINT32                  GlobalIrq;          /* Global system interrupt */
    UINT32                  Flags;              /* Interrupt Source Flags */

} ACPI_MADT_INTERRUPT_SOURCE;

/* Flags field above */

#define ACPI_MADT_CPEI_OVERRIDE     (1)


/*
 * Common flags fields for MADT subtables
 */

/* MADT Local APIC flags (LapicFlags) */

#define ACPI_MADT_ENABLED           (1)         /* 00: Processor is usable if set */

/* MADT MPS INTI flags (IntiFlags) */

#define ACPI_MADT_POLARITY_MASK     (3)         /* 00-01: Polarity of APIC I/O input signals */
#define ACPI_MADT_TRIGGER_MASK      (3<<2)      /* 02-03: Trigger mode of APIC input signals */

/* Values for MPS INTI flags */

#define ACPI_MADT_POLARITY_CONFORMS       0
#define ACPI_MADT_POLARITY_ACTIVE_HIGH    1
#define ACPI_MADT_POLARITY_RESERVED       2
#define ACPI_MADT_POLARITY_ACTIVE_LOW     3

#define ACPI_MADT_TRIGGER_CONFORMS        (0)
#define ACPI_MADT_TRIGGER_EDGE            (1<<2)
#define ACPI_MADT_TRIGGER_RESERVED        (2<<2)
#define ACPI_MADT_TRIGGER_LEVEL           (3<<2)



/*******************************************************************************
 *
 * MCFG - PCI Memory Mapped Configuration table and sub-table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_MCFG
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT8                   Reserved[8];

} ACPI_TABLE_MCFG;


/* Subtable */

typedef struct _ACPI_MCFG_ALLOCATION
{
    UINT64                  Address;            /* Base address, processor-relative */
    UINT16                  PciSegment;         /* PCI segment group number */
    UINT8                   StartBusNumber;     /* Starting PCI Bus number */
    UINT8                   EndBusNumber;       /* Final PCI Bus number */
    UINT32                  Reserved;

} ACPI_MCFG_ALLOCATION;


/*******************************************************************************
 *
 * SBST - Smart Battery Specification Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_SBST
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT32                  WarningLevel;
    UINT32                  LowLevel;
    UINT32                  CriticalLevel;

} ACPI_TABLE_SBST;


/*******************************************************************************
 *
 * SLIT - System Locality Distance Information Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_SLIT
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT64                  LocalityCount;
    UINT8                   Entry[1];           /* Real size = localities^2 */

} ACPI_TABLE_SLIT;


/*******************************************************************************
 *
 * SPCR - Serial Port Console Redirection table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_SPCR
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT8                   InterfaceType;      /* 0=full 16550, 1=subset of 16550 */
    UINT8                   Reserved[3];
    ACPI_GENERIC_ADDRESS    SerialPort;
    UINT8                   InterruptType;
    UINT8                   PcInterrupt;
    UINT32                  Interrupt;
    UINT8                   BaudRate;
    UINT8                   Parity;
    UINT8                   StopBits;
    UINT8                   FlowControl;
    UINT8                   TerminalType;
    UINT8                   Reserved1;
    UINT16                  PciDeviceId;
    UINT16                  PciVendorId;
    UINT8                   PciBus;
    UINT8                   PciDevice;
    UINT8                   PciFunction;
    UINT32                  PciFlags;
    UINT8                   PciSegment;
    UINT32                  Reserved2;

} ACPI_TABLE_SPCR;


/*******************************************************************************
 *
 * SPMI - Server Platform Management Interface table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_SPMI
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT8                   Reserved;
    UINT8                   InterfaceType;
    UINT16                  SpecRevision;       /* Version of IPMI */
    UINT8                   InterruptType;
    UINT8                   GpeNumber;          /* GPE assigned */
    UINT8                   Reserved1;
    UINT8                   PciDeviceFlag;
    UINT32                  Interrupt;
    ACPI_GENERIC_ADDRESS    IpmiRegister;
    UINT8                   PciSegment;
    UINT8                   PciBus;
    UINT8                   PciDevice;
    UINT8                   PciFunction;

} ACPI_TABLE_SPMI;


/*******************************************************************************
 *
 * SRAT - System Resource Affinity Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_SRAT
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT32                  TableRevision;      /* Must be value '1' */
    UINT64                  Reserved;           /* Reserved, must be zero */

} ACPI_TABLE_SRAT;

/* Values for subtable type in ACPI_SUBTABLE_HEADER */

enum AcpiSratType
{
    ACPI_SRAT_TYPE_CPU_AFFINITY     = 0,
    ACPI_SRAT_TYPE_MEMORY_AFFINITY  = 1,
    ACPI_SRAT_TYPE_RESERVED         = 2
};

/* SRAT sub-tables */

typedef struct _ACPI_SRAT_CPU_AFFINITY
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT8                   ProximityDomainLo;
    UINT8                   ApicId;
    UINT32                  Flags;
    UINT8                   LocalSapicEid;
    UINT8                   ProximityDomainHi[3];
    UINT32                  Reserved;           /* Reserved, must be zero */

} ACPI_SRAT_CPU_AFFINITY;

/* Flags */

#define ACPI_SRAT_CPU_ENABLED       (1)         /* 00: Use affinity structure */


typedef struct _ACPI_SRAT_MEM_AFFINITY
{
    ACPI_SUBTABLE_HEADER    Header;
    UINT32                  ProximityDomain;
    UINT16                  Reserved;           /* Reserved, must be zero */
    UINT64                  BaseAddress;
    UINT64                  Length;
    UINT32                  MemoryType;         /* See acpi_address_range_id */
    UINT32                  Flags;
    UINT64                  Reserved1;          /* Reserved, must be zero */

} ACPI_SRAT_MEM_AFFINITY;

/* Flags */

#define ACPI_SRAT_MEM_ENABLED       (1)         /* 00: Use affinity structure */
#define ACPI_SRAT_MEM_HOT_PLUGGABLE (1<<1)      /* 01: Memory region is hot pluggable */
#define ACPI_SRAT_MEM_NON_VOLATILE  (1<<2)      /* 02: Memory region is non-volatile */

/* Memory types */

enum acpi_address_range_id
{
    ACPI_ADDRESS_RANGE_MEMORY   = 1,
    ACPI_ADDRESS_RANGE_RESERVED = 2,
    ACPI_ADDRESS_RANGE_ACPI     = 3,
    ACPI_ADDRESS_RANGE_NVS      = 4,
    ACPI_ADDRESS_RANGE_COUNT    = 5
};


/*******************************************************************************
 *
 * TCPA - Trusted Computing Platform Alliance table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_TCPA
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT16                  Reserved;
    UINT32                  MaxLogLength;       /* Maximum length for the event log area */
    UINT64                  LogAddress;         /* Address of the event log area */

} ACPI_TABLE_TCPA;


/*******************************************************************************
 *
 * WDRT - Watchdog Resource Table
 *
 ******************************************************************************/

typedef struct _ACPI_TABLE_WDRT
{
    ACPI_TABLE_HEADER       Header;             /* Common ACPI table header */
    UINT32                  HeaderLength;       /* Watchdog Header Length */
    UINT8                   PciSegment;         /* PCI Segment number */
    UINT8                   PciBus;             /* PCI Bus number */
    UINT8                   PciDevice;          /* PCI Device number */
    UINT8                   PciFunction;        /* PCI Function number */
    UINT32                  TimerPeriod;        /* Period of one timer count (msec) */
    UINT32                  MaxCount;           /* Maximum counter value supported */
    UINT32                  MinCount;           /* Minimum counter value */
    UINT8                   Flags;
    UINT8                   Reserved[3];
    UINT32                  Entries;            /* Number of watchdog entries that follow */

} ACPI_TABLE_WDRT;

/* Flags */

#define ACPI_WDRT_TIMER_ENABLED     (1)         /* 00: Timer enabled */

#pragma pack(pop)

#endif // TABLES_H_INCLUDED
///////////////////////////////////////////////////////////////////////////////////
//------------------------------------Tables.h-----------------------------------//
//--------------------------Copyright © Sidhant Goel, 2007-----------------------//
///////////////////////////////////////////////////////////////////////////////////
