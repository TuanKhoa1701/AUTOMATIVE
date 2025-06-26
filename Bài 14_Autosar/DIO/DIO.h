/******************************************************* 
 * @file: DIO.H
 * @brief: Header file for Digital Input/Output (DIO) module in AUTOSAR.
 * @details: Đây là tệp tiêu đề cho module DIO trong AUTOSAR, định nghĩa các kiểu dữ liệu và hàm liên quan đến việc đọc và ghi các kênh GPIO.
 * @version: 1.0.0
 * @author: Nguyen Tuan Khoa
********************************************************/

#ifndef DIO_H
#define DIO_H

// This file is part of the AUTOSAR standard.
#include "Std_Types.h"

/******************************************************* 
 * ========================================================
 * DIO Port Definitions
 * ========================================================
 * Macro xác định cổng GPIO dựa trên ID kênh.
 ********************************************************/

#define DIO_GetPort(ChannelId) (
    (((ChannelID) < 16) ? GPIOA : \
     ((ChannelID) < 32) ? GPIOB : \
     ((ChannelID) < 48) ? GPIOC : \
     ((ChannelID) < 64) ? GPIOD : \
     ((ChannelID) < 80) ? GPIOE : \
     ((ChannelID) < 96) ? GPIOF : \
     ((ChannelID) < 112) ? GPIOG : \
     ((ChannelID) < 128) ? GPIOH : NULL))

    /**********************************************************
     * ========================================================
     * DIO Pin Definitions
     * ========================================================
     * Macro xác định chân GPIO dựa trên ID kênh
     **********************************************************/

#define DIO_GetPin(ChannelId) (1<<(ChannelId) % 16)

    /**************************************************** 
     * ========================================================
     * DIO Channel Definitions
     * ========================================================
     * Macro xác định ID kênh DIO dựa trên cổng GPIO và chân.
     ********************************************************/

#define DIO_CHANNEL(GPIO_Port, Pin) \
    ((GPIO_Port) * 16 + (Pin))

    /*******************************************************
     * ========================================================
     * DIO Channel Definitions for Specific Channels
     * ========================================================
     * Các định nghĩa kênh DIO cụ thể cho từng chân GPIO.
     ********************************************************/

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

#define DIO_CHANNEL_C0 (DIO_CHANNEL(GPIOC, 0))
#define DIO_CHANNEL_C1 (DIO_CHANNEL(GPIOC, 1))
#define DIO_CHANNEL_C2 (DIO_CHANNEL(GPIOC, 2))
#define DIO_CHANNEL_C3 (DIO_CHANNEL(GPIOC, 3))
#define DIO_CHANNEL_C4 (DIO_CHANNEL(GPIOC, 4))
#define DIO_CHANNEL_C5 (DIO_CHANNEL(GPIOC, 5))
#define DIO_CHANNEL_C6 (DIO_CHANNEL(GPIOC, 6))
#define DIO_CHANNEL_C7 (DIO_CHANNEL(GPIOC, 7))
#define DIO_CHANNEL_C8 (DIO_CHANNEL(GPIOC, 8))
#define DIO_CHANNEL_C9 (DIO_CHANNEL(GPIOC, 9))
#define DIO_CHANNEL_C10 (DIO_CHANNEL(GPIOC, 10))
#define DIO_CHANNEL_C11 (DIO_CHANNEL(GPIOC, 11))
#define DIO_CHANNEL_C12 (DIO_CHANNEL(GPIOC, 12))
#define DIO_CHANNEL_C13 (DIO_CHANNEL(GPIOC, 13))
#define DIO_CHANNEL_C14 (DIO_CHANNEL(GPIOC, 14))
#define DIO_CHANNEL_C15 (DIO_CHANNEL(GPIOC, 15))


