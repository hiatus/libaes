TEST := test
SLIB := libaes.so

OBJDIR := obj
BINDIR := bin

CFLAGS := -std=gnu99
CWARNS := -Wall -Wextra

OBJS := $(addprefix $(OBJDIR)/, aes.o test.o)
BINS := $(addprefix $(BINDIR)/, libaes.so test)

all: $(OBJS) $(BINS)

$(BINDIR)/$(TEST): $(OBJS)
	@echo [$(CC)] $(TEST)
	@$(CC) -s -o $@ $(OBJS)

$(BINDIR)/$(SLIB): $(OBJDIR)/aes.o
	@echo [$(CC)] $(SLIB)
	@$(CC) -fpic -shared -o $@ $<

$(OBJDIR)/test.o: test.c
	@echo [$(CC)] $@
	@$(CC) $(CFLAGS) $(CWARNS) -c -o $@ $<

$(OBJDIR)/aes.o: aes.c aes.h
	@echo [$(CC)] $@
	@$(CC) $(CFLAGS) -Ofast $(CWARNS) -c -o $@ $<

$(OBJS): | $(OBJDIR)
$(BINS): | $(BINDIR)

$(OBJDIR):
	@mkdir obj

$(BINDIR):
	@mkdir bin

clean:
	@rm -rf obj bin 2> /dev/null || true

.PHONY: clean
