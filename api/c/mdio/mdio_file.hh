#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include <mdio/mdio.h> 

#include "../file.h"
#include "../error.h"
#include "../_sf_FileOps_table.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct sf_File* sf_file;

// Defines a Madagascar filetype
struct sf_File {
    FILE* stream;
    bool ok;
    char* mdio_path;
    char* mdio_dataset_description;
    sf_datatype type;
    sf_dataform form;
};

void mdio_sf_describe_file(const sf_file mdio_file);

void mdio_sf_file_error(bool err);

sf_file mdio_sf_input(const char* path);

sf_file mdio_sf_output(const char* tag);

sf_datatype mdio_sf_gettype(sf_file file);

sf_dataform mdio_sf_getform(sf_file file);

size_t mdio_sf_esize(sf_file file);

void mdio_sf_settype(sf_file file, sf_datatype type);

void mdio_sf_setform(sf_file file, sf_dataform form);

void mdio_sf_fileclose(sf_file file);

void mdio_sf_fileclosedelete(sf_file file);

bool mdio_sf_histint(sf_file file, const char* key, int* par);

bool mdio_sf_histfloat(sf_file file, const char* key, float* par);

void mdio_sf_putint(sf_file file, const char* key, int par);

void mdio_sf_putfloat(sf_file file, const char* key, float par);

void mdio_sf_putstring(sf_file file, const char* key, const char* par);

void mdio_sf_fflush(sf_file file);

off_t mdio_sf_bytes(sf_file file);

off_t mdio_sf_tell(sf_file file);

void mdio_sf_seek(sf_file file, off_t offset, int whence);

bool mdio_sf_endian(void);

void mdio_sf_setpars(sf_file file);

void mdio_sf_expandpars(sf_file file);

size_t mdio_sf_bufsiz(sf_file file);

void mdio_sf_setformat (sf_file file, const char* format);

bool mdio_sf_histints(sf_file file, const char* key, int* par, size_t n);

bool mdio_sf_histlargeint(sf_file file, const char* key, off_t* par);

bool mdio_sf_histdouble(sf_file file, const char* key, double* par);

bool mdio_sf_histfloats(sf_file file, const char* key, float* par, size_t n);

bool mdio_sf_histbool(sf_file file, const char* key, bool* par);

bool mdio_sf_histbools(sf_file file, const char* key, bool* par, size_t n);

char* mdio_sf_histstring(sf_file file, const char* key);

void mdio_sf_fileflush(sf_file file, sf_file src);

void mdio_sf_readwrite(sf_file file, bool flag);

void mdio_sf_putints(sf_file file, const char* key, const int* par, size_t n);

void mdio_sf_putlargeint(sf_file file, const char* key, off_t par);

void mdio_sf_putfloats(sf_file file, const char* key, const float* par, size_t n);

void mdio_sf_putline(sf_file file, const char* line);

void mdio_sf_setaformat(const char* format, int line, int strip);

void mdio_sf_complexwrite(sf_complex* arr, size_t size, sf_file file);

void mdio_sf_complexread(sf_complex* arr, size_t size, sf_file file);

void mdio_sf_charwrite(char* arr, size_t size, sf_file file);

void mdio_sf_ucharwrite(unsigned char* arr, size_t size, sf_file file);

void mdio_sf_charread(char* arr, size_t size, sf_file file);

int mdio_sf_try_charread(const char* test, sf_file file);

int mdio_sf_try_charread2(char* arr, size_t size, sf_file file);

void mdio_sf_ucharread(unsigned char* arr, size_t size, sf_file file);

void mdio_sf_intwrite(int* arr, size_t size, sf_file file);

void mdio_sf_intread(int* arr, size_t size, sf_file file);

void mdio_sf_shortread(short* arr, size_t size, sf_file file);

void mdio_sf_longread(off_t* arr, size_t size, sf_file file);

void mdio_sf_shortwrite(short* arr, size_t size, sf_file file);

