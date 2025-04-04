#include "mdio_file.hh"

void _sf_file_error(bool err) {
    fprintf(stdout, "sf_file error occurred!\n");
}

void chomp(std::string& str) {
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
}

extern "C" {

void mdio_sf_describe_file(const sf_file mdio_file) {
    if(mdio_file->ok){
        printf("mdio path (good): %s\n", mdio_file->mdio_path);
        printf("mdio path (good): %s\n", mdio_file->mdio_dataset_description);
    } else {
        printf("mdio path (bad): %s\n", mdio_file->mdio_path);        
    }    
}

void mdio_sf_file_error(bool err) {
    // demo wrapper ....
    _sf_file_error(err);

    if (err) {
        fprintf(stderr, "sf_file error occurred!\n");
        exit(1);
    }
}

sf_file mdio_sf_input(const char* path) {
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

sf_file mdio_sf_output(const char* tag) {
    (void)tag;
    return (sf_file)malloc(sizeof(struct sf_File));
}

sf_datatype mdio_sf_gettype(sf_file file) {
    return file ? file->type : SF_FLOAT;
}

sf_dataform mdio_sf_getform(sf_file file) {
    return file ? file->form : SF_NATIVE;
}

size_t mdio_sf_esize(sf_file file) {
    return file ? sizeof(float) : 0;
}

void mdio_sf_settype(sf_file file, sf_datatype type) {
    if (file) file->type = type;
}

void mdio_sf_setform(sf_file file, sf_dataform form) {
    if (file) file->form = form;
}

void mdio_sf_fileclose(sf_file file) {
    if (file) {
        free(file);
    }
}

void mdio_sf_fileclosedelete(sf_file file) {
    // TODO(BrianMichell): Add mdio::utils::Delete() here
    if (file) {
        free(file);
    }
}

bool mdio_sf_histint(sf_file file, const char* key, int* par) {
    (void)file; (void)key;
    if (par) *par = 42;
    return true;
}

bool mdio_sf_histfloat(sf_file file, const char* key, float* par) {
    (void)file; (void)key;
    if (par) *par = 3.14f;
    return true;
}

void mdio_sf_putint(sf_file file, const char* key, int par) {
    (void)file; (void)key; (void)par;
}

void mdio_sf_putfloat(sf_file file, const char* key, float par) {
    (void)file; (void)key; (void)par;
}

void mdio_sf_putstring(sf_file file, const char* key, const char* par) {
    (void)file; (void)key; (void)par;
}

void mdio_sf_fflush(sf_file file) {
    (void)file;
}

off_t mdio_sf_bytes(sf_file file) {
    return file ? 1024 : 0;
}

off_t mdio_sf_tell(sf_file file) {
    return file ? 256 : 0;
}

void mdio_sf_seek(sf_file file, off_t offset, int whence) {
    (void)file; (void)offset; (void)whence;
}

bool mdio_sf_endian(void) {
    int num = 1337;
    return (*(char*)&num == 1);
}

void mdio_sf_setpars(sf_file file) {
    sf_warning("sf_setpars is not supported with the MDIO backend");
}

void mdio_sf_expandpars(sf_file file) {
    sf_warning("sf_expandpars is not supported with the MDIO backend");
}

size_t mdio_sf_bufsiz(sf_file file) {
    return file ? 8192 : 0; // Default buffer size
}

void mdio_sf_setformat (sf_file file, const char* format)
/*< Set file format.
  ---
  format has a form "form_type", i.e. native_float, ascii_int, etc.
  >*/
{
    if (NULL != strstr(format,"float")) {
	    mdio_sf_settype(file,SF_FLOAT);
    } else if (NULL != strstr(format,"int")) {
	    mdio_sf_settype(file,SF_INT);
    } else if (NULL != strstr(format,"complex")) {
	    mdio_sf_settype(file,SF_COMPLEX);
    } else if (NULL != strstr(format,"uchar") || 
	       NULL != strstr(format,"byte")) {
	    mdio_sf_settype(file,SF_UCHAR);
    } else if (NULL != strstr(format,"short")) {
	    mdio_sf_settype(file,SF_SHORT);
    } else if (NULL != strstr(format,"long")) {
	    mdio_sf_settype(file,SF_LONG);
    } else if (NULL != strstr(format,"double")) {
	    mdio_sf_settype(file,SF_DOUBLE);
    } else {
	    mdio_sf_settype(file,SF_CHAR);
    }
	
    if (0 == strncmp(format,"ascii_",6)) {
        sf_warning("SF_ASCII is not supported with the MDIO backend. Nothing will happen.");
    } else if (0 == strncmp(format,"xdr_",4)) {
        sf_warning("SF_XDR is not supported with the MDIO backend. Nothing will happen.");
    } else {
        sf_setform(file,SF_NATIVE);
    }
}

bool mdio_sf_histints(sf_file file, const char* key, int* par, size_t n) {
    (void)file; (void)key; (void)n;
    if (par) {
        for (size_t i = 0; i < n; i++) {
            par[i] = 42;
        }
    }
    return true;
}

bool mdio_sf_histlargeint(sf_file file, const char* key, off_t* par) {
    (void)file; (void)key;
    if (par) *par = 42;
    return true;
}

bool mdio_sf_histdouble(sf_file file, const char* key, double* par) {
    (void)file; (void)key;
    if (par) *par = 3.14159265358979;
    return true;
}

bool mdio_sf_histfloats(sf_file file, const char* key, float* par, size_t n) {
    (void)file; (void)key; (void)n;
    if (par) {
        for (size_t i = 0; i < n; i++) {
            par[i] = 3.14f;
        }
    }
    return true;
}

bool mdio_sf_histbool(sf_file file, const char* key, bool* par) {
    (void)file; (void)key;
    if (par) *par = true;
    return true;
}

bool mdio_sf_histbools(sf_file file, const char* key, bool* par, size_t n) {
    (void)file; (void)key; (void)n;
    if (par) {
        for (size_t i = 0; i < n; i++) {
            par[i] = true;
        }
    }
    return true;
}

char* mdio_sf_histstring(sf_file file, const char* key) {
    (void)file; (void)key;
    return strdup("default_string");
}

void mdio_sf_fileflush(sf_file file, sf_file src) {
    (void)file; (void)src;
}

void mdio_sf_readwrite(sf_file file, bool flag) {
    sf_warning("sf_readwrite has no effect with the MDIO backend. This operation will result in a no-op.");
}

void mdio_sf_putints(sf_file file, const char* key, const int* par, size_t n) {
    // This appears to be indended for adding metadata to the header file and not the binary object.
    (void)file; (void)key; (void)par; (void)n;
}

void mdio_sf_putlargeint(sf_file file, const char* key, off_t par) {
    (void)file; (void)key; (void)par;
}

void mdio_sf_putfloats(sf_file file, const char* key, const float* par, size_t n) {
    (void)file; (void)key; (void)par; (void)n;
}

void mdio_sf_putline(sf_file file, const char* line) {
    (void)file; (void)line;
}

void mdio_sf_setaformat(const char* format, int line, int strip) {
    (void)format; (void)line; (void)strip;
}

void mdio_sf_complexwrite(sf_complex* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_complexread(sf_complex* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_charwrite(char* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_ucharwrite(unsigned char* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_charread(char* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

int mdio_sf_try_charread(const char* test, sf_file file) {
    (void)test; (void)file;
    return 0;
}

int mdio_sf_try_charread2(char* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
    return 0;
}

void mdio_sf_ucharread(unsigned char* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_intwrite(int* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_intread(int* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_shortread(short* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_longread(off_t* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_shortwrite(short* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_floatwrite(float* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

void mdio_sf_floatread(float* arr, size_t size, sf_file file) {
    (void)arr; (void)size; (void)file;
}

FILE* mdio_sf_tempfile(char** dataname, const char* mode) {
    (void)mode;
    if (dataname) *dataname = strdup("temp_file");
    return NULL;
}

FILE* mdio_sf_filestream(sf_file file) {
    return file ? file->stream : NULL;
}

void mdio_sf_unpipe(sf_file file, off_t size) {
    (void)file; (void)size;
}

void mdio_sf_close(void) {
    // Stub for closing all temporary files
}

sf_file mdio_sf_tmpfile(char* format) {
    (void)format;
    sf_File* new_file = new sf_File();
    new_file->ok = true;
    new_file->mdio_path = strdup("temp_file");
    new_file->mdio_dataset_description = strdup("temporary file");
    return new_file;
}

void mdio_sf_filefresh(sf_file file) {
    (void)file;
}

void mdio_sf_filecopy(sf_file file, sf_file src, sf_datatype type) {
    (void)file; (void)src; (void)type;
}

void mdio_sf_tmpfileclose(sf_file file) {
    if (file) {
        if (file->mdio_path) free(file->mdio_path);
        if (file->mdio_dataset_description) free(file->mdio_dataset_description);
        delete file;
    }
}

}  // extern "C"

