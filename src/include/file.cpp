#include "file.hpp"


bool loadFile(
	const char* path, /* filepath 														   */ 
	size_t* 	size, /* the size of the out array that was pre-allocated.                 */
	char*		out   /* Where to place the file contents (Buffer of Min-Size 'size')      */
) {
	namespace fs = std::filesystem;
	ifcrash(!fs::exists(path));

	FILE*  file;
	size_t fsize = fs::file_size(path);
	if(*size < fsize || out == nullptr) {
		/* A buffer wasn't allocated OR the size of the buffer is too small. */
		*size = fsize;
		return false;
	}
	file = fopen(path, "rb");
	ifcrash(file == nullptr);


	fsize = fread(out, sizeof(u8), *size, file);
	ifcrashdo(fsize != *size, {
		fclose(file); 
		printf("Something went wrong - file size is %lu bytes, read only %lu bytes\n", *size, fsize);
	});


	fsize = fclose(file);
	ifcrashdo(fsize, {
		printf("Couldn't close file handle. ERROR CODE: %lu\n", fsize);
	});
	return true;
}