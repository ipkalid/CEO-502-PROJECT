#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static long num_steps = 100;
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
    char *substr = malloc(length + 1);

    for (i = 0; i < length; i++)
    {
        substr[i] = str[start + i];
    }
    substr[length] = '\0';

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

    string[non_space_count] = ' ';
    return string;
}

double calculateSpace(int a, int b, double step, char *str)
{
    double space = 0;

    if (find_string(str, "x"))
    {
        if (find_string(str, "sin"))
        {
            int indexOfS = find_index(str, 's');
            char *constantString = substring(str, 0, indexOfS);
            double constantValue = string_to_double(constantString);
            if (strcmp(constantString, "") == 0)
            {
                constantValue = 1;
            }

#pragma omp parallel
            {
                int thread_num = omp_get_thread_num();
                int threads_num = omp_get_num_threads() - 1;
                double step_shift = step + (double)threads_num / (double)num_steps;

                double startingPoint = (double)thread_num / (double)num_steps;

                double partialSpace = 0;
                for (double i = a + startingPoint; i <= b; i += step_shift)
                {

                    double height = sin(i);

                    double width = step;

                    partialSpace += height * width;
                }

#pragma omp critical
                space += partialSpace;
            }

            return constantValue * space;
        }
        if (find_string(str, "cos"))
        {
            int indexOfC = find_index(str, 'c');
            char *constantString = substring(str, 0, indexOfC);
            double constantValue = string_to_double(constantString);
            if (strcmp(constantString, "") == 0)
            {
                constantValue = 1;
            }

#pragma omp parallel
            {
                int thread_num = omp_get_thread_num();
                int threads_num = omp_get_num_threads() - 1;
                double step_shift = step + (double)threads_num / (double)num_steps;

                double startingPoint = (double)thread_num / (double)num_steps;

                double partialSpace = 0;
                for (double i = a + startingPoint; i <= b; i += step_shift)
                {

                    double height = cos(i);

                    double width = step;

                    partialSpace += height * width;
                }

#pragma omp critical
                space += partialSpace;
            }

            return constantValue * space;
        }
        if (find_string(str, "tan"))
        {
            int indexOfS = find_index(str, 't');
            char *constantString = substring(str, 0, indexOfS);
            double constantValue = string_to_double(constantString);
            if (strcmp(constantString, "") == 0)
            {
                constantValue = 1;
            }
#pragma omp parallel
            {
                int thread_num = omp_get_thread_num();
                int threads_num = omp_get_num_threads() - 1;
                double step_shift = step + (double)threads_num / (double)num_steps;

                double startingPoint = (double)thread_num / (double)num_steps;

                double partialSpace = 0;
                for (double i = a + startingPoint; i <= b; i += step_shift)
                {

                    double height = tan(i);
                    ;
                    double width = step;

                    partialSpace += height * width;
                }

#pragma omp critical
                space += partialSpace;
            }

            return constantValue * space;
        }

        if (find_string(str, "^")) // Power
        {
            int indexOfX = find_index(str, 'x');
            int lenOfString = strlen(str);

            char *constantString = substring(str, 0, indexOfX);
            char *powerValueString = substring(str, indexOfX + 2, lenOfString);

            double powerValue = string_to_double(powerValueString);
            double constantValue = string_to_double(constantString);
            if (strcmp(constantString, "") == 0)
            {
                constantValue = 1;
            }

#pragma omp parallel
            {
                int thread_num = omp_get_thread_num();
                int threads_num = omp_get_num_threads() - 1;
                double step_shift = step + (double)threads_num / (double)num_steps;

                double startingPoint = (double)thread_num / (double)num_steps;

                double partialSpace = 0;
                for (double i = a + startingPoint; i <= b; i += step_shift)
                {

                    double height = pow(i, powerValue);
                    ;
                    double width = step;

                    partialSpace += height * width;
                }

#pragma omp critical
                space += partialSpace;
            }

            return constantValue * space;
        }

        // for only X
        int indexOfX = find_index(str, 'x');
        char *constantString = substring(str, 0, indexOfX);
        double constantValue = string_to_double(constantString);
        if (strcmp(constantString, "") == 0)
        {
            constantValue = 1;
        }

#pragma omp parallel
        {
            int thread_num = omp_get_thread_num();
            int threads_num = omp_get_num_threads() - 1;
            double step_shift = step + (double)threads_num / (double)num_steps;

            double startingPoint = (double)thread_num / (double)num_steps;

            double partialSpace = 0;
            for (double i = a + startingPoint; i <= b; i += step_shift)
            {

                double height = i;
                double width = step;

                partialSpace += height * width;
            }

#pragma omp critical
            space += partialSpace;
        }

        return constantValue * space;
    }
    else // Not containing any Variable = Square Area
    {

        return string_to_double(str) * (b - a);
    }
}

int main()
{
    int a = 0;
    int b = 10;
    double space = 0;

    step = 1.0 / (double)num_steps;

    double start_time = omp_get_wtime();

    char function[] = "2x - x^5 + tan(x)";

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
