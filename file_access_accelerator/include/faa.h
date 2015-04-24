#include <stdlib.h>
#define FAA_FLUSH_ALL -1ull

int faa_init(const char *file_name, 
                         size_t block_size,  	// in bytes
			 size_t cache_size,  	// in bytes
			 int compression_level, 
			 size_t *file_size);
int faa_deinit();

void *faa_inc_ref(size_t offset);
int faa_dec_ref(size_t offset);

int faa_flush(size_t offset);

