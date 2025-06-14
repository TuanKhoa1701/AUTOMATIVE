#ifndef STD_TYPES_H
#define STD_TYPES_H
// This file is part of the AUTOSAR standard.
#include <stdint.h>


#define STD_TYPES_SW_MAJOR_VERSION  1U
#define STD_TYPES_SW_MINOR_VERSION  0U
#define STD_TYPES_SW_PATCH_VERSION  0U

#define E_OK        0x00U
#define E_NOT_OK    0x01U

#define STD_HIGH    0x01U
#define STD_LOW     0x00U

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed long sint32_t;
typedef signed long long sint64_t;

typedef float float32_t;
typedef double float64_t;

typedef volatile uint8_t vuint8_t;
typedef volatile uint16_t vuint16_t;
typedef volatile uint32_t vuint32_t;
typedef volatile uint64_t vuint64_t;
typedef volatile sint8_t vsint8_t;
typedef volatile sint16_t vsint16_t;
typedef volatile sint32_t vsint32_t;
typedef volatile sint64_t vsint64_t;

typedef uint8_t Std_ReturnType;

typedef uint8_t boolean;

#ifndef TRUE
    #define TRUE  1U
#endif
#ifndef FALSE
    #define FALSE 0U
#endif
typedef struct {
    uint16_t vendorID;
    uint16_t moduleID;
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
} Std_VersionInfoType;

#define Det_ReportError(moduleId, instanceId, apiId, errorId) 


#define STD_ACTIVE  0x01U
#define STD_IDLE    0x00U

#define STD_ON      0x01U
#define STD_OFF     0x00U

#endif