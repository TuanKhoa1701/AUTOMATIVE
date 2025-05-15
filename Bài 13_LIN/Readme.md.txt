# LIN
## Lý thuyết 
- Giao thức LIN (Local Interconnect Network) nhằm giảm thiểu sự phức tạp và chi phí trong việc truyền thông giữa các thiết bị điện tử đơn giản trong xe.
- LIN thường được sử dụng trong các ứng dụng điều khiển các chức năng không an toàn của xe như điều khiển cửa sổ, đèn, gương, và điều hòa không khí
## Đặc điểm chính của LIN
- Mô hình Master - Slave.
- Node Slave chỉ phản hồi khi có yêu cầu từ node Master.
- Master “quét” yêu cầu tới tất cả các Slave để thăm dò thông tin.
- Master có dữ liệu từ Slave sẽ gửi lên bus CAN để đi tới các LIN khác
- IN sử dụng giao thức UART để truyền/nhận dữ liệu, với khung truyền dữ liệu sẽ là 1 start, 8 data, 1 hoặc 2 .
## Cấu trúc khung truyền:
1. Break Field (>= 13 bit):
-  Phần đầu tiên của khung truyền LIN, bao gồm một chuỗi các bit 0 dài hơn một ký tự truyền thông thường (ít nhất 13 bit). Khi phát ra các node sẽ dừng lại để lắng nghe.
- Nó có vai trò như một tín hiệu báo hiệu cho các node slave biết rằng một khung truyền mới sắp bắt đầu.
2. Sync Field (8 bit):
- Phần thứ hai của khung LIN, luôn có giá trị cố định là 0x55 (01010101 trong nhị phân).
-  Đảm bảo rằng tất cả các node slave trong mạng LIN có thể đồng bộ hóa tốc độ truyền (baud rate) với maste, đóng vai trò như một mẫu chuẩn để slave so sánh và điều chỉnh tốc độ truyền của mình.
- Giá trị 0x55 được chọn vì nó có dạng xen kẽ giữa các bit 0 và 1, giúp dễ dàng phát hiện lỗi trong quá trình đồng bộ hóa.
3. Identifier Field (6 bit ID + 2 bit parity):
- Chứa thông tin quan trọng về loại khung dữ liệu và định danh của node slave cần phản hồi. Trường ID gồm 6 bit dữ liệu và 2 bit kiểm tra chẵn lẻ (parity)
- ID: 6 bit này cho phép có tối đa 64 ID khác nhau, và mỗi ID đại diện cho một nhóm dữ liệu cụ thể
- Parity bit: Hai bit này được sử dụng để kiểm tra chẵn lẻ nhằm phát hiện lỗi khi truyền ID. Parity trong LIN là parity bảo vệ, được tính toán dựa trên 6 bit ID. 
  - P0 được tính bằng phép XOR giữa bit 0, bit 1, bit 2 và bit 4 của ID.
  - P1 được tính bằng phép XOR giữa bit 1, bit 3, bit 4, và bit 5 của ID.
4. Data Filed (16 -64 bit):
-  Phần chứa nội dung dữ liệu chính được truyền giữa các node trong mạng LIN. Trường này có độ dài từ 2 đến 8 byte, tùy thuộc vào loại thông tin được gửi.
- Trường Data chứa thông tin điều khiển, trạng thái của các thiết bị hoặc dữ liệu cảm biến. 
5. Checksum (8 bit): 
- Byte cuối cùng trong khung LIN, được sử dụng để phát hiện lỗi trong quá trình truyền dữ liệu.
- Đảm bảo rằng dữ liệu không bị thay đổi trong quá trình truyền từ master đến slave hoặc ngược lại.
Classic Checksum:
  - Được sử dụng cho tất cả các khung truyền ngoại trừ khung chứa trường ID.
  - Giá trị checksum là tổng (không dấu) của tất cả các byte dữ liệu, sau đó lấy phần bù của tổng này.
  - Đảm bảo rằng nếu có bất kỳ thay đổi nào trong dữ liệu, giá trị checksum sẽ khác đi và hệ thống sẽ phát hiện ra lỗi.
Enhanced Checksum:
  - Được sử dụng trong các phiên bản LIN mới hơn và bao gồm cả byte ID trong phép tính checksum.
  - Phương pháp này cung cấp mức độ bảo mật cao hơn cho dữ liệu, đặc biệt là khi byte ID có vai trò quan trọng trong việc xác định loại thông điệp
  ## Chi tiết quá trình truyền thông LIN
  - Dựa trên cơ chế polling, nghĩa là node master sẽ điều khiển toàn bộ quá trình truyền thông bằng cách gửi các khung điều khiển (header) đến các node slave
- Master gửi Header: Node master gửi khung điều khiển (Header), bao gồm:
  - Break Field: Tín hiệu báo hiệu bắt đầu khung.
  - Sync Field: Byte đồng bộ tốc độ truyền (0x55).
  - Identifier Field (ID): Xác định loại dữ liệu và node slave cần phản hồi.
- Slave hoặc master gửi Response: Sau khi nhận Header, node slave hoặc node master sẽ gửi phản hồi dựa trên yêu cầu của ID trong header:
  - Data Field: Chứa từ 2 đến 8 byte dữ liệu, có thể là lệnh điều khiển hoặc phản hồi cảm biến.
  - Checksum Field: Byte kiểm tra lỗi để đảm bảo tính toàn vẹn của dữ liệu.
