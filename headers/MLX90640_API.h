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

#define MLX90640_PIXEL_IDX(c,r) ((c) + (r)*MLX90640_PIXEL_ROWS)

enum {
    MLX90640_PIXEL_ROWS = 32,
    MLX90640_PIXEL_COLS = 24,
    MLX90640_PIXEL_TOTAL = MLX90640_PIXEL_ROWS *  MLX90640_PIXEL_COLS
};


enum {
    MLX90640_FRAME_PIXEL_START = 0,
    MLX90640_FRAME_AUX_DATA_START = MLX90640_PIXEL_TOTAL,
    MLX90640_FRAME_AUX_DATA_LENGTH = 64,
    MLX90640_FRAME_AUX_DATA_END = MLX90640_FRAME_AUX_DATA_START+MLX90640_FRAME_AUX_DATA_LENGTH,
    MLX90640_FRAME_REGISTER_COPY_LENGTH = 2,
    MLX90640_FRAME_LENGTH = MLX90640_PIXEL_TOTAL + MLX90640_FRAME_AUX_DATA_LENGTH + MLX90640_FRAME_REGISTER_COPY_LENGTH
};

enum {
    MLX90640_EEPROM_LENGTH = 832
};




#define SCALEALPHA 0.000001f
    
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
void MLX90640_CalculateTo(const uint16_t frameData[MLX90640_FRAME_LENGTH], const paramsMLX90640 *params, float emissivity, float tr, float result[MLX90640_PIXEL_TOTAL]);
void MLX90640_BadPixelsCorrection(uint16_t pixels[MLX90640_PIXEL_TOTAL], float to[MLX90640_PIXEL_TOTAL], int mode,const paramsMLX90640 *params);



///Pass known eeprom and frame data and compare to hardcoded expected result
MLX90640_Result_t MLX90640_TestCalculations();


enum MLX90640_TEST_DATA {
    MLX90640_TEST_DATA_EEPROM,
    MLX90640_TEST_DATA_FRAME0,
    MLX90640_TEST_DATA_FRAME1,
    MLX90640_TEST_DATA_FLOAT_COMPARE
} ;

const uint16_t* MLX90640_Test_GetTestData(MLX90640_TEST_DATA which);



#ifdef __cplusplus
} //extern "C"
#endif

#endif
