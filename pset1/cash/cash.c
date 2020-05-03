#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float dollars;
    do
    {
        dollars = get_float("Change owed: ");
    }
    while (dollars < 0);
    int cents = round(dollars * 100);
    int coins = cents / 25;
    cents = cents % 25;
    coins = coins + cents / 10;
    cents = cents % 10;
    coins = coins + cents / 5;
    cents = cents % 5;
    coins = coins + cents / 1;
    printf("%i\n", coins);
}