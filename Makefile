## Author: Steffen Viken Valvaag <steffenv@cs.uit.no> 
LIST_SRC=linkedlist.c
MAP_SRC=map.c  # Insert the file name of your map implementation here
WORDFREQS_SRC=wordfreqs.c common.c $(LIST_SRC) $(MAP_SRC)
TEST_SRC=test_map.c common.c $(LIST_SRC) $(MAP_SRC)
HEADERS=common.h list.h map.h

all: wordfreqs

wordfreqs: $(WORDFREQS_SRC) $(HEADERS) Makefile
	gcc -o $@ $(WORDFREQS_SRC)

test: $(TEST_SRC) $(HEADERS) Makefile
	gcc -o $@ $(TEST_SRC)

clean:
	rm -f *~ *.o *.exe wordfreqs test
