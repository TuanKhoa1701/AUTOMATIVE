# CAN trên stm32f103c8t6
## Cấu hình Clock và GPIO cho CAN
- Kích hoạt Clock cho CAN1
```
void CAN_Clock_Configuration(void) {
    // Kích hoạt clock cho CAN1 và GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}
```
- Cấu hình chân GPIO cho CAN (PA11 và PA12) để giao tiếp.
```
void CAN_GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // Cấu hình PA11 (CAN RX) là Input Pull-up
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Cấu hình PA12 (CAN TX) là Alternate Function Push-Pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
```
## Cấu hình CAN
- Cấu hình cho CAN trong Normal Mode với tốc độ baudrate là 500 Kbps
```
void CAN_Configuration(void) {
    CAN_InitTypeDef CAN_InitStructure;

    // Cấu hình CAN
    CAN_InitStructure.CAN_TTCM = DISABLE;  // Time Triggered Communication Mode
    CAN_InitStructure.CAN_ABOM = ENABLE;   // Automatic Bus-Off Management
    CAN_InitStructure.CAN_AWUM = ENABLE;   // Automatic Wake-Up Mode
    CAN_InitStructure.CAN_NART = DISABLE;  // No Automatic Retransmission
    CAN_InitStructure.CAN_RFLM = DISABLE;  // Receive FIFO Locked Mode
    CAN_InitStructure.CAN_TXFP = ENABLE;   // Transmit FIFO Priority
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  // Chế độ hoạt động bình thường

    // Cấu hình thời gian truyền (bit timing)
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  // Synchronization Jump Width = 1 time quantum
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;  // Bit Segment 1 = 6 time quanta
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;  // Bit Segment 2 = 8 time quanta
    CAN_InitStructure.CAN_Prescaler = 6;  // Tốc độ baudrate = 36 MHz / (Prescaler * 12) = 500 Kbps
    CAN_Init(CAN1, &CAN_InitStructure);
}
```
1. CAN_Mode:
- CAN_Mode_Normal: Chế độ hoạt động bình thường. Trong chế độ này, các node CAN sẽ giao tiếp với nhau như bình thường trên mạng CAN. Mỗi node có thể truyền và nhận dữ liệu từ các node khác.
- CAN_Mode_LoopBack: Chế độ vòng lặp, dùng để kiểm tra mà không cần một bus CAN thực. Khi sử dụng chế độ này, tất cả dữ liệu gửi đi sẽ được nhận lại bởi chính node phát ra. Không có giao tiếp thực sự xảy ra với các node khác trên bus.

- CAN_Mode_Silent: Chế độ im lặng, dùng để theo dõi mà không ảnh hưởng đến mạng CAN. Node trong chế độ này chỉ có thể nhận dữ liệu từ các node khác trên bus, nhưng không thể truyền dữ liệu hoặc phát ra bất kỳ tín hiệu nào.
- CAN_Mode_Silent_LoopBack: Kết hợp cả LoopBack và Silent
2. CAN_TTCM (Time Triggered Communication Mode)
- CAN_TTCM là chế độ giao tiếp theo thời gian, cho phép điều khiển giao tiếp CAN dựa trên các mốc thời gian cố định.
  - ENABLE: Kích hoạt chế độ giao tiếp theo thời gian, giúp tổ chức các thông điệp truyền theo một lịch định sẵn
  - DISABLE: Tắt chế độ này, CAN hoạt động bình thường mà không dựa trên mốc thời gian.
3. CAN_ABOM (Automatic Bus-Off Management):
- CAN_ABOM cho phép node CAN tự động ngắt khỏi bus khi phát hiện quá nhiều lỗi.
  - ENABLE:  Kích hoạt chế độ tự động ngắt kết nối khỏi bus CAN nếu có lỗi Bus-Off. Sau khi bị ngắt kết nối, node sẽ tự động khởi động lại sau một khoảng thời gian nhất định và cố gắng tham gia lại mạng.
  - DISABLE:  Tắt chế độ này, node sẽ không tự động khởi động lại sau khi gặp lỗi Bus-Off. Người dùng phải can thiệp bằng phần mềm để khởi động lại node.
