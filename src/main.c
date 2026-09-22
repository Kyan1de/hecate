/*
 * Kya Beaudry
 * Scratch pad
 */

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct _vtable vtable;

// standard fuction positions
enum funcidx {
    SpeakFunc,
    SayFunc,
};

struct _vtable {
    enum funcidx *defined; // (pointer to) array of defined functions, zero terminated.
    void (*funcarr[])(); // <- should have size of `elements` * sizeof(void(*)()) (function pointers)
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


void object_init(object* self);
void dog_init(dog* self);
void cat_init(cat* self);

// todo: make checked and unchecked forms of each interfacing function  
static enum funcidx object_defined[] = {SpeakFunc,SayFunc,0};
static const vtable object_vtable = {.defined = object_defined, .funcarr = {[SpeakFunc] = (void*)object_speak, [SayFunc] = (void*)object_say}};
static enum funcidx dog_defined[] = {SpeakFunc,SayFunc,0};
static const vtable dog_vtable = {.defined = dog_defined, .funcarr = {[SpeakFunc] = (void*)dog_speak, [SayFunc] = (void*)dog_say}};
static enum funcidx cat_defined[] = {SpeakFunc,SayFunc,0};
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

void speak(void* object) {
    (*(vtable**)object)->funcarr[SpeakFunc]();
}

void say(void* object, const char* input) {
    void(*f)(const char *);
    f = (void(*)(const char *))(*(vtable**)object)->funcarr[SayFunc];
    f(input);
}

int main(void) {
    
    object a;
    dog b;
    cat c;

    object_init(&a);
    dog_init(&b);
    cat_init(&c);

    speak(&a);
    speak(&b);
    speak(&c);

    say(&a, "Testing say");
    say(&b, "woof woof");
    say(&c, "mrreow");

    return 0;
}
