CFLAGS:=-g  -Wall -Wextra -Werror -pedantic -Wold-style-definition 

BUILD:=../build
INCLUDE:=.

HASHI_OBJDIR=../build
HASHI_SRCDIR=../src
HASHI_INCLUDE=../include
HASHI_HEADERS=$(wildcard $(HASHI_INCLUDE)/*.h)
HASHI_SRCS=$(wildcard $(HASHI_SRCDIR)/*.c)
HASHI_OBJ=$(HASHI_SRCS:src/%.c=$(HASHI_OBJDIR)/%.o)

count-words8-arl: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) -o $(BUILD)/$@ $@.c $^ -lm

count-words8: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) -o $(BUILD)/$@ $@.c $^ -lm

read-words8: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) -o $(BUILD)/$@ $@.c $^ -lm

rand-words8: 
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $@.c 

