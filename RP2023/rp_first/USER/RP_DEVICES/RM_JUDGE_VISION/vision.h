#ifndef __VISION_H
#define __VISION_H

#include "stdbool.h"
#include "type.h"
#include "crc.h"

/*---------------------主要修改部分----------------------------*/


#define VISION_FRAME_HEADER		(0xA5)
#define VISION_RX_LEN          16
#define VISION_TX_LEN          15



/* 数据长度 */
typedef enum {
	/* Std */
	LEN_VISION_RX_PACKET	= 16,	// 接收包整包长度
	LEN_VISION_TX_PACKET	= 15,	// 发送包整包长度

	LEN_RX_DATA 			    = LEN_VISION_RX_PACKET - 5,	// 接收数据段长度
	LEN_TX_DATA 			    = LEN_VISION_TX_PACKET - 5,	// 发送数据段长度
	
	LEN_FRAME_HEADER 	  	= 3,	// 帧头长度
	LEN_FRAME_TAILER 		  = 2,	// 帧尾CRC16
} Vision_Data_Length_t;


/*----数据协议------*/

/* 命令号 */
typedef enum {
	CMD_AIM_OFF 		    = 0x00,	// 不启动自瞄
	CMD_AIM_AUTO		    = 0x01,	// 启动自瞄
	CMD_AIM_SMALL_BUFF	= 0x02,	// 识别小符
	CMD_AIM_BIG_BUFF	  = 0x03,	// 识别大符
	CMD_AIM_ANTOP	   	  = 0x04,	// 击打哨兵
	CMD_AIM_ANDF		    = 0x05	// 吊射基地
} Vision_Cmd_Id_t;


/* 接收数据段格式 */
typedef __packed struct 
{
	float 	pitch_angle;
	float 	yaw_angle;	 

	char    is_find_Target;
	char    is_find_Dafu;	

	char    zm_shoot_enable;	   
	
	char    data[LEN_VISION_RX_PACKET];
	float   jiesuan[(LEN_VISION_RX_PACKET-5)/4];
} Vision_Rx_Data_t;      //3float,6char


/* 发送数据段格式 */
typedef __packed struct
{
	float   yaw;
	float   pitch;
	uint8_t fric_speed;		// 射速档位(根据等级来分)
	uint8_t my_color;		// 我自己的颜色

} Vision_Tx_Data_t;


/*----数据协议------*/



/*---------------------主要修改部分end----------------------------*/














/* 帧头字节偏移 */
typedef enum {
	sof			  = 0,
	Cmd_ID		= 1,
	Crc8		  = 2,
	Data		  = LEN_FRAME_HEADER,
	TX_CRC16	= LEN_FRAME_HEADER + LEN_TX_DATA,
} Vision_Frame_Header_Offset_t;

/* 帧头格式 */
typedef __packed struct
{
	uint8_t  			    sof;		// 同步头
	Vision_Cmd_Id_t  	cmd_id;	// 命令码
	uint8_t  			    crc8;		// CRC8校验码
} Vision_Frame_Header_t;

/* 帧尾格式 */
typedef __packed struct 
{
	uint16_t crc16;					// CRC16校验码
} Vision_Frame_Tailer_t;
/* 接收数据段格式 */




/* 接收包格式 */
typedef __packed struct 
{
	Vision_Frame_Header_t FrameHeader;	// 帧头
	Vision_Rx_Data_t	    RxData;		    // 数据
	Vision_Frame_Tailer_t FrameTailer;	// 帧尾	
} Vision_Rx_Packet_t;



/* 发送包格式 */
typedef __packed struct
{
	Vision_Frame_Header_t FrameHeader;	// 帧头
	Vision_Tx_Data_t	    TxData;		    // 数据
	Vision_Frame_Tailer_t FrameTailer;	// 帧尾		
} Vision_Tx_Packet_t;
/**
 *	@brief	视觉模式
 */
typedef enum
{
	VISION_MODE_MANUAL		  = 0,	// 手动模式
	VISION_MODE_AUTO		    = 1,	// 自瞄模式
	VISION_MODE_BIG_BUFF	  = 2,	// 打大符模式
	VISION_MODE_SMALL_BUFF	= 3,	// 打小符模式
} Vision_Mode_t;
/* 辅助标识变量 */
typedef struct
{
	uint8_t 		  my_color;			 // 用0/1表示颜色
	Vision_Mode_t	mode;				   // 视觉模式
	uint8_t  		  rx_data_valid; // 接收数据的正确性
	uint16_t 		  rx_err_cnt;		 // 接收数据的错误统计
	uint32_t		  rx_cnt;				 // 接收数据包的统计
	bool		      rx_data_update;// 接收数据是否更新
	uint32_t 		  rx_time_prev;	 // 接收数据的前一时刻
	uint32_t 		  rx_time_now;	 // 接收数据的当前时刻
	uint16_t 		  rx_time_fps;	 // 帧率
	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;	
} Vision_State_t;


typedef enum {
	
	VISION_ONLINE,
	VISION_OFFLINE,
	
} Vision_State_e;


typedef struct{

	Vision_State_e      state;

	uint8_t		 init_flag;
	uint16_t   offline_cnt;
	uint16_t   offline_max_cnt;

}vision_info_t;



/* 视觉通信数据包格式 */
typedef struct {
	Vision_Rx_Packet_t RxPacket;
	Vision_Tx_Packet_t TxPacket;
	Vision_State_t     State;
} vision_data_t;

typedef struct vision_struct {
	
	vision_info_t	    info;
	vision_data_t     data;
	
	void				     (*init)(struct vision_struct *self);
	void				     (*update)(struct vision_struct *self, uint8_t *rxBuf);
	void				     (*heart_beat)(struct vision_struct *self);
	void             (*send)(struct vision_struct *self,uint8_t *Data);
} vision_t;

extern vision_t	vision;

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/



#endif

