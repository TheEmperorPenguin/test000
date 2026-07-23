#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
    char line[1024];

    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == '\n' || line[0] == '\0') continue;

        char op[4];
        double ax, ay, bx, by, s;

        if (sscanf(line, "%3s", op) != 1) continue;

        if (strcmp(op, "ADD") == 0)
        {
            if (sscanf(line, "%*s %lf %lf %lf %lf", &ax, &ay, &bx, &by) == 4)
                printf("%.4f %.4f\n", ax + bx, ay + by);
        }
        else if (strcmp(op, "SUB") == 0)
        {
            if (sscanf(line, "%*s %lf %lf %lf %lf", &ax, &ay, &bx, &by) == 4)
                printf("%.4f %.4f\n", ax - bx, ay - by);
        }
        else if (strcmp(op, "MUL") == 0)
        {
            if (sscanf(line, "%*s %lf %lf %lf", &ax, &ay, &s) == 3)
                printf("%.4f %.4f\n", ax * s, ay * s);
        }
    }

    return (0);
}
