#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int words = 1;
int letters = 0;
int sentences = 0;

void countLetters(string text);
bool isEndOfSentence(char c);

int main(void)
{
    string text = get_string("Text: ");
    countLetters(text);

    float L, S;
    L = letters / (float) words * 100;
    S = sentences / (float) words * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1) {
        printf("Before Grade 1\n");
    } else if (index >= 16) {
        printf("Grade 16+\n");
    } else {
        printf("Grade %i\n", index);
    }
}

void countLetters(string text)
{
    int spaces = 0;
    for (int i = 0, n = strlen(text); i < n; i++) {
        if (isalpha(text[i])) {
            letters++;
        } else if (isblank(text[i])) {
            spaces++;
        } else if (isEndOfSentence(text[i])) {
            sentences++;
        }
    }
    words = spaces + 1;
}

bool isEndOfSentence(char c)
{
    return c == '.' || c == '!' || c == '?';
}