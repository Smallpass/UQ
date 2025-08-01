# Compileri
CC = gcc
	
# Define the flags for compilation
CFLAGS = -Wall -Wextra -pedantic -std=gnu99
	
# Output executable name
TARGET = uqentropy

# Source files
SRCS = uqentropy.c
	
# Object files
OBJS = $(SRCS:.c=.o)
	
# Linker flags (including math library)
LDFLAGS = -lm
	
# Default target
all: $(TARGET)
	
# Rule to build the target
$(TARGET): $(OBJS)
        $(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	
# Rule to compile source files into object files
%.o: %.c
        $(CC) $(CFLAGS) -c $< -o $@
	
# Specify the file format
format:
        2310reformat.sh uqentropy.c
	
# Clean up build files
clean:
        rm -f $(OBJS) $(TARGET)


#include <stdbool.h>
const char* const parallelArg = "--parallel";
const char* const outputArg = "--output";
const char* const extractArg = "--extract";
const char* const bzip2Arg = "--bzip2";
const char* const gzipArg = "--gzip";
const char* const zipArg = "--zip";
const char* const xzArg = "--xz";
const char* const noneArg = "--none";

const int const commandlineerror = 16;


// Structure to hold information about a valid set of command line arguments
typedef struct {
    bool parallel;
    bool output;
    bool extract;
    bool bzip2;
    bool gzip;
    bool zip;
    bool xz;
    bool none;
    int numFiles;
    char** newuncompressedfilename;
    char** uqzipFileName;

} Parameters;

Parameters process_command_line(int argc, char* argv[])
{
    Parameters params = {.extract = false,
            .parallel = false,
            .output = false,
            .extract = false,
            .bzip2 = false,
            .gzip = false,
            .zip = false,
            .xz = false,
            .none = false,
            .numFiles = 0,
            .newuncompressedfilename = NULL,
            .uqzipFileName = NULL,
            };
    // Skip over program name
    argc--;
    argv++;
    while (argv[0] && strncmp(argv[0], "--", 2) == 0) {
        // Current argument begins with "--"
        if (!strcmp(argv[0], parallelArg) && !params.parallel) {
            params.parallel = true;
        } else if (!strcmp(argv[0], outputArg) && !params.output) {
            params.newcompressedfilename = argv[1];
            argc--;
            argv++;
        } else if (!strcmp(argv[0], bzip2Arg) && !params.bzip2) {
            params.bzip2 = true;
        } else if (!strcmp(argv[0], gzipArg) && !params.gzip) {
            params.gzip = true;
        } else if (!strcmp(argv[0], zipArg) && !params.zip) {
            params.xz = true;
        } else if (!strcmp(argv[0], xzArg) && !params.xz) {
            params.bzip2 = true;
        } else if (!strcmp(argv[0], noneArg) && !params.none) {
            params.none = true;
        } else {
            usage_error();
        }
        // Skip one argument
        argc--;
        argv++;
    }
    // Set the argument to none if not specified
    if (!params.bzip2 && !params.gzip && !params.xz && !params.none) {
        params.none = true;
    }

    // All remaining arguments (if any) are filenames.
    params.numPasswordFiles = argc;
    params.passwordFilenames = argv;
    // Make sure filenames are not empty strings
    while (*argv) {
        if (!argv[0][0]) {
            usage_error();
        }
        argv++;
    }
    return params;
}

void usage_error(void) {
    if (params.extract) {
        printf("./uqzip --extract [--parallel] uqz-file");
    } else {
        printf("./uqzip [--parallel] [--output outputFileName ] [--bzip2|--gzip|--zip|--xz|--none] fileName ... ")
    }
    return exit(commandlineerror);
}



        FILE* file, UqzHeaderSection header, FileInfo fileinfo, char* zipfilename, int currentfile)
{
    bool parallel = fileinfo.parallel;
    char* filename = fileinfo.filename[currentfile];
    int filenamelength = fileinfo.filenamelength[currentfile];
    int datalength = fileinfo.datalength[currentfile];
    uint32_t offset = header.fileRecordOffsets[currentfile];
    fseek(file, offset + 5, SEEK_SET);
    char buffer[datalength + filenamelength];
    if (((int)fread(buffer, sizeof(char), filenamelength, file) != filenamelength) || (filenamelength <= 0)) {
            remove(filename);
            if (!parallel) {
        format_error(zipfilename);
            }
    }

    if ((int) fread(buffer, sizeof(char), datalength, file) != datalength) {
            remove(filename);
            if (!parallel) {
            format_error(zipfilename);
            }
    }


    
int format_check(
        FILE* file, UqzHeaderSection header, FileInfo fileinfo, char* zipfilename, int currentfile)
{
    bool parallel = fileinfo.parallel;
    char* filename = fileinfo.filename[currentfile];
    int filenamelength = fileinfo.filenamelength[currentfile];
    int datalength = fileinfo.datalength[currentfile];
    uint32_t offset = header.fileRecordOffsets[currentfile];
    fseek(file, offset + 5, SEEK_SET);
    char buffer[datalength + filenamelength];
    if (((int)fread(buffer, sizeof(char), filenamelength, file) != filenamelength) || (filenamelength <= 0)) {
            remove(filename);
            if (!parallel) {
        format_error(zipfilename);
            }
    }

    if ((int) fread(buffer, sizeof(char), datalength, file) != datalength) {
            remove(filename);
            if (!parallel) {
            format_error(zipfilename);
            }
    }
    int filefd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS);
    if (filefd == -1) {
        fprintf(stderr, "uqzip: can't write to file \"%s\"\n", filename);
        //remove(filename);
        if (!parallel) {
        exit(17);
        }

    }
    return filefd;


    int* get_file_ids(FILE* file, UqzHeaderSection header, FileInfo fileinfo, char* zipfilename) {
    int numfiles = header.numFiles;
    int* filefds = malloc(sizeof(int)*numfiles);
    for (int i = 0; i < numfiles; i++) {
        int filefd = format_check(file, header, fileinfo, zipfilename, i);
        filefds[i] = filefd;
    }
    return filefds;
}