PAD_MAKE = PAD_BuildPAD_BUILD_OBJECTS
PAD_OBJDIR = PAD_Obj
SELF_DRV = ../..



PAD_DRV_PATH = driver

# String handling format
empty:=
space:= $(empty) $(empty)
#

#File Path file Workspace
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/sys.c
PAD_DRV_BUILD_FILES += ${PAD_DRV_PATH}/gtimer_driver.c
#

# Real output file path from current path
PAD_BUILD_OBJECTS := $(addprefix ${OUTDIR}/${PAD_OBJDIR}/,${PAD_DRV_BUILD_FILES:.c=.o})

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

${PAD_BUILD_OBJECTS}:| ${PAD_OBJDIR}

${PAD_OBJDIR}:
	@${MKDIR} ${OUTDIR}/${PAD_OBJDIR}/${PAD_DRV_PATH}

${PAD_MAKE}: ${PAD_BUILD_OBJECTS}
	@echo "${PAD_DRV_FILE_PATH}"
	@echo "PAD Driver build done!"
	# exit 1