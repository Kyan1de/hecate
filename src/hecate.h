
#include<stdio.h>
#include<stdlib.h>


// this will just let you define the signature and return type in one line. 
#define VirtualFuncTypes(signature_t, return_t) \
    typedef signature_t;\
    typedef return_t

typedef enum _funcidx {
    no_func,
    #ifdef USER_FUNCTIONS
    USER_FUNCTIONS
    #endif
} funcidx;

VirtualFuncTypes(void(*no_func_t)(), void no_func_return_t);

typedef struct _vtable {
    funcidx *defined; // (pointer to) array of defined functions, zero terminated.
    void (*funcarr[])(); // <- should have size of k * sizeof(void(*)()) (function pointers) where k is the highest implemented function index
} vtable;

// default object struct, can be overidden if needed
#ifndef USER_OBJECT
typedef struct _object {
	const vtable* functions;
} object;
#endif


void (*fetch(void* obj, funcidx func))();
inline void (*fetch_unchecked(void* obj, funcidx func))();

#ifdef HEC_IMPL

// hate this syntax. Bad. why is the argument on the inside. gross. hate it.
void (*fetch(void* obj, funcidx func))() {
    int defined = 0;
    for (int ii = 0; (*(vtable**)obj)->defined[ii]!=no_func;ii++) if ((*(vtable**)obj)->defined[ii]==func) defined = 1;
    if (defined) {
        void (*f)() = (*(vtable**)obj)->funcarr[func];
        return f;
    }
    else {
        fprintf(stderr, "Called undefined function on object\n");
        exit(-1);
    }
}

inline void (*fetch_unchecked(void* obj, funcidx func))() {
    return (*(vtable**)obj)->funcarr[func];
}

#endif