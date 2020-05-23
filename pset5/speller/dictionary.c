// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
int num = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int index = hash(word);
    node *ptr = table[index];
    do
    {
        if(!strcasecmp(ptr->word, word))
        {
            return true;
        }
        ptr = ptr->next;
    }
    while (ptr != NULL);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    if (word[0] >= 97)
    {
        return word[0] - 97;
    }
    return word[0] - 65;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    char word[LENGTH + 1];
    if (file != NULL)
    {
        //讀dictionary內的word
        while (fscanf(file, "%s", word) != EOF)
        {
            //將word放進hash table
            int index = hash(word);
            node *n = malloc(sizeof(node));
            if (n != NULL)
            {
                strcpy(n->word, word);
                n->next = table[index];
                table[index] = n;
                num++;
            }
        }
        fclose(file);
        return true;
    }
    else
    {
        return false;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (num > 0)
    {
        return num;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    node *ptr;
    node *temp;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        temp = ptr;
        while (ptr != NULL)
        {
            ptr = ptr->next;
            free(temp);
            temp = ptr;
        }
        if(i == N - 1)
        {
            return true;
        }
    }
    return false;
}
