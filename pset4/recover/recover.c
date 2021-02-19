#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int isNewJPEG(BYTE *buffer);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    char *rawImage = argv[1];

    FILE *file = fopen(rawImage, "r");

    if (file == NULL)
    {
        printf("Could not open the file %s\n", rawImage);
        return 1;
    }

    BYTE buffer[512];
    int counter = 0;

    char *filename = malloc(7);
    int isFirstImage = 1;
    int weAreWritingJPEGs = 0;

    FILE *imgFile;
    while (fread(buffer, 1, 512, file) != 0)
    {
        if (isNewJPEG(buffer))
        {
            weAreWritingJPEGs = 1;
            if (isFirstImage)
            {
                sprintf(filename, "%03i.jpg", counter);
                imgFile = fopen(filename, "w");
                isFirstImage = 0;
            }
            else
            {
                // if it's not the first image, we should close the last
                // one before opening another
                fclose(imgFile);
                sprintf(filename, "%03i.jpg", counter);
                imgFile = fopen(filename, "a");
            }
            fwrite(buffer, 1, 512, imgFile);
            counter++;
        }
        else
        {
            // don't try to write if we didn't even found the first image yet
            if (weAreWritingJPEGs)
            {
                fwrite(buffer, 1, 512, imgFile);
            }
        }
    }

    fclose(imgFile);
    fclose(file);
    free(filename);
}

int isNewJPEG(BYTE *buffer)
{
    return buffer[0] == 0xff &&
           buffer[1] == 0xd8 &&
           buffer[2] == 0xff &&
           (buffer[3] & 0xf0) == 0xe0;
}