#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Command
{ ADD, SUB, MUL, DOT, CROSS, LEN, NORM, INVALID };

enum Command
getCommand(const char *op)
{
    if (strcmp(op, "ADD") == 0) return ADD;
    if (strcmp(op, "SUB") == 0) return SUB;
    if (strcmp(op, "MUL") == 0) return MUL;
    if (strcmp(op, "CROSS") == 0) return CROSS;
    if (strcmp(op, "NORM") == 0) return NORM;
    if (strcmp(op, "LEN") == 0) return LEN;
    if (strcmp(op, "DOT") == 0) return DOT;
    return INVALID;
}

double
my_sqrt(double x)
{
    if (x <= 0) return 0;

    double guess = x;
    double prev;

    do
    {
        prev = guess;
        guess = 0.5 * (guess + x / guess);
    } while ((guess - prev > 1e-5) || (prev - guess > 1e-5));

    return guess;
}

int
main(void)
{
    char line[1024];

    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == '\n' || line[0] == '\0') continue;

        char op[6];
        double ax, ay, bx, by, s;

        if (sscanf(line, "%5s", op) != 1) continue;

        switch (getCommand(op))
        {
            case ADD:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &ax, &ay, &bx, &by) == 4)
                    printf("%.4f %.4f\n", ax + bx, ay + by);
                break;

            case SUB:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &ax, &ay, &bx, &by) == 4)
                    printf("%.4f %.4f\n", ax - bx, ay - by);
                break;

            case MUL:
                if (sscanf(line, "%*s %lf %lf %lf", &ax, &ay, &s) == 3)
                    printf("%.4f %.4f\n", ax * s, ay * s);
                break;

            case DOT:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &ax, &ay, &bx, &by) == 4)
                    printf("%.4f\n", ax * bx + ay * by);
                break;

            case CROSS:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &ax, &ay, &bx, &by) == 4)
                    printf("%.4f\n", ax * by - ay * bx);
                break;

            case LEN:
                if (sscanf(line, "%*s %lf %lf", &ax, &ay) == 2)
                    printf("%.4f\n", my_sqrt(ax * ax + ay * ay));
                break;

            case NORM:
                if (sscanf(line, "%*s %lf %lf", &ax, &ay) == 2)
                {
                    double len = my_sqrt(ax * ax + ay * ay);
                    if (len == 0)
                        printf("0.0000 0.0000\n");
                    else
                        printf("%.4f %.4f\n", ax / len, ay / len);
                }
                break;

            default:
                printf("Invalid command\n");
        }
    }

    return (0);
}
