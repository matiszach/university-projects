#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "ary.h"

const double eps = 1e-10;

bool iszero(double x){
	return fabs(x) < eps;
}

typedef struct inter{
	double a,b;
} inter;

inter make_inter(double a, double b){
	inter res;
	res.a = a;
	res.b = b;
	return res;
}

void swap_inters(inter *A, inter *B){
	inter temp = *A;
	*A = *B;
	*B = temp;
}

void swap_wartosc(wartosc *A, wartosc *B){
	wartosc temp = *A;
	*A = *B;
	*B = temp;
}

void print_wartosc(wartosc w){
	printf("[%f - %f] U [%f - %f]\n", w.a, w.b, w.c, w.d);
}

wartosc make_wartosc(double a, double b, double c, double d, bool separated, bool exact){
	wartosc res;
	res.a = a;
	res.b = b;
	res.c = c;
	res.d = d;
	res.separated = separated;
	res.exact = exact;
	return res;
}

bool equal(double x, double y) {
    return fabs(x - y) < eps;
}

wartosc merge(inter A, inter B){
	if(A.a > B.a) swap_inters(&A, &B);
	if(A.b < B.a && !equal(A.b, B.a)){
		return make_wartosc(A.a, A.b, B.a, B.b, 1, 0);
	}else{
		return make_wartosc(A.a, B.b, NAN, NAN, 0, 0);
	}
}

wartosc wartosc_dokladnosc(double x, double p){
	double l = (1 - p / 100) * x;
	double r = (1 + p / 100) * x;
	return make_wartosc(fmin(l, r), fmax(l, r), NAN, NAN, 0, 0);
}

wartosc wartosc_od_do(double x, double y){
	return make_wartosc(x, y, NAN, NAN, 0, 0);
}

wartosc wartosc_dokladna(double x){
	return make_wartosc(x, x, NAN, NAN, 0, 1);
}

bool in_wartosc(wartosc w, double x){
	if(isnan(w.a)) return 0;
	if((x > w.a || equal(x, w.a)) && (x < w.b || equal(x, w.b))) return 1;
	if(isnan(w.c)) return 0;
	if((x > w.c || equal(x, w.c)) && (x < w.d || equal(x, w.d))) return 1;
	return 0;
}

double min_wartosc(wartosc w){
	if(isnan(w.a)) return NAN;
	if(isinf(w.a)) return -HUGE_VAL;
	return w.a;
}

double max_wartosc(wartosc w){
	if(w.separated){
		if(isnan(w.d)) return NAN;
		if(isinf(w.d)) return HUGE_VAL;
		return w.d;
	}
	if(isnan(w.b)) return NAN;
	if(isinf(w.d)) return HUGE_VAL;
	return w.b;
}

double sr_wartosc(wartosc w){
	if(isnan(min_wartosc(w)) || isnan(max_wartosc(w))) return NAN;
	if(isinf(min_wartosc(w)) && isinf(max_wartosc(w))) return NAN;
	if(isinf(min_wartosc(w))) return -HUGE_VAL;
	if(isinf(max_wartosc(w))) return HUGE_VAL;
	return (min_wartosc(w) + max_wartosc(w)) / 2;
}

inter add(inter A, inter B){
	double l = A.a + B.a;
	double r = A.b + B.b;
	if(iszero(l)) l = 0.0;
	if(iszero(r)) r = -0.0;
	if(iszero(l) && iszero(r)) l = r = 0.0;
	return make_inter(l, r);
}

