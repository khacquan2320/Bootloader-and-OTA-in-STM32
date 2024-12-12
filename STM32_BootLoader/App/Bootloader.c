#include "Bootloader.h"
#include "min.h"
#include "uart.h"
#include "Bootloader_command.h"

#define MIN_PORT  0
#define MIN_ID  1
struct min_context min_ctx;
typedef enum
{
  OTA_IDLE_STATE,
  OTA_START_STATE,
  OTA_SEND_INFOR_STATE,
  OTA_SEND_DATA_STATE,
  OTA_END_STATE

} OTA_State_Typedef;

OTA_State_Typedef ota_state;

void boot_loader_send_data(void* data, uint8_t len)
{
	min_send_frame(&min_ctx,MIN_ID,(uint8_t*)data,len);
}


void ota_send_code(OTA_Code_Name ota_code)
{
	OTACode cmd;
	cmd.command_id = OTA_CODE;
	cmd.len = 1;
	cmd.ota_code = ota_code;
	boot_loader_send_data(&cmd, sizeof(cmd));
}

void ota_send_response(OTA_Respone_Name ack)
{
	OTARespones response;
	response.command_id = OTA_RESPONSE;
	response.len = 1;
	response.ack = ack;
	boot_loader_send_data(&response,sizeof(response));
	
}

void min_application_handler(uint8_t min_id, uint8_t const *min_payload, 
															uint8_t len_payload, uint8_t port)
{
	switch(ota_state)
	{
		case OTA_IDLE_STATE :
		{
			OTACode *ota_code = (OTACode* )min_payload;
			if(ota_code->command_id == OTA_CODE
				&& ota_code->ota_code == START_CODE)
			{
				ota_state = OTA_START_STATE;
				ota_send_response(ACK);
			}
		}
			break;
		case OTA_START_STATE : 
		{
			OTAInfor *infor = (OTAInfor* )min_payload;
			if(infor->command_id == OTA_INFOR)
			{
				// luu ten vs version vao app 
				ota_send_response(ACK);
				ota_state = OTA_SEND_INFOR_STATE;
			}
		}
			break;
		case OTA_SEND_INFOR_STATE : 
		{
			OTAData *data = (OTAData* )min_payload;
			if(data->command_id == OTA_DATA)
			{
				// dong dau tien 
				// lay data luu vao vung app 
				ota_send_response(ACK);
			}
		}
			break;
		case OTA_SEND_DATA_STATE : 
		{
			OTAData* data = (OTAData* )min_payload;
			if(data->command_id == OTA_DATA)
			{
				ota_send_response(ACK);
			}
			else if(data->command_id == OTA_CODE)
			{
				OTACode* code = (OTACode* )min_payload;
				if(code->ota_code == END_CODE)
				{
					ota_state = OTA_END_STATE;
					ota_send_response(ACK);
					// run app 
				}
			}
		}
			break;
		case OTA_END_STATE :
			break;
		default:
			break;
	}
}

void bootloader_request_update()
{
	uint8_t cmd[] = {0x00,1,2 };
	min_send_frame(&min_ctx,MIN_ID, cmd, 3);
}

void bootloader_handle()
{
	uint8_t data;
	uint8_t len;
	if(uart_available() > 0)
	{
		data = uart_read();
		len = 1;
	}
	min_poll(&min_ctx,&data, len);
}

void bootloader_init()
{
	min_init_context(&min_ctx, MIN_PORT);
	ota_state = OTA_IDLE_STATE;
}