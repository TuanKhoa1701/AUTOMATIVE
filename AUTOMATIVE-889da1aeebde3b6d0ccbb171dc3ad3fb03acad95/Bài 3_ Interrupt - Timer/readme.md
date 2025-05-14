# Interrupt - Timer
## Interrupt
1. Interrupt là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển. Nó yêu cầu MCU phải dừng chương trình chính và thực thi chương trình ngắt.Các ngắt có các địa chỉ cố định trong bộ nhớ để giữ các trình phục vụ. Các địa chỉ này gọi là vector ngắt.
2. Các loại Interrupt:
- External Interrupt: xảy ra khi có sự thay đổi điện áp của chân GPIO được cấu hình làm input. Có 4 loại ngắt ngoài: LOW(kích hoạt khi chân ở mức thấp), HIGH (kích hoạt khi chân ở mức cao),RISING (kích hoạt khi chuyển từ thấp lên cao), FALLING (kích hoạt khi chuyển từ cao xuống thấp).
- Timer Interrupt: xảy ra khi giá trị trong thanh ghi đếm của timer tràn. Giá trị tràn được xác định bởi giá trị cụ thể trong thanh ghi đếm của timer; nên reset giá trị thanh ghi timer để có thể tạo được ngắt tiếp theo.
- Communication Interrupt: xảy ra khi có sự kiện truyền/nhận dữ liệu giữ MCU với các thiết bị bên ngoài hay với MCU, thường sử dụng cho UART, SPI, I2C ... để truyền nhận chính xác.
3. Độ ưu tiên ngắt
- Độ ưu tiên ngắt xác định ngắt nào được quyền thực thi khi nhiều ngắt xảy ra đồng thời. STM32 quy định ngắt nào có số thứ tự ưu tiên càng thấp thì có quyền càng cao.
## Timer
1. Timer là một bộ đếm thời gian đếm từng chu kỳ xung clock, có thể nhận xung clock từ chu kỳ ngoài.
2. Timer có các thành phần chính như sau:
- Thanh ghi đếm (counter register): thanh ghi lưu trữ giá trị đếm của timer.
- Prescaler: bộ chia tần số, chia nhỏ bộ xung nhịp gốc, điều chỉnh sau bao nhiêu lần dao động xung nhịp thì bộ timer đếm lên một lần
- Auto-reload register(ARR): Set giá trị để khi timer đếm quá sẽ bị tràn và reset lại bộ đếm.
- Timer modes: up-counting mode(đếm từ 0 lên ARR), down-counting mode (đếm từ ARR xuống 0), center-aligned mode (đếm từ 0 lên ARR rồi đếm ngược xuống 0)
```
	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_InitStruct.TIM_Prescaler = 7200 - 1;
	TIM_InitStruct.TIM_Period = 0xFFFF;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
```
- TIM_ClockDivision: bộ chia xung clock
- TIM_Prescaler: bộ chia tần prescaler
- TIM_Period: Set giá trị tràn
- TIM_CounterMode: Set chế độ đếm ARR.
