#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int startSize;
    do {
        startSize = get_int("Start size: ");
    } while (startSize < 9);

    // Prompt for end size
    int endSize;
    do {
        endSize = get_int("End size: ");
    } while (endSize < startSize);

    // Calculate number of years until we reach threshold
    int size = startSize;
    int years = 0;
    while (size < endSize) {
        size = size + (size / 3) - (size / 4);
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}