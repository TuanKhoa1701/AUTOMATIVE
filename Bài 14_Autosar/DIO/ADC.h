#ifndef _ADC_H_
#define _ADC_H_ 

/***************************************************************** 
    * @brief kiểu dữ liệu cho kênh ADC
    * @details Đây là kiểu dữ liệu dùng để xác định các kênh ADC trên vi điều khiển.
    *          Các kênh này thường được sử dụng để đọc giá trị từ cảm biến hoặc các tín hiệu analog khác.
******************************************************************/
typedef  uint8_t  ADC_ChannelType;

/**************************************************************** 
 *  @brief kiểu dữ liệu cho nhóm ADC
 *  @details Đây là kiểu dữ liệu dùng để xác định các nhóm ADC trên vi điều khiển.
 *          Các nhóm này có thể bao gồm nhiều kênh ADC khác nhau, cho phép đọc đồng thời nhiều tín hiệu analog.
*/
typedef uint8_t ADC_GroupType;
/**************************************************************** 
    *  @brief kiểu dữ liệu cho giá trị ADC
    *  @details Đây là kiểu dữ liệu dùng để lưu trữ giá trị đọc được từ các kênh ADC.
    *          Giá trị này thường là một số nguyên không dấu, đại diện cho mức điện áp tương ứng với tín hiệu analog đã đọc.
*****************************************************************/
typedef uint32_t ADC_ValueGroupType;
/****************************************************************
 *   @brief kiểu dữ liệu cho clock tần số của ADC
 *   @details Đây là kiểu dữ liệu dùng để xác định tần số xung nhịp của ADC.
*****************************************************************/
typedef uint32_t ADC_PrescaleType;
/***************************************************************
 * @brief kiểu dữ liệu cho thời gian chuyển đổi của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định thời gian cần thiết để hoàn thành một phép chuyển đổi ADC.
 *          Thời gian này có thể ảnh hưởng đến độ chính xác và tốc độ của quá trình đọc giá trị ADC.
 ***************************************************************/
typedef uint16_t ADC_ConversionTimeType;
/***************************************************************
 * @brief kiểu dữ liệu cho thời gian lấy mẫu của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định thời gian mà ADC cần để lấy mẫu tín hiệu analog trước khi thực hiện chuyển đổi.
 *          Thời gian này có thể ảnh hưởng đến độ chính xác của phép đo, đặc biệt khi làm việc với các tín hiệu có tần số cao.
 ****************************************************************/
typedef uint16_t ADC_SamplingTimeType;
/***************************************************************
 * @brief kiểu dữ liệu cho độ phân giải của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định độ phân giải của phép đo ADC.
 *          Độ phân giải càng cao thì khả năng phân biệt các mức điện áp càng tốt, nhưng cũng có thể làm tăng thời gian chuyển đổi.
 ****************************************************************/
typedef uint8_t ADC_ResolutionType;
/***************************************************************
 * @brief kiểu dữ liệu cho trạng thái của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định trạng thái hiện tại của ADC, có thể là đang hoạt động, đã hoàn thành hoặc gặp lỗi.
 *          Trạng thái này giúp quản lý và theo dõi quá trình chuyển đổi của ADC.
 ****************************************************************/
typedef enum{
    ADC_IDLE = 0x00,   /**< Trạng thái ADC đang ở chế độ chờ */
    ADC_BUSY = 0x01,   /**< Trạng thái ADC đang bận thực hiện chuyển đổi */
    ADC_COMPLETED = 0x02,   /**< Trạng thái ADC gặp lỗi trong quá trình hoạt động */
    ADC_STREAM_COMPLETED = 0x03 /**< Trạng thái ADC đã hoàn thành việc đọc dữ liệu từ một nhóm kênh */
} ADC_StatusType;
/*************************************************************
 * @brief kiểu dữ liệu cho nguồn kích hoạt của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định nguồn kích hoạt cho quá trình chuyển
 *************************************************************/
