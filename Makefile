CC := gcc

TGT := aes
LIB := libaes.so
TST := $(TGT)-test

LIB_SRC := $(TGT).c
TST_SRC := $(LIB_SRC) $(TST).c

LIB_HDR := $(TGT).h
TST_HDR := $(LIB_HDR)

CWARNS := -Wall -Wextra
CFLAGS := -O3 -std=gnu99

test: $(TST_SRC) $(TST_HDR)
	@echo [CC] $(TST) $(CFLAGS)
	@$(CC) -o $(TST) $(CWARNS) $(TST_SRC) $(CFLAGS)

libaes $(LIB_SRC) $(LIB_HDR):
	@echo [CC] $(TGT).o $(CFLAGS)
	@$(CC) -c -fpic $(CWARNS) $(LIB_SRC) $(CFLAGS)

	@echo [CC] $(LIB) $(CFLAGS)
	@$(CC) -shared -o $(LIB) $(TGT).o

clean:
	@rm *.o 2> /dev/null || true

	@[ ! -f $(TST) ] || rm $(TST)
	@[ ! -f $(LIB) ] || rm $(LIB)
