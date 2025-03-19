# UART Software & UART Hardware
## UART Software
1. Xác định chân GPIO cho UART và cấp clock
```
#define TX_Pin	GPIO_Pin_0
#define RX_Pin	GPIO_Pin_1
#define UART_GPIO	GPIOA

void RCC_Config(){
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
```
2. Cấu hình chân GPIO cho UART
```
void GPIO_Config(){
GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
}
```
- Cấu hình chân Rx là Input floating.
- Cấu hình chân Tx là Output push pull.
3. Khởi tạo các chân trước khi truyền
```
void UARTSoftware_Init(){
GPIO_SetBits(UART_GPIO, TX_Pin);
delay_us(1);
}
```
4. Hàm truyền
```
void UARTSoftware_Transmitt(char c) {
    // Start bit
    GPIO_ResetBits(GPIOA, TX_Pin);
    clock();

    // Truyền các bit dữ liệu (LSB trước)
    for (int i = 0; i < 8; i++) {
        if (c & (1 << i)) {
            GPIO_SetBits(GPIOA, TX_Pin);
        } else {
            GPIO_ResetBits(GPIOA, TX_Pin);
        }
        clock();
    }
		
    // Stop bit
    GPIO_SetBits(GPIOA, TX_Pin);
		clock();
}
```
- Khi chân TX kéo từ 1 xuống 0 là bắt đầu khung truyền, sau đó đợi 1 clock.
- Sử dụng vòng for đọc liên tục 8 bit truyền vào. Kiểm tra data với mức 1, nếu bằng 1 thì kéo chân TX lên HIGH và ngược lại nếu bằng 0 thì để chân TX mức LOW.
- Mỗi bit gửi đi thì kèm theo 1 clock.
- Sau khi gửi đủ 8 bit thì kéo chân TX lên để báo truyền xong.
5. Hàm nhận
```
char UARTSoftware_Receive() {
    char c = 0;

    while (GPIO_ReadInputDataBit(GPIOA, RX_Pin) == 1);
    delay_us(bit_duration + bit_duration / 2);

    for (int i = 0; i < 8; i++) {
				
        if (GPIO_ReadInputDataBit(GPIOA, RX_Pin)) {
            c |= (1 << i);
        }
				clock(); 
    }
    delay_us(bit_duration / 2);

    return c;
}
```
- Chờ start bit
- Delay 1.5 period Time để tín hiệu được ổn định
- Sử dụng hàm for để đọc tín hiệu liên tục và dịch bit, mỗi lần đọc 1 bit sẽ delay 1 period Time
- Đợi Stop bit để hoàn thành 1 data frame
6. Bit kiểm tra chẵn/lẻ Parity
```
typedef enum{
	Parity_Mode_NONE,
	Parity_Mode_ODD,
	Parity_Mode_EVEN
}Parity_Mode;
```
- Khai báo các chế đọ Parity
```
uint16_t Parity_Generate(uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	uint8_t data1 = data;
	for(int i = 0; i < 8; i++){
		if(data1 & 0x01){
			count++;
		}
		data1 >>= 1;
	}
	switch(Mode){
		case Parity_Mode_NONE:
			return data; 
			break;
		case Parity_Mode_ODD:
			if(count % 2){
				return ((data << 1) | 1);
			} else {
				return (data<<1);
			}
			break;
		case Parity_Mode_EVEN:
			if(!(count % 2)){
				return ((data << 1) | 1);
			} else {
				return (data << 1);
			}
			break;
		default:
			return data;
			break;
	}
}
```
- Hàm tạo Parity bit bằng cách đếm số bit 1, từ đó tạo bit 1 hoặc 0 tương ứng với cấu hình Parity Mode.
```
uint8_t Parity_Check(uint8_t data, Parity_Mode Mode){
	uint8_t count = 0;
	for(int i = 0; i < 8; i++){
		if(data & 0x01){
			count++;
		}
		data >>= 1;
	}
	switch(Mode){
		case Parity_Mode_NONE:
			return 1; 
			break;
		case Parity_Mode_ODD:
			return (count % 2);
			break;
		case Parity_Mode_EVEN:
			return (!(count % 2));
			break;
		default:
			return 0;
			break;
	}
}
```
- Sau khi nhận data, kiểm tra bit chẵn/lẻ
## UART Hardware
1. Xác định chân UART
```
#define TX_Pin	GPIO_Pin_9
#define RX_Pin	GPIO_Pin_10
#define UART_GPIO	GPIOA
```
2. Cấu hình chân GPIO
```
void GPIO_Config(){
GPIO_InitTypeDef GPIOInitStruct;
	GPIOInitStruct.GPIO_Pin = RX_Pin;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
	//
	GPIOInitStruct.GPIO_Pin = TX_Pin;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART_GPIO, &GPIOInitStruct);
}
```
- Set output Tx là `GPIO_Mode_AF_PP`, Rx như software.
4. Cấu hình UART:
```
void UART_Config(){
	USART_InitTypeDef USART_InitStruct;
	//USART
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USARTInitStruct);
	USART_Cmd(USART1,ENABLE);
}
```
Các tham số để cấu hình UART:
- USART_Mode: Cấu hình chế độ hoạt động cho UART:
  - USART_Mode_Tx: Cấu hình truyền.
  - USART_Mode_Rx: Cấu hình nhận.
  - Có thể cấu hình cả 2 cùng lúc (song công).
- USART_BaudRate: Cấu hình tốc độ baudrate cho uart.
- USART_HardwareFlowControl: Cấu hình chế độ sử dụng tín hiệu phần cứng để kiểm soát truyền nhận.
- USART_WordLength: Cấu hình số bit mỗi lần truyền.
- USART_StopBits: Cấu hình số lượng stopbits.
- USART_Parity: cấu hình bit kiểm tra chẳn, lẻ.
5. Hàm truyền nhận UART:
```
void USART1_TransmitByte(uint8_t byte) {
    // Wait until the transmit data register is empty (TXE flag is set)
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    // Transmit the byte
    USART_SendData(USART1, byte);

    // Wait until transmission is complete (TC flag is set)
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

uint8_t USART1_ReceiveByte(void){
	uint8_t temp = 0x00;
    // Wait until data is received (RXNE flag is set)
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    // Read the received data
	temp = USART_ReceiveData(USART1);
	return temp;
}
```
Các hàm dùng để sử dụng và tương tác với UART:
- Hàm USART_SendData(USART_TypeDef* USARTx, uint16_t Data), truyền data từ UARTx. Data này đã được thêm bit chẵn/lẻ tùy cấu hình.
- Hàm USART_ReceiveData(USART_TypeDef* USARTx), nhận data từ UARTx.
- Hàm USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG) trả về trạng thái cờ USART_FLAG tương ứng:
  - USART_FLAG_TXE: Cờ truyền, set lên 1 nếu quá trình truyền hoàn tất.
  - USART_FLAG_RXNE: Cờ nhận, set lên 1 nếu quá trình nhận hoàn tất.
  - USART_FLAG_IDLE: Cờ báo đường truyền đang ở chế độ Idle.
  - USART_FLAG_PE: Cờ báo lỗi Parity.