4. CAN_AWUM (Automatic Wake -Up Mode):
- CAN_AWUM cho phép node tự động "thức dậy" từ chế độ ngủ khi nhận được một thông điệp trên bus CAN.
  - ENABLE: Kích hoạt chế độ này. Node sẽ tự động thoát khỏi trạng thái ngủ khi có bất kỳ tín hiệu nào trên bus CAN.
  - DISABLE: Tắt chế độ này, node chỉ có thể thoát khỏi chế độ ngủ thông qua can thiệp của phần mềm.
5. CAN_NART (No Automatic Retransmission):
- CAN_NART quyết định xem khung dữ liệu có tự động được truyền lại nếu không có tín hiệu ACK (Acknowledgment) từ các node khác hay không.
  - ENABLE: Node sẽ không tự động truyền lại khung dữ liệu nếu không nhận được tín hiệu ACK từ các node khác.
  - DISABLE: Node sẽ tự động truyền lại khung dữ liệu nếu không nhận được ACK.
6. CAN_RFLM (Receive FIFO Locked Mode)
- CAN_RFLM quyết định xem FIFO sẽ bị khóa hay ghi đè nếu dữ liệu không được đọc kịp thời.
  - ENABLE: Khi FIFO đã đầy, không có khung dữ liệu mới nào được nhận vào cho đến khi dữ liệu cũ được đọc.
  - DISABLE: Dữ liệu mới sẽ ghi đè lên dữ liệu cũ nếu FIFO đầy.
7. CAN_TXFP (Transmit FIFO Priority)
- CAN_TXFP quyết định độ ưu tiên của các khung dữ liệu trong FIFO.
  - ENABLE: Khung dữ liệu có độ ưu tiên cao hơn sẽ được truyền trước, không tuân theo thứ tự nhập vào FIFO.
  - DISABLE: Khung dữ liệu sẽ được truyền theo thứ tự FIFO (First-In-First-Out), tức là khung nào vào trước sẽ được truyền trước.
## Gửi và nhận dữ liệu CAN
1. Gửi dữ liệu CAN:
Sử dụng hàm CAN_Transmit để gửi một Data Frame với tối đa 8 byte dữ liệu.
```
void CAN_TransmitData(uint8_t* data, uint8_t length) {
    CanTxMsg TxMessage;

    TxMessage.StdId = 0x123;  // Sử dụng định danh 11-bit
    TxMessage.RTR = CAN_RTR_DATA;  // Data Frame
    TxMessage.IDE = CAN_ID_STD;  // Standard ID
    TxMessage.DLC = length;  // Số byte dữ liệu

    // Copy dữ liệu vào trường dữ liệu của khung
    for (int i = 0; i < length; i++) {
        TxMessage.Data[i] = data[i];
    }

    CAN_Transmit(CAN1, &TxMessage);
    while (CAN_TransmitStatus(CAN1, CAN_TransmitStatus_Complete) != CAN_TxStatus_Ok);
}
```
2. Nhận dữ liệu CAN:
Sử dụng hàm CAN_Receive để nhận một Data Frame từ FIFO buffer.
```
void CAN_ReceiveData(uint8_t* data, uint8_t* length) {
    CanRxMsg RxMessage;

    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);  // Nhận từ FIFO 0

    // Copy dữ liệu nhận được
    *length = RxMessage.DLC;  // Độ dài dữ liệu
    for (int i = 0; i < RxMessage.DLC; i++) {
        data[i] = RxMessage.Data[i];
    }
}
```
## Cấu hình bộ lọc CAN:
- CAN_FilterInitTypeDef được sử dụng để cấu hình bộ lọc. Có thể thiết lập các bộ lọc với chế độ lọc theo ID Mask hoặc ID List, với khả năng lọc theo định danh 11-bit (Standard ID) hoặc 29-bit (Extended ID).
  - ID Mask: Node chỉ nhận khung dữ liệu khi định danh khung và định danh của bộ lọc khớp với nhau dựa trên mặt nạ.
  - ID List: Node chỉ nhận các khung có định danh khớp hoàn toàn với một trong các giá trị trong danh sách định danh của bộ lọc.
