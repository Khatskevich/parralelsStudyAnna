PROGPATH=.
INCLUDES=$(PROGPATH)/include
TOOLSPATH=$(PROGPATH)/tools

SRC_FILES=$(PROGPATH)/log/logProc.c $(PROGPATH)/core/data_presentation_controller.c $(PROGPATH)/core/sorted_mesg_stack.c $(PROGPATH)/core/scaner.c archivator.c

all: archivator

archivator: $(SRC_FILES) libz.a
	$(CC) $(SRC_FILES)  -o $@ -I$(INCLUDES) $(TOOLSPATH)/zlib/libz.a -pthread -g
libz.a:
	
	 $(MAKE) -C $(TOOLSPATH)/zlib
	
run: archivator
	./archivator to_compress 
archivator.c: $(INCLUDES)/zlib/zlib.h $(INCLUDES)/data_presentation.h $(INCLUDES)/log.h $(INCLUDES)/scaner.h $(INCLUDES)/sorted_mesg_stack.h  $(INCLUDES)/scaner.h $(INCLUDES)/archivator.h


clean:
	$(MAKE) -C $(TOOLSPATH)/zlib clean
	
