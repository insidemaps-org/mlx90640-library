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


//Status register bits
enum {
    MLX90640_STATUS_SUBPAGE_MASK = 0x0003,
    MLX90640_STATUS_NEW_DATA_IN_RAM_FLAG = 0x0008,
    MLX90640_STATUS_DATA_OVERWRITE_ENABLE = 0x0010
};

//Address Map
enum {
    MLX90640_EEPROM_START = 0x2400,
    MLX90640_EEPROM_END = MLX90640_EEPROM_START + MLX90640_EEPROM_LENGTH,

    MLX90640_RAM_PIXELS_START = 0x400,
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
