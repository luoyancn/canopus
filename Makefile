ifeq ($(VERBOSE),1)
ECHO := 
else
ECHO := @
endif

# Where is the Intel(R) FPGA SDK for OpenCL(TM) software?
ifeq ($(wildcard $(ALTERAOCLSDKROOT)),)
$(error Set ALTERAOCLSDKROOT to the root directory of the Intel(R) FPGA SDK for OpenCL(TM) software installation)
endif
ifeq ($(wildcard $(ALTERAOCLSDKROOT)/host/include/CL/opencl.h),)
$(error Set ALTERAOCLSDKROOT to the root directory of the Intel(R) FPGA SDK for OpenCL(TM) software installation.)
endif

# OpenCL compile and link flags.
AOCL_COMPILE_CONFIG := $(shell aocl compile-config )
AOCL_LINK_CONFIG := $(shell aocl link-config )

# Compilation flags
ifeq ($(DEBUG),1)
CFLAGS += -g
else
CFLAGS += -g -O2 -Wall
endif

# Compiler
CC := gcc

# Target
TARGET := canopus

# Directories
INC_DIRS := include
LIB_DIRS :=

# Files
INCS := $(wildcard )
SRCS := $(wildcard src/*.c)
LIBS := rt pthread

# Make it all!
all : $(TARGET)

# Host executable target.
$(TARGET) : Makefile $(SRCS) $(INCS)
	$(ECHO)$(CC) $(CFLAGS) -fPIC $(foreach D,$(INC_DIRS),-I$D) \
			$(AOCL_COMPILE_CONFIG) $(SRCS) $(AOCL_LINK_CONFIG) \
			$(foreach D,$(LIB_DIRS),-L$D) \
			$(foreach L,$(LIBS),-l$L) \
			-o $(TARGET)

# Standard make targets
clean :
	$(ECHO)rm -f $(TARGET)

.PHONY : all clean
