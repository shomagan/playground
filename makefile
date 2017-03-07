TARGET= mega11

BASE= .
CC= $(TOOL)gcc
LD= $(TOOL)gcc
AS= $(CC) -x assembler-with-cpp
OBJCOPY= $(TOOL)objcopy
OBJDUMP= $(TOOL)objdump
SIZE= $(TOOL)size -d
FLASHER= openocd
RM= rm -f
CP= cp
MD= mkdir

SRCDIR= $(BASE)
OBJDIR= $(BASE)/OBJ
EXEDIR= $(BASE)/EXE
LSTDIR= $(BASE)/LST
PRJDIR= $(BASE)/PRJ
BAKDIR= $(BASE)/BAK

HEX= $(EXEDIR)/$(TARGET).hex
BIN= $(EXEDIR)/$(TARGET).bin
ELF= $(EXEDIR)/$(TARGET).elf
MAP= $(LSTDIR)/$(TARGET).map
LSS= $(LSTDIR)/$(TARGET).lss
OK= $(EXEDIR)/$(TARGET).ok
 
LD_SCRIPT= $(PRJDIR)/$(TARGET).ld

DIRS:= $(SRCDIR)

INCS:= $(patsubst %, -I "%", $(DIRS))

SRCS:= 

OBJS:= $(SRCS)
OBJS+= $(wildcard $(addsuffix /*.cpp, $(DIRS)))
OBJS+= $(wildcard $(addsuffix /*.c, $(DIRS)))
OBJS+= $(wildcard $(addsuffix /*.S, $(DIRS)))
OBJS:= $(notdir $(OBJS))
OBJS:= $(OBJS:.cpp=.o)
OBJS:= $(OBJS:.c=.o)
OBJS:= $(OBJS:.S=.o)
OBJS:= $(patsubst %, $(OBJDIR)/%, $(OBJS))


ARCFILES= $(SRCDIR)/makefile $(SRCDIR)/*.* $(PRJDIR)/*.* $(EXEDIR)/*.* $(FBDIR)/*.*

FLAGS+= $(INCS)
FLAGS+= -MD
FLAGS+= -Wa,-a,-ad=$(addprefix $(LSTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))

AFLAGS= $(FLAGS)

CFLAGS= $(FLAGS)
CFLAGS+= $(OPTIMIZE)
CFLAGS+= -std=gnu99
CFLAGS+= -g3
CFLAGS+= -mfloat-abi=soft -fno-common -fshort-wchar -funsigned-bitfields -fshort-double -funsigned-char -fpack-struct
CFLAGS+= -Wall -Wextra
CFLAGS+= -Wimplicit -Wpointer-arith -Wredundant-decls
CFLAGS+= -Wshadow -Wnested-externs 
CFLAGS+= -Wcast-qual -Wcast-align

LD_FLAGS= -mcpu=$(MCU)
LD_FLAGS+= -nostartfiles 
LD_FLAGS+= -Wl,-Map="$(MAP)",--cref
LD_FLAGS+= -Wl,--gc-sections
LD_FLAGS+= -T$(LD_SCRIPT)
LD_FLAGS+= -lm

all: start dirs $(ELF) $(BIN) $(LSS) $(OK)
#all: start dirs $(ELF) $(BIN) $(HEX) $(LSS) $(OK)

build: clean all

start:
	@echo --- building $(TARGET)

#$(LSS): $(ELF) makefile
#	@echo --- making asm-lst...
#	@$(OBJDUMP) -dC $(ELF) > $(LSS)

$(OK): $(ELF)
	@$(SIZE) $(ELF)
	@echo "Errors: none"

$(ELF):	$(OBJS) makefile
	@echo --- linking...
	$(LD) $(OBJS) $(LIBS) $(LD_FLAGS) -o "$(ELF)"

$(BIN): $(ELF)
	@echo --- make binary...
	@$(OBJCOPY) -O binary --gap-fill 0xff $(ELF) $(BIN)

VPATH:= $(DIRS)

$(OBJDIR)/%.o: %.c makefile
	@echo --- compiling $<...
	$(CC) -c $(CFLAGS) -o $@ $< 

dirs: $(OBJDIR) $(EXEDIR) $(LSTDIR) $(BAKDIR)

$(OBJDIR):
	-@$(MD) $(OBJDIR)

$(EXEDIR):
	-@$(MD) $(EXEDIR)

$(LSTDIR):
	-@$(MD) $(LSTDIR)

$(BAKDIR):
	-@$(MD) $(BAKDIR)

clean:
	-@$(RM) $(OBJDIR)/*.d 2>/dev/null
	-@$(RM) $(OBJDIR)/*.o 2>/dev/null
	-@$(RM) $(LSTDIR)/*.lst 2>/dev/null
	-@$(RM) $(ELF)
	-@$(RM) $(HEX)
	-@$(RM) $(LSS)
	-@$(RM) $(MAP)
	-@$(RM) $(EXE)

archive:
	@echo --- archiving...
	winrar a -agYYYY-MM-DD_HH-MM $(BAKDIR)/$(TARGET)_ $(ARCFILES)
	@echo --- done!

ifeq (,$(findstring build,$(MAKECMDGOALS)))
	ifeq (,$(findstring clean,$(MAKECMDGOALS)))
		ifeq (,$(findstring dirs,$(MAKECMDGOALS)))
			-include $(wildcard $(OBJDIR)/*.d) 
		endif
	endif
endif

