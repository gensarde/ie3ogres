include ../../config.mk

GAME_CODE      ?= arm9
MWCCVER        := dsi/1.2p1
PROC           := arm946e
PROC_S         := arm5te
PROC_LD        := v5te
BUILD_DIR      := build
RTTIFLAG       := -RTTI on
OPTFLAGS       := -O4,p
EXCCFLAGS      := -Cpp_exceptions off

include ../../common.mk


MWLDFLAGS      := -library -nodead -w off -proc $(PROC_LD)
LIB            := $(BUILD_DIR)/libMobiclip.a

.PHONY: all install

INSTALL_PREFIX ?= $(WORK_DIR)/lib/ARM9-TS
INSTALL_LIBDIR := $(INSTALL_PREFIX)/lib

all:
	$(MAKE) patch_mwasmarm
	$(MAKE) $(LIB)

ASM_SRCS := asm/YuvToArgb.s
ASM_SRCS := asm/vx2Code.s
C_SRCS   := \
	src/libMobiclip.cpp \
	src/handle.cpp \
	src/relocator.cpp \
	src/streamfile.cpp \
	src/streammemory.cpp

ALL_OBJS := \
	$(BUILD_DIR)/asm/YuvToArgb.o \
	$(BUILD_DIR)/src/libMobiclip.o \
	$(BUILD_DIR)/src/handle.o \
	$(BUILD_DIR)/src/relocator.o \
	$(BUILD_DIR)/src/streamfile.o \
	$(BUILD_DIR)/src/streammemory.o \
	$(BUILD_DIR)/asm/vx2Code.o

$(LIB): $(ALL_OBJS)
	cd $(BUILD_DIR) && LM_LICENSE_FILE=$(BACK_REL)/$(LM_LICENSE_FILE) $(WINE) $(MWLD) $(MWLDWRAP) $(MWLDFLAGS) -o $(BACK_REL)/$(LIB) $(ALL_OBJS:$(BUILD_DIR)/%=%)

install: all
	$(shell mkdir -p $(INSTALL_LIBDIR))
	cp $(LIB) $(INSTALL_LIBDIR)/

tidy:
	$(RM) -r build

clean: tidy
