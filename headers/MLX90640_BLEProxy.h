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
#ifndef _MLX90640_BLE_PROXY_H_
#define _MLX90640_BLE_PROXY_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


enum {
    MLX90640_BLEProxyResult_Ok = 0,
    MLX90640_BLEProxyResult_InvalidParam = -1,

};


typedef uint8_t MLX90640_BLEProxyResult_t;

enum {
    MLX90640_BLEProxy_MemBlock_RAM = 1,
    MLX90640_BLEProxy_MemBlock_EEPROM = 2,
    MLX90640_BLEProxy_MemBlock_StatusRegister = 3, 
    MLX90640_BLEProxy_MemBlock_ControlRegister = 4,
};

typedef uint8_t MLX90640_BLEProxy_MemBlock_t;


MLX90640_BLEProxyResult_t  MLX90640_BLEProxy_Update(MLX90640_BLEProxy_MemBlock_t which, int offset,int len, const uint8_t* data);

const uint16_t* MLX90640_BLEProxy_GetEEPROMPtr();
const uint16_t* MLX90640_BLEProxy_GetRAMPtr();



#ifdef __cplusplus
}
#endif


#endif //_MLX90640_BLE_PROXY_H_
