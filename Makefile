CFLAGS:=-g -std=c2x -Wall -Wextra -Werror -pedantic -Wold-style-definition -Wunused-function


OBJDIR=build
SRCDIR=src
INCLUDE=include
HEADERS=$(wildcard $(INCLUDE)/*.h)
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRCS:src/%.c=$(OBJDIR)/%.o)

test_all: test_lip test_arl test_buf
	build/test_lip && build/test_arl && build/test_buf


test_buf: utests/test_buf.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@-E.c $<
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@ $^


test_lip: utests/test_lip.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@-E.c $<
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@ $^


test_arl: utests/test_arl.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@-E.c $<
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@ $^


test_hat: utests/test_hat.c $(OBJ) 
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -Iutests -Iscripts -o build/$@-E.c $<
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -Iscripts -o build/$@ $^ -lm

test_hat-E: $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@ build/$@.c $^ -lm

test_mua: utests/test_mua.c
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@-E.c $<
	$(CC) $(CFLAGS) -I$(INCLUDE) -Iutests -o build/$@ $^

collisions-mua: $(OBJ)
	$(CC) -E $(CFLAGS) -I$(INCLUDE) -o build/$@-E.c $@.c
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