void mdio_sf_floatwrite(float* arr, size_t size, sf_file file);

void mdio_sf_floatread(float* arr, size_t size, sf_file file);

FILE* mdio_sf_tempfile(char** dataname, const char* mode);

FILE* mdio_sf_filestream(sf_file file) ;

void mdio_sf_unpipe(sf_file file, off_t size);

void mdio_sf_close(void);

sf_file mdio_sf_tmpfile(char* format);

void mdio_sf_filefresh(sf_file file);

void mdio_sf_filecopy(sf_file file, sf_file src, sf_datatype type);

void mdio_sf_tmpfileclose(sf_file file);

static const sf_FileOps mdio_sf_ops = 
/* Initialize the global file operations table with the MDIO implementations */
{
    .input            = mdio_sf_input,
    .output           = mdio_sf_output,
    .describe_file    = mdio_sf_describe_file,
    .file_error       = mdio_sf_file_error,
    .gettype          = mdio_sf_gettype,
    .getform          = mdio_sf_getform,
    .esize            = mdio_sf_esize,
    .settype          = mdio_sf_settype,
    .setpars          = mdio_sf_setpars,
    .expandpars       = mdio_sf_expandpars,
    .bufsiz           = mdio_sf_bufsiz,
    .setform          = mdio_sf_setform,
    .setformat        = mdio_sf_setformat,
    .fileclose        = mdio_sf_fileclose,
    .fileclosedelete  = mdio_sf_fileclosedelete,
    .histint          = mdio_sf_histint,
    .histints         = mdio_sf_histints,
    .histlargeint     = mdio_sf_histlargeint,
    .histfloat        = mdio_sf_histfloat,
    .histdouble       = mdio_sf_histdouble,
    .histfloats       = mdio_sf_histfloats,
    .histbool         = mdio_sf_histbool,
    .histbools        = mdio_sf_histbools,
    .histstring       = mdio_sf_histstring,
    .fileflush        = mdio_sf_fileflush,
    .readwrite        = mdio_sf_readwrite,
    .fflush           = mdio_sf_fflush,
    .putint           = mdio_sf_putint,
    .putints          = mdio_sf_putints,
    .putlargeint      = mdio_sf_putlargeint,
    .putfloat         = mdio_sf_putfloat,
    .putfloats        = mdio_sf_putfloats,
    .putstring        = mdio_sf_putstring,
    .putline          = mdio_sf_putline,
    .setaformat       = mdio_sf_setaformat,
    .complexwrite     = mdio_sf_complexwrite,
    .complexread      = mdio_sf_complexread,
    .charwrite        = mdio_sf_charwrite,
    .ucharwrite       = mdio_sf_ucharwrite,
    .charread         = mdio_sf_charread,
    .try_charread     = mdio_sf_try_charread,
    .try_charread2    = mdio_sf_try_charread2,
    .ucharread        = mdio_sf_ucharread,
    .intwrite         = mdio_sf_intwrite,
    .intread          = mdio_sf_intread,
    .shortread        = mdio_sf_shortread,
    .longread         = mdio_sf_longread,
    .shortwrite       = mdio_sf_shortwrite,
    .floatwrite       = mdio_sf_floatwrite,
    .floatread        = mdio_sf_floatread,
    .bytes            = mdio_sf_bytes,
    .tell             = mdio_sf_tell,
    .tempfile         = mdio_sf_tempfile,
    .seek             = mdio_sf_seek,
    .filestream       = mdio_sf_filestream,
    .unpipe           = mdio_sf_unpipe,
    .close            = mdio_sf_close,
    .tmpfile          = mdio_sf_tmpfile,
    .filefresh        = mdio_sf_filefresh,
    .filecopy         = mdio_sf_filecopy,
    .tmpfileclose     = mdio_sf_tmpfileclose,
    .endian           = mdio_sf_endian
};

#ifdef __cplusplus
}  // extern "C"
#endif
