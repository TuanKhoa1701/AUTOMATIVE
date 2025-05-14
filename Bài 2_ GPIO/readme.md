
# Lesson 2: GPIO
## Thư viện SPL
- SPL (Standard Peripheral Library) là một thư viện do STMicroelectronics cung cấp để lập trình các vi điều khiển STM32. 
- Đây là một thư viện cấp thấp giúp người dùng làm việc với các ngoại vi của vi điều khiển mà không cần thao tác trực tiếp với thanh ghi (registers).

##  Cấp clock và cấu hình ngoại vi (GPIO)
1. Cấp clock cho ngoại vi:
- Các hàm thường dùng để cấu hình xung clock
```
RCC_APB1PeriphClockCmd
RCC_APB2PeriphClockCmd
RCC_AHBPeriphClockCmd
```
Trong đó nhận 2 parameters:
- Ngoại vi muốn cấp clock.
- Trạng thái ENABLE hay DISABLE
2. Cấu hình ngoại vi
- Các tham số dùng để cấu hình GPIO
```
typedef struct
{
  uint16_t GPIO_Pin;             
/*
!< Lựa chọn chân của GPIO.
This parameter can be any value of @ref GPIO_pins_define 
*/

  GPIOSpeed_TypeDef GPIO_Speed;  
/*
!< Lựa chọn tốc độ tần số cho chân.
This parameter can be a value of @ref GPIOSpeed_TypeDef 
*/

  GPIOMode_TypeDef GPIO_Mode;    
/*
!< Lựa chọn chế độ cho GPIO
This parameter can be a value of @ref GPIOMode_TypeDef 
*/
}GPIO_InitTypeDef;
```
3. Sử dụng ngoại vi
- Các hàm thông dụng để sử dụng và giao tiếp với GPIO
```
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```
Đọc giá trị 1 chân trong GPIO được cấu hình là INPUT
```
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
```
Đọc giá trị nguyên GPIO được cấu hình là INPUT
```
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```
Đọc giá trị 1 chân trong GPIO được cấu hình là OUTPUT
```
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
```
Đọc giá trị nguyên GPIO được cấu hình là OUTPUT
```
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```
Cho giá trị điện áp của 1 chân trong GPIO = 1
```
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
```
Cho giá trị điện áp của 1 chân trong GPIO = 0
```
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
```
Ghi giá trị "BitVal" vào 1 chân trong GPIO
```
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
```
Ghi giá trị "PortVal" vào nguyên GPIO
