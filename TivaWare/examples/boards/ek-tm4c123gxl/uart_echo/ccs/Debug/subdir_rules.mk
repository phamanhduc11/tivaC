################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
startup_ccs.obj: D:/Workspace/tivaC/TivaWare/examples/boards/ek-tm4c123gxl/uart_echo/startup_ccs.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="D:/Workspace/tivaC/TivaWare/examples/boards/ek-tm4c123gxl/uart_echo/ccs" --include_path="D:/Workspace/tivaC/TivaWare" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

uart_echo.obj: D:/Workspace/tivaC/TivaWare/examples/boards/ek-tm4c123gxl/uart_echo/uart_echo.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="D:/Workspace/tivaC/TivaWare/examples/boards/ek-tm4c123gxl/uart_echo/ccs" --include_path="D:/Workspace/tivaC/TivaWare" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


