## Author: Christian Salomonsen <christian.salomonsen@uit.no> 
LIST_SRC=linkedlist.c
MAP_SRC=map.c 
WORDFREQS_SRC=wordfreqs.c common.c $(LIST_SRC) $(MAP_SRC)
HEADERS=common.h list.h map.h

all: wordfreqs

wordfreqs: $(WORDFREQS_SRC) $(HEADERS) Makefile
	gcc -o $@ $(WORDFREQS_SRC)

data:
	./get_data.sh 10

test: wordfreqs data
	./wordfreqs data/trump_tweet_*.txt

clean:
	rm -f *~ *.o *.exe wordfreqs test
	rm -rf data
