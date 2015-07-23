#pragma once

#include "havoc.h"

namespace HW {

bool Initialize();
void Deinitialize();

uint64_t MakeWriteEepromCommand(uint16_t eeprom_addr, uint8_t data);
uint64_t MakeReadEepromCommand(uint16_t eeprom_addr);

bool CmdSend(uint64_t command);
bool CmdReceive(uint64_t* data);

bool ReadEepromByte(uint16_t eeprom_addr, uint8_t* byte);
bool ReadEepromBytes(uint16_t eeprom_addr, uint8_t* out_buf, uint16_t num_bytes);
bool WriteEepromByte(uint16_t eeprom_addr, uint8_t byte);
bool WriteEepromBytes(uint16_t eeprom_addr, const uint8_t* byte_buf, uint16_t num_bytes);

}