#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

static bool validate(struct list_head *q)
{
    struct list_head *node;
    list_for_each (node, q) {
        if (node->next == q)
            break;
        if (strcmp(list_entry(node, list_ele_t, list)->value,
                   list_entry(node->next, list_ele_t, list)->value) > 0)
            return false;
    }
    return true;
}

static struct list_head *q_new()
{
    struct list_head *q = malloc(sizeof(struct list_head));
    if (!q) return NULL;

    INIT_LIST_HEAD(q);
    return q;
}

static void q_free(struct list_head *q)
{
    if (!q) return;

    struct list_head *current = q->next;
    while (current != q) {
        list_ele_t *tmp = list_entry(current, list_ele_t, list);
        current = current->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

bool q_insert_head(struct list_head *q, char *s)
{
    if (!q) return false;

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    char *new_value = strdup(s);
    if (!new_value) {
        free(newh);
        return false;
    }

    newh->value = new_value;

    newh->list.next = q->next;
    q->next->prev = &(newh->list);
    newh->list.prev = q;
    q->next = &(newh->list);

    return true;
}

int main(void)
{
    FILE *fp = fopen("cities.txt", "r");
    if (!fp) {
        perror("failed to open cities.txt");
        exit(EXIT_FAILURE);
    }

    struct list_head *q = q_new();
    char buf[256];
    while (fgets(buf, 256, fp))
        q_insert_head(q, buf);
    fclose(fp);

    list_merge_sort(q);
    assert(validate(q));

    q_free(q);

    return 0;
}
