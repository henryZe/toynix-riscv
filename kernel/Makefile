KERN_LDFLAGS := $(LDFLAGS) -T $(KERNDIR)/kernel.ld -nostdlib

# entry.S must be first, so that it's the first code in the text segment
KERN_SRCFILES = \
		$(KERNDIR)/entry.S \
		$(KERNDIR)/start.c \

# Binary program images to embed within the kernel.
KERN_OBJFILES := $(patsubst $(KERNDIR)/%.S, $(OBJDIR)/$(KERNDIR)/%.o, $(KERN_SRCFILES))
KERN_OBJFILES := $(patsubst $(KERNDIR)/%.c, $(OBJDIR)/$(KERNDIR)/%.o, $(KERN_OBJFILES))

# How to build kernel object files
$(OBJDIR)/$(KERNDIR)/%.o: $(KERNDIR)/%.S
	@echo + as[KERN] $<
	@mkdir -p $(@D)
	$(CC) -nostdinc $(CFLAGS) -c -o $@ $<

$(OBJDIR)/$(KERNDIR)/%.o: $(KERNDIR)/%.c
	@echo + cc[KERN] $<
	@mkdir -p $(@D)
	$(CC) -nostdinc $(CFLAGS) -c -o $@ $<

$(OBJDIR)/$(KERNDIR)/kernel: $(KERN_OBJFILES) $(KERNDIR)/kernel.ld
	$(LD) -o $@ $(KERN_LDFLAGS) $(KERN_OBJFILES)
	$(OBJDUMP) -S $@ > $@.asm
	$(NM) -n $@ > $@.sym
