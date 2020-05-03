#include <stdio.h>
#include <cs50.h>

int get_height();

int main(void)
{
    int height = get_height();
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < height; j++)
        {
            if (j >= height - i - 1)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}


int get_height()
{
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    return height;
}