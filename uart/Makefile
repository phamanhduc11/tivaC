# Tiva Makefile
# #####################################
#
# Part of the uCtools project
# uctools.github.com
#
#######################################
# user configuration:
#######################################
# TARGET: name of the output file
# INCLUDES: list of includes, by default, use Includes directory
INCLUDE_PATH = .
# OUTDIR: directory to use for output
OUTDIR = build
# TIVAWARE_PATH: path to tivaware folder
TIVAWARE_PATH = ../../TivaWare
PROJECT_PATH = ..
-include ${PROJECT_PATH}/mkdefs


all: ${OUTDIR}
all: ${OUTDIR}/libdriver.a

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${OUTDIR} ${wildcard *~}

#
# The rule to create the target directoy.
#
${OUTDIR}:
	@mkdir -p ${OUTDIR}


#
# Rules for building the driver library.
#
${OUTDIR}/libdriver.a: ${OUTDIR}/uart.o
#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${OUTDIR}/*.d} __dummy__
endif
