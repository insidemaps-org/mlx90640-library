#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include "MLX90640_API.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_NONE    "\x1b[30m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define FMT_STRING "%+06.2f "
//#define FMT_STRING "\u2588\u2588\u2588"

#define MLX_I2C_ADDR 0x33

int main(){
    printf("Starting...\n");
    static uint16_t eeMLX90640[832];
    float emissivity = 1;
    uint16_t frame[834];
    float eTa;
    float Vdd;

    std::fstream fs;

    //MLX90640_SetDeviceMode(MLX_I2C_ADDR, 1);
    //MLX90640_SetSubPageRepeat(MLX_I2C_ADDR, 1);
    MLX90640_SetRefreshRate(MLX_I2C_ADDR, 0b010);
    MLX90640_SetChessMode(MLX_I2C_ADDR);
    printf("Configured...\n");

    paramsMLX90640 mlx90640;
    MLX90640_DumpEE(MLX_I2C_ADDR, eeMLX90640);
    MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
    int refresh = MLX90640_GetRefreshRate(MLX_I2C_ADDR);
    printf("EE Dumped... refresh:%d\n",refresh);

    //int frames = 30;
    int subpage;
    static float mlx90640To[768];

    MLX90640_TriggerMeasurement(MLX_I2C_ADDR);
    while (1){
        //while (!MLX90640_CheckInterrupt(MLX_I2C_ADDR)){
        //    std::this_thread::sleep_for(std::chrono::milliseconds(1));
       // }
        MLX90640_GetFrameData(MLX_I2C_ADDR, frame);
        // MLX90640_InterpolateOutliers(frame, eeMLX90640);
        subpage = MLX90640_GetSubPageNumber(frame);
        // Start the next meausrement
        MLX90640_TriggerMeasurement(MLX_I2C_ADDR);
        eTa = MLX90640_GetTa(frame, &mlx90640);
        Vdd = MLX90640_GetVdd(frame, &mlx90640);

        MLX90640_CalculateTo(frame, &mlx90640, emissivity, eTa, mlx90640To);

        MLX90640_BadPixelsCorrection((&mlx90640)->brokenPixels, mlx90640To, 1, &mlx90640);
        MLX90640_BadPixelsCorrection((&mlx90640)->outlierPixels, mlx90640To, 1, &mlx90640);

        printf("Page:%d Ta:%4.1fC Vdd:%4.1fV\n", subpage,(double)eTa,(double)Vdd);

        for(int x = 0; x < 32; x++){
            for(int y = 0; y < 24; y++){
                //std::cout << image[32 * y + x] << ",";
                float val = mlx90640To[32 * (23-y) + x];
                //if(val > 99.99) val = 99.99;
                if(val > 32.0f){
                    printf(ANSI_COLOR_MAGENTA FMT_STRING ANSI_COLOR_RESET, (double)val);
                }
                else if(val > 29.0f){
                    printf(ANSI_COLOR_RED FMT_STRING ANSI_COLOR_RESET, (double)val);
                }
                else if (val > 26.0f){
                    printf(ANSI_COLOR_YELLOW FMT_STRING ANSI_COLOR_YELLOW, (double)val);
                }
                else if ( val > 20.0f ){
                    printf(ANSI_COLOR_GREEN FMT_STRING ANSI_COLOR_RESET, (double)val);
                }
                else if (val > 17.0f) {
                    printf(ANSI_COLOR_CYAN FMT_STRING ANSI_COLOR_RESET, (double)val);
                }
                else if (val > 10.0f) {
                    printf(ANSI_COLOR_BLUE FMT_STRING ANSI_COLOR_RESET, (double)val);
                }
                else {
                    printf(ANSI_COLOR_NONE FMT_STRING ANSI_COLOR_RESET, (double)val);
                }
            }
            std::cout << std::endl;
        }
        printf("\x1b[33A");
    }
    return 0;
}