```
void CAN_FilterConfiguration(void) {
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    // Sử dụng bộ lọc số 0, chế độ ID Mask, định danh 11-bit
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x321 << 5;  // Định danh Standard ID 0x321
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFE0;  // Mặt nạ chỉ so sánh 11-bit ID
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;  // Đưa khung vào FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;

    // Khởi tạo bộ lọc
    CAN_FilterInit(&CAN_FilterInitStructure);
}
```
## Sử dụng ngắt CAN 
STM32F103 hỗ trợ nhiều loại ngắt CAN, bao gồm:
- Ngắt khi có dữ liệu mới trong FIFO.
- Ngắt khi FIFO đầy.
- Ngắt khi một khung dữ liệu được truyền thành công.
- Ngắt khi có lỗi trong quá trình truyền hoặc nhận dữ liệu.
1. Kích hoạt ngắt: 
```
void CAN_Interrupt_Configuration(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    // Kích hoạt ngắt CAN trong NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Kích hoạt ngắt nhận dữ liệu mới vào FIFO0
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}
```
2. Hàm xử lí ngắt:
```
void USB_LP_CAN1_RX0_IRQHandler(void) {
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
        CanRxMsg RxMessage;

        // Nhận dữ liệu từ FIFO0
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

        // Xử lý dữ liệu nhận được...
        // Ví dụ: Lưu trữ dữ liệu hoặc phản hồi lại khung dữ liệu này.

        // Xóa cờ ngắt
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}
```
## Đọc và xử lý lỗi:
```
void check_CAN_error(void) {
    uint8_t error_code = CAN_GetLastErrorCode(CAN1);  // Lấy mã lỗi từ CAN

    if (error_code == CAN_ErrorCode_BitRecessive) {
        // Xử lý lỗi bit recessive (node phát hiện lỗi bit do không đồng bộ tín hiệu)
    } else if (error_code == CAN_ErrorCode_CRC) {
        // Xử lý lỗi CRC (kiểm tra CRC thất bại)
    } else if (error_code == CAN_ErrorCode_Stuff) {
        // Xử lý lỗi stuff (nhiều hơn 5 bit giống nhau liên tiếp)
    }

    // Kiểm tra giá trị Error Counter
    uint8_t txErrorCounter = CAN_GetLSBTransmitErrorCounter(CAN1);  // Lấy bộ đếm lỗi truyền
    uint8_t rxErrorCounter = CAN_GetReceiveErrorCounter(CAN1);  // Lấy bộ đếm lỗi nhận

    if (txErrorCounter > 127 || rxErrorCounter > 127) {
        // Node đang ở trạng thái Passive Error
        // Node đã gặp nhiều lỗi, không thể can thiệp sửa lỗi
    } else if (txErrorCounter <= 127 && rxErrorCounter <= 127) {
        // Node đang ở trạng thái Active Error
        // Node vẫn có thể phát hiện và sửa lỗi chủ động
    }
}
```
## Xử lí ngắt Overload Frame
```
void USB_LP_CAN1_RX0_IRQHandler(void) {
    // Kiểm tra trạng thái FIFO Full
    if (CAN_GetITStatus(CAN1, CAN_IT_FF0) != RESET) {
        // Xử lý khi FIFO đầy
        // Lúc này phần cứng CAN sẽ tự động phát Overload Frame để trì hoãn giao tiếp

        // Xóa cờ FIFO full
        CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
    }
}
```
