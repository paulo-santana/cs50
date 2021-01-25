#include <cs50.h>
#include <stdio.h>

bool isValid(long number);
int getLastDigit(long number);
string check(long number);
bool isAmericanExpress(long number);
bool isMasterCard(long number);
bool isVisa(long number);

int main(void)
{
    // Get the number
    long number = get_long("Number: ");
    printf("%s\n", check(number));

}

// if we get the modulo 10 of a number, we are essencially getting it's last digit
// so 219038920183 mod 10 returns 3
// now, to get another digit, we might want to divide this number by a multiple of 10 first and discard the decimal part
// so 219038920183 / 100 would return 2190389201.83, then we can just do
// 2190389201 modulo 10 to get 1, which is the third-to-last number
bool isValid(long number)
{
    long x = number / 10;
    int total = 0;
    while (x > 0)
    {
        int lastDigitDoubled = 2 * getLastDigit(x);
        // if the last digit is bigger than 4, it's double version is going to have multiple digit
        // tho the max result can only be 2 * 9 = 18, so we can sum up the digits quite easily
        // just subtract 10 from the number, and add 1
        lastDigitDoubled = lastDigitDoubled < 10 ? lastDigitDoubled : lastDigitDoubled - 10 + 1;
        total = total + lastDigitDoubled;
        x = x / 100;
    }
    // now, let's add total to the sum of the other digits
    x = number;
    while (x > 0)
    {
        total = total + getLastDigit(x);
        x = x / 100;
    }
    if (total % 10 == 0)
    {
        return true;
    }
    return false;
}

int getLastDigit(long number)
{
    return number % 10;
}

bool isAmericanExpress(long number)
{
    if ((number >= 340000000000000 && number <= 349999999999999) ||
        (number >= 370000000000000 && number <= 379999999999999)) {
        return true;
    }
    return false;
}

bool isMasterCard(long number)
{
    if (number >= 5100000000000000 && number <= 5599999999999999)
    {
        return true;
    }
    return false;
}

bool isVisa(long number)
{
    if ((number >= 4000000000000 && number <= 4999999999999) ||
        (number >= 4000000000000000 && number <= 4999999999999999)) {
        return true;
    }
    return false;
}

string check(long number)
{
    if (isValid(number))
    {
        if (isAmericanExpress(number))
        {
            return "AMEX";
        }
        else if (isMasterCard(number))
        {
            return "MASTERCARD";
        }
        else if (isVisa(number))
        {
            return "VISA";
        }
        else
        {
            return "INVALID";
        }
    }
    else
    {
        return "INVALID";
    }
}