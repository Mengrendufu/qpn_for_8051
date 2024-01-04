#ifndef m_crc16_h_
#define m_crc16_h_

/*==========================================================================*/
/**
* name       : CRC16_MAXIM
* poly       : 0x8005
* reversePoly: 0xA001
* init       : 0x0000
* refIn      : true
* refOut     : true
* xorOut     : 0xFFFF
*/

// #define INIT_CRC16_MAXIM    0x0000
// #define XOROUT_CRC16_MAXIM  0xFFFF

// void GenerateAndPrintTable_crc16_maxim(void);

// uint16_t crc16_maxim(uint8_t *arr, uint16_t ArraySize);

// uint16_t Update_crc16_maxim(uint16_t PreCrc, uint8_t ch);

/*==========================================================================*/
/**
* name       : CRC16_CCITT
* poly       : 0x1021
* reversePoly: 0x8408
* init       : 0x0000
* refIn      : true
* refOut     : true
* xorOut     : 0x0000
*/

// #define INIT_CRC16_CCITT 0x0000

// void CreatAndPrintTable_crc16_ccitt(void);

// uint16_t crc16_ccitt(uint8_t *arr, uint16_t ArraySize);

// uint16_t Update_crc16_ccitt(uint16_t PreCrc, uint8_t ch);

/*==========================================================================*/
/**
* name       : CRC16_MODBUS
* poly       : 0x8005
* reversePoly: 0xA001
* init       : 0xFFFF
* refIn      : True
* refOut     : True
* xorOut     : 0x0000
*/

// #define INIT_CRC16_MODBUS 0xFFFF
// #define POLY_CRC16_MODBUS 0xA001

// void genShowTable_crc16_modbus(void);

// uint16_t crc16_modbus(uint8_t *arr,
//     uint16_t arr_size);

// uint16_t Update_crc16_modbus(uint16_t pre_crc,
//     uint8_t ch);

/*
******************************************************************************
name       : CRC16_CCITT_FALSE
poly       : 0x1021
reversePoly: 0x8408
init       : 0xFFFF
refIn      : False
refOut     : False
xorOut     : 0x0000
******************************************************************************
*/
#define INIT_CRC16_CCITT_FALSE    0xFFFF
#define POLY_CRC16_CCITT_FALSE    0x1021
#define XOROUT_CRC16_CCITT_FALSE  0x0000

// void gen_print_table_crc16_ccitt_false(void);

uint16_t update_crc16_ccitt_false(uint16_t pre_crc,
    uint8_t ch);

uint16_t crc16_ccitt_false(uint8_t *arr,
    uint16_t arr_size);

#endif  /* m_crc16_h_ */
