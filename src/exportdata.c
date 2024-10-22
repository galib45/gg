#include <stdio.h>
#include <raylib.h>

int main(int argc, char **argv) {
    char *program_name = argv[0];
    if (argc < 2) {
        fprintf(stderr, "[ERROR] No input file provided.\n");
        return -1;
    }
    int dataSize;
    char *filepath = argv[1];
    if (!FileExists(filepath)) {
        fprintf(stderr, "[ERROR] Input file %s doesn't exist.\n", filepath);
        return -1;
    }
    const char *filename = GetFileNameWithoutExt(filepath);
    printf("%s\n", filename);
    unsigned char *fileData = LoadFileData(filepath, &dataSize);
    if (!ExportDataAsCode(fileData, dataSize, TextFormat("%s.h", filename))) {
        printf("Could not export data as code\n");
        return -1;
    }
    return 0;
}
