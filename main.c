#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static long num_steps = 100000000;
double step;

int find_index(char *string, char c)
{
    char *result = strchr(string, c);
    if (result != NULL)
    {
        return result - string;
    }
    else
    {
        return -1;
    }
}

int find_string(char *string, char *search)
{
    char *result = strstr(string, search);
    return (result != NULL);
}
char *substring(char *str, int start, int end)
{
    int i;
    int length = end - start;
    char *substr = malloc(length + 1); // allocate memory for substring

    for (i = 0; i < length; i++)
    {
        substr[i] = str[start + i];
    }
    substr[length] = '\0'; // add null terminator

    return substr;
}

double string_to_double(const char *str)
{
    double d;

    sscanf(str, "%lf", &d);

    if (d == 0)
    {
        return 1;
    }

    return d;
}

char *removeSpacesFromStr(char *string)
{
    int non_space_count = 0;

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] != ' ')
        {
            string[non_space_count] = string[i];
            non_space_count++; // non_space_count incremented
        }
    }

    // Finally placing final character at the string end
    string[non_space_count] = ' ';
    return string;
}

double calculateSpace(int a, int b, double step, char *str)
{
    double space = 0;
    // int search = find_string(str, "x");

    if (find_string(str, "x"))
    {
        if (find_string(str, "sin"))
        {
            int indexOfS = find_index(str, 's');
            char *constantString = substring(str, 0, indexOfS);
            // #pragma omp parallel for reduction(+ \
//                                    : space)
            for (double i = a; i <= b; i += step)
            {
                double height = sin(i);
                double width = step;
                space += height * width;
            }

            return string_to_double(constantString) * space;
        }
        if (find_string(str, "cos"))
        {
            int indexOfC = find_index(str, 'c');
            char *constantString = substring(str, 0, indexOfC);

            for (double i = a; i <= b; i += step)
            {
                double height = cos(i);
                double width = step;
                space += height * width;
            }

            return string_to_double(constantString) * space;
        }
        if (find_string(str, "tan"))
        {
            int indexOfS = find_index(str, 't');
            char *constantString = substring(str, 0, indexOfS);
            for (double i = a; i <= b; i += step)
            {
                double height = tan(i);
                double width = step;
                space += height * width;
            }

            return string_to_double(constantString) * space;
        }

        if (find_string(str, "^")) // Power
        {
            int indexOfX = find_index(str, 'x');
            int lenOfString = strlen(str);

            char *constantString = substring(str, 0, indexOfX);
            char *powerValue = substring(str, indexOfX + 2, lenOfString);

            for (double i = a; i <= b; i += step)
            {
                double height = pow(i, string_to_double(powerValue));
                double width = step;
                space += height * width;
            }

            return string_to_double(constantString) * space;
        }

        // for only X
        int indexOfX = find_index(str, 'x');
        char *constantString = substring(str, 0, indexOfX);
        for (double i = a; i <= b; i += step)
        {
            double height = i;
            double width = step;
            space += height * width;
        }

        return string_to_double(constantString) * space;
    }
    else // Not containing any Variable = Square Area
    {

        return string_to_double(str) * (b - a);
    }
}

int main()
{
    int a = -1;
    int b = 1;
    double space = 0;

    step = 1.0 / (double)num_steps;

    double start_time = omp_get_wtime();

    char function[] = "3x^22 - 300 + sin(x)";

    printf("%s\n", function);

    removeSpacesFromStr(function);

    for (int i = 0, j = 0, k = 0; i < strlen(function); i++)
    {
        if (function[i] == '+' || function[i] == ' ' || function[i] == '-')
        {
            k = i;

            char *operation = substring(function, j, k);
            double newSpace = calculateSpace(a, b, step, operation);

            printf("%s = %f", operation, newSpace);
            if (function[j - 1] == '-')
            {
                space -= newSpace;
            }
            else
            {
                space += newSpace;
            }

            j = ++i;
            free(operation); // free memory

            printf("\n");
        }
    }

    // pi = step * sum;
    double run_time = omp_get_wtime() - start_time;
    printf("\nspace with %ld steps is %f in %lf seconds\n ", num_steps, space, run_time);
    printf("\n");
}

// for (double i = a; i <= b; i += step)
// {
//     double height = i;
//     double width = step;

//     // printf("%f \n", height);

//     space += height * width;

//     // printf("%f \n",i);
// }

// gcc -Wall main.c -o main -lm -fopenmp

// int main()
// {
//     char str[] = "Hello World!";
//     char *hello = substring(str, 0, 5);
//     printf("%s\n", hello); // prints "Hello"
//     // free(hello);           // free memory

//     char *world = substring(str, 6, 11);
//     printf("%s\n", str); // prints "World"
//     free(world);         // free memory

//     return 0;
// }
