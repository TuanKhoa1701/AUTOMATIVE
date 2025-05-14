# SPI Software & SPI Hardware 
## SPI Software:
1. Đầu tiên định nghĩa các chân SPI bằng các chân GPIO và RCC, ở đây là GPIOA.
```
#define SPI_MISO_Pin GPIO_Pin_1
#define SPI_MOSI_Pin GPIO_Pin_2
#define SPI_CS_Pin GPIO_Pin_3
#define SPI_GPIO GPIOA
#define SPI_RCC RCC_APB2Periph_GPIOA
```
2. Cấp Clock cho GPIOA và TIM2
```
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
```
3. Cấu hình cho các chân GPIO cho các hoạt động của SPI.
- Cấu hình chân cho con Master.
```
void GPIO_Config(){
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}
```
- Chân cấu hình cho con Slave
```
void GPIO_Config(){
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = SPI_SCK_Pin | SPI_MOSI_Pin | SPI_CS_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
	
GPIO_InitStructure.GPIO_Pin = SPI_MISO_Pin;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}
```
Trong đó bao gồm:
- SCK: cung cấp xung clock để đồng bộ hoá tín hiệu, cấu hình là Output cho Master và Input cho Slave 
- MOSI: chân tín hiệu truyền từ Master tới Slave, cấu hình Output cho Master và Input cho Slave.
- MISO: chân tín hiệu truyền ngược lại, cấu hình Input cho Master và Output cho Slave.
- CS: chân chọn Slave muốn cấu hình, cấu hình là Output cho Master và Input cho Slave.
4. Tạo hàm khởi tạo SPI trước khi bắt đầu truyền
```
void SPI_Init(){
GPIO_WriteBit(SPI_GPIO, SPI_SCK_Pin, Bit_RESET);
GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
GPIO_WriteBit(SPI_GPIO, SPI_MISO_Pin, Bit_RESET);
GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
}
```
- Reset các chân, SCK, MOSI, MISO về 0 trước khi truyền.
-  Khi chưa truyền thì chân chip select ở mức 1.
5. Tạo delay để mô phỏng xung clock
```
void TIM_Config()
{
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_InitStruct.TIM_Prescaler = 7200 - 1; 
TIM_InitStruct.TIM_Period = 0xFFFF;
TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;    
TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
TIM_Cmd(TIM2, ENABLE);
}

void delay_ms(uint32_t time)
{
TIM_SetCounter(TIM2, 0);
while (TIM_GetCounter(TIM2) < time * 10) {}
}

void Clock()
{
GPIO_WriteBit(SPI_PORT, SPI_SCK, 1);
delay_ms(4);
GPIO_WriteBit(SPI_PORT, SPI_SCK, 0);
delay_ms(4);
}
```
6. Hàm Truyền từ Master
```
void SPI_Master_Transmit(uint8_t u8Data)
{	
	uint8_t u8Mask = 0x80;	
	uint8_t tempData;
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
	delay_ms(1);
	for(int i = 0; i < 8; i++)
    {
		tempData = u8Data & u8Mask;
		if(tempData)
        {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		}
         else
        {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		u8Data = u8Data << 1;
		Clock();
	}
	GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
	delay_ms(1);
}
```
- Khởi tạo biến `u8Mask = 0x80` để chọn dữ liệu bit muốn gửi từ u8Data bắt đầu từ MSB và gán vào biến `tempData`
```
	uint8_t u8Mask = 0x80;	
	uint8_t tempData;
```
- Kéo chân Chip select xuống để chọn slave
```
GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_RESET);
delay_ms(1);
```
- Tạo vòng lặp từng bit trong một byte, truyền tín hiệu qua chân MOSI, sau đó dịch u8Data để kiểm tra và tiếp tục truyền, mỗi lần làm vậy gửi xung clock.
```
for(int i = 0; i < 8; i++)
    {
		tempData = u8Data & u8Mask;
		if(tempData)
        {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_SET);
			delay_ms(1);
		}
         else
        {
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI_Pin, Bit_RESET);
			delay_ms(1);
		}
		u8Data = u8Data << 1;
		Clock();
	}
```
- Sau khi truyền xong 1byte thì setup chân CS lên HIGH để ngừng truyền
```
GPIO_WriteBit(SPI_GPIO, SPI_CS_Pin, Bit_SET);
delay_ms(1);
```
7. Hàm nhận ở Slave
```
uint8_t SPI_Slave_Receive(void)
{
uint8_t dataReceive = 0x00;	
uint8_t temp = 0x00;
while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	for(int i = 0; i < 8; i++)
    { 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin))
        {
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin))
            {
			    temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}
			dataReceive <<= 1;
			dataReceive |= temp;
    	}
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
	return dataReceive;
}
```
- Khởi tạo `dataReceive = 0` để lưu giá trị và `temp` để lấy tín hiệu từ chân `MOSI`
```
uint8_t dataReceive = 0x00;	
uint8_t temp = 0x00;
```
- Check chân CS và xung clock từ Master
```
while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
```
- Tạo vòng lặp từng bit, nếu SCK đang ở mức 1, Slave sẽ đọc giá trị trên chân MOSI và ghi vào temp. Dịch bit trên dataReceive chuẩn bị nhận bit tiếp theo, sau đó chờ cho SCK chuyển xuống LOW để chuẩn bị cho bit tiếp the
```
for(int i = 0; i < 8; i++)
    { 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin))
        {
			while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin))
            {
			    temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI_Pin);
			}
			dataReceive <<= 1;
			dataReceive |= temp;
    	}
		while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK_Pin));
	}
```
- Chờ cho chân CS trở về trạng thái HIGH sau khi truyền 8 bit.
```
while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_CS_Pin));
return dataReceive;
```
## SPI Hardware:
1. Định nghĩa chân cho SPI
```
#define SPI1_NSS 	GPIO_Pin_4
#define SPI1_SCK	GPIO_Pin_5
#define SPI1_MISO 	GPIO_Pin_6
#define SPI1_MOSI 	GPIO_Pin_7
#define SPI1_GPIO 	GPIOA
```

