#ifndef _WINDOWS_H
#define _WINDOWS_H

#include "_macros.h"

#pragma pack(push,1)

//
// Image Format
//


#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_OS2_SIGNATURE                 0x454E      // NE
#define IMAGE_OS2_SIGNATURE_LE              0x454C      // LE
#define IMAGE_VXD_SIGNATURE                 0x454C      // LE
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00


typedef struct _IMAGE_DOS_HEADER        // DOS .EXE header
{
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid specific
    WORD   e_res2[10];                  // Reserved words
    unsigned int   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;


typedef struct _IMAGE_VXD_HEADER        // Windows VXD header
{
    WORD   e32_magic;                   // Magic number
    BYTE   e32_border;                  // The byte ordering for the VXD
    BYTE   e32_worder;                  // The word ordering for the VXD
    unsigned int  e32_level;                   // The EXE format level for now = 0
    WORD   e32_cpu;                     // The CPU type
    WORD   e32_os;                      // The OS type
    unsigned int  e32_ver;                     // Module version
    unsigned int  e32_mflags;                  // Module flags
    unsigned int  e32_mpages;                  // Module # pages
    unsigned int  e32_startobj;                // Object # for instruction pointer
    unsigned int  e32_eip;                     // Extended instruction pointer
    unsigned int  e32_stackobj;                // Object # for stack pointer
    unsigned int  e32_esp;                     // Extended stack pointer
    unsigned int  e32_pagesize;                // VXD page size
    unsigned int  e32_lastpagesize;            // Last page size in VXD
    unsigned int  e32_fixupsize;               // Fixup section size
    unsigned int  e32_fixupsum;                // Fixup section checksum
    unsigned int  e32_ldrsize;                 // Loader section size
    unsigned int  e32_ldrsum;                  // Loader section checksum
    unsigned int  e32_objtab;                  // Object table offset
    unsigned int  e32_objcnt;                  // Number of objects in module
    unsigned int  e32_objmap;                  // Object page map offset
    unsigned int  e32_itermap;                 // Object iterated data map offset
    unsigned int  e32_rsrctab;                 // Offset of Resource Table
    unsigned int  e32_rsrccnt;                 // Number of resource entries
    unsigned int  e32_restab;                  // Offset of resident name table
    unsigned int  e32_enttab;                  // Offset of Entry Table
    unsigned int  e32_dirtab;                  // Offset of Module Directive Table
    unsigned int  e32_dircnt;                  // Number of module directives
    unsigned int  e32_fpagetab;                // Offset of Fixup Page Table
    unsigned int  e32_frectab;                 // Offset of Fixup Record Table
    unsigned int  e32_impmod;                  // Offset of Import Module Name Table
    unsigned int  e32_impmodcnt;               // Number of entries in Import Module Name Table
    unsigned int  e32_impproc;                 // Offset of Import Procedure Name Table
    unsigned int  e32_pagesum;                 // Offset of Per-Page Checksum Table
    unsigned int  e32_datapage;                // Offset of Enumerated Data Pages
    unsigned int  e32_preload;                 // Number of preload pages
    unsigned int  e32_nrestab;                 // Offset of Non-resident Names Table
    unsigned int  e32_cbnrestab;               // Size of Non-resident Name Table
    unsigned int  e32_nressum;                 // Non-resident Name Table Checksum
    unsigned int  e32_autodata;                // Object # for automatic data object
    unsigned int  e32_debuginfo;               // Offset of the debugging information
    unsigned int  e32_debuglen;                // The length of the debugging info. in bytes
    unsigned int  e32_instpreload;             // Number of instance pages in preload section of VXD file
    unsigned int  e32_instdemand;              // Number of instance pages in demand load section of VXD file
    unsigned int  e32_heapsize;                // Size of heap - for 16-bit apps
    BYTE   e32_res3[12];                // Reserved words
    unsigned int  e32_winresoff;
    unsigned int  e32_winreslen;
    WORD   e32_devid;                   // Device ID for VxD
    WORD   e32_ddkver;                  // DDK version for VxD
} IMAGE_VXD_HEADER, *PIMAGE_VXD_HEADER;

//
// File header format.
//

typedef struct _IMAGE_FILE_HEADER
{
    WORD    Machine;
    WORD    NumberOfSections;
    unsigned int   TimeDateStamp;
    unsigned int   PointerToSymbolTable;
    unsigned int   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

#define IMAGE_SIZEOF_FILE_HEADER             20


#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved external references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line numbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Aggressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define IMAGE_FILE_MACHINE_THUMB             0x01c2
#define IMAGE_FILE_MACHINE_AM33              0x01d3
#define IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define IMAGE_FILE_MACHINE_CEF               0x0CEF
#define IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define IMAGE_FILE_MACHINE_CEE               0xC0EE

//
// Directory format.
//

typedef struct _IMAGE_DATA_DIRECTORY
{
    unsigned int   VirtualAddress;
    unsigned int   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

//
// Optional header format.
//

typedef struct _IMAGE_OPTIONAL_HEADER
{
    //
    // Standard fields.
    //

    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    unsigned int   SizeOfCode;
    unsigned int   SizeOfInitializedData;
    unsigned int   SizeOfUninitializedData;
    unsigned int   AddressOfEntryPoint;
    unsigned int   BaseOfCode;
    unsigned int   BaseOfData;

    //
    // NT additional fields.
    //

    unsigned int   ImageBase;
    unsigned int   SectionAlignment;
    unsigned int   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    unsigned int   Win32VersionValue;
    unsigned int   SizeOfImage;
    unsigned int   SizeOfHeaders;
    unsigned int   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    unsigned int   SizeOfStackReserve;
    unsigned int   SizeOfStackCommit;
    unsigned int   SizeOfHeapReserve;
    unsigned int   SizeOfHeapCommit;
    unsigned int   LoaderFlags;
    unsigned int   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64
{
    WORD        Magic;
    BYTE        MajorLinkerVersion;
    BYTE        MinorLinkerVersion;
    unsigned int       SizeOfCode;
    unsigned int       SizeOfInitializedData;
    unsigned int       SizeOfUninitializedData;
    unsigned int       AddressOfEntryPoint;
    unsigned int       BaseOfCode;
    unsigned long long   ImageBase;
    unsigned int       SectionAlignment;
    unsigned int       FileAlignment;
    WORD        MajorOperatingSystemVersion;
    WORD        MinorOperatingSystemVersion;
    WORD        MajorImageVersion;
    WORD        MinorImageVersion;
    WORD        MajorSubsystemVersion;
    WORD        MinorSubsystemVersion;
    unsigned int       Win32VersionValue;
    unsigned int       SizeOfImage;
    unsigned int       SizeOfHeaders;
    unsigned int       CheckSum;
    WORD        Subsystem;
    WORD        DllCharacteristics;
    unsigned long long   SizeOfStackReserve;
    unsigned long long   SizeOfStackCommit;
    unsigned long long   SizeOfHeapReserve;
    unsigned long long   SizeOfHeapCommit;
    unsigned int       LoaderFlags;
    unsigned int       NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_ROM_OPTIONAL_HEADER
{
    WORD   Magic;
    BYTE   MajorLinkerVersion;
    BYTE   MinorLinkerVersion;
    unsigned int  SizeOfCode;
    unsigned int  SizeOfInitializedData;
    unsigned int  SizeOfUninitializedData;
    unsigned int  AddressOfEntryPoint;
    unsigned int  BaseOfCode;
    unsigned int  BaseOfData;
    unsigned int  BaseOfBss;
    unsigned int  GprMask;
    unsigned int  CprMask[4];
    unsigned int  GpValue;
} IMAGE_ROM_OPTIONAL_HEADER, *PIMAGE_ROM_OPTIONAL_HEADER;



#define IMAGE_SIZEOF_ROM_OPTIONAL_HEADER      56
#define IMAGE_SIZEOF_STD_OPTIONAL_HEADER      28
#define IMAGE_SIZEOF_NT_OPTIONAL32_HEADER    224
#define IMAGE_SIZEOF_NT_OPTIONAL64_HEADER    240

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC      0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC       0x107

typedef IMAGE_OPTIONAL_HEADER32             IMAGE_OPTIONAL_HEADER;
typedef PIMAGE_OPTIONAL_HEADER32            PIMAGE_OPTIONAL_HEADER;
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER     IMAGE_SIZEOF_NT_OPTIONAL32_HEADER
#define IMAGE_NT_OPTIONAL_HDR_MAGIC         IMAGE_NT_OPTIONAL_HDR32_MAGIC


typedef struct _IMAGE_NT_HEADERS
{
    unsigned int Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER32 OptionalHeader;
} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;

typedef struct _IMAGE_ROM_HEADERS
{
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_ROM_OPTIONAL_HEADER OptionalHeader;
} IMAGE_ROM_HEADERS, *PIMAGE_ROM_HEADERS;

typedef IMAGE_NT_HEADERS32                  IMAGE_NT_HEADERS;
typedef PIMAGE_NT_HEADERS32                 PIMAGE_NT_HEADERS;


// Subsystem Values

#define IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
#define IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
#define IMAGE_SUBSYSTEM_EFI_ROM              13
#define IMAGE_SUBSYSTEM_XBOX                 14

// DllCharacteristics Entries

//      IMAGE_LIBRARY_PROCESS_INIT           0x0001     // Reserved.
//      IMAGE_LIBRARY_PROCESS_TERM           0x0002     // Reserved.
//      IMAGE_LIBRARY_THREAD_INIT            0x0004     // Reserved.
//      IMAGE_LIBRARY_THREAD_TERM            0x0008     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION 0x0200    // Image understands isolation and doesn't want it
#define IMAGE_DLLCHARACTERISTICS_NO_SEH      0x0400     // Image does not use SEH.  No SE handler may reside in this image
#define IMAGE_DLLCHARACTERISTICS_NO_BIND     0x0800     // Do not bind this image.
//                                           0x1000     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER  0x2000     // Driver uses WDM model
//                                           0x4000     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE     0x8000

// Directory Entries

#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory
//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP
#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory
#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers
#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table
#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors
#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor



//
// Section header format.
//

#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _IMAGE_SECTION_HEADER
{
    BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    union
    {
        unsigned int   PhysicalAddress;
        unsigned int   VirtualSize;
    } Misc;
    unsigned int   VirtualAddress;
    unsigned int   SizeOfRawData;
    unsigned int   PointerToRawData;
    unsigned int   PointerToRelocations;
    unsigned int   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    unsigned int   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

#define IMAGE_SIZEOF_SECTION_HEADER          40

//
// Section characteristics.
//
//      IMAGE_SCN_TYPE_REG                   0x00000000  // Reserved.
//      IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
//      IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
//      IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  // Reserved.
//      IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.

#define IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define IMAGE_SCN_LNK_OTHER                  0x00000100  // Reserved.
#define IMAGE_SCN_LNK_INFO                   0x00000200  // Section contains comments or some other type of information.
//      IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  // Section contents comdat.
//                                           0x00002000  // Reserved.
//      IMAGE_SCN_MEM_PROTECTED - Obsolete   0x00004000
#define IMAGE_SCN_NO_DEFER_SPEC_EXC          0x00004000  // Reset speculative exceptions handling bits in the TLB entries for this section.
#define IMAGE_SCN_GPREL                      0x00008000  // Section content can be accessed relative to GP
#define IMAGE_SCN_MEM_FARDATA                0x00008000
//      IMAGE_SCN_MEM_SYSHEAP  - Obsolete    0x00010000
#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000  //
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000  //
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000  //
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000  //
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000  //
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000  //
#define IMAGE_SCN_ALIGN_128BYTES             0x00800000  //
#define IMAGE_SCN_ALIGN_256BYTES             0x00900000  //
#define IMAGE_SCN_ALIGN_512BYTES             0x00A00000  //
#define IMAGE_SCN_ALIGN_1024BYTES            0x00B00000  //
#define IMAGE_SCN_ALIGN_2048BYTES            0x00C00000  //
#define IMAGE_SCN_ALIGN_4096BYTES            0x00D00000  //
#define IMAGE_SCN_ALIGN_8192BYTES            0x00E00000  //
// Unused                                    0x00F00000
#define IMAGE_SCN_ALIGN_MASK                 0x00F00000

#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE            0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED                 0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                   0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                  0x80000000  // Section is writeable.

typedef struct _IMAGE_IMPORT_DESCRIPTOR
{
    union
    {
        unsigned int   Characteristics;            // 0 for terminating null import descriptor
        unsigned int   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
    };
    unsigned int   TimeDateStamp;                  // 0 if not bound,
    // -1 if bound, and real date\time stamp
    //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
    // O.W. date/time stamp of DLL bound to (Old BIND)

    unsigned int   ForwarderChain;                 // -1 if no forwarders
    unsigned int   Name;
    unsigned int   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;


//
// Resource Format.
//
/* Predefined resource types */
#define    RT_NEWRESOURCE      0x2000
#define    RT_ERROR            0x7fff
#define    RT_CURSOR           1
#define    RT_BITMAP           2
#define    RT_ICON             3
#define    RT_MENU             4
#define    RT_DIALOG           5
#define    RT_STRING           6
#define    RT_FONTDIR          7
#define    RT_FONT             8
#define    RT_ACCELERATORS     9
#define    RT_RCDATA           10
#define    RT_MESSAGETABLE     11
#define    RT_GROUP_CURSOR     12
#define    RT_GROUP_ICON       14
#define    RT_VERSION          16
#define    RT_NEWBITMAP        (RT_BITMAP|RT_NEWRESOURCE)
#define    RT_NEWMENU          (RT_MENU|RT_NEWRESOURCE)
#define    RT_NEWDIALOG        (RT_DIALOG|RT_NEWRESOURCE)
//
// Resource directory consists of two counts, following by a variable length
// array of directory entries.  The first count is the number of entries at
// beginning of the array that have actual names associated with each entry.
// The entries are in ascending order, case insensitive strings.  The second
// count is the number of entries that immediately follow the named entries.
// This second count identifies the number of entries that have 16-bit integer
// Ids as their name.  These entries are also sorted in ascending order.
//
// This structure allows fast lookup by either name or number, but for any
// given resource entry only one form of lookup is supported, not both.
// This is consistent with the syntax of the .RC file and the .RES file.
//

typedef struct _IMAGE_RESOURCE_DIRECTORY
{
    unsigned int   Characteristics;
    unsigned int   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    WORD    NumberOfNamedEntries;
    WORD    NumberOfIdEntries;
    //  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

#define IMAGE_RESOURCE_NAME_IS_STRING        0x80000000
#define IMAGE_RESOURCE_DATA_IS_DIRECTORY     0x80000000
//
// Each directory contains the 32-bit Name of the entry and an offset,
// relative to the beginning of the resource directory of the data associated
// with this directory entry.  If the name of the entry is an actual text
// string instead of an integer Id, then the high order bit of the name field
// is set to one and the low order 31-bits are an offset, relative to the
// beginning of the resource directory of the string, which is of type
// IMAGE_RESOURCE_DIRECTORY_STRING.  Otherwise the high bit is clear and the
// low-order 16-bits are the integer Id that identify this resource directory
// entry. If the directory entry is yet another resource directory (i.e. a
// subdirectory), then the high order bit of the offset field will be
// set to indicate this.  Otherwise the high bit is clear and the offset
// field points to a resource data entry.
//

typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY
{
    union
    {
        struct
        {
            unsigned int NameOffset:31;
            unsigned int NameIsString:1;
        };
        unsigned int   Name;
        WORD    Id;
    };
    union
    {
        unsigned int   OffsetToData;
        struct
        {
            unsigned int   OffsetToDirectory:31;
            unsigned int   DataIsDirectory:1;
        };
    };
} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

//
// For resource directory entries that have actual string names, the Name
// field of the directory entry points to an object of the following type.
// All of these string objects are stored together after the last resource
// directory entry and before the first resource data object.  This minimizes
// the impact of these variable length objects on the alignment of the fixed
// size directory entry objects.
//

typedef struct _IMAGE_RESOURCE_DIRECTORY_STRING
{
    WORD    Length;
    CHAR    NameString[ 1 ];
} IMAGE_RESOURCE_DIRECTORY_STRING, *PIMAGE_RESOURCE_DIRECTORY_STRING;


typedef struct _IMAGE_RESOURCE_DIR_STRING_U
{
    WORD    Length;
    WCHAR   NameString[ 1 ];
} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;


//
// Each resource data entry describes a leaf node in the resource directory
// tree.  It contains an offset, relative to the beginning of the resource
// directory of the data for the resource, a size field that gives the number
// of bytes of data at that offset, a CodePage that should be used when
// decoding code point values within the resource data.  Typically for new
// applications the code page would be the unicode code page.
//

typedef struct _IMAGE_RESOURCE_DATA_ENTRY
{
    unsigned int   OffsetToData;
    unsigned int   Size;
    unsigned int   CodePage;
    unsigned int   Reserved;
} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;


//
// Based relocation format.
//

typedef struct _IMAGE_BASE_RELOCATION
{
    unsigned int   VirtualAddress;
    unsigned int   SizeOfBlock;
    //  WORD    TypeOffset[1];
} IMAGE_BASE_RELOCATION;

#define IMAGE_SIZEOF_BASE_RELOCATION         8

//
// Based relocation types.
//

#define IMAGE_REL_BASED_ABSOLUTE              0
#define IMAGE_REL_BASED_HIGH                  1
#define IMAGE_REL_BASED_LOW                   2
#define IMAGE_REL_BASED_HIGHLOW               3
#define IMAGE_REL_BASED_HIGHADJ               4
#define IMAGE_REL_BASED_MIPS_JMPADDR          5
#define IMAGE_REL_BASED_MIPS_JMPADDR16        9
#define IMAGE_REL_BASED_IA64_IMM64            9
#define IMAGE_REL_BASED_DIR64                 10

typedef struct _IMAGE_EXPORT_DIRECTORY
{
    unsigned int   Characteristics;
    unsigned int   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    unsigned int   Name;
    unsigned int   Base;
    unsigned int   NumberOfFunctions;
    unsigned int   NumberOfNames;
    unsigned int   AddressOfFunctions;     // RVA from base of image
    unsigned int   AddressOfNames;         // RVA from base of image
    unsigned int   AddressOfNameOrdinals;  // RVA from base of image
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

// DllCharacteristics Entries

//      IMAGE_LIBRARY_PROCESS_INIT            0x0001     // Reserved.
//      IMAGE_LIBRARY_PROCESS_TERM            0x0002     // Reserved.
//      IMAGE_LIBRARY_THREAD_INIT             0x0004     // Reserved.
//      IMAGE_LIBRARY_THREAD_TERM             0x0008     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE 0x0040     // DLL can move.
#define IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY    0x0080     // Code Integrity Image
#define IMAGE_DLLCHARACTERISTICS_NX_COMPAT    0x0100     // Image is NX compatible
#define IMAGE_DLLCHARACTERISTICS_NO_ISOLATION 0x0200     // Image understands isolation and doesn't want it
#define IMAGE_DLLCHARACTERISTICS_NO_SEH       0x0400     // Image does not use SEH.  No SE handler may reside in this image
#define IMAGE_DLLCHARACTERISTICS_NO_BIND      0x0800     // Do not bind this image.
//                                            0x1000     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_WDM_DRIVER   0x2000     // Driver uses WDM model
//                                            0x4000     // Reserved.
#define IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE     0x8000

typedef struct _IMAGE_NT_HEADERS64
{
    unsigned int Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

// COM+ 2.0 header structure.

typedef struct IMAGE_COR20_HEADER
{
    // Header versioning

    unsigned int                   cb;
    WORD                    MajorRuntimeVersion;
    WORD                    MinorRuntimeVersion;
    // Symbol table and startup information

    IMAGE_DATA_DIRECTORY    MetaData;
    unsigned int                   Flags;
    // DDBLD - Added next section to replace following lin

    // DDBLD - Still verifying, since not in NT SDK

    // unsigned int                   EntryPointToken;

    // If COMIMAGE_FLAGS_NATIVE_ENTRYPOINT is not set,

    // EntryPointToken represents a managed entrypoint.

    // If COMIMAGE_FLAGS_NATIVE_ENTRYPOINT is set,

    // EntryPointRVA represents an RVA to a native entrypoint.

    union
    {
        unsigned int               EntryPointToken;
        unsigned int               EntryPointRVA;
    };
    // DDBLD - End of Added Area


    // Binding information

    IMAGE_DATA_DIRECTORY    Resources;
    IMAGE_DATA_DIRECTORY    StrongNameSignature;

    // Regular fixup and binding information

    IMAGE_DATA_DIRECTORY    CodeManagerTable;
    IMAGE_DATA_DIRECTORY    VTableFixups;
    IMAGE_DATA_DIRECTORY    ExportAddressTableJumps;

    // Precompiled image info (internal use only - set to zero)

    IMAGE_DATA_DIRECTORY    ManagedNativeHeader;

} IMAGE_COR20_HEADER, *PIMAGE_COR20_HEADER;

typedef enum ReplacesCorHdrNumericDefines
{
    // COM+ Header entry point flags.

    COMIMAGE_FLAGS_ILONLY               =0x00000001,
    COMIMAGE_FLAGS_32BITREQUIRED        =0x00000002,
    COMIMAGE_FLAGS_IL_LIBRARY           =0x00000004,
    COMIMAGE_FLAGS_STRONGNAMESIGNED     =0x00000008,
    // DDBLD - Added Next Line - Still verifying general usage

    COMIMAGE_FLAGS_NATIVE_ENTRYPOINT    =0x00000010,
    // DDBLD - End of Add

    COMIMAGE_FLAGS_TRACKDEBUGDATA       =0x00010000
                                         // Other kinds of flags follow

} ReplacesCorHdrNumericDefines;

typedef struct _IMAGE_TLS_DIRECTORY32
{
    unsigned int StartAddressOfRawData;
    unsigned int EndAddressOfRawData;
    unsigned int AddressOfIndex;
    unsigned int AddressOfCallBacks;
    unsigned int SizeOfZeroFill;
    unsigned int Characteristics;
} IMAGE_TLS_DIRECTORY32,*PIMAGE_TLS_DIRECTORY32;
typedef struct _IMAGE_TLS_DIRECTORY64
{
    unsigned long long StartAddressOfRawData;
    unsigned long long EndAddressOfRawData;
    unsigned long long AddressOfIndex;
    unsigned long long AddressOfCallBacks;
    unsigned int SizeOfZeroFill;
    unsigned int Characteristics;
} IMAGE_TLS_DIRECTORY64, *PIMAGE_TLS_DIRECTORY64;

typedef struct _IMAGE_BOUND_IMPORT_DESCRIPTOR
{
    unsigned int TimeDateStamp;
    WORD OffsetModuleName;
    WORD NumberOfModuleForwarderRefs;
} IMAGE_BOUND_IMPORT_DESCRIPTOR,*PIMAGE_BOUND_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_DEBUG_DIRECTORY
{
    unsigned int Characteristics;
    unsigned int TimeDateStamp;
    WORD  MajorVersion;
    WORD  MinorVersion;
    unsigned int Type;
    unsigned int SizeOfData;
    unsigned int AddressOfRawData;
    unsigned int PointerToRawData;
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

typedef struct
{
    unsigned int     Size;
    unsigned int     TimeDateStamp;
    WORD      MajorVersion;
    WORD      MinorVersion;
    unsigned int     GlobalFlagsClear;
    unsigned int     GlobalFlagsSet;
    unsigned int     CriticalSectionDefaultTimeout;
    unsigned long long DeCommitFreeBlockThreshold;
    unsigned long long DeCommitTotalFreeThreshold;
    unsigned long long LockPrefixTable;
    unsigned long long MaximumAllocationSize;
    unsigned long long VirtualMemoryThreshold;
    unsigned long long ProcessAffinityMask;
    unsigned int     ProcessHeapFlags;
    WORD      CSDVersion;
    WORD      Reserved1;
    unsigned long long EditList;
    unsigned long long SecurityCookie;
    unsigned long long SEHandlerTable;
    unsigned long long SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;

typedef struct
{
    unsigned int   Size;
    unsigned int   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    unsigned int   GlobalFlagsClear;
    unsigned int   GlobalFlagsSet;
    unsigned int   CriticalSectionDefaultTimeout;
    unsigned int   DeCommitFreeBlockThreshold;
    unsigned int   DeCommitTotalFreeThreshold;
    unsigned int   LockPrefixTable;            // VA
    unsigned int   MaximumAllocationSize;
    unsigned int   VirtualMemoryThreshold;
    unsigned int   ProcessHeapFlags;
    unsigned int   ProcessAffinityMask;
    WORD    CSDVersion;
    WORD    Reserved1;
    unsigned int   EditList;                   // VA
    unsigned int   SecurityCookie;             // VA
    unsigned int   SEHandlerTable;             // VA
    unsigned int   SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY32, *PIMAGE_LOAD_CONFIG_DIRECTORY32;

#pragma pack(pop)

#endif // _WINDOWS_H
