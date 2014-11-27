################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../Blockable.o \
../BufferToggle.o \
../Client.o \
../Grid.o \
../Player.o \
../Server.o \
../socket.o \
../socketserver.o \
../thread.o 

CPP_SRCS += \
../Blockable.cpp \
../BufferToggle.cpp \
../Client.cpp \
../Grid.cpp \
../Player.cpp \
../Server.cpp \
../socket.cpp \
../socketserver.cpp \
../thread.cpp 

OBJS += \
./Blockable.o \
./BufferToggle.o \
./Client.o \
./Grid.o \
./Player.o \
./Server.o \
./socket.o \
./socketserver.o \
./thread.o 

CPP_DEPS += \
./Blockable.d \
./BufferToggle.d \
./Client.d \
./Grid.d \
./Player.d \
./Server.d \
./socket.d \
./socketserver.d \
./thread.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


