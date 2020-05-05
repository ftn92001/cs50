#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    string text = get_string("Text: ");
    float L = 0, W = 0, S = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (isalpha(text[i]))
        {
            L++;
        }

        if (text[i] == ' ')
        {
            W++;
        }

        if (text[i] == '?' || text[i] == '.' || text[i] == '!')
        {
            S++;
        }
    }

    W++; //The last word.
    L = L / W * 100;
    S = S / W * 100;
    int grade = round(0.0588 * L - 0.296 * S - 15.8);

    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}