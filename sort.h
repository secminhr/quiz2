#include <string.h>
#include "list.h"

typedef struct __element {
    char *value;
    struct __element *next;
    struct list_head list;
} list_ele_t;

static list_ele_t *get_middle(struct list_head *list)
{
    struct list_head *fast = list->next, *slow;
    list_for_each (slow, list) {
        if (fast->next == list || fast->next->next == list)
            break;
        fast = fast->next->next;
    }
    return list_entry(slow, list_ele_t, list);
}

static void list_merge(struct list_head *lhs,
                       struct list_head *rhs,
                       struct list_head *head)
{
    INIT_LIST_HEAD(head);
    if (list_empty(lhs)) {
        list_splice_tail(lhs, head);
        return;
    }
    if (list_empty(rhs)) {
        list_splice_tail(rhs, head);
        return;
    }

    while (!list_empty(lhs) && !list_empty(rhs)) {
        char *lv = list_entry(lhs->next, list_ele_t, list)->value;
        char *rv = list_entry(rhs->next, list_ele_t, list)->value;
        struct list_head *tmp = strcmp(lv, rv) <= 0 ? lhs->next : rhs->next;
        list_del(tmp);
        list_add_tail(tmp, head);
    }
    list_splice_tail(list_empty(lhs) ? rhs : lhs, head);
}

void list_merge_sort(struct list_head *q)
{
    if (list_is_singular(q))
        return;

    struct list_head left;
    struct list_head sorted;
    INIT_LIST_HEAD(&left);
    list_cut_position(&left, q, &get_middle(q)->list);
    list_merge_sort(&left);
    list_merge_sort(q);
    list_merge(&left, q, &sorted);
    INIT_LIST_HEAD(q);
    list_splice_tail(&sorted, q);
}
