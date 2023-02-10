
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../headers/MLX90640_API.h"



#define VERSION_1_0 0x00001000
#define MAGIC 0x494D7469
//'IMti' - >InsideMaps thermal image



MLX90640_Result_t MLX90640_FileIO_Read(const char* filename, MLX90640_FileStructure_t* out)
{
    MLX90640_Result_t result = 0;
    FILE* f = fopen(filename,"rb");
    if (f) {
        size_t len = fread(out,sizeof(out->header),1,f);
        if (len == sizeof(out->header)) {
            size_t datalen = sizeof(out->data);
            if (out->header.version == VERSION_1_0 && datalen == out->header.data_len) {
                len = fread(&out->data,datalen,1,f);
                if (len == datalen) {

                } else {
                    result = MLX90640_Result_FileIOError;
                }
            } else {
                result = MLX90640_Result_FileIOVersion;
            }
        } else{
            result = MLX90640_Result_FileIOError;
        }
    } else {
        result = MLX90640_Result_FileIOError;
    }

    if (f) {
        fclose(f);
    }
    return result;
}

MLX90640_Result_t MLX90640_FileIO_Write(const char* filename, 
    const paramsMLX90640* params,
    const uint16_t eeprom[MLX90640_EEPROM_LENGTH],
    const uint16_t frame[MLX90640_FRAME_LENGTH],
    const float Tobject[MLX90640_PIXEL_TOTAL]
) 
{

    MLX90640_Result_t result = 0;
    FILE* f = fopen(filename,"wb");
    if (f) {
        MLX90640_FileStructure_t temp;
        temp.header.magic = MAGIC;
        temp.header.version = VERSION_1_0;
        temp.header.reserved = 0;
        temp.header.data_len = sizeof(temp.data);
        temp.data.Ta = MLX90640_GetTa(frame,params);
        temp.data.Vdd = MLX90640_GetVdd(frame,params);
        memcpy(temp.data.eeprom,eeprom,sizeof(temp.data.eeprom));
        memcpy(temp.data.frame,frame,sizeof(temp.data.frame));
        memcpy(temp.data.Tobject,Tobject,sizeof(temp.data.Tobject));

        size_t len = fwrite(&temp,sizeof(temp),1,f);
        if (len != sizeof(temp)) {
            result = MLX90640_Result_FileIOError;
        }
    }


    if (f) {
        fclose(f);
    }
    return result;



}


