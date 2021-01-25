#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

string cypher(string text, char key[26]);

int main(int argc, string argv[]) {
    if (argc != 2) {
        printf("Usage: ./substitution key\n");
        return 1;
    } else if (strlen(argv[1]) != 26) {
        printf("Key must contain 26 characters.\n");
        return 1;
    }  else {
        char key[26];
        for (int i = 0; i < 26; i++) {
            key[i] = toupper(argv[1][i]);
        }
        for (int i = 0; i < 26; i++) {
            // check if there is any invalid characters in the key
            if (!isalpha(key[i])) {
                printf("Only alphabetic characters allawed\n");
                return 1;
            }
            // check if there are repeated characters in the key
            for (int j = 0; j < i; j++) {
                if (key[j] == key[i]) {
                    printf("Repeated characeters are not allowed in the key\n");
                    return 1;
                }
            }
        }
        
        string plaintext = get_string("plaintext: ");
        string ciphertext = cypher(plaintext, key);
        printf("ciphertext: %s\n", ciphertext);
    }

    return 0;
}

string cypher(string text, string key) {
    // calculate the shift for each letter in the key
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int shifts[26];
    for (int i = 0; i < 26; i++) {
        shifts[i] = key[i] - alphabet[i];
    }

    // apply the shift to the current letter acording to its position in the alphabet
    for (int i = 0, n = strlen(text); i < n; i++) {
        if (isalpha(text[i])) {
            int letterPosition = toupper(text[i]) - 65;
            text[i] = text[i] + shifts[letterPosition];
        }
    }
    return text;
}