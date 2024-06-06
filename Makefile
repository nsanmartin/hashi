CFLAGS:=-g  -Wall -Wextra -Werror -pedantic -Wold-style-definition 


OBJDIR=build
SRCDIR=src
INCLUDE=include
HEADERS=$(wildcard $(INCLUDE)/*.h)
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRCS:src/%.c=$(OBJDIR)/%.o)

test_mua: utests/test_mua.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@-E.c $^
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@ $^

collisions: $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o build/$@ $@.c $^

test_mutarr: utests/test_mutarr.c $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o build/$@ $^ 


$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS)		\
		-I$(INCLUDE)    \
		-c -o $@     	\
		$< 

tags:
	ctags --languages=c -R .

cscope:
	cscope -b -k -R

clean:
	find build -type f -delete
	if [ -f tags ]; then rm tags; fi

