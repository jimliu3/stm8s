/*
Header Name: stm8s_max72xx.h
Author: Joydip Dutta
*/

#include "stm8s.h"
#include "stm8s_spi.h"
#include "stm8s_conf.h"

#define decode_mode_reg 	0x09
#define intensity_reg 		0x0A
#define scan_limit_reg 		0x0B
#define shutdown_reg 			0x0C
#define display_test_reg	0x0F
 
#define shutdown_cmd 0x00
#define run_cmd      0x01
 
#define no_test_cmd 0x00
#define test_cmd    0x01

extern const char alphabets[26];
extern const uint8_t alpha_char[26][8];



int string_len(char *string1);
void MAX7219_init(void);
void display_clear(void);
void display_char(int alphabet_sequence);
void display_string(const char string[]);




