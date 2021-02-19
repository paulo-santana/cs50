#include <math.h>
#include <stdio.h>
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE rowAbove[width], currentRow[width], rowBelow[width];
    arrCopy(width, image[0], currentRow);
    arrCopy(width, image[1], rowBelow);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxRed = 0, GxGreen = 0, GxBlue = 0,
                GyRed = 0, GyGreen = 0, GyBlue = 0;

            int left = j - 1,
                right = j + 1;

            // Will only have a row above if it's not the first line
            if (i > 0)
            {
                GyRed += (rowAbove[j].rgbtRed * -2);
                GyGreen += (rowAbove[j].rgbtGreen * -2);
                GyBlue += (rowAbove[j].rgbtBlue * -2);

                if (left >= 0)
                {
                    GxRed += (rowAbove[left].rgbtRed * -1);
                    GxGreen += (rowAbove[left].rgbtGreen * -1);
                    GxBlue += (rowAbove[left].rgbtBlue * -1);

                    GyRed += (rowAbove[left].rgbtRed * -1);
                    GyGreen += (rowAbove[left].rgbtGreen * -1);
                    GyBlue += (rowAbove[left].rgbtBlue * -1);
                }

                if (right < width)
                {
                    GxRed += rowAbove[right].rgbtRed;
                    GxGreen += rowAbove[right].rgbtGreen;
                    GxBlue += rowAbove[right].rgbtBlue;

                    GyRed += rowAbove[right].rgbtRed * -1;
                    GyGreen += rowAbove[right].rgbtGreen * -1;
                    GyBlue += rowAbove[right].rgbtBlue * -1;
                }
            }

            if (left >= 0)
            {
                GxRed += (currentRow[left].rgbtRed * -2);
                GxGreen += (currentRow[left].rgbtGreen * -2);
                GxBlue += (currentRow[left].rgbtBlue * -2);
            }

            if (right < width)
            {
                GxRed += currentRow[right].rgbtRed * 2;
                GxGreen += currentRow[right].rgbtGreen * 2;
                GxBlue += currentRow[right].rgbtBlue * 2;
            }

            // Will only have a row below if it's not the last line
            if (i < height - 1)
            {
                GyRed += (rowBelow[j].rgbtRed * 2);
                GyGreen += (rowBelow[j].rgbtGreen * 2);
                GyBlue += (rowBelow[j].rgbtBlue * 2);

                if (left >= 0)
                {
                    GxRed += (rowBelow[left].rgbtRed * -1);
                    GxGreen += (rowBelow[left].rgbtGreen * -1);
                    GxBlue += (rowBelow[left].rgbtBlue * -1);

                    GyRed += (rowBelow[left].rgbtRed);
                    GyGreen += (rowBelow[left].rgbtGreen);
                    GyBlue += (rowBelow[left].rgbtBlue);
                }

                if (right < width)
                {
                    GxRed += rowBelow[right].rgbtRed;
                    GxGreen += rowBelow[right].rgbtGreen;
                    GxBlue += rowBelow[right].rgbtBlue;

                    GyRed += (rowBelow[right].rgbtRed);
                    GyGreen += (rowBelow[right].rgbtGreen);
                    GyBlue += (rowBelow[right].rgbtBlue);
                }
            }
            int newChannel = round(sqrt(pow(GxRed, 2) + pow(GyRed, 2)));
            image[i][j].rgbtRed = newChannel > 255 ? 255 : newChannel;

            newChannel = round(sqrt(pow(GxGreen, 2) + pow(GyGreen, 2)));
            image[i][j].rgbtGreen = newChannel > 255 ? 255 : newChannel;

            newChannel = round(sqrt(pow(GxBlue, 2) + pow(GyBlue, 2)));
            image[i][j].rgbtBlue = newChannel > 255 ? 255 : newChannel;
        }
        arrCopy(width, currentRow, rowAbove);
        arrCopy(width, rowBelow, currentRow);

        if (i < height - 1)
        {
            arrCopy(width, image[i + 2], rowBelow);
        }
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