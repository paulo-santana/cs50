#include <cs50.h>
#include <stdio.h>

int main (void) {
    // Ask the user's name and print's it
    string name = get_string("What's your name?: ");
    printf("hello, %s\n", name);
}