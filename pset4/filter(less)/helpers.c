#include "helpers.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtGreen = image[i][j].rgbtBlue;
            image[i][j].rgbtRed = image[i][j].rgbtBlue;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int b, g, r;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            b = round(.131 * image[i][j].rgbtBlue + .534 * image[i][j].rgbtGreen + .272 * image[i][j].rgbtRed);
            g = round(.168 * image[i][j].rgbtBlue + .686 * image[i][j].rgbtGreen + .349 * image[i][j].rgbtRed);
            r = round(.189 * image[i][j].rgbtBlue + .769 * image[i][j].rgbtGreen + .393 * image[i][j].rgbtRed);
            if (b > 255)
            {
                b = 255;
            }
            if (g > 255)
            {
                g = 255;
            }
            if (r > 255)
            {
                r = 255;
            }
            image[i][j].rgbtBlue = b;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtRed = r;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = temp;
            temp = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtGreen = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = temp;
            temp = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtRed = image[i][j].rgbtRed;
            image[i][j].rgbtRed = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int b[height][width];
    int g[height][width];
    int r[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 4 corners
            if (i == 0 && j == 0)
            {
                b[i][j] = round((image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 4.0);
                g[i][j] = round((image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) /
                                4.0);
                r[i][j] = round((image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 4.0);
            }
            else if (i == 0 && j == width - 1)
            {
                b[i][j] = round((image[0][width - 2].rgbtBlue + image[0][width - 1].rgbtBlue + image[1][width - 2].rgbtBlue + image[1][width -
                                 1].rgbtBlue) / 4.0);
                g[i][j] = round((image[0][width - 2].rgbtGreen + image[0][width - 1].rgbtGreen + image[1][width - 2].rgbtGreen + image[1][width -
                                 1].rgbtGreen) / 4.0);
                r[i][j] = round((image[0][width - 2].rgbtRed + image[0][width - 1].rgbtRed + image[1][width - 2].rgbtRed + image[1][width -
                                 1].rgbtRed) / 4.0);
            }
            else if (i == height - 1 && j == 0)
            {
                b[i][j] = round((image[height - 2][0].rgbtBlue + image[height - 2][1].rgbtBlue + image[height - 1][0].rgbtBlue + image[height -
                                 1][1].rgbtBlue) / 4.0);
                g[i][j] = round((image[height - 2][0].rgbtGreen + image[height - 2][1].rgbtGreen + image[height - 1][0].rgbtGreen +  image[height -
                                 1][1].rgbtGreen) / 4.0);
                r[i][j] = round((image[height - 2][0].rgbtRed + image[height - 2][1].rgbtRed + image[height - 1][0].rgbtRed + image[height -
                                 1][1].rgbtRed) / 4.0);
            }
            else if (i == height - 1 && j == width - 1)
            {
                b[i][j] = round((image[height - 2][width - 2].rgbtBlue + image[height - 2][width - 1].rgbtBlue + image[height - 1][width -
                                 2].rgbtBlue + image[height - 1][width - 1].rgbtBlue) / 4.0);
                g[i][j] = round((image[height - 2][width - 2].rgbtGreen + image[height - 2][width - 1].rgbtGreen + image[height - 1][width -
                                 2].rgbtGreen + image[height - 1][width - 1].rgbtGreen) / 4.0);
                r[i][j] = round((image[height - 2][width - 2].rgbtRed + image[height - 2][width - 1].rgbtRed + image[height - 1][width -
                                 2].rgbtRed + image[height - 1][width - 1].rgbtRed) / 4.0);
            }
            // edge
            else if (i == 0)
            {
                b[i][j] = round((image[0][j - 1].rgbtBlue + image[0][j].rgbtBlue + image[0][j + 1].rgbtBlue + image[1][j - 1].rgbtBlue +
                                 image[1][j].rgbtBlue + image[1][j + 1].rgbtBlue) / 6.0);
                g[i][j] = round((image[0][j - 1].rgbtGreen + image[0][j].rgbtGreen + image[0][j + 1].rgbtGreen + image[1][j - 1].rgbtGreen +
                                 image[1][j].rgbtGreen + image[1][j + 1].rgbtGreen) / 6.0);
                r[i][j] = round((image[0][j - 1].rgbtRed + image[0][j].rgbtRed + image[0][j + 1].rgbtRed + image[1][j - 1].rgbtRed +
                                 image[1][j].rgbtRed + image[1][j + 1].rgbtRed) / 6.0);
            }
            else if (i == height - 1)
            {
                b[i][j] = round((image[height - 2][j - 1].rgbtBlue + image[height - 2][j].rgbtBlue + image[height - 2][j + 1].rgbtBlue +
                                 image[height - 1][j - 1].rgbtBlue + image[height - 1][j].rgbtBlue + image[height - 1][j + 1].rgbtBlue) / 6.0);
                g[i][j] = round((image[height - 2][j - 1].rgbtGreen + image[height - 2][j].rgbtGreen + image[height - 2][j + 1].rgbtGreen +
                                 image[height - 1][j - 1].rgbtGreen + image[height - 1][j].rgbtGreen + image[height - 1][j + 1].rgbtGreen) / 6.0);
                r[i][j] = round((image[height - 2][j - 1].rgbtRed + image[height - 2][j].rgbtRed + image[height - 2][j + 1].rgbtRed +
                                 image[height - 1][j - 1].rgbtRed + image[height - 1][j].rgbtRed + image[height - 1][j + 1].rgbtRed) / 6.0);
            }
            else if (j == 0)
            {
                b[i][j] = round((image[i - 1][0].rgbtBlue + image[i - 1][1].rgbtBlue + image[i][0].rgbtBlue + image[i][1].rgbtBlue + image[i +
                                 1][0].rgbtBlue + image[i + 1][1].rgbtBlue) / 6.0);
                g[i][j] = round((image[i - 1][0].rgbtGreen + image[i - 1][1].rgbtGreen + image[i][0].rgbtGreen + image[i][1].rgbtGreen + image[i +
                                 1][0].rgbtGreen + image[i + 1][1].rgbtGreen) / 6.0);
                r[i][j] = round((image[i - 1][0].rgbtRed + image[i - 1][1].rgbtRed + image[i][0].rgbtRed + image[i][1].rgbtRed + image[i +
                                 1][0].rgbtRed + image[i + 1][1].rgbtRed) / 6.0);
            }
            else if (j == width - 1)
            {
                b[i][j] = round((image[i - 1][width - 2].rgbtBlue + image[i - 1][width - 1].rgbtBlue + image[i][width - 2].rgbtBlue + image[i][width
                                 - 1].rgbtBlue + image[i + 1][width - 2].rgbtBlue + image[i + 1][width - 1].rgbtBlue) / 6.0);
                g[i][j] = round((image[i - 1][width - 2].rgbtGreen + image[i - 1][width - 1].rgbtGreen + image[i][width - 2].rgbtGreen +
                                 image[i][width - 1].rgbtGreen + image[i + 1][width - 2].rgbtGreen + image[i + 1][width - 1].rgbtGreen) / 6.0);
                r[i][j] = round((image[i - 1][width - 2].rgbtRed + image[i - 1][width - 1].rgbtRed + image[i][width - 2].rgbtRed + image[i][width -
                                 1].rgbtRed + image[i + 1][width - 2].rgbtRed + image[i + 1][width - 1].rgbtRed) / 6.0);
            }
            // middle
            else
            {
                b[i][j] = round((image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue +
                                 image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j +
                                         1].rgbtBlue) / 9.0);
                g[i][j] = round((image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j -
                                 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen +
                                 image[i + 1][j + 1].rgbtGreen) / 9.0);
                r[i][j] = round((image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j - 1].rgbtRed +
                                 image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j +
                                         1].rgbtRed) / 9.0);
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = b[i][j];
            image[i][j].rgbtGreen = g[i][j];
            image[i][j].rgbtRed = r[i][j];
        }
    }
    return;
}
