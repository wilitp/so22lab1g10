#include "queue.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct s_queue {
    unsigned int len;
    copy_fun_t copy_fun;
    struct s_node *last;
    struct s_node *first;
};

struct s_node {
    void *elem;
    struct s_node *next;
};

static struct s_node *create_node(void *e) {

    struct s_node *new_node = malloc(sizeof(struct s_node));
    assert(new_node != NULL);
    new_node->elem = e;
    new_node->next = NULL;
    return new_node;
}

static struct s_node *destroy_node(struct s_node *node) {
    node->next = NULL;
    free(node->elem);
    free(node);
    node = NULL;
    return node;
}

static bool invrep(queue q) {
    return q != NULL &&
           // first y last son los dos nulos o los dos no nulos
           ((q->first == NULL && q->last == NULL) ||
            (q->first != NULL && q->last != NULL));
}

queue queue_empty(copy_fun_t copy_fun) {
    assert(copy_fun != NULL);
    queue q = NULL;
    q = malloc(sizeof(struct s_queue));
    q->first = NULL;
    q->last = NULL;
    q->len = 0;
    assert(invrep(q) && queue_is_empty(q));
    return q;
}

queue queue_enqueue(queue q, void *e) {
    assert(invrep(q));
    struct s_node *new_node = create_node(e);
    if (q->first == NULL) {
        q->first = (q->last = new_node);
    } else {
        q->last->next = new_node;
        q->last = new_node;
    }

    ++q->len;
    assert(invrep(q));
    assert(!queue_is_empty(q));
    return q;
}

bool queue_is_empty(queue q) {
    assert(invrep(q));
    return q->first == NULL;
}

void *queue_first(queue q) {
    assert(invrep(q) && !queue_is_empty(q));
    return q->first->elem;
}
unsigned int queue_size(queue q) { return q->len; }

queue queue_dequeue(queue q) {
    assert(invrep(q) && !queue_is_empty(q));

    struct s_node *killme = q->first;
    q->first = q->first->next;
    // Si el ultimo es tambien el primero, tambien lo destruimos
    if (q->last == killme) {
        q->last = NULL;
    }
    killme = destroy_node(killme);

    --(q->len);
    assert(invrep(q));

    return q;
}

queue queue_copy(queue q) {
    queue copy = queue_empty(q->copy_fun);

    copy->len = q->len;

    if (q->len == 0) {
        copy->first = copy->last = NULL;
    } else {

        struct s_node *node = q->first;

        while (node != NULL) {

            // Nuevo nodo con el elemento copiado
            struct s_node *new_node =
                create_node((*(q->copy_fun))(q->first->elem));

            // Encolamos el nuevo nodo
            queue_enqueue(copy, new_node);
            if (node->next == NULL) {
                // Si es el ultimo nodo lo ponemos en last
                copy->last = new_node;
            }
        }
    }
    assert(invrep(copy));
    return copy;
}

queue queue_destroy(queue q) {
    assert(invrep(q));
    struct s_node *node = q->first;
    while (node != NULL) {
        struct s_node *killme = node;
        node = node->next;
        killme = destroy_node(killme);
    }
    free(q);
    q = NULL;
    assert(q == NULL);
    return q;
}