![image](https://github.com/user-attachments/assets/6b9547ce-33a0-4234-8de5-ef0aaa6a7261)

2. Cấu hình xung clock và chân GPIO cho SPI:
```
void RCC_Config() 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SPI1_NSS | SPI1_SCK | SPI1_MISO | SPI1_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_Init(SPI1_GPIO, &GPIO_InitStructure);
}
```
3. Cấu hình SPI:
```
void SPI_Config(){
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}
```
Các tham số cấu hình:
- SPI_Mode: Quy định chế độ hoạt động của thiết bị SPI. 
- SPI_Direction: Quy định kiểu truyền của thiết bị.
- SPI_BaudRatePrescaler: Hệ số chia clock cấp cho Module SPI.
- SPI_CPOL: Cấu hình giá trị SCK khi chế độ nghỉ: SPI_CPOL_Low (mức 0), SPI_CPOL_High (mức 1)
- SPI_CPHA: khoảnh khắc lấy mẫu dữ liệu từ Slave theo SCL: SPI_CPHA_1Edge: (ở cạnh xung đầu tiên), SPI_CPHA_2Edge: (ở cạnh xung thứ hai).
- SPI_DataSize: Cấu hình số bit truyền. 8 hoặc 16 bit.
- SPI_FirstBit: Cấu hình chiều truyền của các bit là MSB hay LSB.
- SPI_CRCPolynomial: Cấu hình số bit CheckSum cho SPI.
- SPI_NSS: Cấu hình chân SS là điều khiển bằng thiết bị hay phần mềm.
4. Hàm truyền nhận của SPI
```
void SPI_Send1Byte(uint8_t data){
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_RESET);
   
    SPI_I2S_SendData(SPI1, data);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==0);
   
    GPIO_WriteBit(SPI1_GPIO, SPI1_NSS, Bit_SET);
}

uint8_t SPI_Receive1Byte(void){
    uint8_t temp;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)==1);
    temp = (uint8_t)SPI_I2S_ReceiveData(SPI1);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==0);
    return temp;
}
```
Một số hàm để làm việc với SPI:
- HàmSPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data), tùy vào cấu hình datasize là 8 hay 16 bit sẽ truyền đi 8 hoặc 16 bit dữ liệu. Hàm nhận 2 tham số là bộ SPI sử dụng và data cần truyền.
- Hàm SPI_I2S_ReceiveData(SPI_TypeDef* SPIx) trả về giá trị đọc được trên SPIx. Hàm trả về 8 hoặc 16 bit data.
- Hàm SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG) trả về giá trị 1 cờ trong thanh ghi của SPI. Các cờ thường được dùng:
  - SPI_I2S_FLAG_TXE: Cờ báo truyền, cờ này sẽ set lên 1 khi truyền xong data trong buffer.
  - SPI_I2S_FLAG_RXNE: Cờ báo nhận, cờ này set lên 1 khi nhận xong data.
  - SPI_I2S_FLAG_BSY: Cờ báo bận,set lên 1 khi SPI đang bận truyền nhận.
