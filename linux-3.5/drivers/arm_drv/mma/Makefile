TARGET			:= arm
BIN			:= $(TARGET).bin
START_OBJ		:= start.o
OBJS			:= main.o
OBJS			+= mmu.o vectors.o irq.o
OBJS			+= mma7660.o
DUMP			:= $(TARGET).s

CROSS_COMPILE	:= arm-linux-

CC			:= $(CROSS_COMPILE)gcc
LD			:= $(CROSS_COMPILE)ld
AS			:= $(CROSS_COMPILE)as
OBJCOPY			:= $(CROSS_COMPILE)objcopy
OBJDUMP			:= $(CROSS_COMPILE)objdump
NM			:= $(CROSS_COMPILE)nm

CFLAGS			:= -Wall -I./include/ -fno-builtin

###########################################

all:$(TARGET)
	@$(OBJCOPY) -O binary $< $(BIN)
	@ls -lh $(BIN)

$(TARGET):$(START_OBJ) $(OBJS)
	@$(LD) $(START_OBJ) $(OBJS) -o $(TARGET) -Tld.lds
	@$(OBJDUMP) -d $(TARGET) > $(DUMP)
	@$(NM) $(TARGET) > System.map	

%.o:%.S
	@$(AS) -o $@ $<

%.o:%.s
	@$(AS) -o $@ $<

%.o:%.c
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -rf $(TARGET) $(OBJS) $(START_OBJ) $(DUMP) System.map $(BIN)
