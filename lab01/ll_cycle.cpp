#include "ll_cycle.h"

bool ll_has_cycle(node *head) {
    /* Ваш код должен быть написан только внутри этой функции */
    if(head == nullptr)
    return false;
    
    node* hare = head;
    node* turtle = head;
    while(true)
    {
        if(hare->next == nullptr)
        return false;
        hare = hare->next; 
        if(hare->next == nullptr)
        return false;
        hare = hare->next;
        turtle = turtle->next;
        if(hare == turtle)
        return true;
    }
}
