#ifndef SENEGAL_SVM_H
#define SENEGAL_SVM_H

#include "sinstructions.h"
#include "stable.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef enum {
    OK,
    COMPILE_TIME_ERROR,
    RUNTIME_ERROR
} InterpretationResult;

typedef enum {
    CONSTRUCTOR,
    TYPE_FUNCTION,
    METHOD,
    PROGRAM
} FunctionType;

typedef struct {
    GCObject gc;
    int arity;
    Instructions instructions;
    GCString* id;

    int upvalueCount;
} GCFunction;

typedef struct sVM VM;

typedef Constant (*NativeFunc)(VM* vm, int arity, Constant* args);

typedef struct {
    uint8_t index;
    bool isLocal;
} Upvalue;

typedef struct {
    GCObject gc;
    NativeFunc function;
} GCNative;

typedef struct GCUpvalue {
    GCObject gc;

    Constant* place;
    Constant closed;

    struct GCUpvalue* next;
} GCUpvalue;

typedef struct {
    GCObject gc;
    GCFunction* function;

    GCUpvalue** upvalues;
    int upvalueCount;
} GCClosure;

typedef struct {
    GCObject gc;
    GCString* id;

    bool isFinal;
    bool isStrict;

    // TODO: support adding class fields from within class body
    Table fields;

    Table methods;
} GCClass;

typedef struct {
    GCObject gc;
    GCClass* class;
    Table fields;
} GCInstance;

typedef struct {
    GCObject gc;
    Constant receiver;
    GCClosure* method;
} GCInstanceMethod;

#define IS_CLASS(c) isGCType(c, GC_CLASS)
#define IS_FUNCTION(c) isGCType(c, GC_FUNCTION)
#define IS_INSTANCE(c) isGCType(c, GC_INSTANCE)
#define IS_INSTANCE_METHOD(c) isGCType(c, GC_INSTANCE_METHOD)
#define IS_NATIVE(c) isGCType(c, GC_NATIVE)
#define IS_CLOSURE(c) isGCType(c, GC_CLOSURE)

#define AS_CLASS(c) ((GCClass*)AS_GC_OBJ(c))
#define AS_FUNCTION(c) ((GCFunction*)AS_GC_OBJ(c))
#define AS_INSTANCE(c) ((GCInstance*)AS_GC_OBJ(c))
#define AS_INSTANCE_METHOD(c) ((GCInstanceMethod*)AS_GC_OBJ(c))
#define AS_NATIVE(c) (((GCNative*)AS_GC_OBJ(c))->function)
#define AS_CLOSURE(c) (((GCClosure*)AS_GC_OBJ(c)))

typedef struct {
    GCClosure* closure;
    uint8_t* pc;
    Constant* constants;
} CallFrame;

struct sVM {
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    Constant stack[STACK_MAX];
    Constant* stackTop;

    size_t bytesAllocated;
    size_t nextGC;

    GCObject* gcObjects;

    Table globals;
    Table strings;

    GCUpvalue* openUpvalues;

    int grayCount;
    int grayCapacity;
    GCObject** grayStack;

    // Class method names
    GCString* constructString;

    GCClass* boolClass;
    GCClass* stringClass;
    GCClass* numClass;
};


void initVM(VM* vm);

InterpretationResult interpret(VM* vm, const char* source);

void push(VM* vm, Constant constant);
Constant pop(VM* vm);

GCClass* newClass(VM* vm, GCString* id, bool isFinal, bool isStrict);
GCFunction* newFunction(VM* vm);
GCInstance* newInstance(VM* vm, GCClass* class);
GCInstanceMethod* newInstanceMethod(VM* vm, Constant receiver, GCClosure* method);
GCNative* newNative(VM* vm, NativeFunc function);
GCClosure* newClosure(VM* vm, GCFunction* function);
GCUpvalue* newUpvalue(VM* vm, Constant* constant);

#endif //SENEGAL_SVM_H