#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TSoldier {
    struct TSoldier* m_Next;
    int              m_PersonalID;
    char             m_SecretRecord[64];
} TSoldier;

// Merges two singly-linked lists by interleaving their nodes.
// Nodes from p1 and p2 alternate in the result, starting with p1.
// If one list is shorter, the remaining nodes of the longer list are appended.
TSoldier* mergePlatoons(TSoldier* p1, TSoldier* p2) {
    TSoldier* merged  = NULL;
    TSoldier* current = NULL;

    if (p1 && p2) {
        merged  = p1;  p1 = p1->m_Next;
        current = merged;
        current->m_Next = p2;  p2 = p2->m_Next;
        current = current->m_Next;
    } else if (p1) {
        merged = p1;  p1 = p1->m_Next;
        current = merged;
    } else if (p2) {
        merged = p2;  p2 = p2->m_Next;
        current = merged;
    } else {
        return NULL;
    }

    while (p1 || p2) {
        if (p1) { current->m_Next = p1;  current = current->m_Next;  p1 = p1->m_Next; }
        if (p2) { current->m_Next = p2;  current = current->m_Next;  p2 = p2->m_Next; }
    }

    return merged;
}

// Splits src into two roughly equal halves stored in *p1 and *p2.
// If src has fewer than 2 nodes, both outputs are set to NULL.
// For odd-length lists, the last node is discarded.
void splitPlatoon(TSoldier* src, TSoldier** p1, TSoldier** p2) {
    *p1 = NULL;
    *p2 = NULL;
    if (!src) return;

    int len = 0;
    TSoldier* current = src;
    while (current) { current = current->m_Next; len++; }
    if (len == 1) return;

    *p1 = src;
    TSoldier* node = src;
    for (int i = 0; i < len / 2 - 1; i++)
        node = node->m_Next;

    *p2 = node->m_Next;
    node->m_Next = NULL;

    // Discard the trailing node when the total length is odd
    if (len % 2 != 0) {
        node = *p2;
        for (int i = 0; i < len / 2 - 1; i++)
            node = node->m_Next;
        free(node->m_Next);
        node->m_Next = NULL;
    }
}

// Frees all nodes in the list.
void destroyPlatoon(TSoldier* src) {
    while (src) {
        TSoldier* temp = src;
        src = src->m_Next;
        free(temp);
    }
}


// --- Tests ---

static TSoldier* createSoldier(int id, TSoldier* next) {
    TSoldier* s = (TSoldier*)malloc(sizeof(*s));
    s->m_PersonalID = id;
    s->m_Next       = next;
    return s;
}

int main(void) {
    TSoldier *a, *b, *c;

    // Merge two equal-length lists and split back
    a = createSoldier(0, createSoldier(1, createSoldier(2, createSoldier(3, createSoldier(4, NULL)))));
    b = createSoldier(10, createSoldier(11, createSoldier(12, createSoldier(13, createSoldier(14, NULL)))));
    c = mergePlatoons(a, b);
    assert(c != NULL
           && c->m_PersonalID == 0
           && c->m_Next != NULL
           && c->m_Next->m_PersonalID == 10
           && c->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_PersonalID == 1
           && c->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_PersonalID == 11
           && c->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 2
           && c->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 12
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 3
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 13
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 4
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 14
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    splitPlatoon(c, &a, &b);
    assert(a != NULL
           && a->m_PersonalID == 0
           && a->m_Next != NULL
           && a->m_Next->m_PersonalID == 10
           && a->m_Next->m_Next != NULL
           && a->m_Next->m_Next->m_PersonalID == 1
           && a->m_Next->m_Next->m_Next != NULL
           && a->m_Next->m_Next->m_Next->m_PersonalID == 11
           && a->m_Next->m_Next->m_Next->m_Next != NULL
           && a->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 2
           && a->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    assert(b != NULL
           && b->m_PersonalID == 12
           && b->m_Next != NULL
           && b->m_Next->m_PersonalID == 3
           && b->m_Next->m_Next != NULL
           && b->m_Next->m_Next->m_PersonalID == 13
           && b->m_Next->m_Next->m_Next != NULL
           && b->m_Next->m_Next->m_Next->m_PersonalID == 4
           && b->m_Next->m_Next->m_Next->m_Next != NULL
           && b->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 14
           && b->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    destroyPlatoon(a);
    destroyPlatoon(b);

    // Merge lists of different lengths (p1 shorter)
    a = createSoldier(0, createSoldier(1, createSoldier(2, NULL)));
    b = createSoldier(10, createSoldier(11, createSoldier(12, createSoldier(13, createSoldier(14, NULL)))));
    c = mergePlatoons(a, b);
    assert(c != NULL
           && c->m_PersonalID == 0
           && c->m_Next != NULL
           && c->m_Next->m_PersonalID == 10
           && c->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_PersonalID == 1
           && c->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_PersonalID == 11
           && c->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 2
           && c->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 12
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 13
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 14
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    splitPlatoon(c, &a, &b);
    assert(a != NULL
           && a->m_PersonalID == 0
           && a->m_Next != NULL
           && a->m_Next->m_PersonalID == 10
           && a->m_Next->m_Next != NULL
           && a->m_Next->m_Next->m_PersonalID == 1
           && a->m_Next->m_Next->m_Next != NULL
           && a->m_Next->m_Next->m_Next->m_PersonalID == 11
           && a->m_Next->m_Next->m_Next->m_Next == NULL);
    assert(b != NULL
           && b->m_PersonalID == 2
           && b->m_Next != NULL
           && b->m_Next->m_PersonalID == 12
           && b->m_Next->m_Next != NULL
           && b->m_Next->m_Next->m_PersonalID == 13
           && b->m_Next->m_Next->m_Next != NULL
           && b->m_Next->m_Next->m_Next->m_PersonalID == 14
           && b->m_Next->m_Next->m_Next->m_Next == NULL);
    destroyPlatoon(a);
    destroyPlatoon(b);

    // Merge two even-length lists
    a = createSoldier(0, createSoldier(1, createSoldier(2, NULL)));
    b = createSoldier(10, createSoldier(11, createSoldier(12, createSoldier(13, NULL))));
    c = mergePlatoons(a, b);
    assert(c != NULL
           && c->m_PersonalID == 0
           && c->m_Next != NULL
           && c->m_Next->m_PersonalID == 10
           && c->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_PersonalID == 1
           && c->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_PersonalID == 11
           && c->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 2
           && c->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 12
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_PersonalID == 13
           && c->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);
    splitPlatoon(c, &a, &b);
    assert(a != NULL
           && a->m_PersonalID == 0
           && a->m_Next != NULL
           && a->m_Next->m_PersonalID == 10
           && a->m_Next->m_Next != NULL
           && a->m_Next->m_Next->m_PersonalID == 1
           && a->m_Next->m_Next->m_Next == NULL);
    assert(b != NULL
           && b->m_PersonalID == 11
           && b->m_Next != NULL
           && b->m_Next->m_PersonalID == 2
           && b->m_Next->m_Next != NULL
           && b->m_Next->m_Next->m_PersonalID == 12
           && b->m_Next->m_Next->m_Next == NULL);
    destroyPlatoon(a);
    destroyPlatoon(b);

    printf("All tests passed.\n");
    return 0;
}
