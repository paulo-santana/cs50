#include <cs50.h>
#include <stdio.h>

void draw_whitespaces(int amount);
void draw_blocks(int amount);

int main(void)
{
    // prompt the user for a height
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    int gap_size = 2;

    // there should be a loop to print each row of the pyramids
    // and two loops inside it to draw the row according to its number of blocks
    // the first subloop should "draw" some amount of white spaces
    // then, the second loop should draw the pyramids
    // the number of blocks to be drawn is the number of the current row
    // the number of white spaces to be drawn is the height minus the number of the current row


    // draw the rows
    for (int i = 1; i <= height; i++)
    {
        draw_whitespaces(height - i);
        draw_blocks(i);
        draw_whitespaces(gap_size);
        draw_blocks(i);

        printf("\n");
    }
}

void draw_whitespaces(int amount)
{
    // draw white spaces
    for (int j = 0; j < amount; j++)
    {
        printf(" ");
    }
}

void draw_blocks(int amount)
{
    // draw the blocks
    for (int j = 0; j < amount; j++)
    {
        printf("#");
    }
}
