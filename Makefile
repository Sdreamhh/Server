SRC_DIR = ./src/
INC_DIR = ./inc/


TARGET = server
CC = gcc

CFLAGS += -c 
CFLAGS += -I$(INC_DIR)

LDFLAGS += -lpthread

SOURCE = $(wildcard $(SRC_DIR)*.c)
OBJECT = $(patsubst %.c, %.o, $(SOURCE))

$(TARGET): $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ $< $(CFLAGS)

### compile for riscv

MILKV_DUO_SDK=./duo-sdk
TOOLCHAIN_DIR=${MILKV_DUO_SDK}/riscv64-linux-musl-x86_64
SYSROOT=${MILKV_DUO_SDK}/rootfs
TOOLCHAIN_PREFIX=${TOOLCHAIN_DIR}/bin/riscv64-unknown-linux-musl-

RISCV_LDFLAGS = -mcpu=c906fdv -march=rv64imafdcv0p7xthead -mcmodel=medany -mabi=lp64d
RISCV_CFLAGS = -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

RISCV_CC = $(TOOLCHAIN_PREFIX)gcc

RISCV_LDFLAGS += -L$(SYSROOT)/lib
RISCV_LDFLAGS += -L$(SYSROOT)/usr/lib
RISCV_LDFLAGS += -lwiringx
RISCV_LDFLAGS += -lpthread
RISCV_SOURCE = $(wildcard *.c)
RISCV_OBJECT = $(patsubst %.c,%_riscv.o,$(SOURCE))

SUFFIX = _riscv


server_riscv: $(RISCV_OBJECT)
	$(RISCV_CC) -o $@ $(RISCV_OBJECT) $(RISCV_LDFLAGS)
%_riscv.o: %.c
	$(RISCV_CC) -o $@ $< $(CFLAGS)


.PHONY: clean
clean: 
	-rm $(TARGET) server_riscv

.PHONY: cla
cla: clean
	-rm $(SRC_DIR)*.o 
	


