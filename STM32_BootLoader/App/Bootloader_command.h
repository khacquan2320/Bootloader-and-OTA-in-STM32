#ifndef BOOTLOADER_COMMAND_H
#define BOOTLOADER_COMMAND_H
#include <stdint.h>
#include "stm32f1xx.h"
enum
{
	OTA_CODE,
	OTA_INFOR,
	OTA_DATA,
	OTA_RESPONSE
};

typedef enum
{
  START_CODE,
  END_CODE,
  REQUEST_CODE
} OTA_Code_Name;

typedef enum
{
  ACK,
  NACK
} OTA_Respone_Name;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t ota_code;  // start stop request
}__attribute__((packed)) OTACode;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t ack;

}__attribute__((packed)) OTARespones;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t name[50];
  uint8_t version[5];
}__attribute__((packed)) OTAInfor;

typedef struct
{
  uint8_t command_id;
  uint8_t len;
  uint8_t data[16];

}__attribute__((packed)) OTAData;

#endif
