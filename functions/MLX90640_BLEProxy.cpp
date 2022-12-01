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
#include <MLX90640_BLEProxy.h>
#include <stdio.h>
#include <string.h>


enum MLX90640_AddressMap
{
	MLX90640_ROMStart = 0x0000,
	MLX90640_ROMEnd = 0x0400,
	MLX90640_RAMStart = 0x0400,
	MLX90640_RAMEnd = 0x0800,
	MLX90640_RAM_BufLen = MLX90640_RAMEnd - MLX90640_RAMStart + 1 + 3,
	MLX90640_EEPROMStart = 0x2400,
	MLX90640_EEPROMEnd = 0x2742,
	MLX90640_EEPROM_BufLen = MLX90640_EEPROMEnd - MLX90640_EEPROMStart + 1 + 3,
	MLX90640_RegsStart = 0x8000,
	MLX90640_RegsEnd = 0x8017,
	MLX90640_StatusRegister = 0x8000,
	MLX90640_ControlRegister = 0x800D
};

static uint16_t eeprom_data[MLX90640_EEPROM_BufLen];
static uint16_t ram_data[MLX90640_EEPROM_BufLen];

static uint16_t status_register_data = 0xFFFF;
static uint16_t control_register_data = 0xFFFF;

#define length_of(_array) (sizeof(_array) / sizeof(_array[0]))

enum {
    MLX90640_FRAME_LENGTH = 833+2,
    HERO_MemBank_MagicNumber_Synthetic = 0x1234,
    HERO_MemBank_MagicNumber_Eeprom = 0xBABE,
    HERO_MemBank_MagicNumber_RAM = 0xC0DE


};


const uint16_t* MLX90640_BLEProxy_GetEEPROMPtr() {
    if (eeprom_data[MLX90640_FRAME_LENGTH] != HERO_MemBank_MagicNumber_RAM) {
        printf("MLX90640_BLEProxyResult_GetEEPROMPtr: Read frame: Magic number is not correct: 0x%04x expected:0x%04x\n",eeprom_data[MLX90640_FRAME_LENGTH],HERO_MemBank_MagicNumber_RAM);
        return nullptr;
    }

	if ((eeprom_data[0x000F]&0xFF) != 0x33 ||
		eeprom_data[0x000C] != 0x1901 ||
		eeprom_data[0x000D] != 0x0000 ||
		eeprom_data[0x000E] != 0x0000) 
	{
		//Read EEPROM does not seem correct
		printf("LX90640_BLEProxyResult_GetEEPROMPtr:  EEPROM data dump from Hero does not seem correct.\n");
		for (int addr=0;addr<=0xF;addr++) {
			printf("ee[0x%04x]=0x%04x\n",0x2400+addr,eeprom_data[addr]);
		}
		printf("\n");
	}


    return eeprom_data;
}
const uint16_t* MLX90640_BLEProxy_GetRAMPtr() {
    if (ram_data[MLX90640_FRAME_LENGTH] != HERO_MemBank_MagicNumber_Eeprom) {
        printf("MLX90640_BLEProxyResult_GetEEPROMPtr: Read frame: Magic number is not correct: 0x%04x expected:0x%04x\n",ram_data[MLX90640_FRAME_LENGTH],HERO_MemBank_MagicNumber_Eeprom);
        return nullptr;
    }
    return ram_data;
}


MLX90640_BLEProxyResult_t  MLX90640_BLEProxy_Update(MLX90640_BLEProxy_MemBlock_t which, int offset,int len, const uint8_t* data) {
    uint16_t *ptr = nullptr;

    uint16_t *ptr_end = nullptr;

	if (which == MLX90640_BLEProxy_MemBlock_EEPROM)
	{
		ptr = &eeprom_data[0];
        ptr_end = &eeprom_data[length_of(eeprom_data)];
	}
	else if (which == MLX90640_BLEProxy_MemBlock_RAM)
	{
		ptr = &ram_data[0];
        ptr_end = &ram_data[length_of(ram_data)];
    }
	else if (which == MLX90640_BLEProxy_MemBlock_ControlRegister || which == MLX90640_BLEProxy_MemBlock_StatusRegister )
    {
        if (len != 2 || offset !=0) {
            printf("MLX90640_BLEProxy: Invalid parameter");
            return MLX90640_BLEProxyResult_InvalidParam;
        }
		ptr = which == MLX90640_BLEProxy_MemBlock_ControlRegister ? &status_register_data : &control_register_data;
        ptr_end = ptr+1;
	} else {
        printf("MLX90640_BLEProxy: Invalid parameter");
        return MLX90640_BLEProxyResult_InvalidParam;
    }

    ptr += offset/2;
    len /= 2;
    for (int i=0;i<len;i++) {
        if (ptr >= ptr_end) {
			break;
        }
        *ptr = data[i*2] << 8 | data[i*2+1]; //Data is little endian
        ptr++;
    }

    return MLX90640_BLEProxyResult_Ok;
}





void MLX90640_I2CInit(void)
{
	//read ROM
	//read EEPROM
	//read RAM
}

int MLX90640_I2CGeneralReset(void)
{
	MLX90640_I2CInit();
	return 0;
}



int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{
	if (slaveAddr != 0x33) {
		printf("MLX90640_I2CRead:(MLX90640_BLEProxy: ) slaveAddr=0x%02x should be 0x33\n",slaveAddr);
		return -1;
	}
	(void)slaveAddr;
	uint16_t *ptr = nullptr;
	if (MLX90640_EEPROMStart <= startAddress && startAddress + nMemAddressRead < MLX90640_EEPROMEnd)
	{
		ptr = &eeprom_data[startAddress - MLX90640_EEPROMStart];
	}
	else if (MLX90640_RAMStart <= startAddress && startAddress + nMemAddressRead < MLX90640_RAMEnd)
	{
		ptr = &ram_data[startAddress - MLX90640_RAMStart];
	}
	else if (MLX90640_RegsStart <= startAddress && startAddress + nMemAddressRead < MLX90640_RegsEnd)
	{
		if (startAddress == MLX90640_StatusRegister && nMemAddressRead==1) {
			*data = status_register_data;
			return 0;
		} else if (startAddress == MLX90640_ControlRegister && nMemAddressRead==1) {
			*data = control_register_data;
			return 0;
		} else {
			printf("MLX90640_I2CRead: Unknown register read address 0x%04x len=%d\n",startAddress,nMemAddressRead);
			*data = 0xFFFF;
			return -1;
		}
	}
	else
	{
		printf("MLX90640_I2CRead:(MLX90640_BLEProxy: ) Outside known memory area address 0x%04x len=%d\n",startAddress,nMemAddressRead);
		return -1;
	}

	memcpy(data, ptr, nMemAddressRead* sizeof(uint16_t));

	return 0;
}

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
	(void)slaveAddr;
	(void)writeAddress;
	(void)data;
    printf("MLX90640_BLEProxy: proxy cannot write I2C\n");
    return -1;
}

void MLX90640_I2CFreqSet(int freq)
{
	(void)freq;
}





