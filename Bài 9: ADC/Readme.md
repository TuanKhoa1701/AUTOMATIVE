# ADC
## Định nghĩa
- ADC (Analog-to-Digital Converter) là 1 mạch điện tử lấy điện áp tương tự là  m đầu vào và chuyển đổi nó thành dữ liệu số (1 giá trị đại diện cho mức điện áp trong mã nhị phân), hoạt động theo cách chia mức tín hiệu tương tự thành nhiều mức khác nhau
![image](https://github.com/user-attachments/assets/fcb37b20-d454-4e0b-afc4-f6677e717693)

- Độ phân giải: Số bit mà ADC dùng để xác định số lượng mức sẽ chia từ phạm vi điện áp tương tự.
Độ phân giải càng cao => Càng nhiều mức => Kết quả càng chính xác
- Tần số lấy mẫu: Quy định tần suất mà tín hiệu tương tự được lấy mẫu. Tần số lấy mẫu càng cao => Lấy được nhiều mẫu hơn => Kết quả càng chính xác
## Sử dụng ADC trong STM32
- Regular Conversion:
  - Single: ADC chỉ đọc 1 kênh duy nhất, và chỉ đọc khi nào được yêu cầu.
  - Single Continuous: ADC sẽ đọc một kênh duy nhất, nhưng đọc dữ liệu nhiều lần liên tiếp (Có thể được biết đến như sử dụng DMA để đọc dữ liệu và ghi vào bộ nhớ). 
  - Scan: Multi-Channels: Quét qua và đọc dữ liệu nhiều kênh, nhưng chỉ đọc khi nào được yêu cầu.
  - Scan: Continuous Multi-Channels Repeat: Quét qua và đọc dữ liệu nhiều kênh, nhưng đọc liên tiếp nhiều lần giống như Single Continous. 
- Injected Conversion: Trong trường hợp nhiều kênh hoạt động. Khi kênh có mức độ ưu tiên cao hơn có thể tạo ra một Injected Trigger. Khi gặp Injected Trigger thì ngay lập tức kênh đang hoạt động bị ngưng lại để kênh được ưu tiên kia có thể hoạt động.
- Các tham số cấu hình cho bộ ADC được tổ chức trong struct ADC_InitTypeDef, Gồm:
  - ADC_Mode:  Cấu hình chế độ hoạt động cho ADC là đơn (Independent) hay đa, ngoài ra còn có các chế độ ADC chuyển đổi tuần tự các kênh (regularly) hay chuyển đổi khi có kích hoạt (injected).
  - ADC_NbrOfChannel: Số kênh ADC để cấu hình
  - ADC_ContinuousConvMode: Cấu hình bộ ADC có chuyển đổi liên tục hay không, Enable để cấu hình ADC  chuyển đổi lien tục, nếu cấu hình Disable, ta phải gọi lại lệnh đọc ADC để bắt đầu quá trình chuyển đổi. 
  - ADC_ExternalTrigConv: Enable để sử dụng tín hiệu trigger. 
  - ADC_ScanConvMode: Cấu hình chế độ quét ADC lần lượt từng kênh. Enable nếu sử dụng chế độ quét này.
  - ADC_DataAlign: Cấu hình căn lề cho data. Vì bộ ADC xuất ra giá trị 12bit, được lưu vào biến 16 hoặc 32 bit nên phải căn lề các bit về trái hoặc phải.
- Ngoài các tham số trên, cần cấu hình thêm thời gian lấy mẫu, thứ tự kênh ADC khi quét, ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime):
  - Rank: Ưu tiên của kênh ADC.
  - SampleTime: Thời gian lấy mẫu tín hiệu.

- ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState): Bắt đầu quá trình chuyển đổi.
- ADC_GetConversionValue(ADC_TypeDef* ADCx): Đọc giá trị chuyển đổi được ở các kênh ADC tuần tự.
- ADC_GetDualModeConversionValue(void): Trả về giá trị chuyển đổi cuối cùng của ADC1, ADC2 ở chế độ kép.





