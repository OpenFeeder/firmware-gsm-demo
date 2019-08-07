#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/uart1.c mcc_generated_files/mcc.c mcc_generated_files/spi1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/traps.c mcc_generated_files/pin_manager.c mcc_generated_files/tmr3.c mcc_generated_files/rtcc.c mcc_generated_files/tmr1.c main.c app.c app_debug.c app_power.c led_status.c delay.c app_timers_callback.c radio_alpha_trx.c Services.c timer.c temps.c master_api.c appGSM/appGSM3_SIM800.c mcc_generated_files/driver/driverGSM3_SIM800.c mcc_generated_files/driver_uart/uart.c mcc_generated_files/uart2.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/spi1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/app.o ${OBJECTDIR}/app_debug.o ${OBJECTDIR}/app_power.o ${OBJECTDIR}/led_status.o ${OBJECTDIR}/delay.o ${OBJECTDIR}/app_timers_callback.o ${OBJECTDIR}/radio_alpha_trx.o ${OBJECTDIR}/Services.o ${OBJECTDIR}/timer.o ${OBJECTDIR}/temps.o ${OBJECTDIR}/master_api.o ${OBJECTDIR}/appGSM/appGSM3_SIM800.o ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o ${OBJECTDIR}/mcc_generated_files/uart2.o
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/spi1.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/tmr3.o.d ${OBJECTDIR}/mcc_generated_files/rtcc.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/app.o.d ${OBJECTDIR}/app_debug.o.d ${OBJECTDIR}/app_power.o.d ${OBJECTDIR}/led_status.o.d ${OBJECTDIR}/delay.o.d ${OBJECTDIR}/app_timers_callback.o.d ${OBJECTDIR}/radio_alpha_trx.o.d ${OBJECTDIR}/Services.o.d ${OBJECTDIR}/timer.o.d ${OBJECTDIR}/temps.o.d ${OBJECTDIR}/master_api.o.d ${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d ${OBJECTDIR}/mcc_generated_files/uart2.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/spi1.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/rtcc.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/app.o ${OBJECTDIR}/app_debug.o ${OBJECTDIR}/app_power.o ${OBJECTDIR}/led_status.o ${OBJECTDIR}/delay.o ${OBJECTDIR}/app_timers_callback.o ${OBJECTDIR}/radio_alpha_trx.o ${OBJECTDIR}/Services.o ${OBJECTDIR}/timer.o ${OBJECTDIR}/temps.o ${OBJECTDIR}/master_api.o ${OBJECTDIR}/appGSM/appGSM3_SIM800.o ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o ${OBJECTDIR}/mcc_generated_files/uart2.o

# Source Files
SOURCEFILES=mcc_generated_files/uart1.c mcc_generated_files/mcc.c mcc_generated_files/spi1.c mcc_generated_files/interrupt_manager.c mcc_generated_files/traps.c mcc_generated_files/pin_manager.c mcc_generated_files/tmr3.c mcc_generated_files/rtcc.c mcc_generated_files/tmr1.c main.c app.c app_debug.c app_power.c led_status.c delay.c app_timers_callback.c radio_alpha_trx.c Services.c timer.c temps.c master_api.c appGSM/appGSM3_SIM800.c mcc_generated_files/driver/driverGSM3_SIM800.c mcc_generated_files/driver_uart/uart.c mcc_generated_files/uart2.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256GB406
MP_LINKER_FILE_OPTION=,--script=p24FJ256GB406.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/spi1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/rtcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app.o: app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app.o.d 
	@${RM} ${OBJECTDIR}/app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app.c  -o ${OBJECTDIR}/app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app_debug.o: app_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_debug.o.d 
	@${RM} ${OBJECTDIR}/app_debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app_debug.c  -o ${OBJECTDIR}/app_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app_debug.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app_debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app_power.o: app_power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_power.o.d 
	@${RM} ${OBJECTDIR}/app_power.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app_power.c  -o ${OBJECTDIR}/app_power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app_power.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app_power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/led_status.o: led_status.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/led_status.o.d 
	@${RM} ${OBJECTDIR}/led_status.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  led_status.c  -o ${OBJECTDIR}/led_status.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/led_status.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/led_status.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/delay.o: delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/delay.o.d 
	@${RM} ${OBJECTDIR}/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  delay.c  -o ${OBJECTDIR}/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/delay.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app_timers_callback.o: app_timers_callback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_timers_callback.o.d 
	@${RM} ${OBJECTDIR}/app_timers_callback.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app_timers_callback.c  -o ${OBJECTDIR}/app_timers_callback.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app_timers_callback.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app_timers_callback.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/radio_alpha_trx.o: radio_alpha_trx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/radio_alpha_trx.o.d 
	@${RM} ${OBJECTDIR}/radio_alpha_trx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  radio_alpha_trx.c  -o ${OBJECTDIR}/radio_alpha_trx.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/radio_alpha_trx.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/radio_alpha_trx.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Services.o: Services.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Services.o.d 
	@${RM} ${OBJECTDIR}/Services.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Services.c  -o ${OBJECTDIR}/Services.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Services.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Services.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/timer.o: timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer.o.d 
	@${RM} ${OBJECTDIR}/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer.c  -o ${OBJECTDIR}/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/temps.o: temps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/temps.o.d 
	@${RM} ${OBJECTDIR}/temps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  temps.c  -o ${OBJECTDIR}/temps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/temps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/temps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/master_api.o: master_api.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/master_api.o.d 
	@${RM} ${OBJECTDIR}/master_api.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  master_api.c  -o ${OBJECTDIR}/master_api.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/master_api.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/master_api.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appGSM/appGSM3_SIM800.o: appGSM/appGSM3_SIM800.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appGSM" 
	@${RM} ${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d 
	@${RM} ${OBJECTDIR}/appGSM/appGSM3_SIM800.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appGSM/appGSM3_SIM800.c  -o ${OBJECTDIR}/appGSM/appGSM3_SIM800.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o: mcc_generated_files/driver/driverGSM3_SIM800.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/driver" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/driver/driverGSM3_SIM800.c  -o ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o: mcc_generated_files/driver_uart/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/driver_uart" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/driver_uart/uart.c  -o ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/uart2.o: mcc_generated_files/uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart2.c  -o ${OBJECTDIR}/mcc_generated_files/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/mcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/spi1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/traps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr3.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/rtcc.o: mcc_generated_files/rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/rtcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/rtcc.c  -o ${OBJECTDIR}/mcc_generated_files/rtcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/rtcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/rtcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/tmr1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app.o: app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app.o.d 
	@${RM} ${OBJECTDIR}/app.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app.c  -o ${OBJECTDIR}/app.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app_debug.o: app_debug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_debug.o.d 
	@${RM} ${OBJECTDIR}/app_debug.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app_debug.c  -o ${OBJECTDIR}/app_debug.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app_debug.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app_debug.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app_power.o: app_power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_power.o.d 
	@${RM} ${OBJECTDIR}/app_power.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app_power.c  -o ${OBJECTDIR}/app_power.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app_power.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app_power.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/led_status.o: led_status.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/led_status.o.d 
	@${RM} ${OBJECTDIR}/led_status.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  led_status.c  -o ${OBJECTDIR}/led_status.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/led_status.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/led_status.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/delay.o: delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/delay.o.d 
	@${RM} ${OBJECTDIR}/delay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  delay.c  -o ${OBJECTDIR}/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/delay.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/delay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/app_timers_callback.o: app_timers_callback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/app_timers_callback.o.d 
	@${RM} ${OBJECTDIR}/app_timers_callback.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  app_timers_callback.c  -o ${OBJECTDIR}/app_timers_callback.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/app_timers_callback.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/app_timers_callback.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/radio_alpha_trx.o: radio_alpha_trx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/radio_alpha_trx.o.d 
	@${RM} ${OBJECTDIR}/radio_alpha_trx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  radio_alpha_trx.c  -o ${OBJECTDIR}/radio_alpha_trx.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/radio_alpha_trx.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/radio_alpha_trx.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Services.o: Services.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Services.o.d 
	@${RM} ${OBJECTDIR}/Services.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Services.c  -o ${OBJECTDIR}/Services.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Services.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/Services.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/timer.o: timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/timer.o.d 
	@${RM} ${OBJECTDIR}/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  timer.c  -o ${OBJECTDIR}/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/timer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/temps.o: temps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/temps.o.d 
	@${RM} ${OBJECTDIR}/temps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  temps.c  -o ${OBJECTDIR}/temps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/temps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/temps.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/master_api.o: master_api.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/master_api.o.d 
	@${RM} ${OBJECTDIR}/master_api.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  master_api.c  -o ${OBJECTDIR}/master_api.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/master_api.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/master_api.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/appGSM/appGSM3_SIM800.o: appGSM/appGSM3_SIM800.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/appGSM" 
	@${RM} ${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d 
	@${RM} ${OBJECTDIR}/appGSM/appGSM3_SIM800.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  appGSM/appGSM3_SIM800.c  -o ${OBJECTDIR}/appGSM/appGSM3_SIM800.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/appGSM/appGSM3_SIM800.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o: mcc_generated_files/driver/driverGSM3_SIM800.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/driver" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/driver/driverGSM3_SIM800.c  -o ${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/driver/driverGSM3_SIM800.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o: mcc_generated_files/driver_uart/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/driver_uart" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/driver_uart/uart.c  -o ${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/driver_uart/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/mcc_generated_files/uart2.o: mcc_generated_files/uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart2.c  -o ${OBJECTDIR}/mcc_generated_files/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -I"mcc_generated_files" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/mcc_generated_files/uart2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/alpha_trx868s_basic_v1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
