# DMA
## Định nghĩa
- DMA – Direct memory access được sử dụng với mục đích truyền data với tốc độ cao từ thiết bị ngoại vi đến bộ nhớ cũng như từ bộ nhớ đến bộ nhớ.
 
  ![image](https://github.com/user-attachments/assets/38ac5c78-340e-4616-ab6f-77648bbfd697)
  
- Với DMA, dữ liệu sẽ được truyền đi nhanh chóng mà không cần đến bất kỳ sự tác động nào của CPU. Điều này sẽ giữ cho tài nguyên của CPU được rảnh rỗi cho các thao tác khác. Đồng thời tránh việc data nhận về từ ngoại vi bị mất mát
## Sử dụng DMA trong STM32F103C8T6
STM32F103C8T6 có 2 bộ DMA. DMA1 bao gồm 7 kênh, DMA2 bao gồm 5 kênh:
- Có 2 chế độ hoạt động.
- Mỗi kênh có thể được cấu hình riêng.
- Mỗi kênh có thể phục vụ cho nhiều ngoại vi khác nhau, nhưng không đồng thời.
- Có mức ưu tiên để lập trình cho các kênh
Có thể sử dụng ngắt DMA với 5 cờ báo ngắt (DMA Half Transfer, DMA Transfer complete, DMA Transfer Error, DMA FIFO Error, Direct Mode Error).
![image](https://github.com/user-attachments/assets/ce24bb88-583c-4032-aceb-319d579e8a05)

DMA có 2 chế độ hoạt động là normal và circular:
-  Normal mode: Với chế độ này, DMA truyền dữ liệu cho tới khi truyền đủ 1 lượng dữ liệu giới hạn đã khai báo DMA sẽ dừng hoạt động. Muốn nó tiếp tục hoạt động thì phải khởi động lại
- Circular mode: Với chế độ này, Khi DMA truyền đủ 1 lượng dữ liệu giới hạn đã khai báo thì nó sẽ truyền tiếp về vị trí ban đầu (Cơ chế như Ring buffer).
Các tham số cho bộ DMA được cấu hình trong struct DMA_InitTypeDef. Gồm:
- DMA_PeripheralBaseAddr: Cấu hình địa chỉ của ngoại vi cho DMA. Đây là địa chỉ mà DMA sẽ lấy data hoặc truyền data tới cho ngoại vi.
- DMA MemoryBaseAddr: Cấu hình địa chỉ vùng nhớ cần ghi/ đọc data .
- DMA_DIR: Cấu hình hướng truyền DMA, từ ngoại vi tới vùng nhớ hay từ vùng nhớ tới ngoại vi.
- DMA_BufferSize: Cấu hình kích cỡ buffer. Số lượng dữ liệu muốn gửi/nhận qua DMA.
- DMA_PeripheralInc: Cấu hình địa chỉ ngoại vi có tăng sau khi truyền DMA hay không.
- DMA_MemoryInc: Cấu hình địa chỉ bộ nhớ có tăng lên sau khi truyền DMA hay không.
- DMA_PeripheralDataSize: Cấu hình độ lớn data của ngoại vi.
- DMA_MemoryDataSize: Cấu hình độ lớn data của bộ nhớ.
- DMA_Mode: Cấu hình mode hoạt động.
- DMA_Priority: Cấu hình độ ưu tiên cho kênh DMA.
- DMA_M2M: Cấu hình sử dụng truyền từ bộ nhớ đếm bộ nhớ cho kênh DMA.
# PWM
## Định nghĩa:
Trong điều khiển động cơ servo, tín hiệu PWM (Pulse Width Modulation) được sử dụng để chỉ định góc mà động cơ servo sẽ xoay đến. Tín hiệu PWM có hai yếu tố quan trọng:
- Tần số: Là số lần tín hiệu lặp lại trong một giây. Đối với servo, tần số thông thường là 50Hz (tức là, chu kỳ lặp lại sau mỗi 20ms).
- Độ rộng xung (Pulse Width): Là thời gian tín hiệu ở mức cao trong mỗi chu kỳ. Độ rộng xung thường được đo bằng microsecond (µs) và quyết định góc mà servo sẽ xoay đến. Tỉ lệ độ rộng xung với chu kì xung gọi là chu kì nhiệm vụ (Duty Cycle).
- ![image](https://github.com/user-attachments/assets/fb29a449-6ce0-40f9-996a-3eac05f6fa4b)

## Cấu hình trong STM32F103C8T6
Ta phải cấu hình chân chế độ AF_PP để gán chân GPIO với 1 kênh của timer mà ta cấu hình chế độ PWM.
```
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PA0 là TIM2_CH1
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Chế độ Alternate Function Push-Pull
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);
```
Cấu hình timer chế độ PWM trong struct TIM_OCInitTypeDef:
- TIM_OCMode: Chọn chế độ hoạt động cho Output Compare. PWM1 thì kênh timer sẽ ở mức 1 khi nhỏ hơn TIM_Pulse và mức 0 khi lớn hơn. PWM2 ngược lại so với PWM1.
- TIM_OutputState: cho phép tín hiệu PWM được xuất ra từ chân tương ứng của MCU.
- TIM_Pulse: Đặt giá trị đầu cho độ rộng xung (giá trị timer dùng để so sánh).
- TIM_OCPolarity: Đặt cực tính của tín hiệu ngõ ra TIM_OCPolarity_High sẽ làm xung ở mức 1 (HIGH) khi giá trị đếm còn nhỏ hơn TIM_Pulse còn TIM_OCPolarity_Low sẽ làm xung ở mức 0 (LOW) khi giá trị đếm còn hơn hơn TIM_Pulse.
Gọi hàm TIM_OCxInit(); để cấu hình cho kênh x tương ứng.

Hàm TIM_OCxPreloadConfig(); cấu hình Timer hoạt động với tính năng Preload (TIM_OCPreload_Enable) hay không (TIM_OCPreload_Disable).

- Tính năng Preload là tính năng mà hệ thống sẽ chờ cho tới khi timer tạo ra sự kiện Update Event thì mới nạp lại giá trị so sánh mới (TIM_Pulse)
- Sự kiện Update Event là sự kiện xảy ra khi timer đã đếm đến giá trị tối đa được cấu hình và sẽ quay lại 0 để bắt đầu chuu kỳ mới.
Gọi hàm TIM_Cmd(); để cho phép Timer hoạt động.
```
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_Cmd(TIM2);
```
Để thay đổi độ rộng xung xuất ra, sử dụng hàm TIM_SetComparex(TIMx, pulseWidth); với Timer sử dụng là TIMx và độ rộng pulseWidth.




