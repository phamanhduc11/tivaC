MAKE = make
MCU=TM4C123GH6PM

#
# The base directory for TivaWare.
#
PROJECT_PATH = .
TIVAWARE_PATH = TivaWare

#
# Where to find header files that do not live in the source directory.
#
INCLUDE_PATH = .
INCLUDE_PATH += ${TIVAWARE_PATH}

#
# Include the common make definitions.
#
include ${PROJECT_PATH}/mkdefs
#
# The default rule, which causes the main example to be built.
#
all: driver
all: ${OUTDIR}
all: ${OUTDIR}/main.axf

driver:
	${MAKE} -C uart
#
# The rule to clean out all the build products.
#
clean:
	rm -rf ${OUTDIR} ${wildcard *~}

#
# The rule to create the target directory.
#
${OUTDIR}:
	mkdir -p ${OUTDIR}

#
# Rules for building the main example.
#
${OUTDIR}/main.axf: ${OUTDIR}/startup_gcc.o
${OUTDIR}/main.axf: ${OUTDIR}/main.o
${OUTDIR}/main.axf: ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a
${OUTDIR}/main.axf: ${MCU}.ld
SCATTERgcc_main=${MCU}.ld
ENTRY_main=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C123_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${OUTDIR}/*.d} __dummy__
endif
