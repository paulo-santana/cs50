#include <cs50.h>
#include <stdio.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2) {
        printf("Player 1 wins!");
    } else if (score2 > score1) {
        printf("Player 2 wins!");
    } else {
        printf("Tie!");
    }
    printf("\n");
}

int compute_score(string word)
{
    // change every letter to upper case
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] >= 'a' && word[i] <= 'z') {
            word[i] = (char) (word[i] - 32);
        }
    }

    // get the value of each letter, discarding special characters
    int total = 0;
    for (int i = 0; word[i] != '\0'; i++) {
       int position = word[i] - 65;
       if (position >= 0 && position <= 25) {
           total += POINTS[position];
       }
    }
    return total;
}
