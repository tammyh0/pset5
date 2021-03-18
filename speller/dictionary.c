// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1]; // bc of the null terminating character at the end of each string
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26; // for each letter in the alphabet

// Number of words in dictionary as global variable
int WORDS = 0;

// Hash table
node *table[N]; // this is a STATIC ARRAY

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int code = hash(word);
    node *temp = table[code];
    if (temp != NULL)
    {
        // Has more than one node, so traverse
        while (temp->next != NULL)
        {
            if (strcasecmp(word, temp->word) == 0) // found word in dictionary!
            {
                return true;
            }
            else
            {
                temp = temp->next;
            }
        }
        // Check last node
        if (strcasecmp(word, temp->word) == 0) // found word in dictionary!
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    
    return false;
}

// Hashes word to a number aka returns hash code
unsigned int hash(const char *word)
{
    // Returns hashcode based on the first lowercase letter in dict
    char firstLetter = tolower(word[0]); 
    int letterValue = (int) firstLetter;
    int hashCode = letterValue - 97;

    return hashCode;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens dictionary file
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }

    // Loads dictionary words into memory
    char word[LENGTH + 1]; 
    while (fscanf(f, "%s", word) != EOF)
    {
        // add end terminating nul char?? is this already done?
        //word[index] = '\0';
        
        // Insert word to hash table as a new node
        unsigned int code = hash(word);
        node *n = malloc(sizeof(node));
        if (n != NULL)
        {
            strcpy(n->word, word);
            // First node word in bucket
            if (table[code] == NULL)
            {
                n->next = NULL;
            }
            // Add node word to existing list
            else
            {
                n->next = table[code];
            }
            table[code] = n;
            
            // Update size of dictionary
            WORDS++;
        }
        else
        {
            return false;
        }
    }

    // Close file
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (WORDS > 0)
    {
        return WORDS;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    if (size() != 0)
    {
        // Go through every array index
        for (int i = 0; i < N; i++)
        {
            // Free linked list
            if (table[i] != NULL)
            {
                node *temp = table[i];
                node *temp2 = table[i]->next;
                while (temp2 != NULL)
                {
                    free(temp);
                    temp = temp2;
                    temp2 = temp->next;
                }
                
                // Free last node
                free(temp);
            }
        }
        
        return true;
    }
    
    return false;
}
