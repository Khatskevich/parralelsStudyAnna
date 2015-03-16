PROGPATH=..
INCLUDES=$(PROGPATH)/include
log_test: $(INCLUDES)/log.h $(PROGPATH)/log/log.c log_test.c
	$(CC) -o log_test log_test.c $(PROGPATH)/log/log.c -pthread -I$(INCLUDES)
	./log_test
clean:
	rm log_test
