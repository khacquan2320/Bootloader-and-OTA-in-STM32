extern "C"
{
#include "min.h"
}
#include "download_file.h"
#include "bootloader_command.h"
#include "untils.h"

#define MIN_PORT  0
#define MIN_ID  1
#define MIN_Serial  Serial2
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

void bootloader_send_data(void *data, uint8_t len)
{
  min_send_frame(&min_ctx, MIN_ID, (uint8_t *)data , len );
}

void ota_send_code(OTA_Code_Name ota_code)
{
  OTACode cmd;
  cmd.command_id = OTA_CODE;
  cmd.len = 1;
  cmd.ota_code = ota_code;
  bootloader_send_data(&cmd , sizeof(cmd) );
}
void ota_send_respone(OTA_Respone_Name ack)
{
  OTAResponse response;
  response.command_id = OTA_RESPONSE;
  response.len = 1;
  response.ack = ack;
  bootloader_send_data(&response , sizeof(response) );
}

void ota_send_infor()
{
  OTAInfor infor;
  infor.command_id = OTA_INFOR;
  infor.len = sizeof(infor.name) + sizeof(infor.version);
  strcpy((char *)&infor.name, "nhap nhay led");
  strcpy((char *)&infor.version, "1.2.3");
  bootloader_send_data(&infor, sizeof(infor));
}
extern char file_hex[];
uint16_t index_file_hex = 0;

void ota_send_data(uint8_t *data, uint8_t len)
{
  OTAData cmd;
  cmd.command_id = OTA_DATA;
  cmd.len = len;
  memcpy(&cmd.data, data, len);
  bootloader_send_data(&cmd, sizeof(cmd));

}

void min_application_handler(uint8_t min_id, uint8_t const *min_payload,
                             uint8_t len_payload, uint8_t port)
{
  uint8_t hex_data[21];
  switch (ota_state)
  {
    case OTA_IDLE_STATE:
      {
        ///min_payload : 0x00,  0x01, 0x02
        // if(min_payload[0] == OTA_CODE && min_payload[2] == REQUEST_CODE)
        OTACode *ota_code = (OTACode *)min_payload;
        if (ota_code->command_id == OTA_CODE 
        && ota_code->ota_code == REQUEST_CODE)
        {
          ota_state = OTA_START_STATE;
          ota_send_code(START_CODE);
        }
        else
        {
          
        }
      }
      break;
    case OTA_START_STATE:
      {
        //neu nhan ack se gui infor
        OTAResponse *response =  (OTAResponse *)min_payload;
        if (response->command_id == OTA_RESPONSE && response->ack == ACK)
        {
          ota_state = OTA_SEND_INFOR_STATE;
          ota_send_infor();
        }
        else
        {
          ota_state  = OTA_IDLE_STATE;
        }
      }
      break;
    case OTA_SEND_INFOR_STATE:
      {
        OTAResponse *response =  (OTAResponse *)min_payload;
        if (response->command_id == OTA_RESPONSE && response->ack == ACK)
        {

          char *token = strtok(file_hex, "\n");
          convert_string_intel_hex_to_array_hex(token, hex_data);
          check_some(hex_data, hex_data[0] + 5);
          swap_4_byte(&hex_data[4], hex_data[0]);
          ota_send_data(&hex_data[4], hex_data[0]);
        }
        else
        {
          ota_state  = OTA_IDLE_STATE;
        }
      }
      break;
    case OTA_SEND_DATA_STATE:
      {
        OTAResponse *response =  (OTAResponse *)min_payload;
        if (response->command_id == OTA_RESPONSE && response->ack == ACK)
        {
          char *token = strtok(NULL, "\n");
          if (token != NULL)
          {
            convert_string_intel_hex_to_array_hex(token, hex_data);
            if (hex_data[3] == 0x00)
            {
              if (check_some(hex_data, hex_data[0] + 5) == -1)
              {
                ota_state = OTA_END_STATE;
                return;
              }
              swap_4_byte(&hex_data[4], hex_data[0]);
              ota_send_data(&hex_data[4], hex_data[0]);
            }
            else
            {
              ota_send_code(END_CODE);
              ota_state = OTA_END_STATE;
            }
          }
          else
          {

          }
        }
      }
      break;
    case OTA_END_STATE:
      OTAResponse *response =  (OTAResponse *)min_payload;
      if (response->command_id == OTA_RESPONSE && response->ack == ACK)
      {
        //do something
        ota_state = OTA_IDLE_STATE;
      }
      break;

    default:
      break;
  }
}
void host_bootloader_handle()
{
  uint8_t c;
  uint8_t len = 0;
  if (MIN_Serial.available() > 0)
  {
    c = MIN_Serial.read();
    len = 1;
  }
  min_poll(&min_ctx, &c, len);
}
void host_bootloader_init()
{
  min_init_context(&min_ctx, MIN_PORT);
  MIN_Serial.begin(115200);
  pinMode(2, OUTPUT);
  ota_state = OTA_IDLE_STATE;
}
