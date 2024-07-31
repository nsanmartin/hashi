CFLAGS:=-g  -Wall -Wextra -Werror -pedantic -Wold-style-definition 
CXXFLAGS:=-g  -Wall -Wextra -Werror -pedantic 

BUILD:=../build
INCLUDE:=.

HASHI_OBJDIR=../build
HASHI_SRCDIR=../src
HASHI_INCLUDE=../include
HASHI_HEADERS=$(wildcard $(HASHI_INCLUDE)/*.h)
HASHI_SRCS=$(wildcard $(HASHI_SRCDIR)/*.c)
HASHI_OBJ=$(HASHI_SRCS:src/%.c=$(HASHI_OBJDIR)/%.o)

all: \
	collisions \
	count-words8 \
	rand-word8-count \
	rand-word8s \
	stdin-count \
	stdin-count-cppmap \


collisions: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) -I. \
		-o $(BUILD)/$@ $@.c $^ -lm


rand-word8-count: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

count-words8: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm

stdin-count-cppmap: 
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$@ $@.c $^

stdin-count: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm


rand-word8s: 
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $@.c 


slot-sizes-words8: $(HASHI_OBJ)
	$(CC) $(CFLAGS) \
		-I$(INCLUDE) -I$(HASHI_INCLUDE) -o $(BUILD)/$@-E.c $@.c $^ -lm
	$(CC) $(CFLAGS) \
		-I$(INCLUDE) -I$(HASHI_INCLUDE) -o $(BUILD)/$@ $@.c $^ -lm

collisions: $(HASHI_OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(HASHI_INCLUDE) \
		-o $(BUILD)/$@ $@.c $^ -lm
