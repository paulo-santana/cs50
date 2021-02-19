#include <math.h>
#include <stddef.h>
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
            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
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
    RGBTRIPLE rowsCopy[3][width];
    arrCopy(width, image[0], rowsCopy[1]);
    arrCopy(width, image[1], rowsCopy[2]);
    int invalidRow = 0;
    // loop through each line
    for (int i = 0; i < height; i++)
    {
        // for each pixel
        for (int j = 0; j < width; j++)
        {
            int pixelCount = 0;
            int totalRed = 0, totalGreen = 0, totalBlue = 0;
            // workking with the three copies, the rows above, current and below
            for (int n = 0; n < 3; n++)
            {
                // check if the current row we are working with is valid
                if (n != invalidRow)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        // if the current pixel's neighbours are inside the image
                        if (
                            j + l >= 0 &&
                            j + l < width
                        )
                        {
                            totalRed += rowsCopy[n][j + l].rgbtRed;
                            totalGreen += rowsCopy[n][j + l].rgbtGreen;
                            totalBlue += rowsCopy[n][j + l].rgbtBlue;
                            pixelCount++;
                        }
                    }
                }
            }

            image[i][j].rgbtRed = round((float) totalRed / pixelCount);
            image[i][j].rgbtGreen = round((float) totalGreen / pixelCount);
            image[i][j].rgbtBlue = round((float) totalBlue / pixelCount);

        }

        arrCopy(width, rowsCopy[1], rowsCopy[0]);
        arrCopy(width, rowsCopy[2], rowsCopy[1]);

        // if the next rowsCopy[2] is still inside the image
        if (i + 2 < height)
        {
            arrCopy(width, image[i + 2], rowsCopy[2]);
            invalidRow = -1; // there would be no invalid row
        }
        else
        {
            // if not, then rowsCopy[2] will be an invalid row
            invalidRow = 2;
        }
    }
}

void arrCopy(int size, RGBTRIPLE *arr, RGBTRIPLE *target)
{
    for (int i = 0; i < size; i++)
    {
        target[i] = arr[i];
    }
}