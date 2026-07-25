#include <stdio.h>
#include <string.h>
#include <stdbool.h>

enum Command
{ ADD, SUB, MUL, DOT, CROSS, LEN, NORM, INVALID, DT, STEP, INIT, FORCE };

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
    if (strcmp(op, "DT") == 0) return DT;
    if (strcmp(op, "STEP") == 0) return STEP;
    if (strcmp(op, "INIT") == 0) return INIT;
    if (strcmp(op, "FORCE") == 0) return FORCE;
    return INVALID;
}

typedef
struct vector_2d
{   double x;
    double y;
} vector_2d;

typedef
struct rigid_body_2d
{   vector_2d position;
    vector_2d velocity;
} rigid_body_2d;

typedef
struct aabb
{   double min_x;
    double min_y;
    double max_x;
    double max_y;
} aabb;

//  Should use <math.h> but the tests don't compile with -lm
//  hence this function.
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
vector_2d
add_vector_2d(vector_2d a, vector_2d b)
{
    vector_2d result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

vector_2d
sub_vector_2d(vector_2d a, vector_2d b)
{
    vector_2d result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

vector_2d
mul_vector_2d(vector_2d v, double scalar)
{
    vector_2d result;

    result.x = v.x * scalar;
    result.y = v.y * scalar;

    return result;
}

double
dot_vector_2d(vector_2d a, vector_2d b)
{
    return a.x * b.x + a.y * b.y;
}

double
cross_vector_2d(vector_2d a, vector_2d b)
{
    return a.x * b.y - a.y * b.x;
}

double
length_vector_2d(vector_2d v)
{
    return my_sqrt(v.x * v.x + v.y * v.y);
}

vector_2d
normalize_vector_2d(vector_2d v)
{
    vector_2d result = {0, 0};

    double len = length_vector_2d(v);

    if (len != 0)
    {
        result.x = v.x / len;
        result.y = v.y / len;
    }

    return result;
}

void print_vector_2d(vector_2d v)
{
    printf("%.4f %.4f\n", v.x, v.y);
}

rigid_body_2d
step_body_explicit(rigid_body_2d body, vector_2d force, double mass, double dt)
{
    vector_2d acceleration = mul_vector_2d(force, 1.0 / mass);

    // Position uses OLD velocity
    body.position = add_vector_2d(
        body.position,
        mul_vector_2d(body.velocity, dt)
    );

    // Then update velocity
    body.velocity = add_vector_2d(
        body.velocity,
        mul_vector_2d(acceleration, dt)
    );

    return body;
}

rigid_body_2d
step_body_symplectic(rigid_body_2d body, vector_2d force, double mass, double dt)
{
    vector_2d acceleration = mul_vector_2d(force, 1.0 / mass);

    body.velocity = add_vector_2d(
        body.velocity,
        mul_vector_2d(acceleration, dt)
    );

    body.position = add_vector_2d(
        body.position,
        mul_vector_2d(body.velocity, dt)
    );

    return body;
}

bool
is_aabb_overlap(aabb a, aabb b)
{
    if (a.max_x >= b.min_x  &&  a.min_x <= b.max_x &&
        a.max_y >= b.min_y  &&  a.min_y <= b.max_y)
    {
        return true;
    }
    return false;
}

int
main(void)
{
    char line[1024];

    double dt, mass = 0;

    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == '\n' || line[0] == '\0') continue;

        char op[6];
        int steps;
        vector_2d a, b, result;
        aabb box1, box2;
        double s;
        rigid_body_2d body;

        if (sscanf(line, "%5s", op) != 1) continue;

        switch (getCommand(op))
        {
            case ADD:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y) == 4)
                {
                    result = add_vector_2d(a, b);
                    print_vector_2d(result);
                }
                break;

            case SUB:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y) == 4)
                {
                    result = sub_vector_2d(a, b);
                    print_vector_2d(result);
                }
                break;

            case MUL:
                if (sscanf(line, "%*s %lf %lf %lf", &a.x, &a.y, &s) == 3)
                {
                    result = mul_vector_2d(a, s);
                    print_vector_2d(result);
                }
                break;

            case DOT:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y) == 4)
                {
                    double dot = dot_vector_2d(a, b);
                    printf("%.4f", dot);
                }
                break;

            case CROSS:
                if (sscanf(line, "%*s %lf %lf %lf %lf", &a.x, &a.y, &b.x, &b.y) == 4)
                {
                    double cross = cross_vector_2d(a, b);
                    printf("%.4f", cross);
                }
                break;

            case LEN:
                if (sscanf(line, "%*s %lf %lf", &a.x, &a.y) == 2)
                {
                    double len = length_vector_2d(a);
                    printf("%.4f", len);
                }
                break;

            case NORM:
                if (sscanf(line, "%*s %lf %lf", &a.x, &a.y) == 2)
                {
                    result = normalize_vector_2d(a);
                    print_vector_2d(result);
                }
                break;

            case DT:
                if (sscanf(line, "%*s %lf %*s %d %*s %lf", &dt, &steps, &mass) == 3)
                {}
                else if (sscanf(line, "%*s %lf %*s %lf", &dt, &mass) == 2)
                {}
                break;

            case STEP:
                if (sscanf(line,
                    "%*s %lf %lf %lf %lf %lf %lf",
                    &body.position.x, &body.position.y,
                    &body.velocity.x, &body.velocity.y,
                    &a.x, &a.y) == 6)
                {
                    body = step_body_symplectic(body, a, mass, dt);

                    printf("%.4f %.4f %.4f %.4f\n",
                        body.position.x,
                        body.position.y,
                        body.velocity.x,
                        body.velocity.y);
                }
                break;

            case INIT:
                if (sscanf(line, "%*s %lf %lf %lf %lf",
                            &body.position.x,
                            &body.position.y,
                            &body.velocity.x,
                            &body.velocity.y) == 4
                )
                {}
                break;

            case FORCE:
                if (sscanf(line, "%*s %lf %lf", &a.x, &a.y) == 2)
                {
                    int i;

                    rigid_body_2d explicit_body = body;
                    rigid_body_2d symplectic_body = body;

                    for (int i = 0; i < steps; i++ )
                    {
                        explicit_body = step_body_explicit(explicit_body, a, mass, dt);
                        symplectic_body = step_body_symplectic(symplectic_body, a, mass, dt);
                    }

                    printf("EXPLICIT %.4f %.4f %.4f %.4f\n",
                        explicit_body.position.x,
                        explicit_body.position.y,
                        explicit_body.velocity.x,
                        explicit_body.velocity.y);

                    printf("SYMPLECTIC %.4f %.4f %.4f %.4f\n",
                        symplectic_body.position.x,
                        symplectic_body.position.y,
                        symplectic_body.velocity.x,
                        symplectic_body.velocity.y);
                }
                break;

            default:
                if (sscanf(line,
                    "%lf %lf %lf %lf %lf %lf %lf %lf",
                    &box1.min_x, &box1.min_y,
                    &box1.max_x, &box1.max_y,
                    &box2.min_x, &box2.min_y,
                    &box2.max_x, &box2.max_y) == 8)
                {
                    is_aabb_overlap(box1, box2) ? printf("OVERLAP\n") : printf("NO_OVERLAP\n");
                }
                else
                {
                    printf("Invalid command\n");
                }
        }
    }

    return (0);
}
