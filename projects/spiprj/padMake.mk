PAD_MAKE = PAD_BuildPAD_BUILD_OBJECTS
PAD_OBJDIR = PAD_Obj
SELF_DRV = ../..



PAD_DRV_PATH = driver

# String handling format
empty:=
space:= $(empty) $(empty)
#

#File Path file Workspace
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/gpio.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/gtimer_driver.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/i2c.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/sys.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/spi.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/device/eeprom.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/debug.cpp

#

# Real output file path from current path
PAD_BUILD_OBJECTS := $(addprefix ${OUTDIR}/${PAD_OBJDIR}/,${PAD_DRV_BUILD_FILES:.c=.o}) $(addprefix ${OUTDIR}/${PAD_OBJDIR}/,${PAD_DRV_BUILD_FILES:.cpp=.o})

# Real check dir path
PAD_DRV_FILE_PATH = ${SELF_DRV}/${PAD_DRV_PATH}
PAD_OBJ_DRV_PATH = ${OUTDIR}/${PAD_OBJDIR}/${PAD_DRV_PATH}

# Real get file path
PAD_DRV_BUILD_FILES := $(addprefix ${SELF_DRV}/,${PAD_DRV_BUILD_FILES})


${PAD_OBJ_DRV_PATH}/%.o: ${PAD_DRV_FILE_PATH}/%.c
	@if [ -d "${OUTDIR}" ];                         \
	 then                                           \
	  ${CC} ${CFLAGS} -D${COMPILER} -o $@ $^;       \
	  echo "[CC] $@ from $^";  \
	fi

${PAD_OBJ_DRV_PATH}/%.o: ${PAD_DRV_FILE_PATH}/%.cpp
	@if [ -d "${OUTDIR}" ];                         \
	 then                                           \
	  ${CXX} ${CXXFLAGS} -D${COMPILER} -o $@ $^;       \
	  echo "[CXX] $@ from $^";  \
	fi

${PAD_BUILD_OBJECTS}:| ${PAD_OBJDIR}

${PAD_OBJDIR}:
	@${MKDIR} ${OUTDIR}/${PAD_OBJDIR}/${PAD_DRV_PATH}
	@${MKDIR} ${OUTDIR}/${PAD_OBJDIR}/${PAD_DRV_PATH}/device

${PAD_MAKE}: ${PAD_BUILD_OBJECTS}
	@echo "${PAD_DRV_FILE_PATH}"
	@echo "PAD Driver build done!"
	# exit 1