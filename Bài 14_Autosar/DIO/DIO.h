#ifndef DIO_H
#define DIO_H

// This file is part of the AUTOSAR standard.
#include "Std_Types.h"

#define DIO_GetPort(ChannelId) (
    (((ChannelID) < 16) ? GPIOA : \
     ((ChannelID) < 32) ? GPIOB : \
     ((ChannelID) < 48) ? GPIOC : \
     ((ChannelID) < 64) ? GPIOD : \
     ((ChannelID) < 80) ? GPIOE : \
     ((ChannelID) < 96) ? GPIOF : \
     ((ChannelID) < 112) ? GPIOG : \
     ((ChannelID) < 128) ? GPIOH : NULL))
)
#define DIO_GetPin(ChannelId) (1<<(ChannelId) % 16)

#define DIO_CHANNEL(GPIO_Port, Pin) \
    ((GPIO_Port) * 16 + (Pin))
#define DIO_CHANNEL_A0 (DIO_CHANNEL(GPIOA, 0))
#define DIO_CHANNEL_A1 (DIO_CHANNEL(GPIOA, 1))
#define DIO_CHANNEL_A2 (DIO_CHANNEL(GPIOA, 2))
#define DIO_CHANNEL_A3 (DIO_CHANNEL(GPIOA, 3))
#define DIO_CHANNEL_A4 (DIO_CHANNEL(GPIOA, 4))
#define DIO_CHANNEL_A5 (DIO_CHANNEL(GPIOA, 5))
#define DIO_CHANNEL_A6 (DIO_CHANNEL(GPIOA, 6))
#define DIO_CHANNEL_A7 (DIO_CHANNEL(GPIOA, 7))
#define DIO_CHANNEL_A8 (DIO_CHANNEL(GPIOA, 8))
#define DIO_CHANNEL_A9 (DIO_CHANNEL(GPIOA, 9))
#define DIO_CHANNEL_A10 (DIO_CHANNEL(GPIOA, 10))
#define DIO_CHANNEL_A11 (DIO_CHANNEL(GPIOA, 11))
#define DIO_CHANNEL_A12 (DIO_CHANNEL(GPIOA, 12))
#define DIO_CHANNEL_A13 (DIO_CHANNEL(GPIOA, 13))
#define DIO_CHANNEL_A14 (DIO_CHANNEL(GPIOA, 14))
#define DIO_CHANNEL_A15 (DIO_CHANNEL(GPIOA, 15))

#define DIO_CHANNEL_B0 (DIO_CHANNEL(GPIOB, 0))
#define DIO_CHANNEL_B1 (DIO_CHANNEL(GPIOB, 1))
#define DIO_CHANNEL_B2 (DIO_CHANNEL(GPIOB, 2))
#define DIO_CHANNEL_B3 (DIO_CHANNEL(GPIOB, 3))
#define DIO_CHANNEL_B4 (DIO_CHANNEL(GPIOB, 4))
#define DIO_CHANNEL_B5 (DIO_CHANNEL(GPIOB, 5))
#define DIO_CHANNEL_B6 (DIO_CHANNEL(GPIOB, 6))
#define DIO_CHANNEL_B7 (DIO_CHANNEL(GPIOB, 7))
#define DIO_CHANNEL_B8 (DIO_CHANNEL(GPIOB, 8))
#define DIO_CHANNEL_B9 (DIO_CHANNEL(GPIOB, 9))
#define DIO_CHANNEL_B10 (DIO_CHANNEL(GPIOB, 10))
#define DIO_CHANNEL_B11 (DIO_CHANNEL(GPIOB, 11))
#define DIO_CHANNEL_B12 (DIO_CHANNEL(GPIOB, 12))
#define DIO_CHANNEL_B13 (DIO_CHANNEL(GPIOB, 13))
#define DIO_CHANNEL_B14 (DIO_CHANNEL(GPIOB, 14))
#define DIO_CHANNEL_B15 (DIO_CHANNEL(GPIOB, 15))


#define DIO_PORT_A 0
#define DIO_PORT_B 1
#define DIO_PORT_C 2
#define DIO_PORT_D 3
typedef uint8_t Dio_ChannelType;
typedef uint8_t Dio_PortType;
typedef uint8_t Dio_LevelType;
typedef struct 
{
    Dio_PortType port;
    uint16_t mask;
    uint8_t offset;
}
 Dio_ChannelGroupType;
typedef uint8_t Dio_LevelType;
typedef uint16_t Dio_PortLevelType;


#endif