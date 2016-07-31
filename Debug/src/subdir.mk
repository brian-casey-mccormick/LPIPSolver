################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BranchAndBound.cpp \
../src/Comment.cpp \
../src/Matrix.cpp \
../src/Simplex.cpp \
../src/Vector.cpp \
../src/driver.cpp 

OBJS += \
./src/BranchAndBound.o \
./src/Comment.o \
./src/Matrix.o \
./src/Simplex.o \
./src/Vector.o \
./src/driver.o 

CPP_DEPS += \
./src/BranchAndBound.d \
./src/Comment.d \
./src/Matrix.d \
./src/Simplex.d \
./src/Vector.d \
./src/driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


