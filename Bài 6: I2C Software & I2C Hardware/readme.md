# I2C Software & I2C Hardware
## I2C Software
1. Định nghĩa chân cho I2C và cấp clock:
```
#define I2C_SCL 	GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7
#define I2C_GPIO 	GPIOB
```
- I2C_SCL: gửi tín hiệu clock
- I2C_SDA: truyền tín hiệu giữa Master và Slave
```
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
```
2. Cấu hình GPIO cho I2C:
```
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_SDA| I2C_SCL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}
```
- Cả 2 chân SCL và SDA đều hoạt động ở chế độ open-drain.
3. Reset các chân khi không truyền và đơn giản hoá việc gọi hàm:
```
void I2C_Config(){
	WRITE_SDA_1;
	delay_us(1);
	WRITE_SCL_1;
	delay_us(1);
}
#define WRITE_SDA_0 	GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1 	GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0 	GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1 	GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)
```
4. Các điều kiện để bắt đầu và kết thúc truyền I2C:
```
void I2C_Start(){
	
	WRITE_SCL_1;  	
	delay_us(3);	
	WRITE_SDA_1;
	delay_us(3);
	WRITE_SDA_0;	//SDA reset xuống 0 trước SCL.
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}
```
- Kéo SDA xuống 0 trước SCL một khoảng Delay nhỏ.
```
void I2C_Stop(){
	
	WRITE_SDA_0;
	delay_us(3);
	WRITE_SCL_1; 	//SCL set lên 1 trước SDA.
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}
```
- Kéo SCL lên 1 trước SDA 1 khoảng delay nhỏ.
5. Hàm truyền:
```
status I2C_Write(uint8_t u8Data){	
	uint8_t i;
	status stRet;
	for(int i=0; i< 8; i++){		// ghi data
		if (u8Data & 0x80) {
			WRITE_SDA_1;
		} else {
			WRITE_SDA_0;
		}
		delay_us(3);
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(5);
		u8Data <<= 1;
	}
	WRITE_SDA_1;		   // đợi ACK
	delay_us(3);
	WRITE_SCL_1;					
	delay_us(3);
	
	if (READ_SDA_VAL) {	//đọc ACK
		stRet = NOT_OK;				
	} else {
		stRet = OK;					
	}

	delay_us(2);
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}
```
- Bắt đầu với việc lấy một byte dữ liệu (u8Data) và sẽ gửi từng bit một qua chân SDA bằng cách sử dụng phép | với bit mask 0x80.
- Nếu bit đó là 1, thì chân SDA sẽ được kéo lên mức cao, và nếu không thì sẽ kéo xuống mức thấp. 
- Sau đó, chân SCL được kéo lên mức cao để tạo xung clock trên SDA, rồi lại kéo về mức thấp để kết thúc một chu kỳ. Dịch trái 1 bit `u8Data <<= 1`
- Sau khi đã gửi đủ 8 bit, bạn kéo chân SDA, SCL lên 1 để chuẩn bị nhận bit ACK/NACK từ thiết bị bên nhận.
- Nếu chân SDA ở mức cao khi bạn đọc nó, điều đó có nghĩa là thiết bị bên nhận đã không xác nhận (NACK), trong khi mức thấp có nghĩa là thiết bị bên nhận đã xác nhận (ACK).
6. Hàm nhận:
```
uint8_t I2C_Read(ACK_Bit _ACK){	
	uint8_t i;						
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);	
	for (i = 0; i < 8; ++i) {
		u8Ret <<= 1;
		WRITE_SCL_1;
		delay_us(3);
		if (READ_SDA_VAL) {
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}
if (_ACK) {	
		WRITE_SDA_0;
	} else {
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}
```
- Trong vòng lặp for, hàm sẽ thực hiện việc đọc từng bit từ bên nhận, tổng cộng 8 bit. Mỗi lần lặp, biến u8Ret sẽ được dịch trái (u8Ret <<= 1) để lưu MSB trước.
- Sau khi đọc đủ 8 bit, hàm kiểm tra tham số _ACK để xác định xem có nên gửi bit ACK hay NACK không.
- Nếu _ACK là true, chân SDA sẽ được kéo xuống mức thấp (WRITE_SDA_0) để gửi bit ACK. Nếu không, chân SDA sẽ được giữ ở mức cao (WRITE_SDA_1) để gửi bit NACK.
## I2C Hardware
1. Cấu hình chân và cấp clock cho I2C:
```
void GPIO_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

```
2. Cấu hình tham số cho I2C:
```
void I2C_Config(){
	I2C_InitTypeDef I2C_InitStructure;
	//Set the clock speed of I2C. It has to be equal with the external device
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	//I2C mode
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//I2C device adress
	I2C_InitStructure.I2C_OwnAddress1 = 0x33; 
	//I2C Acknowladge configuration
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//Enable the I2C with the prepared configuration
	I2C_Init(I2C1, &I2C_InitStructure);
	//And start the I2C 
	I2C_Cmd(I2C1, ENABLE);
}
```
- I2C_Mode: Cấu hình chế độ hoạt động cho I2C:
      
  - I2C_Mode_I2C: Chế độ I2C FM(Fast Mode);
  - I2C_Mode_SMBusDevice&I2C_Mode_SMBusHost: Chế độ SM(Slow Mode).
- I2C_ClockSpeed: Cấu hình clock cho I2C, tối đa 100khz với SM và 400khz ở FM.
- I2C_DutyCycle: Cấu hình chu kì nhiệm vụ của xung:
  - I2C_DutyCycle_2: Thời gian xung thấp/ xung cao =2;
  - I2C_DutyCycle_16_9: Thời gian xung thấp/ xung cao =16/9;
- I2C_OwnAddress1: Cấu hình địa chỉ thiết bị đang cấu hình.
- I2C_Ack: Cấu hình ACK, có sử dụng ACK hay không.
- I2C_AcknowledgedAddress: Cấu hình số bit địa chỉ. 7 hoặc 10 bit
3. Hàm gửi địa chỉ, hàm gửi/nhận byte data
- Gửi bit địa chỉ, đợi Slave xác nhận
```
I2C_GenerateSTART(I2C1, ENABLE);
 //Waiting for flag
 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
I2C_Send7bitAddress(I2C1, 0x44, I2C_Direction_Transmitter);
//And check the transmitting
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
```
- Hàm gửi data
```
void Send_I2C_Data(uint8_t data)
{
	I2C_SendData(I2C1, data);
	// wait for the data trasnmitted flag
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
```
- Hàm nhận data
```
uint8_t Read_I2C_Data(){
	
	uint8_t data = I2C_ReceiveData(I2C1);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
	return data;
}
```
Các hàm để tương tác và làm việc với I2C:
- Hàm I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction), gửi đi 7 bit address để xác định slave cần giao tiếp. Hướng truyền được xác định bởi I2C_Direction để thêm bit RW.
- Hàm I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data) gửi đi 8 bit data.
- Hàm I2C_ReceiveData(I2C_TypeDef* I2Cx) trả về 8 bit data.
- Hàm I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT) trả về kết quả kiểm tra I2C_EVENT tương ứng:
  - I2C_EVENT_MASTER_MODE_SELECT: Đợi Bus I2C về chế độ rảnh.
  - I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: Đợi xác nhận của Slave với yêu cầu ghi của Master.
  - I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED: Đợi xác nhận của Slave với yêu cầu đọc của Master.
  - I2C_EVENT_MASTER_BYTE_TRANSMITTED: Đợi truyền xong 1 byte data từ Master.
  - I2C_EVENT_MASTER_BYTE_RECEIVED: Đợi Master nhận đủ 1 byte data.
