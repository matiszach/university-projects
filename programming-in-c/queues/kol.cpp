#include "kol.h"
#include <cstdlib>
#include <vector>
#include <cassert>

// list implementation

struct list{
    interesant *head, *tail;
};

list create_list(){
    interesant* head = (interesant*)malloc(sizeof(interesant));
    interesant* tail = (interesant*)malloc(sizeof(interesant));
    list res = {head, tail};
    res.head->next = res.tail;
    res.head->prev = NULL;
    res.tail->next = NULL;
    res.tail->prev = res.head;
    return res;
}

bool is_empty(list &l){
    return l.head->next == l.tail;
}

interesant* front(list &l){
    return l.head->next;
}

interesant* back(list &l){
    return l.tail->prev;
}

void link(interesant *i, interesant* old, interesant* nw){
    if(i){
        if(i->prev == old) i->prev = nw;
        else i->next = nw;
    }
}

void push_front(list &l, interesant* i){
    i->prev = l.head;
    i->next = l.head->next;
    link(l.head->next, l.head, i);
    l.head->next = i;
}

void push_back(list &l, interesant* i){
    i->next = l.tail;
    i->prev = l.tail->prev;
    link(l.tail->prev, l.tail, i);
    l.tail->prev = i;
}

void remove(interesant* i){
    link(i->prev, i, i->next);
    link(i->next, i, i->prev);
}

void pop_front(list &l){
    if(!is_empty(l)) remove(front(l));
}

void pop_back(list &l){
    if(!is_empty(l)) remove(back(l));
}

void reverse(list &l){
    std::swap(l.head, l.tail);
    std::swap(l.head->prev, l.head->next);
    std::swap(l.tail->prev, l.tail->next);
}

void merge(list &l1, list &l2){
    interesant* b = l1.head;
    interesant* f = l2.tail;
    link(b->next, b, f->prev);
    link(f->prev, f, b->next);
    free(b);
    free(f);
    l2.tail = l1.tail;
}

// struct for holding queues

struct anchor{
    list lst;
};

std::vector<anchor> queues;

// program functions

void otwarcie_urzedu(int m){
    queues.resize(m);
    for(int i = 0; i < m; ++i){
        queues[i] = {create_list()};
    }
}

interesant *nowy_interesant(int k){
    static int curr_nr = -1;
    interesant* tmp = (interesant*)malloc(sizeof(interesant));
    tmp->nr = ++curr_nr;
    push_back(queues[k].lst, tmp);
    return tmp;
}

int numerek(interesant* i){
    return i->nr;
}

interesant *obsluz(int k){
    if(is_empty(queues[k].lst)) return NULL;
    interesant* tmp = front(queues[k].lst);
    pop_front(queues[k].lst);
    return tmp;
}

void zmiana_okienka(interesant *i, int k){
    remove(i);
    push_back(queues[k].lst, i);
}

void zamkniecie_okienka(int k1, int k2){
    if(is_empty(queues[k1].lst)) return;
    merge(queues[k1].lst, queues[k2].lst);
    queues[k1].lst = create_list();
}

std::vector<interesant*> fast_track(interesant* i1, interesant* i2){
    std::vector<interesant*> candL = {i1}, candR = {i1};
    interesant *l = i1, *r = i1;
    interesant *preL = i1, *preR = i1;
    while(l != i2 && r != i2){
        //move l
        if(l){
            if(l->prev != preL){
                preL = l;
                l = l->prev;
            }
            else{
                preL = l;
                l = l->next;
            }
            candL.push_back(l);
        }
        //move r
        if(r){
            if(r->next != preR){
                preR = r;
                r = r->next;
            }
            else{
                preR = r;
                r = r->prev;
            }
            candR.push_back(r);
        }
    }
    if(l == i2){
        if(preL != l->prev){
            link(l->prev, l, i1->next);
            link(i1->next, i1, l->prev);
        }
        else{
            link(l->next, l, i1->next);
            link(i1->next, i1, l->next);
        }
        return candL;
    }
    else{
        if(preR != r->prev){
            link(r->prev, r, i1->prev);
            link(i1->prev, i1, r->prev);
        }
        else{
            link(r->next, r, i1->prev);
            link(i1->prev, i1, r->next);
        }
        return candR;
    }
}

void naczelnik(int k){
    reverse(queues[k].lst);
}

std::vector<interesant*> zamkniecie_urzedu(){
    std::vector<interesant*> res;
    for(int i = 0; i < (int)queues.size(); ++i){
        interesant* curr = queues[i].lst.head;
        interesant* pre = NULL;
        while(curr != queues[i].lst.tail){
            if(curr->next != pre){
                pre = curr;
                curr = curr->next;
            }
            else{
                pre = curr;
                curr = curr->prev;
            }
            if(curr != queues[i].lst.tail) res.push_back(curr);
        }
        free(queues[i].lst.head);
        free(queues[i].lst.tail);
    }
    return res;
}
