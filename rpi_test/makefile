all:
	g++ -DGATTLIB_LOG_LEVEL=10 -g -O0 -std=c++17 -I../headers step.cpp ../functions/MLX90640_API.cpp ../functions/MLX90640_Test.cpp  ../functions/MLX90640_LINUX_I2C_Driver.cpp  -o step.exe
clean:
	rm -rf  step.exe

# 
#../functions/MLX90640_LINUX_I2C_Driver.cpp 

#-Wfloat-conversion -Wdouble-promotion -Wpedantic -Wall -Wextra 