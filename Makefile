TRGT := aes
TEST := test
SLIB := lib$(TRGT).so

OBJDIR := obj
BINDIR := bin

CFLAGS := -std=gnu99
CWARNS := -Wall -Wextra

BINS := $(addprefix $(BINDIR)/, $(SLIB)   $(TEST))
OBJS := $(addprefix $(OBJDIR)/, $(TRGT).o $(TEST).o)

all: $(OBJS) $(BINS)

$(BINDIR)/$(TEST): $(OBJS)
	@echo [$(CC)] $@
	@$(CC) -s -o $@ $(OBJS)

$(BINDIR)/$(SLIB): $(OBJDIR)/$(TRGT).o
	@echo [$(CC)] $@
	@$(CC) -fpic -shared -o $@ $<

$(OBJDIR)/$(TEST).o: $(TEST).c
	@echo [$(CC)] $@
	@$(CC) $(CFLAGS) $(CWARNS) -c -o $@ $<

$(OBJDIR)/$(TRGT).o: $(TRGT).c $(TRGT).h
	@echo [$(CC)] $@
	@$(CC) $(CFLAGS) -Ofast $(CWARNS) -c -o $@ $<

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
