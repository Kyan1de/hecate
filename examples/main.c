
#include<stdint.h>
#include<stdio.h>
#include<stdarg.h>

#define HEC_IMPL
#define USER_FUNCTIONS SpeakFunc,SayFunc,VariadicFunc

#include"hecate.h"

/*
 * vtables are useful for dynamic/object oriented programming
 * for example:
 */

VirtualFuncTypes(void(*SpeakFunc_t)(), void SpeakFunc_return_t);
VirtualFuncTypes(void(*SayFunc_t)(const char*), void SayFunc_return_t);
VirtualFuncTypes(void(*VariadicFunc_t)(int, ...), void VariadicFunc_return_t);

typedef struct _dog {
	object super;
} dog;

typedef struct _cat {
	object super;
} cat;


void dog_speak(void) {printf("woof!\n");};
void cat_speak(void) {printf("meow\n");};

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

static funcidx dog_defined[] = {SpeakFunc,SayFunc, VariadicFunc, 0};
static const vtable dog_vtable = {.defined = dog_defined, .funcarr = {[VariadicFunc] = (void*)dog_VA,[SpeakFunc] = (void*)dog_speak, [SayFunc] = (void*)dog_say}};
static funcidx cat_defined[] = {SpeakFunc,SayFunc,0};
static const vtable cat_vtable = {.defined = cat_defined, .funcarr = {[SpeakFunc] = (void*)cat_speak, [SayFunc] = (void*)cat_say}};

void dog_init(dog* self) {*self = (dog){.super = (object){.functions = &dog_vtable}};}
void cat_init(cat* self) {*self = (cat){.super = (object){.functions = &cat_vtable}};}

int main(void) {
    dog a;
    cat b;

    dog_init(&a);
    cat_init(&b);

    VariadicFunc_t f = (void*)fetch(&a, VariadicFunc);
    f(2, 1, 2);

    SpeakFunc_t g = (void*)fetch(&b, SpeakFunc);
    g();

    return 0;
}
