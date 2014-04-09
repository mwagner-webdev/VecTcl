/* to be included with CMD and OP defined
 * it defines an elementwise binary operator
 * which works by iterating over all elements
 * for compatible operands */
#ifndef BINOP_LOOP
typedef int (binop_loop_fun) (Tcl_Interp *interp, Tcl_Obj *naObj1, Tcl_Obj *naObj2, Tcl_Obj **resultObj);
#define BINOP_LOOP_FUN(C, T1, T2) BINOP_LOOP_FUN1(C, T1, T2)
#define BINOP_LOOP_FUN1(C, T1, T2) C##_loop_##T1##_##T2
#define DECLARE_BINOP(T1, T2) static binop_loop_fun BINOP_LOOP_FUN(CMD, T1, T2)
#define BINOP_LOOP
#endif

#define LOOPTBL LOOPTABLE_IMP1(CMD)
#define LOOPTABLE_IMP1(C) LOOPTABLE_IMP2(C)
#define LOOPTABLE_IMP2(C) C##_table

DECLARE_BINOP(int, int);
DECLARE_BINOP(int, double);
DECLARE_BINOP(int, NumArray_Complex);
DECLARE_BINOP(double, int);
DECLARE_BINOP(double, double);
DECLARE_BINOP(double, NumArray_Complex);
DECLARE_BINOP(NumArray_Complex, int);
DECLARE_BINOP(NumArray_Complex, double);
DECLARE_BINOP(NumArray_Complex, NumArray_Complex);


static binop_loop_fun * LOOPTBL[3][3] = {
	{ BINOP_LOOP_FUN(CMD, int, int), BINOP_LOOP_FUN(CMD, int, double), BINOP_LOOP_FUN(CMD, int, NumArray_Complex)},
	{ BINOP_LOOP_FUN(CMD, double, int), BINOP_LOOP_FUN(CMD, double, double), BINOP_LOOP_FUN(CMD, double, NumArray_Complex),},
	{ BINOP_LOOP_FUN(CMD, NumArray_Complex, int), BINOP_LOOP_FUN(CMD, NumArray_Complex, double), BINOP_LOOP_FUN(CMD, NumArray_Complex, NumArray_Complex),},
	
};


int CMD( 
		ClientData dummy,
		Tcl_Interp *interp,
		int objc,
		Tcl_Obj *const *objv)
{	
	Tcl_Obj *naObj1, *naObj2, *resultObj;
	NumArrayInfo *info1, *info2;

	if (objc != 3) {
		Tcl_WrongNumArgs(interp, 1, objv, "numarray1 numarray2");
		return TCL_ERROR;
	}
	
    naObj1 = objv[1];
	naObj2 = objv[2];
	
	if (Tcl_ConvertToType(interp, naObj1, &NumArrayTclType) != TCL_OK) {
		return TCL_ERROR;
	}

	if (Tcl_ConvertToType(interp, naObj2, &NumArrayTclType) != TCL_OK) {
		return TCL_ERROR;
	}
	
	info1 = naObj1->internalRep.twoPtrValue.ptr2;
	info2 = naObj2->internalRep.twoPtrValue.ptr2;

	if (LOOPTBL[info1->type][info2->type](interp, naObj1, naObj2, &resultObj) != TCL_OK) {
		return TCL_ERROR;
	}
	
	Tcl_SetObjResult(interp, resultObj);

	return TCL_OK;
}

/* Implement the inner loop for the binary operators 
 * for all datatypes */

/* (int,int) -> int */
#ifdef INTRES
	#define TRES INTRES
#else
	#define TRES int
#endif
#define T1 int
#define T2 int

#ifdef OPINT
#define OP OPINT
#endif

#include "binop_loop.h"


/* (int,double) -> double */
#ifdef DBLRES
	#define TRES DBLRES
#else
	#define TRES double
#endif
#define T1 int
#define T2 double
#ifdef OPDBL
	#define OP OPDBL
#endif
#include "binop_loop.h"

/* (double, int) -> double */
#ifdef DBLRES
	#define TRES DBLRES
#else
	#define TRES double
#endif
#define T1 double
#define T2 int
#ifdef OPDBL
#define OP OPDBL
#endif
#include "binop_loop.h"

/* (double, double) -> double */
#ifdef DBLRES
	#define TRES DBLRES
#else
	#define TRES double
#endif
#define T1 double
#define T2 double
#ifdef OPDBL
#define OP OPDBL
#endif
#include "binop_loop.h"

/* (int, complex) -> complex */
#ifdef CPLXRES
	#define TRES CPLXRES
#else
	#define TRES NumArray_Complex
#endif
#define T1 int
#define T2 NumArray_Complex
#ifdef OPCPLX
#define OP OPCPLX
#endif
#include "binop_loop.h"

/* (double, complex) -> complex */
#ifdef CPLXRES
	#define TRES CPLXRES
#else
	#define TRES NumArray_Complex
#endif
#define T1 double
#define T2 NumArray_Complex
#ifdef OPCPLX
#define OP OPCPLX
#endif
#include "binop_loop.h"

/* (int, complex) -> complex */
#ifdef CPLXRES
	#define TRES CPLXRES
#else
	#define TRES NumArray_Complex
#endif
#define T1 NumArray_Complex
#define T2 int
#ifdef OPCPLX
#define OP OPCPLX
#endif
#include "binop_loop.h"

/* (double, complex) -> complex */
#ifdef CPLXRES
	#define TRES CPLXRES
#else
	#define TRES NumArray_Complex
#endif
#define T1 NumArray_Complex
#define T2 double
#ifdef OPCPLX
#define OP OPCPLX
#endif
#include "binop_loop.h"

/* (complex, complex) -> complex */
#ifdef CPLXRES
	#define TRES CPLXRES
#else
	#define TRES NumArray_Complex
#endif
#define T1 NumArray_Complex
#define T2 NumArray_Complex
#ifdef OPCPLX
#define OP OPCPLX
#endif
#include "binop_loop.h"


#undef GETOP1
#undef GETOP2
#undef GETOP_IMP

#undef CMD
#undef OP
#undef OPINT
#undef OPDBL
#undef OPCPLX
#undef INTRES
#undef DBLRES
#undef CPLXRES
