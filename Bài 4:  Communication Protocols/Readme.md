# Các chuẩn giap tiếp cơ bản (Communication protocols)
## Truyền nhận dữ liệu:
- Các MCU giao tiếp với các MCU khác hay các thiết bị ngoại vi khác thông qua tín hiệu điện áp. Vi điều khiển sẽ sử dụng các chuẩn giao thức khác nhau để truyền và nhận như UART, SPI, I2C ...
## SPI (serial Peripheral Interface):
1. SPI là chuẩn giao tiếp có đồng bộ (sử dụng xung clock để đồng bộ tín hiệu) và hoạt động ở chế độ full_Duplex (song công), có thể kết nối với nhiều Slave, hoạt động với tốc độ cao, thường đạt từ Mbps đến hàng chục Mbps.
2. SPI yêu cầu 4 chân cho mỗi Slave:
- SCK: Master cung cấp xung clock cho slave để việc truyền nhận được đồng nhất với nhau.
- MOSI (Master Out Slave In): Master sẽ truyền tín hiệu cho Slave
- MISO (Master In Slave Out): Slave sẽ truyền tín hiệu cho Master.
- SS/CS (Slave Select/Chip Select): Chọn thiết bị Slave cụ thể. Chủ động kéo chân SS xuống mức Low nếu muốn giao tiếp với Slave cụ thể.

  ![image](https://github.com/user-attachments/assets/d274c482-be62-4560-aef4-7d9d199367af)

3. Các chế độ của SPI:
- Mode 0 (CPOL = 0, CPHA = 0): SCL ở mức thấp khi không hoạt động, lấy mẫu ở cạnh lên đầu tiên.
- Mode 1 (CPOL = 0, CPHA = 1): SCL ở mức thấp khi không hoạt động, lấy mẫu ở cạnh xuống thứ 2.
- Mode 2 (CPOL = 1, CPHA = 0): SCL ở mức cao khi không hoạt động, lấy mẫu ở cạnh xuống đầu tiên.
- Mode 3 (CPOL = 1, CPHA = 1): SCL ở mức cao khi không hoạt động, lấy mẫu ở cạng lên thứ 2.

  ![image](https://github.com/user-attachments/assets/dd954117-1ce5-4721-a85f-d308b98eaf56)

4. Cách hoạt động của SPI:
- Master sẽ kéo chân SS của Slave muốn truyền xuống mức Low để báo hiệu bắt đầu truyền.
- Xung clock sẽ được cấp bởi Master, với mỗi xung Clock, 1 bit sẽ truyền Master vào Slave và ngược lại
- Cập nhật lại thanh ghi và dịch 1 bit để tiếp tục truyền. Cứ như vậy sau 8 chu kỳ xung Clock thì truyền nhận xong 1 byte.
## I2C (Inter - Integrated Circuit):
1. I2C là chuẩn giao thức nối tiếp có đồng bộ (sử dụng Clock từ Master), bán song công (half_Duplex), có thể kết nối nhiều thiết bị trên một bus (kể cả nhiều Master), so với SPI thì I2C sử dụng ít dây hơn (2 dây : SDA và SCL).
2. I2C sử dụng 2 dây để truyền dữ liệu giữa các thiết bị:
- SDA (Serial Data): Dây dữ liệu dùng để các thiết bị truyền và nhận.
- SCL (Serial Clock): Dây Clock tạo tín hiệu đồng bộ hoá quá trình truyền nhận dữ liệu.
3. Cách hoạt động của I2C:
- Bus của SCL và SDA đều hoạt động ở chế độ open-drain, vì vậy để tránh ngắn mạch khi có một bus lên 1 và một bus xuống 0 thì sẽ một điện trở được nối lên nguồn mặc định ở mức cao.
- Điều kiện Start và Stop: khởi động khi SDA chuyển từ cao xuống thấp trước khi SCL chuyển từ cao xuống thấp, dừng lại khi SDA chuyển từ thấp lên cao sau khi SCL chuyển từ thấp lên cao.
- Khung địa chỉ: Gửi từ 7-10 bit để xác định địa chỉ của Slave muốn giao tiếp, bit cuối cùng sẽ xác định xem là read hay write.
- Bit ACK/NACK: Slave có địa chỉ trùng khớp sẽ kéo SDA xuống LOW nếu nhận được địa chỉ, nếu không bus SDA vẫn đang ở mức HIGH, Master sẽ nhận biết là NACK.
4. Quá trình truyền nhận:

  ![image](https://github.com/user-attachments/assets/e54d2e5e-f37a-442e-a759-d9f00fbe2201)

- Khi bắt đầu, Master sẽ gửi điều kiện start, tiếp tục gửi địa chỉ của Slave mà nó muốn giao tiếp cùng với bit read/write.
- Slave sẽ so sánh địa chỉ vừa nhận so với chính nó, trùng khớp thì gửi ACK kéo SDA về LOW, không thì SDA vẫn mức HIGH và nhận biết là NACK.
- Master sẽ gửi hoặc nhận khung dữ liệu, với mỗi khung thì phía nhận sẽ trả về Ack.
- Để dừng truyền thì Master truyền điều kiện stop.
## UART (Universal Asynchronous Receiver-Transmitter)
1. UART là một giao thức truyền thông không đồng bộ (không sử dụng Clock chung mà sử dụng baudrate), truyền nối tiếp (dữ liệu truyền từng bit qua dây, bắt đầu bit stop, bit dữ liệu, parity bit, stop bit).
2. UART sử dụng 2 dây chính:
- TX (Transmit): Dây truyền dữ liệu.
- RX (Receive): Dây nhận dữ liệu
  
  ![image](https://github.com/user-attachments/assets/0dc7c929-63cb-44e6-8da4-c5a3bf81a4d7)

3. Quá trình truyền nhận:
- Cấu hình baudrate: cấu hình cả 2 thiết bị truyền nhận cùng baudrate (9600, 115200...) để cả 2 truyền nhận đúng lúc.
- Khi bắt đầu, gửi bit khởi động (đưa mức tín hiệu từ mức cao xuống thấp).
- Truyền các bit data: Dữ liệu truyền từng bit (5-9 bit dữ liệu), truyền LSB trước.
- Parity Bit: Bit tuỳ chọn, dùng để kiểm tra lỗi, có thể chọn odd parity hoặc even parity.
- Bit dừng: Bit stop (1 hoặc 2 bit) được gửi để báo hiệu kết thúc một byte dữ liệu (đưa mức tín hiệu trở lại mức cao).

![image](https://github.com/user-attachments/assets/7ec973a7-1a7d-4956-9039-2080c5b2a0ce)

