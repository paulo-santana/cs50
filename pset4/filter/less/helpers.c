#include <math.h>
#include "helpers.h"

void arrCopy(int size, RGBTRIPLE *arr, RGBTRIPLE *target);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the average between the colors
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;

            BYTE average = (BYTE) round((red + green + blue) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;

            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255: sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // the 2nd half of the image will already be mirrored by the time we reach it
    // if width is an odd number, we can discard the middle pixel as it wont need to change places with anyone
    int halfPixel = width / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < halfPixel; j++)
        {
            RGBTRIPLE leftPixel = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = leftPixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE rowAbove[width], currentRow[width], rowBelow[width];
    arrCopy(width, image[0], currentRow);
    arrCopy(width, image[1], rowBelow);
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width; j++)
        {
            int totalRed = 0,
                totalGreen = 0,
                totalBlue = 0;
            int pixelCount = 0;

            // loop through the current pixel's neighbours
            // neighbours above
            if (i > 0)
            {
                for (int n = -1; n <= 1; n++)
                {
                    if (j + n >= 0 && j + n < width)
                    {
                        totalRed += rowAbove[j + n].rgbtRed;
                        totalGreen += rowAbove[j + n].rgbtGreen;
                        totalBlue += rowAbove[j + n].rgbtBlue;
                        pixelCount++;
                    }
                }
            }

            // neighbours in the middle including itself
            for (int n = -1; n <= 1; n++)
            {
                if (j + n >= 0 && j + n < width)
                {
                    totalRed += currentRow[j + n].rgbtRed;
                    totalGreen += currentRow[j + n].rgbtGreen;
                    totalBlue += currentRow[j + n].rgbtBlue;
                    pixelCount++;
                }
            }

            // neighbours below
            if (i < height - 1)
            {
                for (int n = -1; n <= 1; n++)
                {
                    if (j + n >= 0 && j + n < width)
                    {
                        totalRed += rowBelow[j + n].rgbtRed;
                        totalGreen += rowBelow[j + n].rgbtGreen;
                        totalBlue += rowBelow[j + n].rgbtBlue;
                        pixelCount++;
                    }
                }
            }

            image[i][j].rgbtRed = round((float) totalRed / pixelCount);
            image[i][j].rgbtGreen = round((float) totalGreen / pixelCount);
            image[i][j].rgbtBlue = round((float) totalBlue / pixelCount);

        }
        // setup the rows for the next iteration
        arrCopy(width, currentRow, rowAbove);
        arrCopy(width, rowBelow, currentRow);
        if (i < height - 1)
            arrCopy(width, image[i + 2], rowBelow);
    }
    return;
}


void arrCopy(int size, RGBTRIPLE *arr, RGBTRIPLE *target)
{
    for (int i = 0; i < size; i++)
    {
        target[i] = arr[i];
    }
}