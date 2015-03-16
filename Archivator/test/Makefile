PROGPATH=..
INCLUDES=$(PROGPATH)/include
log_test: $(INCLUDES)/log.h $(PROGPATH)/log/log.c log_test.c
	$(CC) -o log_test_out log_test.c $(PROGPATH)/log/log.c -pthread -I$(INCLUDES)
	./log_test_out
clean:
	rm log_test_out*
