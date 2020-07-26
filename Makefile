OBJDIR := obj
BINDIR := bin
SRCDIR := src
TSTDIR := $(SRCDIR)/test

TARGET := aes
TESTER := $(TARGET)-test
SHARED := lib$(TARGET).so

CFLAGS := -std=gnu99
CWARNS := -Wall -Wextra

TSTFLAGS := -O2
AESFLAGS := -O3 -ffast-math

BINS := $(addprefix $(BINDIR)/, $(SHARED)   $(TESTER))
OBJS := $(addprefix $(OBJDIR)/, $(TARGET).o $(TESTER).o)

all: $(OBJS) $(BINS)

$(BINDIR)/$(TESTER): $(OBJS)
	@echo [$(CC)] $@
	@$(CC) -s -o $@ $(OBJS)

$(BINDIR)/$(SHARED): $(OBJDIR)/$(TARGET).o
	@echo [$(CC)] $@
	@$(CC) -fpic -shared -o $@ $<

$(OBJDIR)/$(TESTER).o: $(TSTDIR)/$(TESTER).c
	@echo [$(CC)] $@
	@$(CC) $(CWARNS) $(CFLAGS) $(TSTFLAGS) -c -o $@ $<

$(OBJDIR)/$(TARGET).o: $(SRCDIR)/$(TARGET).c $(SRCDIR)/$(TARGET).h
	@echo [$(CC)] $@
	@$(CC) $(CWARNS) $(CFLAGS) $(AESFLAGS) -c -o $@ $<

$(OBJS): | $(OBJDIR)
$(BINS): | $(BINDIR)

$(OBJDIR):
	@mkdir $(OBJDIR)

$(BINDIR):
	@mkdir $(BINDIR)

clean:
	@echo [rm] $(OBJDIR) $(BINDIR)
	@rm -rf $(OBJDIR) $(BINDIR) 2> /dev/null || true

.PHONY: clean
