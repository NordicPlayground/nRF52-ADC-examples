PROJECT_NAME := saadc_pca10036

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) ) 

TEMPLATE_PATH = ../../../../../components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source common to all targets
C_SOURCE_FILES += \
$(abspath ../../../../../components/toolchain/system_nrf52.c) \
$(abspath ../../main.c) \
$(abspath ../../../../../components/libraries/util/app_error.c) \
$(abspath ../../../../../components/libraries/util/app_error_weak.c) \
$(abspath ../../../../../components/libraries/fifo/app_fifo.c) \
$(abspath ../../../../../components/libraries/util/app_util_platform.c) \
$(abspath ../../../../../components/libraries/util/nrf_assert.c) \
$(abspath ../../../../../components/libraries/uart/retarget.c) \
$(abspath ../../../../../components/libraries/uart/app_uart_fifo.c) \
$(abspath ../../../../../components/drivers_nrf/delay/nrf_delay.c) \
$(abspath ../../../../../components/drivers_nrf/common/nrf_drv_common.c) \
$(abspath ../../../../../components/drivers_nrf/ppi/nrf_drv_ppi.c) \
$(abspath ../../../../../components/drivers_nrf/saadc/nrf_drv_saadc.c) \
$(abspath ../../../../../components/drivers_nrf/timer/nrf_drv_timer.c) \
$(abspath ../../../../../components/drivers_nrf/uart/nrf_drv_uart.c) \
$(abspath ../../../../../components/drivers_nrf/hal/nrf_saadc.c) \

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath ../../../../../components/toolchain/gcc/gcc_startup_nrf52.s)

#includes common to all targets
INC_PATHS  = -I$(abspath ../../config)
INC_PATHS += -I$(abspath ../../../../bsp)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/ppi)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/nrf_soc_nosd)
INC_PATHS += -I$(abspath ../../../../../components/device)
INC_PATHS += -I$(abspath ../../../../../components/libraries/uart)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/hal)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/timer)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/delay)
INC_PATHS += -I$(abspath ../../../../../components/toolchain/CMSIS/Include)
INC_PATHS += -I$(abspath ../..)
INC_PATHS += -I$(abspath ../../../../../components/libraries/util)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/uart)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/common)
INC_PATHS += -I$(abspath ../../../../../components/toolchain)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/config)
INC_PATHS += -I$(abspath ../../../../../components/drivers_nrf/saadc)
INC_PATHS += -I$(abspath ../../../../../components/libraries/fifo)
INC_PATHS += -I$(abspath ../../../../../components/toolchain/gcc)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

#flags common to all targets
CFLAGS  = -DNRF52_PAN_16
CFLAGS += -DNRF52_PAN_23
CFLAGS += -DNRF52
CFLAGS += -DNRF52_PAN_4
CFLAGS += -DNRF52_PAN_7
CFLAGS += -DNRF52_PAN_1
CFLAGS += -DNRF52_PAN_3
CFLAGS += -DNRF52_PAN_2
CFLAGS += -DNRF52_PAN_9
CFLAGS += -DNRF52_PAN_8
CFLAGS += -DNRF52_PAN_11
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DNRF52_PAN_39
CFLAGS += -DNRF52_PAN_38
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DNRF52_PAN_17
CFLAGS += -DNRF52_PAN_65
CFLAGS += -DNRF52_PAN_33
CFLAGS += -DNRF52_PAN_32
CFLAGS += -DNRF52_PAN_30
CFLAGS += -DNRF52_PAN_37
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_35
CFLAGS += -DNRF52_PAN_34
CFLAGS += -DNRF52_PAN_10
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF52_PAN_12
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DNRF52_PAN_28
CFLAGS += -DNRF52_PAN_29
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DBOARD_PCA10036
CFLAGS += -DNRF52_PAN_24
CFLAGS += -DNRF52_PAN_25
CFLAGS += -DNRF52_PAN_26
CFLAGS += -DNRF52_PAN_27
CFLAGS += -DNRF52_PAN_42
CFLAGS += -DNRF52_PAN_43
CFLAGS += -DNRF52_PAN_40
CFLAGS += -DNRF52_PAN_41
CFLAGS += -DNRF52_PAN_46
CFLAGS += -DNRF52_PAN_47
CFLAGS += -DNRF52_PAN_44
CFLAGS += -DNRF52_PAN_63
CFLAGS += -DNRF52_PAN_48
CFLAGS += -DNRF52_PAN_49
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums 
# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF52_PAN_16
ASMFLAGS += -DNRF52_PAN_23
ASMFLAGS += -DNRF52
ASMFLAGS += -DNRF52_PAN_4
ASMFLAGS += -DNRF52_PAN_7
ASMFLAGS += -DNRF52_PAN_1
ASMFLAGS += -DNRF52_PAN_3
ASMFLAGS += -DNRF52_PAN_2
ASMFLAGS += -DNRF52_PAN_9
ASMFLAGS += -DNRF52_PAN_8
ASMFLAGS += -DNRF52_PAN_11
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DNRF52_PAN_39
ASMFLAGS += -DNRF52_PAN_38
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DNRF52_PAN_17
ASMFLAGS += -DNRF52_PAN_65
ASMFLAGS += -DNRF52_PAN_33
ASMFLAGS += -DNRF52_PAN_32
ASMFLAGS += -DNRF52_PAN_30
ASMFLAGS += -DNRF52_PAN_37
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_35
ASMFLAGS += -DNRF52_PAN_34
ASMFLAGS += -DNRF52_PAN_10
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DNRF52_PAN_28
ASMFLAGS += -DNRF52_PAN_29
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DBOARD_PCA10036
ASMFLAGS += -DNRF52_PAN_24
ASMFLAGS += -DNRF52_PAN_25
ASMFLAGS += -DNRF52_PAN_26
ASMFLAGS += -DNRF52_PAN_27
ASMFLAGS += -DNRF52_PAN_42
ASMFLAGS += -DNRF52_PAN_43
ASMFLAGS += -DNRF52_PAN_40
ASMFLAGS += -DNRF52_PAN_41
ASMFLAGS += -DNRF52_PAN_46
ASMFLAGS += -DNRF52_PAN_47
ASMFLAGS += -DNRF52_PAN_44
ASMFLAGS += -DNRF52_PAN_63
ASMFLAGS += -DNRF52_PAN_48
ASMFLAGS += -DNRF52_PAN_49

#default target - first one defined
default: clean nrf52832_xxaa

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf52832_xxaa

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf52832_xxaa

C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf52832_xxaa: OUTPUT_FILENAME := nrf52832_xxaa
nrf52832_xxaa: LINKER_SCRIPT=saadc_gcc_nrf52.ld

nrf52832_xxaa: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Assembly file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<
# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex: 
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex
echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
flash: nrf52832_xxaa
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex -f nrf52  --chiperase
	nrfjprog --reset -f nrf52

## Flash softdevice