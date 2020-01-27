#pragma once
#include <stdint.h>
#include <sfdafx.h>

uint16_t calc_checksum(uint16_t * data, uint32_t data_len);
uint16_t get_checksum_ip(uint8_t * data);
uint16_t get_checksum_tcp(uint8_t * data);