wartosc plus(wartosc a, wartosc b){
	if(isnan(a.a) || isnan(b.a)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(a.separated && b.separated){
		if(isnan(a.c) || isnan(b.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
		return make_wartosc(-HUGE_VAL, HUGE_VAL, NAN, NAN, 0, 0);
	}
	if(!a.separated && b.separated) swap_wartosc(&a, &b);
	inter ab_ab = add(make_inter(a.a, a.b), make_inter(b.a, b.b));
	if(a.separated){
		if(isnan(a.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
		inter cd_ab = add(make_inter(a.c, a.d), make_inter(b.a, b.b));
		return merge(ab_ab, cd_ab);
	}
	bool exact = a.exact & b.exact;
	return make_wartosc(ab_ab.a, ab_ab.b, NAN, NAN, 0, exact);
}

inter substract(inter A, inter B){
	double l = fmin(fmin(A.a - B.a, A.a - B.b), fmin(A.b - B.a, A.b - B.b));
	double r = fmax(fmax(A.a - B.a, A.a - B.b), fmax(A.b - B.a, A.b - B.b));
	if(iszero(l)) l = 0.0;
	if(iszero(r)) r = -0.0;
	if(iszero(l) && iszero(r)) l = r = 0.0;
	return make_inter(l, r);
}

wartosc minus(wartosc a, wartosc b){
	if(isnan(a.a) || isnan(b.a)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(a.separated && b.separated){
		if(isnan(a.c) || isnan(b.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
		return make_wartosc(-HUGE_VAL, HUGE_VAL, NAN, NAN, 0, 0);
	}
	if(!a.separated && b.separated) swap_wartosc(&a, &b);
	inter ab_ab = substract(make_inter(a.a, a.b), make_inter(b.a, b.b));
	if(a.separated){
		if(isnan(a.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
		inter cd_ab = substract(make_inter(a.c, a.d), make_inter(b.a, b.b));
		return merge(ab_ab, cd_ab);
	}
	bool exact = a.exact & b.exact;
	return make_wartosc(ab_ab.a, ab_ab.b, NAN, NAN, 0, exact);
}

inter multiply(inter A, inter B){
	double l = fmin(fmin(A.a * B.a, A.a * B.b), fmin(A.b * B.a, A.b * B.b));
	double r = fmax(fmax(A.a * B.a, A.a * B.b), fmax(A.b * B.a, A.b * B.b));
	if(iszero(l)) l = 0.0;
	if(iszero(r)) r = -0.0;
	if(iszero(l) && iszero(r)) l = r = 0.0;
	return make_inter(l, r);
}

void print_inter(inter x){
	printf("[%f - %f]\n", x.a, x.b);
}

wartosc merge4(inter A, inter B, inter C, inter D){
	if(A.a > B.a) swap_inters(&A, &B);
	if(B.a > C.a) swap_inters(&B, &C);
	if(C.a > D.a) swap_inters(&C, &D);
	if(A.a > B.a) swap_inters(&A, &B);
	if(B.a > C.a) swap_inters(&B, &C);
	if(A.a > B.a) swap_inters(&A, &B);
	inter l = A;
	inter r = make_inter(NAN, NAN);
	bool separated = 0;
	if(A.b > B.a || equal(A.b, B.a)){
		l.b = fmax(l.b, B.b);
	}else{
		separated = 1;
		r = B;
	}
	if(!separated && (B.b > C.a || equal(B.b, C.a))){
		l.b = fmax(l.b, C.b);
	}else{
		if(!separated) r = C;
		else r.b = fmax(r.b, C.b);
		separated = 1;
	}
	if(!separated && (C.b > D.a || equal(C.b, D.a))){
		l.b = fmax(l.b, D.b);
	}else{
		if(!separated) r = D;
		else r.b = fmax(r.b, D.b);
		separated = 1;
	}
	return make_wartosc(l.a, l.b, r.a, r.b, separated, 0);
}

wartosc razy(wartosc a, wartosc b){
	if(isnan(a.a) || isnan(b.a)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(a.separated && b.separated){
		if(isnan(a.c) || isnan(b.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
		inter ab_ab = multiply(make_inter(a.a, a.b), make_inter(b.a, b.b));
		inter ab_cd = multiply(make_inter(a.a, a.b), make_inter(b.c, b.d));
		inter cd_ab = multiply(make_inter(a.c, a.d), make_inter(b.a, b.b));
		inter cd_cd = multiply(make_inter(a.c, a.d), make_inter(b.c, b.d));
		return merge4(ab_ab, ab_cd, cd_ab, cd_cd);
	}
	if(!a.separated && b.separated) swap_wartosc(&a, &b);
	if(a.separated){
		if(isnan(a.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
		inter ab_ab = multiply(make_inter(a.a, a.b), make_inter(b.a, b.b));
		inter cd_ab = multiply(make_inter(a.c, a.d), make_inter(b.a, b.b));
		return merge(ab_ab, cd_ab);
	}
	if(a.exact && iszero(a.a)) return make_wartosc(0, 0, NAN, NAN, 0, 1);
	if(b.exact && iszero(b.a)) return make_wartosc(0, 0, NAN, NAN, 0, 1);
	inter res = multiply(make_inter(a.a, a.b), make_inter(b.a, b.b));
	bool exact = a.exact & b.exact;
	return make_wartosc(res.a, res.b, NAN, NAN, 0, exact);
}

inter division(inter A, inter B){
	inter res;
	res.a = fmin(fmin(A.a / B.a, A.a / B.b), fmin(A.b / B.a, A.b / B.b));
	res.b = fmax(fmax(A.a / B.a, A.a / B.b), fmax(A.b / B.a, A.b / B.b));
	if(iszero(res.a)) res.a = 0.0;
	if(iszero(res.b)) res.b = -0.0;
	if(iszero(res.a) && iszero(res.b)) res.a = res.b = 0.0;
	return res;
}

wartosc merge3(inter A, inter B, inter C){
	if(A.a > B.a) swap_inters(&A, &B);
	if(B.a > C.a) swap_inters(&B, &C);
	if(A.a > B.a) swap_inters(&A, &B);
	inter l = A;
	inter r = make_inter(NAN, NAN);
	bool separated = 0;
	if(A.b > B.a || equal(A.b, B.a)){
		l.b = fmax(l.b, B.b);
	}else{
		separated = 1;
		r = B;
	}
	if(!separated && (B.b > C.a || equal(B.b, C.a))){
		l.b = fmax(l.b, C.b);
	}else{
		if(!separated) r = C;
		else r.b = fmax(r.b, C.b);
		separated = 1;
	}
	return make_wartosc(l.a, l.b, r.a, r.b, separated, 0);
}

wartosc podzielic(wartosc a, wartosc b){
	if(isnan(a.a) || isnan(b.a)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(a.separated && isnan(a.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(b.separated && isnan(b.c)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(b.exact && iszero(b.a)) return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	if(a.exact && iszero(a.a)) return make_wartosc(0, 0, NAN, NAN, 0, 1);
	if(b.exact){
		double l = a.a / b.a;
		double r = a.b / b.a;
		if(l > r){
			double tmp = l;
			l = r;
			r = tmp;
		}
		return make_wartosc(l, r, NAN, NAN, 0, 0);
	}
	if(iszero(b.a) && iszero(b.b)){
		return make_wartosc(NAN, NAN, NAN, NAN, 0, 0);
	}
	if(!b.separated && b.a < 0 && b.b > 0){
		wartosc _b = make_wartosc(b.a, -0.0, +0.0, b.b, 1, 0);
		return podzielic(a, _b);
	}
	if(b.separated && b.a < 0 && b.b > 0){
		wartosc _b = make_wartosc(b.a, -0.0, +0.0, b.b, 1, 0);
		inter c = division(make_inter(a.a, a.b), make_inter(b.c, b.d));
		wartosc temp = podzielic(a, _b);
		if(!temp.separated) return merge(make_inter(temp.a, temp.b), c);
		return merge3(make_inter(temp.a, temp.b), make_inter(temp.c, temp.d), c);
	}
	if(b.separated && b.c < 0 && b.d > 0){
		wartosc _b = make_wartosc(b.c, -0.0, +0.0, b.d, 1, 0);
		inter c = division(make_inter(a.a, a.b), make_inter(b.a, b.b));
		wartosc temp = podzielic(a, _b);
		if(!temp.separated) return merge(make_inter(temp.a, temp.b), c);
		return merge3(make_inter(temp.a, temp.b), make_inter(temp.c, temp.d), c);
	}
	if(a.separated && b.separated){
		inter ab_ab = division(make_inter(a.a, a.b), make_inter(b.a, b.b));
		inter ab_cd = division(make_inter(a.a, a.b), make_inter(b.c, b.d));
		inter cd_ab = division(make_inter(a.c, a.d), make_inter(b.a, b.b));
		inter cd_cd = division(make_inter(a.c, a.d), make_inter(b.c, b.d));
		return merge4(ab_ab,ab_cd,cd_ab,cd_cd);
	}
	if(a.separated){
		inter ab_ab = division(make_inter(a.a, a.b), make_inter(b.a, b.b));
		inter cd_ab = division(make_inter(a.c, a.d), make_inter(b.a, b.b));
		return merge(ab_ab, cd_ab);
	}
	if(b.separated){
		inter ab_ab = division(make_inter(a.a, a.b), make_inter(b.a, b.b));
		inter ab_cd = division(make_inter(a.a, a.b), make_inter(b.c, b.d));
		return merge(ab_ab, ab_cd);
	}
	if(iszero(a.b)) a.b = -0.0;
	if(iszero(b.b)) b.b = -0.0;
	inter res = division(make_inter(a.a, a.b), make_inter(b.a, b.b));
	bool exact = a.exact & b.exact;
	return make_wartosc(res.a, res.b, NAN, NAN, 0, exact);
}
