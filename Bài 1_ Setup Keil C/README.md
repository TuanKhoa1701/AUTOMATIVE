
# Lesson 1: Setup Keil C
## Keil C
1. Tải Keil C
- Cài đặt Keil C về máy :https://www.keil.com/download/
- Cài đặt ST-link driver để nạp và debug:  https://www.keil.com/download/ 
- Cài đặt thư viện chuẩn của STM32F103c8t6 cho keil: https://www.keil.arm.com/devices/
2. Tạo project đầu tiên:
- Trong mục Keil C, chọn Project, tạo một proeject mới và chọn dòng vi điều khiển mà bạn muốn sử dụng
<img width="960" alt="Screenshot 2025-02-25 113623" src="https://github.com/user-attachments/assets/d506fc90-d2c6-4b0b-aae9-1a64ee147057" />

- Nhấn OK, chọn thư viện cần thiết
<img width="960" alt="Screenshot 2025-02-25 223634" src="https://github.com/user-attachments/assets/f3e3d7a7-8e89-4b16-ab5b-9e1b5c679118" />

- Tạo file main.c và add vào Source group
<img width="960" alt="Screenshot 2025-02-25 114656" src="https://github.com/user-attachments/assets/bd26d412-62aa-448d-9e0d-06b8b87306df" />

##  Blink Led
1. Cấu hình clock
- Trước khi cấu hình hoạt động của ngoại vi, chúng ta cần cấu hình xung clock thông qua bus.
  ![image](https://github.com/user-attachments/assets/aecf826d-4343-4ed8-8598-13c8598df8a8)

- GPIOC cung cấp clock thông qua bus APB2.
 ![image](https://github.com/user-attachments/assets/88e226ac-ed10-4999-aa30-bf2703ce4ca4)

- Định nghĩa cho thanh ghi RCC_APB2ENR 
```
#define RCC_APB2ENR *((unsigned int*)0x40021018)
```
- Set bit IOPCEN lên 1 để cấp clock cho GPIOC.
```
RCC_APB2ENR |= (1<<4);
```
2. Cấu hình chế độ hoạt động
- Xác định và cấu hình chân trong thanh ghi Port configuration register, trong đó bao gồm:

    a.CRL: cấu hình chân từ 0-7 trong Portx
  
![image](https://github.com/user-attachments/assets/f42dc85a-d845-4e3e-8a34-1101f85f2eb3)

    b.CRH: cấu hình chân từ 8-15 trong Portx
![image](https://github.com/user-attachments/assets/0df08bb5-188c-4b9b-b31b-0fe1d4707f4e)

- Các cặp bit như CNFy và MODEy xác định thông số và chế độ hoạt động
  
  ![image](https://github.com/user-attachments/assets/ca93551a-6430-4656-9cac-714f095f963e)

- Để blink Led trên PortC 13, ta sẽ sử dụng thanh ghi CRH, định nghĩa thanh ghi GPIOC_CRH
```
#define GPIOC_CRH *((unsigned int*)0x40021004)
```
- Ta sẽ set cặp bit CNF13 xuống 0 và MODE13 lên 1.
```
GPIOC_CRH &= ~((1<<23) | (1<<22));
GPIOC_CRH |= ((1<<21) | (1<<20));
```
- Hàm delay tự tạo 
```
void Delay(unsigned int time)
{
    for(int i=0; i< time;i++){}
}
```
- Kết hợp hàm delay ở trên, sử dụng thanh ghi Port output data register (GPIOx_ODR) để ghi giá trị lên PC13 
```
#define GPIOC_ODR *((unsigned int*)0x4002100C)

GPIOC_ODR |= (1<<13);
Delay(1000000);
GPIOC_ODR &= ~(1<<13);
Delay(1000000);
```
## Button Led
- Cấp clock cho GPIOA để cấu hình làm nút nhấn ` RCC->APB2ENR |= (1<<2)| (1<<4);`
- Cấu hình chế độ chân cho PA0 pull-up (nút nhấn, tín hiệu đọc được là LOW) hoặc pull-down( nút nhấn, tín hiệu đọc được là HIGH)
  ![image](https://github.com/user-attachments/assets/4edb2a12-e446-4e29-9505-f58ebcfcf507)

- Ta sẽ cấu hình chân PA0 làm Input pull up
```
GPIOA->CRL &= ~((1<<0) |(1<<1)|(1<<2));
GPIOA->CRL |= (1<<3);
GPIOA->ODR |= 1;
```
- Đọc thanh ghi GPIOA->IDR để điều khiển Led
```
if(GPIOA->IDR & (1<<0)==0)
{
    GPIOA->ODR = 0 << 13;
}
else
{
    GPIOA->ODR = 1 << 13;
}
```
