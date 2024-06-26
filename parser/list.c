/*
 *  OBJ Loader from taylormt@kixor.net
 *  Original source files can be found here:
 *      http://kixor.net/dev/objloader/
 *
 * Some modifications has been made by the ACU team at EPITA, 2018
 */
#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// internal helper functions
char list_is_full(list *listo)
{
    return (listo->item_count == listo->current_max_size);
}

void list_grow(list *old_listo)
{
    int i;
    list new_listo;

    list_make(&new_listo, old_listo->current_max_size * 2, old_listo->growable++);

    for (i = 0; i < old_listo->current_max_size; i++)
        list_add_item(&new_listo, old_listo->items[i] , old_listo->names[i]);

    list_free(old_listo);

    //copy new structure to old list
    old_listo->names = new_listo.names;
    old_listo->items = new_listo.items;
    old_listo->item_count = new_listo.item_count;
    old_listo->current_max_size = new_listo.current_max_size;
    old_listo->growable = new_listo.growable;
}
//end helpers

void list_make(list *listo, int start_size, char growable)
{
    listo->names = malloc(sizeof(char*) * start_size);
    listo->items = malloc(sizeof(void*) * start_size);
    listo->item_count = 0;
    listo->current_max_size = start_size;
    listo->growable = growable;
}

int list_add_item(list *listo, void *item, char *name)
{
    if (list_is_full(listo))
    {
        if (listo->growable)
            list_grow(listo);
        else
            return -1;
    }

    listo->names[listo->item_count] = NULL;
    if (name != NULL)
    {
        listo->names[listo->item_count] = strdup(name);
    }

    listo->items[listo->item_count] = item;
    listo->item_count++;

    return listo->item_count - 1;
}

char* list_print_items(list *listo)
{
    for (int i = 0; i < listo->item_count; i++)
        printf("%s\n", listo->names[i]);
    return NULL;
}

void* list_get_index(list *listo, int indx)
{
    if (indx < listo->item_count)
        return listo->items[indx];
    return NULL;
}

void* list_get_item(list *listo, void *item_to_find)
{
    for (int i = 0; i < listo->item_count; i++)
    {
        if (listo->items[i] == item_to_find)
            return listo->items[i];
    }
    return NULL;
}

void* list_get_name(list *listo, char *name_to_find)
{
    for (int i = 0; i < listo->item_count; i++)
    {
        if (strncmp(listo->names[i], name_to_find, strlen(name_to_find)) == 0)
            return listo->items[i];
    }
    return NULL;
}

int list_find(list *listo, char *name_to_find)
{
    for (int i = 0; i < listo->item_count; i++)
    {
        if(strncmp(listo->names[i], name_to_find, strlen(name_to_find)) == 0)
            return i;
    }
    return -1;
}

void list_delete_item(list *listo, void *item)
{
    for (int i = 0; i < listo->item_count; i++)
    {
        if (listo->items[i] == item)
            list_delete_index(listo, i);
    }
}

void list_delete_name(list *listo, char *name)
{
    if (name == NULL)
        return;
    for (int i = 0; i < listo->item_count; i++)
    {
        int item_name = strlen(name);
        if (name != NULL && (strncmp(listo->names[i], name, item_name) == 0) )
            list_delete_index(listo, i);
    }
}

void list_delete_index(list *listo, int indx)
{
    //remove item
    if (listo->names[indx] != NULL)
        free(listo->names[indx]);
    //restructure
    for (int j = indx; j < listo->item_count - 1; j++)
    {
        listo->names[j] = listo->names[j + 1];
        listo->items[j] = listo->items[j + 1];
    }
    listo->item_count--;
    return;
}

void list_delete_all(list *listo)
{
    for (int i = listo->item_count - 1; i >= 0; i--)
        list_delete_index(listo, i);
}

void list_free(list *listo)
{
    list_delete_all(listo);
    free(listo->names);
    free(listo->items);
}

void list_print_list(list *listo)
{
    printf("count: %i/%i\n", listo->item_count, listo->current_max_size);
    for (int i = 0; i < listo->item_count; i++)
        printf("list[%i]: %s\n", i, listo->names[i]);
}
