CC     ?= gcc   # ${HOME}/gcc/gcc830_from_gcc830/bin/gcc
CWARN   = -Wall -Wextra -Winline -Wdisabled-optimization
COPT    = -O3
CFLAGS  = -Isrc $(CWARN) $(COPT) -std=gnu11 -pedantic
LDFLAGS = -L.
LDLIBS  = -lsimpletrie

TARGET = libsimpletrie.a
SRC  = $(wildcard src/*.c)
OBJS = $(SRC:.c=.o)

TEST_DIR = test
TESTS = $(addprefix $(TEST_DIR)/,t01)


$(TARGET): $(OBJS)
	$(AR) rcs $@ $(OBJS)


test: $(TARGET) $(TESTS)
.PHONY: test

$(TESTS): % : %.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS) -DNDEBUG

valgrind:
	valgrind -v --leak-check=full --track-origins=yes --error-exitcode=1 \
		test/t01
.PHONY: valgrind


clean:
	$(RM) $(TARGET) $(OBJS) $(TESTS) $(addsuffix .o,$(TESTS))
.PHONY: clean
