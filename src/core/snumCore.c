#include <stdio.h>
#include <float.h>
#include "snumCore.h"
#include "../includes/sparser.h"
#include "../includes/sapi.h"

#define NAN (0.0 / 0.0)
#define INFINITY (1.0 / 0.0)
#define NEGATIVE_INFINITY (-1.0 / 0.0)
#define MAX_FINITE 1.7976931348623157e+308
#define MIN_POSITIVE 5e-324

static Constant numIsFinite(VM* vm, int arity, Constant* args) {
  register double num = AS_NUMBER(args[0]);
  return BOOL_CONST(num != INFINITY && num != NEGATIVE_INFINITY && num != NAN);
}

static Constant numIsInfinite(VM* vm, int arity, Constant* args) {
  register double num = AS_NUMBER(args[0]);
  return BOOL_CONST(num == INFINITY ||num == NEGATIVE_INFINITY);
}

static Constant numToString(VM* vm, int arity, Constant* args) {

  char numString[3 + DBL_MANT_DIG - DBL_MIN_EXP];

  sprintf(numString, "%lf", AS_NUMBER(args[0]));

  return GC_OBJ_CONST(copyString(vm, NULL, numString, strlen(numString)));
}

static Constant numIsNan(VM* vm, int arity, Constant* args) {
  return BOOL_CONST(AS_NUMBER(args[0]) == NAN);
}

static Constant numIsNeg(VM* vm, int arity, Constant* args) {
  return BOOL_CONST(AS_NUMBER(args[0]) < 0);
}

void initNumClass(VM *vm) {
  vm->numClass = newClass(vm, copyString(vm, NULL, "num", 3), false, false);
  defineClassNativeFunc(vm, "isFinite", numIsFinite, vm->numClass);
  defineClassNativeFunc(vm, "isInfinite", numIsInfinite, vm->numClass);
  defineClassNativeFunc(vm, "toString", numToString, vm->numClass);
  defineClassNativeFunc(vm, "isNaN", numIsNan, vm->numClass);
  defineClassNativeFunc(vm, "isNeg", numIsNeg, vm->numClass);

  defineClassNativeField(vm, "type", GC_OBJ_CONST(copyString(vm, NULL, "num", 3)), vm->numClass);
  defineClassNativeField(vm, "nan", NUM_CONST(NAN), vm->numClass);
  defineClassNativeField(vm, "infinity", NUM_CONST(INFINITY), vm->numClass);
  defineClassNativeField(vm, "negInfinity", NUM_CONST(NEGATIVE_INFINITY), vm->numClass);
  defineClassNativeField(vm, "maxFinite", NUM_CONST(MAX_FINITE), vm->numClass);
  defineClassNativeField(vm, "minPositive", NUM_CONST(MIN_POSITIVE), vm->numClass);
}
