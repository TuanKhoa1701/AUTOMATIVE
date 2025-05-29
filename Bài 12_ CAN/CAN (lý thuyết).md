# CAN
## Giới thiệu
- Giao thức CAN (Controller Area Network) là một giao thức truyền thông được sử dụng rộng rãi trong các hệ thống nhúng, đặc biệt là ứng dụng trong lĩnh vực ô tô và hệ thống công nghiệp.
![image](https://github.com/user-attachments/assets/5ccd9df5-5bee-46b7-8091-9f37a42ee62b)

## Kiến trúc
- Giao thức CAN sử dụng tô-pô bus để kết nối các thiết bị với nhau, nghĩa là tất cả các thiết bị (node) đều được kết nối song song vào một cặp dây truyền thông chung được gọi là CAN bus.
- CAN bus này gồm hai dây tín hiệu chính, là:
  - CANH (CAN High): Dây tín hiệu cao.
  - CANL (CAN Low): Dây tín hiệu thấp.
- Các tín hiệu truyền qua bus CAN là tín hiệu vi sai (differential signaling), nghĩa là thông tin được mã hóa dựa trên sự chênh lệch điện áp giữa hai dây CANH và CANL.
- Hai dây tín hiệu này được xoắn lại tạo thành đường dây xoắn đôi giúp:
  - Giảm thiểu nhiễu từ môi trường bên ngoài: Khi các dây được xoắn lại, mỗi đoạn của cặp dây sẽ nhận nhiễu với cường độ khác nhau và theo các hướng ngược nhau, làm triệt tiêu phần lớn nhiễu điện từ.
  - Giảm thiểu nhiễu xuyên âm: Việc xoắn đôi các dây giúp giảm hiện tượng này bằng cách phân tán nhiễu xuyên âm ra khắp chiều dài của cáp.
## Điện trở kết thúc và trở kháng đặc trưng
- Tín hiệu chạy qua dây cáp khi có sự thay đổi đột ngột về trở kháng thì tín hiệu sẽ bị phản xạ ngược lại gây ra nhiễu hoặc méo tín hiệu
- Điện trở ở cuối đường dây có chức năng: 
  - Khớp với điện trở đực trưng của đoạn dây (120 Ohmn)
  - Ngăn chặn hiện tượng phản xạ của tín hiệu
- Trở kháng đặc trưng của dây phụ thuộc:
  - Đường kính của dây
  - Khoảng cách giữa 2 dây (CAN_H và CAN_L)
  - Loại vật liệu cách điện của 2 dây
  ## Các thiết bị trên bus CAN:
Mạng CAN hỗ trợ nhiều loại thiết bị khác nhau trên cùng một bus, mỗi thiết bị được gọi là một node.
![image](https://github.com/user-attachments/assets/be4c947a-6dac-4c53-af08-5fd70c35e4fc)

Các node nếu muốn gửi và nhận dữ liệu CAN thì bên trong các node cần có những thành phần sau:
1.  Bộ điều khiển CAN (CAN Controller): Đây là thành phần chính trong node CAN, có nhiệm vụ xử lý toàn bộ giao tiếp CAN.
  - Gửi và nhận thông điệp CAN.
  - Điều khiển truy cập vào bus CAN (arbitration).
  - Phát hiện và xử lý các lỗi truyền thông CAN.
  - Kiểm soát việc truyền lại thông điệp khi gặp lỗi.
  - Cung cấp giao diện giữa các vi điều khiển và bus CAN.
2. Transceiver CAN (CAN Transceiver):
  - Chuyển đổi tín hiệu số từ bộ điều khiển CAN thành tín hiệu điện áp dạng differential (CANH và CANL) để gửi lên bus CAN và ngược lại
  - Đảm bảo tín hiệu truyền và nhận trên bus CAN có độ chính xác và tốc độ cao.
3. Vi điều khiển (Microcontroller): là thành phần trung tâm điều khiển hoạt động của node CAN.
  - Đọc và xử lý thông điệp CAN.
  - Tạo ra thông điệp CAN để truyền đi.
  - Quản lý các khung dữ liệu, bit arbitration và quá trình xử lý lỗi.
  - Điều khiển hành vi của node (ví dụ: bật/tắt node, reset node khi gặp lỗi bus-off).
 ## Tín hiệu trên Bus CAN
CAN Transceiver chuyển đổi qua lại tín hiệu số (CAN Controller) và tín hiệu tương tự (CAN Transceiver)
![image](https://github.com/user-attachments/assets/9ca9ed58-a0a7-43c9-a3dd-099ef882bb91)

Trong giao thức CAN:
- Mức dominant, nghĩa là mức "1".
- Mức recessive, nghĩa là mức "0".
Có 2 dạng truyền:
- CAN low speed: tốc độ là 125 kb/s
- CAN high speed:tốc độ đạt từ 125 kb/s tới 1 Mb/s, 
Vi sai ở mức dominant (0) là >= 0,9 (V), recessive (1) là =< 0,5 (V).

CAN có 2 cơ chế để chống nhiễu tốt: một là dùng tín hiệu vi sai (hiệu điện thế không đổi -> tín hiệu không bị sai lệch) và 2 dây được xoắn vào nhau (Nhiễu cảm ứng sẽ luân phiên đảo chiều qua từng vòng xoắn → tự triệt tiêu).
## Đặc điểm giao tiếp CAN
1. Không cần máy tính chủ (No Master-Slave architecture).
2. Truyền thông quảng bá (Broadcast Communication)
3. Trang chấp quyền gửi (arbitration)
- Khi nhiều node muốn truyền dữ liệu, chúng đều bắt đầu gửi thông điệp của mình lên bus. Tín hiệu được gửi đồng thời và mỗi node sẽ kiểm tra từng bit của dữ liệu trên bus.
- Mỗi bit trong ID sẽ được truyền từng cái một (từ bit MSB - Most Significant Bit). Nếu một node gửi bit recessive (1) nhưng nhận thấy trên bus có bit dominant (0), nghĩa là có một node khác có ưu tiên cao hơn đang chiếm quyền truyền dữ liệu. Lúc này, node này sẽ ngừng truyền và chuyển sang chế độ nghe (listen).
- Node có ID thấp hơn (tức là có nhiều bit dominant hơn ở đầu) sẽ tiếp tục quá trình truyền cho đến khi toàn bộ ID được gửi đi, trong khi các node khác ngừng gửi và chuyển sang chế độ chờ.
- Các node không thắng quá trình arbitrage sẽ không bị mất dữ liệu mà chỉ đơn giản là đợi lượt tiếp theo để cố gắng truyền lại thông điệp của mình.
- Khi một node thua trong quá trình arbitration, nó sẽ tạm dừng việc truyền nhưng không xóa dữ liệu của mình.
- Node thua sẽ chuyển sang trạng thái chờ và lắng nghe bus. Khi bus không còn bận (tức là node thắng đã gửi xong thông điệp), node thua sẽ thử lại và tham gia tranh chấp quyền gửi ở lần tiếp theo.
- Quá trình này đảm bảo rằng không có dữ liệu bị mất trong quá trình tranh chấp, vì các node thua sẽ tiếp tục gửi thông điệp của mình vào thời điểm thích hợp.
4. Giao tiếp song công (Full-duplex Communication)
5. Phát hiện và xử lý lỗi tự động
## Khung truyền của CAN
1. Data Frame:

![image](https://github.com/user-attachments/assets/227b22b6-81d4-4858-91ab-dca9826b64bb)

- Khung phổ biến nhất trong CAN, dùng để truyền dữ liệu giữa các node với nhau.
Cấu trúc:
- SOF: 1 bit bắt đầu (dominant để bắt đầu truyền)
- Arbitration Field: 
  - Chứa địa chỉ node gửi và node nhận, 11 bit (tiêu chuẩn) hoặc 29 bit (mở rộng).
  - Bit RTR để phân biệt Data Frame (0) hay Remote Frame (1).
  - SRR (Substitute Remote Request):Bit này chỉ có ở khung mở rộng và giá trị là 1 (recessive bit).
- Control Field: chứa DLC chỉ ra số byte dữ liệu cần truyền (0-8 byte).
  - Khung chuẩn gồm IDE, r0 và DLC (4 bit).Khung mở rộng thêm r1
  - Bit r0, r1(hai bit dự trữ): phải được truyền là recessive bit bởi bộ truyền nhưng không quan tâm đến 2 bit này.
- Data Field: Chứa dữ liệu thực tế cần truyền.
- CRC Field (Cyclic Redundancy Check): Dùng để phát hiện lỗi trong quá trình truyền thông qua mạng.
  
  ![image](https://github.com/user-attachments/assets/8db4b79a-3ba2-4bab-b003-dfaafc4b8a05)

  - CRC Sequence gồm 15 bit CRC tuần tự.
  - CRC Delimiter: là 1 recessive bit làm nhiệm vụ phân cacshhs trường CRC với ACK
- ACK Field (Acknowledgment): Node nhận sẽ gửi tín hiệu ACK
  ![image](https://github.com/user-attachments/assets/8c6e1ab3-c8e2-4f74-a9de-d5232527d94a)

  - ACK Slot: có độ dài 1 bit, Node truyền dữ liệu sẽ truyền bit này là recessive. Khi một hoặc nhiều Node nhận chính xác thông điệp( không có lỗi và đã so sánh với CRC Sequence trùng khớp) thì nố sẽ báo lại cho bộ truyền bằng cách truyền ra 1 dominant bit này vị trí ACK Slot để ghi đè lên recessive bit của bộ truyền.
  - ACK Delimiter: có độ dài 1 bit, nó luôn là một recessive bit.Như vậy ACK bit luon đặt giữa 2 recessive bit của CRC Delimiter và ACK Delimiter. 
- EOF là 7 recessive để kết thúc frame.
2. Remote Frame:
  
![image](https://github.com/user-attachments/assets/ab7c4461-0fd9-4b01-8585-3d2d0473a345)

![image](https://github.com/user-attachments/assets/29ccfea9-9b12-4a6f-b298-57ca0b2d0995)

- Một loại khung trong giao thức CAN được sử dụng để yêu cầu một node khác gửi dữ liệu qua Data Frame.
Cấu trúc: giống như Data Frame nhưng có 2 điểm khác
- bit RTR được cấp lên mức 1
- không có data Field
3. Eror Frame:
  
  ![image](https://github.com/user-attachments/assets/4aed4bff-88ea-4de0-a273-c6fb3b8da0b4)

- Khung được tự động phát ra bởi một node CAN khi nó phát hiện lỗi trong quá trình giao tiếp và yêu cầu quá trình truyền thông được khởi động lại.
- Trong giao thức CAN, có ba loại Error Frame:
  - Active Error Frame: Được phát ra bởi một node đang trong trạng thái Active Error, có thể can thiệp để sửa lỗi. Node này sẽ phát ra 6 bit Error Flag. (6 bit dominant)
  - Passive Error Frame: Được phát ra bởi một node trong trạng thái Passive Error, khi nó đã gặp nhiều lỗi nhưng không thể sửa lỗi. 12 bit Error Flag được gửi thay vì 6 bit (6 bit dominant và 6 bit recessive)
  - Bus Off: Node rơi vào trạng thái Bus Off khi số lượng lỗi tích lũy vượt qua một ngưỡng cao hơn. Node bị ngắt kết nối hoàn toàn khỏi bus.
Cấu trúc Frame:
- Error Flag: 6 hoặc 12 bit, phụ thuộc vào trạng thái lỗi (Active hoặc Passive).
- Error Delimiter: 8 bit để phân tách Error Frame với các khung khác.
Chuyển đổi giữa các trạng thái Active và Passive
- Error Counter: Các node CAN quản lý một Error Counter để theo dõi số lượng lỗi mà chúng gặp phải. Mỗi khi một lỗi được phát hiện, giá trị của Error Counter sẽ tăng lên.
  - Khi Error Counter vượt qua ngưỡng 127, node sẽ chuyển từ trạng thái Active Error sang Passive Error. Trong trạng thái này, node sẽ phát ra Passive Error Frame nếu phát hiện lỗi.
  - Nếu Error Counter tiếp tục tăng và vượt ngưỡng 255, node sẽ chuyển sang trạng thái Bus Off, tức là node sẽ bị loại khỏi mạng CAN và không thể giao tiếp thêm cho đến khi được reset lại.
- Error Frame xảy ra khi một node phát hiện một trong các lỗi sau trong quá trình truyền dữ liệu:
  - Bit Error: Xảy ra khi một node phát hiện bit truyền ra không giống với bit nhận được.
  - CRC Error: Xảy ra khi có lỗi trong quá trình kiểm tra mã CRC.
  - ACK Error: Xảy ra khi node không nhận được tín hiệu ACK từ các node khác.
  - Form Error: Xảy ra khi một trường trong khung không tuân theo định dạng đúng của giao thức CAN.
  - Stuff Error: Xảy ra khi có nhiều hơn 5 bit giống nhau liên tiếp trong một khung (CAN sử dụng Bit stuffing để tránh điều này). Khi bất kỳ lỗi nào được phát hiện, node phát ra Error Frame và các node khác trên mạng CAN sẽ tạm dừng quá trình giao tiếp và xử lý lại khung.
4. Overload Frame

![image](https://github.com/user-attachments/assets/98de6855-c341-48e7-a360-322c577a0089)

- Một loại khung đặc biệt trong giao thức CAN được sử dụng để trì hoãn việc truyền dữ liệu khi một node trong mạng CAN cần thêm thời gian để xử lý. 
- Khung này không chứa dữ liệu, mà chỉ báo hiệu rằng một node đang quá tải và cần thời gian trước khi tiếp tục giao tiếp
Overload Frame không phải do người dùng phát ra, mà được tự động phát ra bởi phần cứng CAN khi cần thiết. 
- Node CAN sẽ phát ra Overload Frame khi một trong các điều kiện sau xảy ra:
  - Node không thể xử lý tiếp dữ liệu do buffer đã đầy hoặc cần thêm thời gian xử lý dữ liệu.
  - Node không thể nhận khung mới do có quá trình xử lý nội bộ cần hoàn thành trước. 
Cấu trúc của Overload Frame:
- Overload Flag: 6 bit dominant (bit 0) để báo hiệu trạng thái quá tải.
- Overload Delimiter: 8 bit recessive (bit 1), để phân tách khung quá tải với các khung khác và báo hiệu kết thúc Overload Frame.
Các điều kiện có thể gây ra Overload Frame:
- FIFO (First-In-First-Out) buffer trong node nhận đầy và node cần thêm thời gian để xử lý dữ liệu đã nhận.
- Tạm dừng nội bộ trong node để hoàn tất việc xử lý một sự kiện trước khi nhận thêm khung dữ liệu mới.
- Node cần xử lý một yêu cầu cao cấp khác (như một yêu cầu từ phần mềm ứng dụng).
## Tốc độ truyền và giới hạn vật lý:
- Một số yếu tố quan trọng ảnh hưởng đến hiệu suất của mạng CAN bao gồm tốc độ truyền (baud rate), chiều dài của bus, và điện trở kết cuối (termination resistor).
1. Tốc độ Baud của CAN: 
- Tốc độ baud là tốc độ truyền dữ liệu trên bus CAN, thường được đo bằng kbps (kilobits per second) hoặc Mbps (megabits per second).
- Mạng CAN hỗ trợ dải tốc độ baud từ 10 kbps đến 1 Mbps.
Ảnh hưởng của tốc độ baud:
- Chiều dài tối đa của bus: Tốc độ truyền càng cao, chiều dài tối đa của bus càng ngắn do ảnh hưởng của thời gian lan truyền tín hiệu trên bus. 
- Độ trễ: Tốc độ baud càng cao, độ trễ của việc truyền thông tin trên mạng càng giảm, giúp cải thiện khả năng đáp ứng của hệ thống.
2. Chiều dài dây 
- Chiều dài của bus trong mạng CAN bị giới hạn bởi tốc độ baud và chất lượng của dây dẫn (bus). Khi tốc độ baud tăng lên, thời gian bit ngắn lại, nghĩa là tín hiệu phải đến các node nhận nhanh hơn. Do đó, chiều dài tối đa của bus phải giảm 
## Quá trình cấu hình và thiết lập CAN
1. Thiết lập tốc độ Baud:
- Tốc độ baud (baud rate) là tốc độ truyền dữ liệu trên bus CAN, được tính bằng số bit truyền trên giây (bps - bit per second).

  ![image](https://github.com/user-attachments/assets/47bac14d-6467-4a71-bef7-71c6aa0fe372)

Các yếu tố ảnh hưởng đến tốc độ Baud:
- Thời gian mấu (Sample Point):
  - Thời gian mẫu là thời điểm mà tín hiệu trên bus CAN được đọc để xác định giá trị của một bit (dominant hoặc recessive). Mẫu thường được lấy ở vị trí cuối mỗi bit để đảm bảo tín hiệu đã ổn định.
  - Vị trí của điểm mẫu (sample point) được tính toán dựa trên tỷ lệ phần trăm trong một khoảng thời gian bit. Điểm mẫu lý tưởng thường nằm ở khoảng 75% đến 90% thời gian của một bit.
- Bộ chia thời gian (Time Segment): Một bit trong mạng CAN được chia thành các phân đoạn thời gian (time segment), bao gồm:
  - Sync Segment: là đoạn đầu tiên của mỗi bit và có độ dài cố định là 1 time quanta (TQ). Nó đảm bảo rằng tất cả các node đều nhận biết sự bắt đầu của một bit tại cùng một thời điểm. Node nhận sẽ phát hiện ra cạnh thay đổi (rising edge hoặc falling edge) của tín hiệu CAN tại đoạn này để điều chỉnh thời gian của chính nó, đảm bảo đồng bộ với các node khác trên bus.
  - Propagation Segment: Đoạn này bù đắp thời gian cần thiết để tín hiệu lan truyền qua bus CAN từ node gửi đến node nhận. Tín hiệu cần thời gian để di chuyển từ một node đến một node khác, và thời gian này phụ thuộc vào chiều dài của bus và tốc độ truyền. Propagation Segment được cấu hình sao cho nó bao gồm độ trễ lan truyền và thời gian trễ xử lý của cả phần cứng CAN.
  - Phase Segment 1 (PS1) và Phase Segment 2 (PS2): Hai phân đoạn này được sử dụng để đồng bộ hóa tín hiệu và bù đắp cho các sai lệch về thời gian hoặc độ trễ nhỏ trong quá trình truyền. 
    + PS1 là đoạn thời gian trước điểm lấy mẫu. Đoạn này cho phép node điều chỉnh thời gian đọc tín hiệu để đồng bộ với tín hiệu thực tế trên bus.
    + PS2 là đoạn thời gian sau điểm lấy mẫu. Nó có thể được kéo dài hoặc thu ngắn để bù trừ sự sai lệch nhỏ giữa các node, giữ cho tất cả node đồng bộ với nhau. Đây là đoạn để kết thúc 1 bit.
- Đơn vị phân đoạn thời gian: Time Quanta:
  - Time Quanta (TQ) là đơn vị thời gian nhỏ nhất mà một bit trong giao thức CAN được chia thành. Mỗi phân đoạn thời gian trong 1 bit được tính bằng số lượng TQ.
  - Mỗi node trên bus CAN sử dụng cùng một đơn vị TQ, và tổng số TQ trong một bit sẽ quyết định tốc độ baud của hệ thống.
`Bit Time=Sync Segment+Propagation Segment+PS1+PS2`
tốc độ Baud: 
`Tốc độ baud = 1/ Bit Time (bps)`
## Bộ lọc CAN:
- Bộ lọc CAN cho phép các node lựa chọn và chỉ nhận những thông điệp cần thiết, dựa trên ID của thông điệp hoặc các tiêu chí khác.
Bộ lọc CAN hoạt động dựa trên hai thành phần chính:
- Mask (Mặt nạ): Quy định những bit nào trong ID của thông điệp cần được kiểm tra.
- Filter (Bộ lọc): Được dùng để so sánh các bit của ID thông điệp với giá trị quy định trong bộ lọc. Nếu các bit này khớp với mask, thông điệp sẽ được chấp nhận và xử lý.
1. Mask
- Mask là một dãy bit mà các bit có giá trị 1 sẽ được kiểm tra, còn các bit có giá trị 0 sẽ bị bỏ qua. 
- Chỉ định cụ thể những phần của ID thông điệp mà node sẽ quan tâm, trong khi bỏ qua các phần không quan trọng. Mask giúp xác định phạm vi ID mà node quan tâm.
2. Filter
- Filter là giá trị mà các bit trong ID của thông điệp phải khớp với, dựa trên mask. Các bit được phép kiểm tra thông qua mask sẽ được so sánh với filter. Nếu ID thông điệp trùng khớp với giá trị của bộ lọc (sau khi áp dụng mask), thông điệp sẽ được chấp nhận. Nếu không trùng khớp, thông điệp sẽ bị bỏ qua, node sẽ không xử lý nó.




