#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    char *filename = argv[1];

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Could not open the file %s\n", filename);
        return 1;
    }
}