PROGPATH=.
INCLUDES=$(PROGPATH)/include
TOOLSPATH=$(PROGPATH)/tools
CORE=$(PROGPATH)/core
CFLAGS=-I$(INCLUDES) -O3 $(TOOLSPATH)/zlib/libz.a -g


SRC_MAIN_FILES=$(PROGPATH)/log/logProc.c $(CORE)/faa_cache.c $(CORE)/faa_data_structure.c $(CORE)/file_access_accelerator.c
SRC_ADDITION_FILES=$(CORE)/faa_fast_cache.c $(CORE)/faa_hash.c
SRC_FILES=$(SRC_MAIN_FILES) $(SRC_ADDITION_FILES)

all: $(TOOLSPATH)/zlib/libz.a faa.a

faa.a: $(SRC_FILES:.c=.o)
	ar rsc faa.a $(SRC_FILES:.c=.o) $(TOOLSPATH)/zlib/libz.a

faa: $(SRC_FILES) libz.a
	$(CC) -c -fPIC $(SRC_FILES)  -o $@ -I$(INCLUDES) $(TOOLSPATH)/zlib/libz.a
$(TOOLSPATH)/zlib/libz.a:
	 $(MAKE) -C $(TOOLSPATH)/zlib
	
$(CORE)/file_access_accelerator.c: $(INCLUDES)/zlib/zlib.h $(INCLUDES)/faa.h $(INCLUDES)/log.h $(INCLUDES)/faa_cache.h $(INCLUDES)/faa_data_structure.h $(INCLUDES)/faa_internal.h


clean:
	$(MAKE) -C $(TOOLSPATH)/zlib clean
	rm $(SRC_FILES:.c=.o)
	rm faa.a
cleanall: clean
	$(MAKE) -C $(TOOLSPATH)/zlib clean
