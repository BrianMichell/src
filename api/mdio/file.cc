#include "mdio_file.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {

struct sf_File {
    FILE* stream;
    sf_datatype type;
    sf_dataform form;
};

void sf_file_error(bool err) {
    if (err) {
        fprintf(stderr, "sf_file error occurred!\n");
        exit(1);
    }
}

sf_file sf_input(const char* tag) {
    (void)tag;
    return (sf_file)malloc(sizeof(struct sf_File));
}

sf_file sf_output(const char* tag) {
    (void)tag;
    return (sf_file)malloc(sizeof(struct sf_File));
}

sf_datatype sf_gettype(sf_file file) {
    return file ? file->type : SF_FLOAT;
}

sf_dataform sf_getform(sf_file file) {
    return file ? file->form : SF_NATIVE;
}

size_t sf_esize(sf_file file) {
    return file ? sizeof(float) : 0;
}

void sf_settype(sf_file file, sf_datatype type) {
    if (file) file->type = type;
}

void sf_setform(sf_file file, sf_dataform form) {
    if (file) file->form = form;
}

void sf_fileclose(sf_file file) {
    if (file) {
        free(file);
    }
}

void sf_fileclosedelete(sf_file file) {
    if (file) {
        free(file);
    }
}

bool sf_histint(sf_file file, const char* key, int* par) {
    (void)file; (void)key;
    if (par) *par = 42;
    return true;
}

bool sf_histfloat(sf_file file, const char* key, float* par) {
    (void)file; (void)key;
    if (par) *par = 3.14f;
    return true;
}

void sf_putint(sf_file file, const char* key, int par) {
    (void)file; (void)key; (void)par;
}

void sf_putfloat(sf_file file, const char* key, float par) {
    (void)file; (void)key; (void)par;
}

void sf_putstring(sf_file file, const char* key, const char* par) {
    (void)file; (void)key; (void)par;
}

void sf_fflush(sf_file file) {
    (void)file;
}

off_t sf_bytes(sf_file file) {
    return file ? 1024 : 0;
}

off_t sf_tell(sf_file file) {
    return file ? 256 : 0;
}

void sf_seek(sf_file file, off_t offset, int whence) {
    (void)file; (void)offset; (void)whence;
}

bool sf_endian(void) {
    int num = 1;
    return (*(char*)&num == 1);
}

} // extern "C"

