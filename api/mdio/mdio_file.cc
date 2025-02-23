
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include <mdio/mdio.h> 


void _sf_file_error(bool err) {
    fprintf(stdout, "sf_file error occurred!\n");
}

void chomp(std::string& str) {
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
}

extern "C" {

#include "../c/file.h"

// Defines a Madagascar filetype
struct sf_File {
    FILE* stream;
    bool ok;
    char* mdio_path;
    char* mdio_dataset_description;
    sf_datatype type;
    sf_dataform form;
};


void sf_describe_file(const sf_File* const mdio_file) {
    if(mdio_file->ok){
        printf("mdio path (good): %s\n", mdio_file->mdio_path);
        printf("mdio path (good): %s\n", mdio_file->mdio_dataset_description);
    } else {
        printf("mdio path (bad): %s\n", mdio_file->mdio_path);        
    }    
}

void sf_file_error(bool err) {
    // demo wrapper ....
    _sf_file_error(err);

    if (err) {
        fprintf(stderr, "sf_file error occurred!\n");
        exit(1);
    }
}

sf_file sf_input(const char* path) {
    std::string _path(path);
    // remove new line chars
    chomp(_path);

    auto mdio_dataset = mdio::Dataset::Open(
        std::string(_path), mdio::constants::kOpen
    ).result();
    
    // ... don't judge me ...
    sf_File* new_file = new sf_File();

    new_file->ok = mdio_dataset.ok();
    new_file->mdio_path = strdup(path);

    if(!mdio_dataset.ok()){
        std::cout << mdio_dataset.status() << std::endl;
        return new_file;
    }

    std::ostringstream oss;
    oss << mdio_dataset.value();
    new_file->mdio_dataset_description = strdup(oss.str().c_str());

    return new_file;
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
    int num = 1337;
    return (*(char*)&num == 1);
}

} // extern "C"

