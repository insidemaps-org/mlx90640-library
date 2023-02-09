/**
 * @copyright (C) Jorgen Birkler 2022
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _MLX90640_REGISTERS_H_
#define _MLX90640_REGISTERS_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#define MLX90640_NO_ERROR 0
#define MLX90640_I2C_NACK_ERROR 1
#define MLX90640_I2C_WRITE_ERROR 2
#define MLX90640_BROKEN_PIXELS_NUM_ERROR 3
#define MLX90640_OUTLIER_PIXELS_NUM_ERROR 4
#define MLX90640_BAD_PIXELS_NUM_ERROR 5
#define MLX90640_ADJACENT_BAD_PIXELS_ERROR 6
#define MLX90640_EEPROM_DATA_ERROR 7
#define MLX90640_FRAME_DATA_ERROR 8
#define MLX90640_MEAS_TRIGGER_ERROR 9

#define BIT_MASK(x) (1UL << (x))
#define REG_MASK(sbit,nbits) ~((~(~0UL << (nbits))) << (sbit))

#define MLX90640_EEPROM_START_ADDRESS 0x2400
#define MLX90640_EEPROM_DUMP_NUM 832
#define MLX90640_PIXEL_DATA_START_ADDRESS 0x0400
#define MLX90640_PIXEL_NUM 768
#define MLX90640_LINE_NUM 24
#define MLX90640_COLUMN_NUM 32
#define MLX90640_LINE_SIZE 32
#define MLX90640_COLUMN_SIZE 24
#define MLX90640_AUX_DATA_START_ADDRESS 0x0700
#define MLX90640_AUX_NUM 64
#define MLX90640_STATUS_REG 0x8000
#define MLX90640_INIT_STATUS_VALUE 0x0030
#define MLX90640_STAT_FRAME_MASK BIT_MASK(0) 
#define MLX90640_GET_FRAME(reg_value) (reg_value & MLX90640_STAT_FRAME_MASK)
#define MLX90640_STAT_DATA_READY_MASK BIT_MASK(3) 
#define MLX90640_GET_DATA_READY(reg_value) (reg_value & MLX90640_STAT_DATA_READY_MASK)

#define MLX90640_CTRL_REG 0x800D
#define MLX90640_CTRL_TRIG_READY_MASK BIT_MASK(15) 
#define MLX90640_CTRL_REFRESH_SHIFT 7
#define MLX90640_CTRL_REFRESH_MASK REG_MASK(MLX90640_CTRL_REFRESH_SHIFT,3)
#define MLX90640_CTRL_RESOLUTION_SHIFT 10
#define MLX90640_CTRL_RESOLUTION_MASK REG_MASK(MLX90640_CTRL_RESOLUTION_SHIFT,2)
#define MLX90640_CTRL_MEAS_MODE_SHIFT 12
#define MLX90640_CTRL_MEAS_MODE_MASK BIT_MASK(12)

#define MLX90640_MS_BYTE_SHIFT 8
#define MLX90640_MS_BYTE_MASK 0xFF00
#define MLX90640_LS_BYTE_MASK 0x00FF
#define MLX90640_MS_BYTE(reg16) ((reg16 & MLX90640_MS_BYTE_MASK) >> MLX90640_MS_BYTE_SHIFT)
#define MLX90640_LS_BYTE(reg16) (reg16 & MLX90640_LS_BYTE_MASK)
#define MLX90640_MSBITS_6_MASK 0xFC00
#define MLX90640_LSBITS_10_MASK 0x03FF
#define MLX90640_NIBBLE1_MASK 0x000F
#define MLX90640_NIBBLE2_MASK 0x00F0
#define MLX90640_NIBBLE3_MASK 0x0F00
#define MLX90640_NIBBLE4_MASK 0xF000
#define MLX90640_NIBBLE1(reg16) ((reg16 & MLX90640_NIBBLE1_MASK))
#define MLX90640_NIBBLE2(reg16) ((reg16 & MLX90640_NIBBLE2_MASK) >> 4)
#define MLX90640_NIBBLE3(reg16) ((reg16 & MLX90640_NIBBLE3_MASK) >> 8)
#define MLX90640_NIBBLE4(reg16) ((reg16 & MLX90640_NIBBLE4_MASK) >> 12)

#define POW2(x) pow(2, (double)x) 

#define SCALEALPHA 0.000001
    



//Status register bits
enum {
    MLX90640_STATUS_SUBPAGE_MASK = 0x0001,
    MLX90640_STATUS_NEW_DATA_IN_RAM_FLAG = 0x0008,
    MLX90640_STATUS_DATA_OVERWRITE_ENABLE = 0x0010
};

//Address Map
enum {
    MLX90640_EEPROM_START = 0x2400,
    MLX90640_EEPROM_END = MLX90640_EEPROM_START + MLX90640_EEPROM_LENGTH,

    MLX90640_RAM_PIXELS_START = 0x0400,
    MLX90640_RAM_PIXELS_END = MLX90640_RAM_PIXELS_START + MLX90640_PIXEL_TOTAL,

    MLX90640_RAM_AUX_DATA_START = 0x0700,
    MLX90640_RAM_AUX_DATA_LENGTH = MLX90640_FRAME_AUX_DATA_LENGTH,
    MLX90640_RAM_AUX_DATA_END = MLX90640_RAM_AUX_DATA_START + MLX90640_RAM_AUX_DATA_LENGTH,



    MLX90640_STATUS_REGISTER = 0x8000,
    MLX90640_CONTROL_REGISTER = 0x800D,
    MLX90640_I2C_CONTROL_REGISTER = 0x800F,



};



enum {
    MLX90640_FRAME_CONTROL_REGISTER = 832,
    MLX90640_FRAME_STATUS_REGISTER = 833,
    MLX90640_FRAME_SUBPAGE = 833


};


#ifdef __cplusplus
}
#endif

#endif //_MLX90640_REGISTERS_H_
