# Ngắt ngoài, Ngắt Timer, Ngắt truyền thông
## Ngắt ngoài
- External interrupt (EXTI) hay còn gọi là ngắt ngoài. Là 1 sự kiện ngắt xảy ra khi có tín hiệu can thiệp từ bên ngoài, từ phần cứng, người sử dụng hay ngoại vi,… 
- Ngắt ngoài của chip STM32F103 bao gồm có 16 line:
  ![image](https://github.com/user-attachments/assets/2c6d54da-37d3-4d1b-9f0c-18f983663363)

- Các Line0, Line1, Line2, Line3, Line4 sẽ được phân vào các vector ngắt riêng biệt EXTI0, EXTI1, EXTI2, EXTI3, EXTI4, còn từ Line5->Line9 sẽ được phân vào vector ngắt EXTI9_5, Line10->Line15 được phân vào vecotr EXTI15_10.
- Hàm `GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)` liên kết 1 chân với một EXTI line để cấu hình chân ở chế độ sử dụng ngắt ngoài:
  - GPIO_PortSource: Chọn Port để sử dụng làm nguồn cho ngắt ngoài.
  - GPIO_PinSource: Chọn Pin để cấu hình.
- Các tham số ngắt ngoài được cấu hình trong struct EXTI_InitTypeDef, gồm:
  - EXTI_Line: Xác định EXTI line cụ thể sẽ được cấu hình.
  - EXTI_Mode: Xác định chế độ hoạt động của EXTI, có hai chế độ là Interrupt hoặc Event.
  - EXTI_Trigger: Xác định loại cạnh xung sẽ kích hoạt ngắt.
  - EXTI_LineCmd: Kích hoạt (ENABLE) hoặc vô hiệu hóa (DISABLE) EXTI line.
- NVIC (Nested Vectored Interrupt Controller) chịu trách nhiệm quản lý và xử lý các ngắt. NVIC cho phép MCU xử lý nhiều ngắt từ các nguồn khác nhau, có thể ưu tiên ngắt và hỗ trợ ngắt lồng nhau.
  - Priority Group xác định cách phân chia bit giữa Preemption Priority và Subpriority. Sử dụng hàm `NVIC_PriorityGroupConfig(uint32_t PriorityGroup)` để chọn priority group cho NVIC.
  - Preemption Priority xác định mức độ ưu tiên chính của ngắt và quy định ngắt nào có thể lồng vào ngắt nào.
  - Subpriority chỉ có ý nghĩa khi các ngắt có cùng mức Preemption Priority.
    ![image](https://github.com/user-attachments/assets/031c9a56-23bb-4291-88c1-98c2e1d043f0)

- Các tham số được cấu hình trong struct NVIC_InitTypeDef, bao gồm:
  - NVIC_IRQChannel: Xác định mã của kênh ngắt cần được cấu hình.
  - NVIC_IRQChannelPreemptionPriority: Xác định mức độ ưu tiên Preemption Priority cho kênh ngắt.
  - NVIC_IRQChannelSubPriority: Xác định mức độ ưu tiên phụ Subpriority cho kênh ngắt.
  - NVIC_IRQChannelCmd: Cho phép ngắt.
- Ngắt trên từng line có hàm phục riêng của từng line. Có tên cố định: EXTIx_IRQHandler() (x là line ngắt tương ứng). 
- Hàm EXTI_GetITStatus(EXTI_Linex), Kiểm tra cờ ngắt của line x tương ứng. 
- Hàm EXTI_ClearITPendingBit(EXTI_Linex): Xóa cờ ngắt ở line x.
## Ngắt Timer
- Sử dụng ngắt timer, ta vẫn cấu hình các tham số trong TIM_TimeBaseInitTypeDef bình thường, riêng TIM_Period, đây là số lần đếm mà sau đó timer sẽ ngắt. 
- Hàm TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE) kích hoạt ngắt cho TIMERx tương ứng.
- Ở NVIC, ta cấu hình TIM_IRQn để khớp với line ngắt timer.
- Hàm phục vụ ngắt Timer được đặt tên : TIMx_IRQHandler() với x là timer tương ứng.
## Ngắt truyền thông
- Trước khi cho phép UART hoạt động, cần kích hoạt ngắt UART bằng cách gọi hàm `USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);`
Ở cấu hình NVIC cho UART cũng như các ngoại vi khác, cần chọn line ngắt tương ứng với bộ ngoại vi cần sử dụng.



