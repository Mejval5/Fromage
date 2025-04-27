#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <fcntl.h>
#include <io.h>

struct node {
    struct node *prev;
    struct node *next;
    int data;
};

struct list {
    struct node *head;
    struct node *tail;
};

struct list* create_linked_list(void)
{
    struct list *linkedList = malloc(sizeof(struct list));
    struct node *lastNode = nullptr;
    
    struct node *firstNode = malloc(sizeof(struct node));
    firstNode->prev = lastNode;
    firstNode->next = nullptr;
    firstNode->data = 0;
    
    lastNode = firstNode;
    linkedList->head = lastNode;
    
    for (int i = 1; i < 100; i++)
    {        
        struct node *newNode = malloc(sizeof(struct node));
        newNode->prev = lastNode;
        newNode->next = nullptr;
        newNode->data = i;
        lastNode->next = newNode;
        lastNode = newNode;
    }
    
    linkedList->tail = lastNode;

    return linkedList;
}

struct node* find_number(const struct list* list, const int itemToFind)
{
    struct node *item = list->head;
    while (item != nullptr)
    {
        if (item->data == itemToFind)
        {
            return item;
        }

        item = item->next;
    }

    return nullptr;
}

struct node* find_number_at(const struct list* list, const int index)
{
    struct node *item = list->head;
    int i = 0;
    while (item != nullptr)
    {
        if (i == index)
        {
            return item;
        }

        item = item->next;
        i++;
    }

    return nullptr;
}

void insert_boy_at(const struct list* list, const int index, const int value)
{
    struct node *item = list->head;
    int i = 0;
    while (item != nullptr)
    {
        if (i == index)
        {
            struct node *newNode = malloc(sizeof(struct node));
            newNode->data = value;
            item->prev->next = newNode;
            newNode->prev = item->prev;
            item->prev = newNode;
            newNode->next = item;
            return;
        }

        i++;
        item = item->next;
    }
}

void delete_boy_at(struct list* list, const int index)
{    
    struct node *item = list->head;
    if (index == 0)
    {
        struct node *newFirstNode = list->head->next;
        newFirstNode->prev = nullptr;
        free(list->head);
        list->head = newFirstNode;
    }
    
    int i = 1;
    while (item != nullptr)
    {
        if (i == index)
        {
            struct node *prevNode = item->prev;
            struct node *nextNode = item->next;
            
            prevNode->next = nextNode;
            
            if (nextNode == nullptr)
            {
                list->head = prevNode;
                return;
            }

            nextNode->prev = prevNode;
            return;
        }

        i++;
        item = item->next;
    }
}

void free_up_nodes(struct list* list)
{    
    struct node *item = list->head;
    int i = 0;
    while (item != nullptr)
    {
        struct node *nextItem = item->next;
        free(item);

        item = nextItem;
        i++;
    }

    list->head = nullptr;
    list->tail = nullptr;
}

int main(void)
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    wprintf(L"Hi\n");
    
    struct list *linkedList = create_linked_list();

    const struct node *item = find_number(linkedList, 69);
    if (item != nullptr)
    {
        wprintf(L"Found my boy %d\n", item->data);
        wprintf(L"Found him at %p\n", item);
    }
    else
    {
        wprintf(L"cry");
    }
    
    const struct node *item2 = find_number_at(linkedList, 10);
    if (item != nullptr)
    {
        wprintf(L"Found my second boy %d\n", item2->data);
        wprintf(L"Found him at %p\n", item2);
    }
    else
    {
        wprintf(L"cry");
    }
    
    insert_boy_at(linkedList, 10, 420);
    
    const struct node *item3 = find_number_at(linkedList, 10);
    if (item != nullptr)
    {
        wprintf(L"Found my third boy %d\n", item3->data);
        wprintf(L"Found him at %p\n", item3);
    }
    else
    {
        wprintf(L"cry");
    }

    delete_boy_at(linkedList, 10);
    
    const struct node *item4 = find_number_at(linkedList, 10);
    if (item != nullptr)
    {
        wprintf(L"Found my fourth boy %d\n", item4->data);
        wprintf(L"Found him at %p\n", item4);
    }
    else
    {
        wprintf(L"cry");
    }
    
    const struct node *item5 = find_number_at(linkedList, 69);
    if (item != nullptr)
    {
        wprintf(L"Found my fifth boy %d\n", item5->data);
        wprintf(L"Found him at %p\n", item5);
    }
    else
    {
        wprintf(L"cry");
    }

    free_up_nodes(linkedList);
    free(linkedList);
    return 0;
}
