#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

const double eps = 1e-10;
bool equals(double x, double y){
    return fabs(x - y) <= eps;
}

// [structures]
typedef struct point{
    double x, y;
} point;

typedef struct segment{
    point st, nd;
} segment;

typedef struct vector2d{
    double x, y;
} vector2d;

typedef struct rect{
    point bl, tr;
} rect;

typedef struct circle{
    point s;
    double r;
} circle;

typedef struct figure{
    bool type;
    rect r;
    circle c;
} figure;

typedef struct operation{
    figure *base;
    struct operation *parent;
    segment oper;
} operation;

// [functions]
figure make_rect(point *bl, point *tr){
    return (figure){1, (rect){*bl, *tr}, (circle){(point){0, 0}, 0}};
}

figure make_circle(point *s, double *r){
    return (figure){0, (rect){(point){0, 0}, (point){0, 0}}, (circle){*s, *r}};
}

double dist(point *a, point *b){
    return (a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
}

double cross_prod(vector2d *a, vector2d *b){
    return a->x * b->y - a->y * b->x;
}

// check on which side of the segment the point is
int side(point *p, segment *s){
    vector2d a = {s->nd.x - s->st.x, s->nd.y - s->st.y};
    vector2d b = {p->x - s->st.x, p->y - s->st.y};
    double val = cross_prod(&a, &b);
    if(equals(val, 0)) return 0;
    else if(val < 0) return -1;
    else return 1;
}

bool in_rect(rect *a, point *p){
    return (p->x >= a->bl.x - eps)
        && (p->x <= a->tr.x + eps)
        && (p->y >= a->bl.y - eps)
        && (p->y <= a->tr.y + eps);
}

bool in_circle(circle *c, point *p){
    return dist(&(c->s), p) <= c->r * c->r + eps;
}

// check if the point is inside the figure
bool is_in(figure *f, point *p){
    if(f->type) return in_rect(&(f->r), p);
    return in_circle(&(f->c), p);
}

// reflect the point
point reflect(point *p, segment *s){
	// edge cases
    if(equals(s->st.x, s->nd.x)) return (point){s->st.x * 2 - p->x, p->y};
    if(equals(s->st.y, s->nd.y)) return (point){p->x, s->st.y * 2 - p->y};
    // general case
    double a = (s->nd.y - s->st.y) / (s->nd.x - s->st.x);
    double b = s->st.y - a * s->st.x;
    double inv_a = -1 / a;
    double inv_b = p->y - inv_a * p->x;
    double x = (inv_b - b) / (a - inv_a);
    double y = a * x + b;
    vector2d v = (vector2d){p->x - x, p->y - y};
    return (point){x - v.x, y - v.y};
}

// recursive reverse folding simulation
int solve(operation *o, point *p){
    if(o->parent == NULL) return is_in(o->base, p);
    if(side(p, &(o->oper)) == -1) return 0;
    if(side(p, &(o->oper)) == 0) return solve(o->parent, p);
    point refl = reflect(p, &(o->oper));
    return solve(o->parent, p) + solve(o->parent, &refl);
}

int main()
{
    int n, q;
    if(scanf("%d%d", &n, &q)){};
    
    operation *operations = (operation*)malloc(sizeof(operation) * (size_t)n);
    
    // READ INPUT
    
    for(int i = 0; i < n; ++i){
        char type;
        if(scanf(" %c", &type)){};
        
        if(type == 'P'){
            point a, b;
            if(scanf("%lf%lf%lf%lf", &a.x, &a.y, &b.x, &b.y)){};
            figure *f = (figure*)malloc(sizeof(figure));
            *f = make_rect(&a, &b);
            operations[i] = (operation){f, NULL, (segment){(point){0, 0},(point){0, 0}}};
        }
        
        if(type == 'K'){
            point s;
            double r;
            if(scanf("%lf%lf%lf", &s.x, &s.y, &r)){};
            figure *f = (figure*)malloc(sizeof(figure));
            *f = make_circle(&s, &r);
            operations[i] = (operation){f, NULL, (segment){(point){0, 0},(point){0, 0}}};
        }
        
        if(type == 'Z'){
            int k;
            point a, b;
            if(scanf("%d%lf%lf%lf%lf", &k, &a.x, &a.y, &b.x, &b.y)){};
            operations[i] = (operation){NULL, &operations[k - 1], (segment){a, b}};
        }
    }
    
    // ANSWER QUERIES
    
    for(int i = 0; i < q; ++i){
        int k;
        point p;
        if(scanf("%d%lf%lf", &k, &p.x, &p.y)){};
        printf("%d\n", solve(&operations[k - 1], &p));
    }
}
