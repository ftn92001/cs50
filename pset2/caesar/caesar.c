#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    int key = 0;
    // number of argument is 1
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }


    for (int i = 0; i < strlen(argv[1]); i++)
    {
        // not decimal digit argument
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }

        key = key + pow(10, strlen(argv[1]) - i - 1) * (argv[1][i] - 48);
    }

    string plaintext = get_string("plaintext: ");

    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isalpha(plaintext[i]))
        {
            plaintext[i] += (key % 26);
            if (!isalpha(plaintext[i]))
            {
                plaintext[i] -= 26;
            }
        }
    }
    printf("ciphertext: %s\n", plaintext);
}