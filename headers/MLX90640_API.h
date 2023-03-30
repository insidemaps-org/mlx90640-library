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
#ifndef _MLX90640_API_H_
#define _MLX90640_API_H_

#ifdef __cplusplus
extern "C" {
#endif


enum {
    MLX90640_I2C_ADDR_DEFAULT = 0x33
};

#define MLX90640_PIXEL_IDX(c,r) ((c) + (r)*MLX90640_PIXEL_COLS)

enum {
    MLX90640_PIXEL_ROWS = 24,
    MLX90640_PIXEL_COLS = 32,
    MLX90640_PIXEL_TOTAL = MLX90640_PIXEL_ROWS *  MLX90640_PIXEL_COLS
};


enum {
    MLX90640_FRAME_PIXEL_START = 0,
    MLX90640_FRAME_AUX_DATA_START = MLX90640_PIXEL_TOTAL,
    MLX90640_FRAME_AUX_DATA_LENGTH = 64,
    MLX90640_FRAME_AUX_DATA_END = MLX90640_FRAME_AUX_DATA_START+MLX90640_FRAME_AUX_DATA_LENGTH,
    MLX90640_FRAME_REGISTER_COPY_LENGTH = 2,
    MLX90640_FRAME_LENGTH = MLX90640_PIXEL_TOTAL + MLX90640_FRAME_AUX_DATA_LENGTH + MLX90640_FRAME_REGISTER_COPY_LENGTH,
    MLX90640_FRAME_SUBPAGE_REGISTER = 833
};

enum {
    MLX90640_EEPROM_LENGTH = 832
};





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
    
typedef struct
    {
        int16_t kVdd;
        int16_t vdd25;
        float KvPTAT;
        float KtPTAT;
        uint16_t vPTAT25;
        float alphaPTAT;
        int16_t gainEE;
        float tgc;
        float cpKv;
        float cpKta;
        uint8_t resolutionEE;
        uint8_t calibrationModeEE;
        float KsTa;
        float ksTo[5];
        int16_t ct[5];
        uint16_t alpha[MLX90640_PIXEL_TOTAL];    
        uint8_t alphaScale;
        int16_t offset[MLX90640_PIXEL_TOTAL];    
        int8_t kta[768];
        uint8_t ktaScale;    
        int8_t kv[768];
        uint8_t kvScale;
        float cpAlpha[2];
        int16_t cpOffset[2];
        float ilChessC[3]; 
        uint16_t brokenPixels[5];
        uint16_t outlierPixels[5];  
    } paramsMLX90640;
    

///<0 indicates error
///0 indicates success
typedef int MLX90640_Result_t;
enum {
    MLX90640_Result_FileIOVersion = -129,
    MLX90640_Result_FileIOError = -128,

    MLX90640_Result_Ok = 0
};

///Either error or positive number indicating subpage
typedef MLX90640_Result_t MLX90640_SubPage_t;

//Communication with MLX90640 device
///////////////////////////////////////////////////////

MLX90640_Result_t MLX90640_DumpEE(uint8_t slaveAddr, uint16_t eeData[MLX90640_EEPROM_LENGTH]);
MLX90640_Result_t MLX90640_SynchFrame(uint8_t slaveAddr);
MLX90640_Result_t MLX90640_TriggerMeasurement(uint8_t slaveAddr);
MLX90640_SubPage_t MLX90640_GetFrameData(uint8_t slaveAddr, uint16_t frameData[MLX90640_FRAME_LENGTH]);
MLX90640_Result_t MLX90640_VerifyFrameData(const uint16_t frameData[MLX90640_FRAME_LENGTH]);
MLX90640_Result_t MLX90640_SetResolution(uint8_t slaveAddr, uint8_t resolution);
MLX90640_Result_t MLX90640_GetCurResolution(uint8_t slaveAddr);
MLX90640_Result_t MLX90640_SetRefreshRate(uint8_t slaveAddr, uint8_t refreshRate);   
MLX90640_Result_t MLX90640_GetRefreshRate(uint8_t slaveAddr);  
MLX90640_Result_t MLX90640_GetSubPageNumber(const uint16_t frameData[MLX90640_FRAME_LENGTH]);
MLX90640_Result_t MLX90640_GetCurMode(uint8_t slaveAddr); 
MLX90640_Result_t MLX90640_SetInterleavedMode(uint8_t slaveAddr);
MLX90640_Result_t MLX90640_SetChessMode(uint8_t slaveAddr);


//Calculation of values
///////////////////////////////////////////////////////
MLX90640_Result_t MLX90640_ExtractParameters(const uint16_t eeData[MLX90640_EEPROM_LENGTH], paramsMLX90640 *mlx90640);
float MLX90640_GetVdd(const uint16_t frameData[MLX90640_FRAME_LENGTH], const paramsMLX90640 *params);
float MLX90640_GetTa(const uint16_t frameData[MLX90640_FRAME_LENGTH], const paramsMLX90640 *params);
void MLX90640_GetImage(const uint16_t frameData[MLX90640_FRAME_LENGTH], const paramsMLX90640 *params, float result[MLX90640_PIXEL_TOTAL]);
void MLX90640_CalculateTo(const uint16_t frameData[MLX90640_FRAME_LENGTH], const paramsMLX90640 *params, float emissivity, float Treflected, float result[MLX90640_PIXEL_TOTAL]);
void MLX90640_BadPixelsCorrection(const uint16_t pixels[MLX90640_PIXEL_TOTAL], float to[MLX90640_PIXEL_TOTAL], int mode,const paramsMLX90640 *params);
enum {
    MLX90640_Zone_1 = 0,
    MLX90640_Zone_2 = 1,
    MLX90640_Zone_3 = 2
};

typedef int MLX90640_Zone_t;
inline MLX90640_Zone_t MLX90640_Zone_55fov(int pixelindex) {
    int r = pixelindex/MLX90640_PIXEL_COLS;
    int c = pixelindex%MLX90640_PIXEL_COLS;
    r -= MLX90640_PIXEL_ROWS/2;
    c -= MLX90640_PIXEL_COLS/2;
    if (r<0) r=-r;
    if (c<0) c=-c;
    if (r < 8 && c < 8) {
        return MLX90640_Zone_1;
    } else {
        return MLX90640_Zone_2;
    }
}


///File IO
//////////////////////////////////////////////////////////////////////////////////
typedef struct  {
    uint32_t magic;
    uint32_t version;
    uint32_t reserved;
    uint32_t data_len;
} MLX90640_FileStructureHeader_t;

typedef struct  {
    float Ta;
    float Vdd;
    uint16_t eeprom[MLX90640_EEPROM_LENGTH];
    uint16_t frame[MLX90640_FRAME_LENGTH];
    float Tobject[MLX90640_PIXEL_TOTAL];

}MLX90640_FileStructureData_t;

typedef struct  {
    MLX90640_FileStructureHeader_t header;
    MLX90640_FileStructureData_t data;
} MLX90640_FileStructure_t;

MLX90640_Result_t MLX90640_FileIO_Read(const char* filename, MLX90640_FileStructure_t* out);

MLX90640_Result_t MLX90640_FileIO_Write(
    const char* filename, 
    const paramsMLX90640* params,
    const uint16_t eeprom[MLX90640_EEPROM_LENGTH],
    const uint16_t frame[MLX90640_FRAME_LENGTH],
    const float Tobject[MLX90640_PIXEL_TOTAL]
);



///Pass known eeprom and frame data and compare to hardcoded expected result
//////////////////////////////////////////////////////////////////////////////////
MLX90640_Result_t MLX90640_TestCalculations(int testCase);


enum MLX90640_TEST_DATA {
    MLX90640_TEST_DATA_EEPROM,
    MLX90640_TEST_DATA_FRAME0,
    MLX90640_TEST_DATA_FRAME1,
    MLX90640_TEST_DATA_FLOAT_COMPARE,
    MLX90640_TEST_DATA_ALT_EEPROM,
    MLX90640_TEST_DATA_ALT_FRAME0,
    MLX90640_TEST_DATA_ALT_FRAME1,
} ;
const uint16_t* MLX90640_Test_GetTestData(enum MLX90640_TEST_DATA which);



#ifdef __cplusplus
} //extern "C"
#endif

#endif