typedef enum{
    ADC_TRIGG_SRC_SW = 0x00, /**< Nguồn kích hoạt là SW (bằng tay) */
    ADC_TRIGG_SRC_HW = 0x01, /**< Nguồn kích hoạt là HW (tự động) */
}ADC_TrìggSourceType;
/************************************************************* 
 * @brief kiểu dữ liệu cho chế độ chuyển đổi của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định chế độ chuyển đổi của ADC, có thể là chuyển đổi đơn lẻ hoặc liên tục.
**************************************************************/
typedef enum {
    ADC_CONV_MODE_ONESHOT = 0x00, /**< Chế độ chuyển đổi đơn lẻ */
    ADC_CONV_MODE_CONTINUOUS = 0x01 /**< Chế độ chuyển đổi liên tục */
} ADC_ConversionModeType;
/******************************************************************
    * @brief kiểu dữ liệu cho ưu tiên của nhóm ADC
    * @details Đây là kiểu dữ liệu dùng để xác định mức độ ưu tiên của các nhóm ADC khi thực hiện chuyển đổi.
    *          Các nhóm có ưu tiên cao hơn sẽ được xử lý trước trong trường hợp có nhiều nhóm cùng yêu cầu chuyển đổi.
*******************************************************************/
typedef uint8_t ADC_GroupPriorityType;
/****************************************************************** 
 * @brief cấu trúc định nghĩa một nhóm ADC
 * @details Cấu trúc này chứa các thông tin cần thiết để cấu hình và quản lý một nhóm ADC
*/
typedef struct {
    ADC_GroupType ID; /**< ID của nhóm ADC */
    ADC_ChannelType Channels; /**< Danh sách các kênh ADC trong nhóm */
    ADC_ConversionModeType ConvMode; /**< Chế độ chuyển đổi (đơn lẻ hoặc liên tục) */
    ADC_PrescaleType Prescaler; /**< Tần số xung nhịp của ADC */
    ADC_SamplingTimeType SamplingTime; /**< Thời gian lấy mẫu */
    ADC_ResolutionType Resolution; /**< Độ phân giải của phép đo */
    ADC_TrìggSourceType TrìggSource; /**< Nguồn kích hoạt cho quá trình chuyển đổi */
    ADC_GroupPriorityType Priority; /**< Mức độ ưu tiên của nhóm */
    ADC_ResultAlignmentType ResultAlignment; /**< Căn chỉnh kết quả (trái hoặc phải) */
    ADC_StreamNumSamplesType NumSamples; /**< Số lượng mẫu cần đọc trong luồng DMA ADC */
    ADC_StreamBufferModeType StreamBufferMode; /**< Chế độ bộ đệm của luồng DMA ADC */
    uint8_t enable_Notification; /**< Cờ cho phép thông báo ngắt */
    (void) (*NotificationCb)(void); /**< Callback notification (optional) */
}ADC_GroupDefType;
/**************************************************************** 
 * @brief cấu trúc định nghĩa cấu hình tổng thể cho driver ADC
 * @details Cấu trúc này chứa danh sách các nhóm ADC và số lượng nhóm.
*****************************************************************/

typedef struct {
    ADC_GroupDefType* Groups; /**< Danh sách các nhóm ADC */
    uint8_t NumGroups; /**< Số lượng nhóm ADC */  
} ADC_ConfigType;
/*********************************************************** 
    * @brief kiểu dữ liệu cho số lượng mẫu cho DMA ADC
    * @details Đây là kiểu dữ liệu dùng để xác định số lượng mẫu cần đọc trong một luồng DMA ADC.
    *          Số lượng mẫu này có thể ảnh hưởng đến độ chính xác và tốc độ của quá trình đọc giá trị ADC.
************************************************************/
typedef uint8_t ADC_StreamNumSamplesType;
/*********************************************************** 
 * @brief kiểu dữ liệu cho chế độ bộ đệm của luồng DMA ADC
 * @details Đây là kiểu dữ liệu dùng để xác định cách thức bộ đệm dữ liệu trong luồng DMA ADC.
 *          Có thể là bộ đệm tuyến tính hoặc vòng lặp, tùy thuộc vào yêu cầu của ứng dụng.
 ***********************************************************/
typedef enum{
    ADC_STREAM_BUFFER_LINEAR = 0x00,
    ADC_STREAM_BUFFER_CIRCULAR = 0x01
}ADC_StreamBufferModeType;
/***********************************************************
 * @brief kiểu dữ liệu cho chế độ truy cập của nhóm ADC
 * @details Đây là kiểu dữ liệu dùng để xác định cách thức truy cập vào các kênh trong nhóm ADC. 
************************************************************/
typedef enum{
    ADC_ACCESS_MODE_SINGLE = 0x00, /**< Chế độ truy cập đơn lẻ */
    ADC_ACCESS_MODE_STREAM = 0x01, /**< Chế độ truy cập luồng */
} ADC_GroupAccessModeType;
/************************************************************ 
 * @brief kiểu dữ liệu cho tín hiệu kích hoạt phần cứng của ADC
 * @details Đây là kiểu dữ liệu dùng để xác định tín hiệu kích hoạt phần cứng cho ADC, có thể là cạnh lên, cạnh xuống hoặc cả hai.
***************************************************************/
typedef enum{
    ADC_HW_TRIG_RISING_EDGE = 0x00, /**< Kích hoạt phần cứng bằng cạnh lên */
    ADC_HW_TRIG_FALLING_EDGE = 0x01, /**< Kích hoạt phần cứng bằng cạnh xuống */
    ADC_HW_TRIG_BOTH_EDGES = 0x02, /**< Kích hoạt phần cứng bằng cả hai cạnh */
}ADC_HwTriggerSignalType;

typedef enum{
    ADC_ALIGN_LEFT = 0x00, /**< Căn trái */
    ADC_ALIGN_RIGHT = 0x01 /**< Căn phải */
} ADC_ResultAlignmentType;

