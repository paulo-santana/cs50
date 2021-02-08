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
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;

            BYTE average = round((blue + green + red) / 3.0);

            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int middlePixel = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < middlePixel; j++)
        {
            RGBTRIPLE leftPixel = image[i][width - 1 - j];
            image[i][width - 1 - j] = image[i][j];
            image[i][j] = leftPixel;
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

void arrCopy(int size, RGBTRIPLE *arr, RGBTRIPLE *target)
{
    for (int i = 0; i < size; i++)
    {
        target[i] = arr[i];
    }
}