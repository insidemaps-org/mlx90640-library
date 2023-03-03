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
#include <MLX90640_API.h>
#include <MLX90640_Registers.h>
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

enum {
    MLX90640_BLEPROXY_FRAME_LENGTH = MLX90640_FRAME_LENGTH + 1,
    HERO_MemBank_MagicNumber_TestSequence = 0x1234,
    HERO_MemBank_MagicNumber_Eeprom = 0xBABE,
    HERO_MemBank_MagicNumber_RAM = 0xC0DE


};


static uint16_t test_sequence_data[MLX90640_BLEPROXY_FRAME_LENGTH];


static uint16_t eeprom_data[MLX90640_BLEPROXY_FRAME_LENGTH];
static uint8_t eeprom_updated = 0;
static uint16_t ram_frame_data[MLX90640_BLEPROXY_FRAME_LENGTH];
static uint8_t ram_frame_updated = 0;

static uint16_t status_register_data = 0xFFFF;
static uint16_t control_register_data = 0xFFFF;
static uint16_t control_register_requested_data = 0xFFFF;

#define length_of(_array) (sizeof(_array) / sizeof(_array[0]))

const uint16_t* MLX90640_BLEProxy_GetTestSequencePtr() {
	return test_sequence_data;
}

const uint16_t* MLX90640_BLEProxy_GetEEPROMPtr() {
    if (eeprom_data[MLX90640_FRAME_LENGTH] != HERO_MemBank_MagicNumber_Eeprom) {
        printf("MLX90640_BLEProxyResult_GetEEPROMPtr: Read frame: Magic number is not correct: 0x%04x expected:0x%04X\n",eeprom_data[MLX90640_FRAME_LENGTH],HERO_MemBank_MagicNumber_Eeprom);
        return 0;
    }

    if (!eeprom_updated) {
        printf("MLX90640_BLEProxyResult_GetEEPROMPtr: EEPROM data not updated!\n");
        return 0;
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
    if (!ram_frame_updated) {
        printf("\nMLX90640_BLEProxy_GetRAMPtr: RAM data not updated!\n");
        return 0;
    }

    if (ram_frame_data[MLX90640_FRAME_LENGTH] != HERO_MemBank_MagicNumber_RAM) {
        printf("MLX90640_BLEProxy_GetRAMPtr: Read frame: Magic number is not correct: 0x%04x expected:0x%04x\n",ram_frame_data[MLX90640_FRAME_LENGTH],HERO_MemBank_MagicNumber_RAM);
        return 0;
    }
    return ram_frame_data;
}


MLX90640_BLEProxyResult_t  MLX90640_BLEProxy_Update(MLX90640_BLEProxy_MemBlock_t which, int offset,int len, const uint8_t* data) {
    uint16_t* ptr = 0;

    const uint16_t* ptr_end = 0;

	const uint16_t* ptr_ctrl = 0;
	const uint16_t* ptr_status = 0;
	const uint16_t* ptr_magic_number = 0;
	uint16_t expected_magic_number = 0;



	if (which == MLX90640_BLEProxy_MemBlock_TestSequence)
	{
		ptr = &test_sequence_data[0];
        ptr_end = &test_sequence_data[length_of(test_sequence_data)];
		ptr_magic_number = ptr + MLX90640_FRAME_MAGIC_NUMBER_INDEX;
		expected_magic_number = HERO_MemBank_MagicNumber_TestSequence;
		ram_frame_updated = 0;
		eeprom_updated = 0;
	}
	else if (which == MLX90640_BLEProxy_MemBlock_EEPROM)
	{
		ptr = &eeprom_data[0];
        ptr_end = &eeprom_data[length_of(eeprom_data)];
		ptr_magic_number = ptr + MLX90640_FRAME_MAGIC_NUMBER_INDEX;
		expected_magic_number = HERO_MemBank_MagicNumber_Eeprom;
		ram_frame_updated = 0;
	}
	else if (which == MLX90640_BLEProxy_MemBlock_RAM)
	{
		ptr = &ram_frame_data[0];
        ptr_end = &ram_frame_data[length_of(ram_frame_data)];

		ptr_ctrl = ptr + MLX90640_FRAME_CONTROL_REGISTER;
		ptr_status = ptr + MLX90640_FRAME_STATUS_REGISTER;
		ptr_magic_number = ptr + MLX90640_FRAME_MAGIC_NUMBER_INDEX;
		expected_magic_number = HERO_MemBank_MagicNumber_RAM;
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
    for (int i=0;i<len;i++,ptr++) {
        if (ptr < ptr_end) {
	        *ptr = data[i*2] << 8 | data[i*2+1]; //Data is little endian
        }
		if (ptr == ptr_ctrl) {
			control_register_data = *ptr;
			control_register_requested_data = control_register_data;
		}
		if (ptr == ptr_status) {
			status_register_data = *ptr;

			status_register_data |= MLX90640_STAT_DATA_READY_MASK;
		}
		if (ptr == ptr_magic_number) {
			if (*ptr != expected_magic_number) {
				printf("MLX90640_BLEProxy: Invalid magic number 0x%04X expected 0x%04X\n",*ptr ,expected_magic_number);
        		return MLX90640_BLEProxyResult_InvalidParam;
			} else {
				if (which == MLX90640_BLEProxy_MemBlock_EEPROM) {
					eeprom_updated = 1;
				}
				else if (which == MLX90640_BLEProxy_MemBlock_RAM) {
					ram_frame_updated = 1;
				}
			}
		}
    }


    return MLX90640_BLEProxyResult_Ok;
}





void MLX90640_I2CInit(void)
{
	//read ROM
	//read EEPROM
	//read RAM
	eeprom_updated = 0;
	ram_frame_updated = 1;
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
	const uint16_t *ptr = 0;
	if (MLX90640_EEPROMStart <= startAddress && startAddress + nMemAddressRead < MLX90640_EEPROMEnd)
	{
		ptr = MLX90640_BLEProxy_GetEEPROMPtr();
		if (!ptr) {
			return -1;
		}
		ptr += startAddress - MLX90640_EEPROMStart;
	}
	else if (MLX90640_RAMStart <= startAddress && startAddress + nMemAddressRead < MLX90640_RAMEnd)
	{
		ptr = MLX90640_BLEProxy_GetRAMPtr();
		if (!ptr) {
			return -1;
		}
		ptr += startAddress - MLX90640_RAMStart;
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

	if (slaveAddr != 0x33) {
		printf("MLX90640_I2CRead:(MLX90640_BLEProxy: ) slaveAddr=0x%02x should be 0x33\n",slaveAddr);
		return -1;
	}

	if (writeAddress == MLX90640_ControlRegister) {
		control_register_requested_data = data;
		return 0;
	} else if (writeAddress == MLX90640_StatusRegister)  {
		//ram_frame_updated = 0;
		return 0;
	} else {

	    printf("MLX90640_BLEProxy: I2C proxy cannot write addr=0x%04x data=0x%04x unless its control register\n",writeAddress,data);
	}
    return -1;
}


int MLX90640_BLEProxy_HaveControlRegisterUpdate(uint16_t* value) {
	if (control_register_requested_data != control_register_data) {
		*value = control_register_requested_data;
		control_register_data = control_register_requested_data;
		return 1;
	} else {
		return 0;
	}
}


void MLX90640_I2CFreqSet(int freq)
{
	(void)freq;
}