/**************************************************************** 
 * @brief Hàm khởi tạo driver ADC với cấu hình chỉ định
 * @details Hàm này sẽ thiết lập các kênh ADC, cấu hình tần số, thời gian lấy mẫu, độ phân giải và các thông số khác dựa trên cấu hình được cung cấp.
 * @param[in] ConfigPtr: Con trỏ tới cấu hình ADC
 * @return void
******************************************************************/
void ADC_Init(const ADC_ConfigType* ConfigPtr);
/*****************************************************************
 * @brief Hàm lưu trữ kết quả đọc từ ADC vào bộ đệm
 * @details Hàm này sẽ lưu trữ kết quả đọc từ các kênh ADC vào một bộ đệm đã được chỉ định, cho phép truy cập sau này.
 * @param[in] group: Nhóm ADC cần lưu trữ kết quả
 * @param[out] buffer: Con trỏ tới bộ đệm để lưu trữ kết quả
 * @param[in] numSamples: Số lượng mẫu cần lưu trữ
******************************************************************/
void ADC_SetupResultBuffer(ADC_GroupType group, ADC_ValueGroupType* buffer, uint8_t numSamples);
/***************************************************************** 
 * @brief Hàm giải phóng tài nguyên và tắt tất cả kênh ADC
 * @details Hàm này sẽ giải phóng tất cả các tài nguyên đã được sử dụng bởi driver ADC, bao gồm các kênh, bộ đệm và cấu hình.
 * @return void
*******************************************************************/
void ADC_Deinit(void);
/***************************************************************** 
 * @brief Hàm bắt đầu quá trình chuyển đổi ADC cho một nhóm kênh
 * @details Hàm này sẽ kích hoạt quá trình chuyển đổi cho nhóm kênh ADC đã được cấu hình, có thể là chế độ đơn lẻ hoặc liên tục.
 * @param[in] group: Nhóm ADC cần bắt đầu chuyển đổi
 * @return void
 *****************************************************************/
void ADC_StartGroupConversion(ADC_GroupType group);
/***************************************************************** 
 * @brief Hàm dừng quá trình chuyển đổi ADC cho một nhóm kênh
 * @details Hàm này sẽ dừng quá trình chuyển đổi hiện tại cho nhóm kênh ADC, nếu đang hoạt động.
 * @param[in] group: Nhóm ADC cần dừng chuyển đổi
 * @return void
 *****************************************************************/
void ADC_StopGroupConversion(ADC_GroupType group);
/***************************************************************** 
 * @brief Hàm đọc kết quả từ một nhóm kênh ADC
 * @details Hàm này sẽ đọc kết quả từ các kênh ADC trong nhóm đã chỉ định và lưu trữ vào bộ đệm đã được cấu hình.
 * @param[in] group: Nhóm ADC cần đọc kết quả
 * @param[out] buffer: Con trỏ tới bộ đệm để lưu trữ kết quả
 * @return void
 *****************************************************************/
void ADC_ReadGroup(ADC_GroupType group, ADC_ValueGroupType* buffer);
/***************************************************************** 
 * @brief Hàm đăng ký callback cho thông báo ngắt của nhóm ADC
 * @details Hàm này sẽ đăng ký một hàm callback để được gọi khi có thông báo ngắt từ nhóm ADC.
 * @param[in] group: Nhóm ADC cần đăng ký callback
 * @return void
 *****************************************************************/
void ADC_EnableGroupNotification(ADC_GroupType group);
/***************************************************************** 
 * @brief Hàm hủy đăng ký callback cho thông báo ngắt của nhóm ADC
 * @details Hàm này sẽ hủy đăng ký hàm callback đã đăng ký trước đó cho nhóm ADC.
 * @param[in] group: Nhóm ADC cần hủy đăng ký callback
 * @return void
 *****************************************************************/
void ADC_DisableGroupNotification(ADC_GroupType group);
/***************************************************************** 
 * @brief Hàm đọc trạng thái hiện tại của nhóm ADC
 * @details Hàm này sẽ trả về trạng thái hiện tại của nhóm ADC, có thể là đang hoạt động, đã hoàn thành hoặc gặp lỗi.
 * @param[in] group: Nhóm ADC cần kiểm tra trạng thái
 * @return Trạng thái hiện tại của nhóm ADC (ADC_StatusType)
 *****************************************************************/
ADC_StatusType ADC_GetGroupStatus(ADC_GroupType group);
/***************************************************************** 
 * @brief Hàm đọc giá trị từ một kênh ADC cụ thể
 * @details Hàm này sẽ đọc giá trị từ kênh ADC đã chỉ định và trả về giá trị đó.
 * @param[in] channel: Kênh ADC cần đọc giá trị
 * @return Giá trị đọc được từ kênh ADC (ADC_ValueGroupType)
 *****************************************************************/
void ADC_GetVersionInfo (Std_VersionInfoType* versioninfo);
#endif 
