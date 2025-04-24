# Flash - Bootloader
## Flash
RAM
- Tốc độ đọc/ghi nhanh
- Dữ liệu bị mất khi ngưng cấp nguồn
Flash
- Tốc độ ghi chậm, tốc độ đọc nhanh.
- Dữ liệu không bị mất khi ngưng cấp điện.
- Phải xóa Flash trước khi tiến hành ghi lại dữ liệu. Không thể xóa từng byte mà phải xóa theo Page (1 vùng nhớ được phân chia kích thước rõ ràng).
- Chỉ có thể đọc hoặc ghi theo khối 2/4 byte.

EPROM
- Tương tự như Flash nhưng có thể đọc ghi theo từng byte

Bộ nhớ Flash:
- Bộ nhớ Flash được tổ chức gồm một khối chính (Main Block) có dung lượng lên đến 128kB (hoặc 64kB).
- Bộ nhớ Flash trong STM32 được chia nhỏ thành các Page, 1 Page có 1kB, có tới 64 đến 128 kB
- Bộ nhớ Flash có thể được thao tác trên từng ghi trên từng Word (4 bytes) hoặc halfword (2byte).
-  Flash phải yêu cầu xóa trước khi ghi. Ngoài ra không thể xóa từng byte mà phải xóa cả page bộ nhớ tại đó.
 
Chương trình xoá Flash:

![image](https://github.com/user-attachments/assets/b08e1165-92e0-4a0c-bfdf-e01512fadd23)

```
void Flash_Erase(uint32_t pageAddress) {
    FLASH_Unlock(); 
    while (FLASH_GetFlagStatus(FLASH_FLAG_BSY)) {} 
    
    FLASH->CR |= FLASH_CR_PER;          
    FLASH->AR = pageAddress;           
    FLASH->CR |= FLASH_CR_STRT;         

    while (FLASH->SR & FLASH_SR_BSY) {} 
    FLASH->CR &= ~FLASH_CR_PER;        
    FLASH_Lock(); 
}
```
- Đầu tiên tắt cờ LOCK của Flash ` FLASH_Unlock();
- Sau khi FLash đã Unlock, cờ CR_PER được set lên 1` FLASH->CR |= FLASH_CR_PER;`
- Địa chỉ của Page cần xóa được ghi vào AR `FLASH->AR = pageAddress;`
- Set bit CR_STRT lên 1 để bắt đầu quá trình xóa `FLASH->CR |= FLASH_CR_STRT;`
- Kiểm tra cờ BSY đợi hoàn tất quá trình xóa `while (FLASH->SR & FLASH_SR_BSY) {}`
Chương trình ghi vào Flash:
```
void Flash_Erase(uint32_t address, uint32_t data) {
    FLASH_Unlock(); 
    
    FLASH->CR |= FLASH_CR_PG;          
    *(uint32_t*)address = data;                 
    while (FLASH->SR & FLASH_SR_BSY) {} 
    FLASH->CR &= ~FLASH_CR_PG;   

    FLASH_Lock(); 
}
```
- Tương tự quá trình xóa, tắt Cờ LOCK `FLASH_Unlock();`
- Sau khi xác nhận đã Unlock, CỜ CR_PG được set lên 1` FLASH->CR |= FLASH_CR_PG;`
- Quá trình ghi dữ liệu vào địa chỉ tương ứng sẽ được thực thi.`*(uint32_t*)address = data;`
- Kiểm tra cờ BSY để đợi quá trình ghi hoàn tất`while (FLASH->SR & FLASH_SR_BSY) {}`
## Bootloader
- Bootloader là một chương trình và là chương trình đầu tiên chạy khi Chip hoạt động.
- Nhiệm vụ của Bootloader là kiểm tra các điều kiện để lựa chọn thực thi một trong các chương trình: Firmware update mới nhất, Firmware được nạp vào do hãng cung cấp trước khi xuất ra thị trường hoặc Current Firmware bản Firmware hiện tại đang chạy mỗi khi CPU reset. 
Hoạt động của CPU được mô tả như sau:
- Bắt đầu, hoặc mỗi khi CPU reset, PC nhảy đến địa chỉ bắt đầu (với boot 0-0 là thực thi vùng nhớ tại Flash, PC-> 0x08000000) , đây là vị trí lưu chương trình bootloader. 
- Lấy địa chỉ của App Current được lưu trên Flash.
- Hàm boot nhảy đến vị trí lưu App Current.
- Thực thi Firmware tại vị trí nhảy đến.
- Lặp lại mỗi khi khởi động hoặc có Reset.
```
int main(){
	RCC_DeInit();
	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk| SCB_SHCSR_BUSFAULTENA_Msk| SCB_SHCSR_MEMFAULTENA_Msk);
	
	__set_MSP(*(__IO uint32_t*)(ADDR_STR_BLINK));
	
	uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_BLINK+4);
	
	void (*reset_handler)(void) = (void *)JumpAddress;
	reset_handler();
	while(1){
	
	}
}

```
- Chương trình sẽ chạy tuần tự vào trong main, hàm RCC_Deinit(); reset bộ RCC về trạng thái reset mặc định.
- Vô hiệu hóa các bit kiểm tra lỗi trong thanh ghi SCB->SHCSR.
- Gọi hàm __set_MSP() để điều khiển Main Stack Pointer đến địa chỉ Firmware Blinkled đã nạp trước đó.
- Đặt địa chỉ có PC là ô nhớ tiếp theo sau MSP, tức là PC nhảy đến hàm Reset_Handler() của Firmware Blinkled.
- Cấu hình hàm reset_handler(); để sau khi reset, PC sẽ nhảy đến vị trí PC đã chọn.
- Gọi hàm reset để di chuyển PC.