#define DIO_CHANNEL_D0 (DIO_CHANNEL(GPIOD, 0))
#define DIO_CHANNEL_D1 (DIO_CHANNEL(GPIOD, 1))
#define DIO_CHANNEL_D2 (DIO_CHANNEL(GPIOD, 2))
#define DIO_CHANNEL_D3 (DIO_CHANNEL(GPIOD, 3))
#define DIO_CHANNEL_D4 (DIO_CHANNEL(GPIOD, 4))
#define DIO_CHANNEL_D5 (DIO_CHANNEL(GPIOD, 5))
#define DIO_CHANNEL_D6 (DIO_CHANNEL(GPIOD, 6))
#define DIO_CHANNEL_D7 (DIO_CHANNEL(GPIOD, 7))
#define DIO_CHANNEL_D8 (DIO_CHANNEL(GPIOD, 8))
#define DIO_CHANNEL_D9 (DIO_CHANNEL(GPIOD, 9))
#define DIO_CHANNEL_D10 (DIO_CHANNEL(GPIOD, 10))
#define DIO_CHANNEL_D11 (DIO_CHANNEL(GPIOD, 11))
#define DIO_CHANNEL_D12 (DIO_CHANNEL(GPIOD, 12))
#define DIO_CHANNEL_D13 (DIO_CHANNEL(GPIOD, 13))
#define DIO_CHANNEL_D14 (DIO_CHANNEL(GPIOD, 14))
#define DIO_CHANNEL_D15 (DIO_CHANNEL(GPIOD, 15))

/**********************************************************
 * ========================================================
 * DIO Port Definitions
 * ======================================================== 
 * Các định nghĩa cổng DIO.
 **********************************************************/

#define DIO_PORT_A 0
#define DIO_PORT_B 1
#define DIO_PORT_C 2
#define DIO_PORT_D 3

/**********************************************************
 * ========================================================
 * Định nghĩa kiểu dữ liệu cho DIO Driver
 * ========================================================
 * @typedef Dio_ChannelType
 * @brief kieu dữ liệu đại diện cho một kênh DIO.
 * @details kiểu được định danh cho một pin cụ thể.
 **********************************************************/

typedef uint8_t Dio_ChannelType;

/**********************************************************
 * ======================================================== 
 * Định nghĩa kiểu dữ liệu cho Port DIO
 * ========================================================
 * @typedef Dio_PortType
 * @brief kieu dữ liệu đại diện cho một cổng DIO.
 * @details kiểu này được sử dụng để xác định cổng GPIO mà kênh DIO thuộc về.
 **********************************************************/

typedef uint8_t Dio_PortType;

/**********************************************************
 * ========================================================
 * Định nghĩa kiểu dữ liệu cho DIO Port Level
 * ========================================================
 * @typedef Dio_PortLevelType
 * @brief kieu dữ liệu đại diện cho mức độ của một cổng DIO.
 * @details kiểu này được sử dụng để đọc hoặc ghi mức độ của toàn bộ cổng GPIO.
 **********************************************************/

typedef uint8_t Dio_LevelType;

/**********************************************************
 * ========================================================
 * Định nghĩa kiểu dữ liệu cho DIO Channel Group
 * ========================================================
 * @typedef Dio_ChannelGroupType
 * @brief kiểu dữ liệu đại diện cho một nhóm kênh DIO.
 * @details kiểu này được sử dụng để xác định một nhóm các kênh DIO trong cùng một cổng.
 **********************************************************/

typedef struct 
{
    Dio_PortType port;
    uint16_t mask;
    uint8_t offset;
} Dio_ChannelGroupType;

/**********************************************************
 * ========================================================
 * Định nghĩa kiểu dữ liệu cho DIO Port Level
 * ========================================================
 * @typedef Dio_PortLevelType
 * @brief kieu dữ liệu đại diện cho mức độ của một cổng DIO.
 * @details kiểu này được sử dụng để đọc hoặc ghi mức độ của toàn bộ cổng GPIO.
 **********************************************************/

typedef uint8_t Dio_LevelType;

/**********************************************************
 * ========================================================
 * Định nghĩa kiểu dữ liệu cho DIO Port Level
 * ========================================================
 * @typedef Dio_PortLevelType
 * @brief kieu dữ liệu đại diện cho mức độ của một cổng DIO.
 * @details kiểu này được sử dụng để đọc hoặc ghi mức độ của toàn bộ cổng GPIO.
 **********************************************************/

typedef uint16_t Dio_PortLevelType;


#endif
