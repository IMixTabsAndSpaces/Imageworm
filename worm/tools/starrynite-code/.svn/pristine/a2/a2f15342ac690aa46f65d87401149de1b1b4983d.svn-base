cdef extern from "struct_frame.h":
    ctypedef struct Frame "struct frame_t":
        int width

cdef extern from "stdbool.h":
    ctypedef struct bool "bool":
        pass
    cdef extern bool c_true "true"
    cdef extern bool c_false "false"
        
cdef extern from "run_starrynite.h":    
    cdef int c_run_starrynite "run_starrynite" (char *input_prefix,
        char *param_file_name, bool skip_svm_classifier)
    
def run_starrynite(input_prefix, param_file_name, skip_svm_classifier):
    cdef bool c_skip_svm_classifier
    if skip_svm_classifier:
        c_skip_svm_classifier = c_true
    else:
        c_skip_svm_classifier = c_false
    return c_run_starrynite(input_prefix, param_file_name,
        c_skip_svm_classifier)
        

cdef Frame x

x.width = 6

class Foo(object):
    def __init__(self, x):
        self.x = 10

#cdef extern class x.Bar(Foo):
#    pass

cdef class A:
    cdef public int x
    cdef int z

def h():
    print A.x

class B(A):
    def y(self):
        print self.x

def f(y):
    x.width = y

def g():
    return x.width

#cdef public struct frame "Wow":
#    pass
# %{
# #if HAVE_CONFIG_H
# #include <config.h>
# #endif
# 
# #include <stdbool.h>
# #include <stdio.h>
# 
# #include "frame_t.h"
# #include "movie_t.h"
# #include "parameter_t.h"
# #include "struct_nucleus.h"
# #include "partner_t.h"
# #include "tool_t.h"
# 
# #include "calibrate-noise.h"
# #include "cell-identity.h"
# #include "CleanUpTools.h"
# #include "error-codes.h"
# #include "FreeParameters.h"
# #include "identify-nuclei.h"
# #include "LoadParameterFile.h"
# #include "logfile.h"
# #include "noise-filter.h"
# #include "RemoveGhostNuclei.h"
# #include "report-lineage.h"
# #include "SetupMovie.h"
# #include "Trace.h"
# #include "types.h"
# #include "xalloc.h"
# %}
# 
# // C language and library functions
# 
# %include "stdint.i"
# 
# %include "cpointer.i"
# %pointer_class(uint32_t, uint32Ptr);
# %pointer_class(uint8_t, uint8Ptr);
# %pointer_class(int, intPtr);
# %pointer_class(FILE, filePtr);
# 
# %pointer_functions(uint32_t, uint32p);
# %pointer_functions(int *, intpp);
# %pointer_functions(uint8_t **, uint8ppp);
# 
# %include "cmalloc.i"
# %malloc(int)
# %free(int)
# 
# extern void free(void *ptr);
# extern FILE *fopen(const char *path, const char *mode); 
# extern FILE *fdopen(int fd, const char *mode); 
# extern int fclose(FILE *fp);
# 
# 
# // Types
# 
# %include "frame_t.h"
# %include "movie_t.h"
# %include "nucleus_t.h"
# %include "parameter_t.h"
# %include "partner_t.h"
# %include "tool_t.h"
# 
# 
# // calibrate-noise.h
# 
# %ignore SN__CALIBRATE_NOISE_H;
# %include "calibrate-noise.h"
# 
# 
# // cell-identity.h
# 
# %ignore SN__CELL_IDENTITY_H;
# %include "cell-identity.h"
# 
# 
# // CleanUpTools.h
# 
# %ignore SN__CLEANUPTOOLS_H;
# %include "CleanUpTools.h"
# 
# 
# // error-codes.h
# 
# %ignore SN__ERROR_CODES_H;
# %include "error-codes.h"
# 
# 
# // FreeParameters.h
# 
# %ignore SN__FREEPARAMETERS_H;
# %include "FreeParameters.h"
# 
# 
# // identify-nuclei.h
# 
# %ignore SN__IDENTIFY_NUCLEI_H;
# %include "identify-nuclei.h"
# 
# 
# // LoadParameterFile.h
# 
# %ignore SN__LOADPARAMETERFILE_H;
# %include "LoadParameterFile.h"
# 
# 
# // logfile.h
# 
# %ignore SN__LOGFILE_H;
# %include "logfile.h"
# 
# 
# // noise-filter.h
# 
# %ignore SN__NOISE_FILTER_H;
# %include "noise-filter.h"
# 
# 
# // RemoveGhostNuclei.h
# 
# %ignore SN__REMOVEGHOSTNUCLEI_H;
# %include "RemoveGhostNuclei.h"
# 
# 
# // report-lineage.h
# 
# %ignore SN__REPORT_LINEAGE_H;
# %include "report-lineage.h"
# 
# 
# // SetupMovie.h
# 
# %ignore SN__SETUPMOVIE_H;
# %include "SetupMovie.h"
# 
# 
# // Trace.h
# 
# %ignore SN__TRACE_H;
# %include "Trace.h"
# 
# 
# // types.h
# 
# %ignore SN__TYPES_H;
# %ignore PARTNER_t;
# %ignore NUCLEUS_t;
# %ignore NUCLEUS_IDENTITY_SIZE;
# %ignore PARAMETER_t_movie;
# %include "types.h"
# 
# 
# // xalloc.h
# 
# %ignore SN__XALLOC_H;
# %include "xalloc.h"
