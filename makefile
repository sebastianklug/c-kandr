ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif

.PHONY: clean
.PHONY: test

PATHU = unity/src/
PATHS = src/
PATHT = test/
PATHB = build/
PATHD = build/depends/
PATHO = build/objs/
PATHR = build/results/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)*.c)

COMPILE=gcc -c
LINK=gcc
DEPEND=gcc -MM -MG -MF
CFLAGS=-I. -I$(PATHU) -I$(PATHS) -DTEST

RESULTS = $(patsubst $(PATHT)Test%.c,$(PATHR)Test%.txt,$(SRCT) )

PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

$(PATHR)%.txt: $(PATHB)%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

#$(PATHB)Test%.$(TARGET_EXTENSION): $(PATHO)Test%.o $(PATHO)%.o $(PATHO)unity.o #$(PATHD)Test%.d
#	$(LINK) -o $@ $^

# TestInput.out links TestInput.o + Input.o + unity.o
$(PATHB)TestInput.$(TARGET_EXTENSION): $(PATHO)TestInput.o $(PATHO)Input.o $(PATHO)unity.o
	$(LINK) -o $@ $^

# TestPointers.out links TestPointers.o + Pointers.o + Input.o + unity.o
# (Pointers.c uses getch/ungetch from Input.c)
$(PATHB)TestPointers.$(TARGET_EXTENSION): $(PATHO)TestPointers.o $(PATHO)Pointers.o $(PATHO)Input.o $(PATHO)unity.o
	$(LINK) -o $@ $^

# Example: TestBitOps.out links TestBitOps.o + BitOps.o + unity.o
$(PATHB)TestBitOps.$(TARGET_EXTENSION): $(PATHO)TestBitOps.o $(PATHO)BitOps.o $(PATHO)unity.o
	$(LINK) -o $@ $^

$(PATHB)TestString.$(TARGET_EXTENSION): $(PATHO)TestString.o $(PATHO)String.o $(PATHO)unity.o
	$(LINK) -o $@ $^

$(PATHB)TestTempConv.$(TARGET_EXTENSION): $(PATHO)TestTempConv.o $(PATHO)TempConv.o $(PATHO)unity.o
	$(LINK) -o $@ $^

$(PATHB)TestText.$(TARGET_EXTENSION): $(PATHO)TestText.o $(PATHO)Text.o $(PATHO)unity.o
	$(LINK) -o $@ $^

$(PATHO)%.o:: $(PATHT)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHS)%.c
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHO)%.o:: $(PATHU)%.c $(PATHU)%.h
	$(COMPILE) $(CFLAGS) $< -o $@

$(PATHD)%.d:: $(PATHT)%.c
	$(DEPEND) $@ $<

$(PATHB):
	$(MKDIR) $(PATHB)

$(PATHD):
	$(MKDIR) $(PATHD)

$(PATHO):
	$(MKDIR) $(PATHO)

$(PATHR):
	$(MKDIR) $(PATHR)

clean:
	$(CLEANUP) $(PATHO)*.o
	$(CLEANUP) $(PATHB)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)*.txt

.PRECIOUS: $(PATHB)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)%.d
.PRECIOUS: $(PATHO)%.o
.PRECIOUS: $(PATHR)%.txt
