/*
 * Kya Beaudry
 * Scratch pad
 */

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

typedef struct _vtable vtable;
typedef enum _funcidx funcidx;

// standard fuction positions
enum _funcidx {
    SpeakFunc = 1,
    SayFunc,
    VariadicFunc,
};

struct _vtable {
    funcidx *defined; // (pointer to) array of defined functions, zero terminated.
    void (*funcarr[])(); // <- should have size of k * sizeof(void(*)()) (function pointers) where k is the highest implemented function index
};

/*
 * vtables are useful for dynamic/object oriented programming
 * for example:
 */

typedef struct _object object;
typedef struct _dog dog;
typedef struct _cat cat;

struct _object {
	const vtable* functions;
};

struct _dog {
	object super;
};

struct _cat {
	object super;
};


void object_speak(void) {printf("unimplemented\n");};
void dog_speak(void) {printf("woof!\n");};
void cat_speak(void) {printf("meow\n");};

void object_say(const char* str) {printf("%s\n", str);};
void cat_say(const char* str) {printf("the cat says \"%s\"\n", str);};
void dog_say(const char* str) {printf("the dog says \"%s\"\n", str);};

void dog_VA(int n, ...) {
    va_list args;
    va_start(args, n);
    for (int i = 0; i < n; i++)
        printf("%d ", va_arg(args, int));
    printf("\n");
    va_end(args);
}

void object_init(object* self);
void dog_init(dog* self);
void cat_init(cat* self);

// todo: make checked and unchecked forms of each interfacing function  
static funcidx object_defined[] = {SpeakFunc,SayFunc,0};
static const vtable object_vtable = {.defined = object_defined, .funcarr = {[SpeakFunc] = (void*)object_speak, [SayFunc] = (void*)object_say}};
static funcidx dog_defined[] = {SpeakFunc,SayFunc, VariadicFunc, 0};
static const vtable dog_vtable = {.defined = dog_defined, .funcarr = {[VariadicFunc] = (void*)dog_VA,[SpeakFunc] = (void*)dog_speak, [SayFunc] = (void*)dog_say}};
static funcidx cat_defined[] = {SpeakFunc,SayFunc,0};
static const vtable cat_vtable = {.defined = cat_defined, .funcarr = {[SpeakFunc] = (void*)cat_speak, [SayFunc] = (void*)cat_say}};

void object_init(object* self) {*self = (object){.functions = &object_vtable};}
void dog_init(dog* self) {*self = (dog){.super = (object){.functions = &dog_vtable}};}
void cat_init(cat* self) {*self = (cat){.super = (object){.functions = &cat_vtable}};}

object* new_object() {
    object* ret = malloc(sizeof(object));
    *ret = (object){.functions = &object_vtable};
    return ret;
}

dog* new_dog() {
    dog* ret = malloc(sizeof(dog));
    *ret = (dog){.super = {.functions = &dog_vtable}};
    return ret;
}

cat* new_cat() {
    cat* ret = malloc(sizeof(cat));
    *ret = (cat){.super = {.functions = &cat_vtable}};
    return ret;
}

// hate this syntax. Bad. why is the argument on the inside. gross. hate it.
void (*fetch(void* obj, funcidx func))() {
    int defined = 0;
    for (int ii = 0; (*(vtable**)obj)->defined[ii]!=0;ii++) if ((*(vtable**)obj)->defined[ii]==func) defined = 1;
    if (defined) {
        void (*f)() = (*(vtable**)obj)->funcarr[func];
        return f;
    }
    else {
        fprintf(stderr, "Called undedfined function on object\n");
        exit(-1);
    }
}

inline void (*fetch_unchecked(object* obj, funcidx func))() {
    return (*(vtable**)obj)->funcarr[func];
}

int main(void) {
    object a;
    dog b;
    cat c;

    object_init(&a);
    dog_init(&b);
    cat_init(&c);

    void (*f)(int, ...) = (void*)fetch(&b, VariadicFunc);
    f(2, 1, 2);

    // i dont even want to think about how id do variadics

    return 0;
}
