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
        {
            arrCopy(width, image[i + 2], rowBelow);
        }
    }
    return;
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
            if (i == 2 && j == 3)
            {
                printf("chegou");
            }
            // compute Gx
            // row above
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