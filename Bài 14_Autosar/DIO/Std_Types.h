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

typedef uint8_t boolean;

#ifndef TRUE
#define TRUE  1U
#endif
#ifndef FALSE
#define FALSE 0U
#endif

#define STD_ACTIVE  0x01U
#define STD_IDLE    0x00U

#define STD_ON      0x01U
#define STD_OFF     0x00U

typedef uint8_t Std_ReturnType;

#endif