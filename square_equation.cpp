#include "square_equation.h"

int comparison_on_zero_value (const double value)
{
    if (fabs(value) > EPS)
    {
        return NO_ZERO;
    }
    else
    {
        return ZERO;
    }
}

int solve_quadratic_equation (const double a, const double b, const double c, double *x1, double *x2)
{
    assert (isfinite(a));
    assert (isfinite(b));
    assert (isfinite(c));
    assert (x1);
    assert (x2);

    if (comparison_on_zero_value (a) == NO_ZERO)
    {
        if (comparison_on_zero_value (c) == ZERO)
        {
            *x2 = 0;
            int ammount_solutions = solve_linear_equation (a, b, x1) + ONE_SOLUTION;
            if (ammount_solutions == TWO_SOLUTIONS)
            {
                return TWO_SOLUTIONS;
            }
            else
            {
                if (*x1 == 0)
                {
                    return ONE_SOLUTION;
                }
                else
                {
                    return TWO_SOLUTIONS;
                }
            }
        }
        else
        {
            double discriminant = calc_discr (a, b, c);
            if (discriminant >= EPS)
            {
                double sqrt_d = sqrt (discriminant);
                *x1 = (-b - sqrt_d) / (2 * a);
                *x2 = (-b + sqrt_d) / (2 * a);
                return TWO_SOLUTIONS;
            }
            else if (comparison_on_zero_value (discriminant) == ZERO)
            {
                *x1 = - b / (2 * a);
                return ONE_SOLUTION;
            }
            else
            {
                return NO_SOLUTIONS;
            }
        }
    }
    else return solve_linear_equation (b, c, x1);
}

double calc_discr (const double a, const double b, const double c)
{
    assert (isfinite(a));
    assert (isfinite(b));
    assert (isfinite(c));

    double value_of_discr = b * b - 4 * a * c;

    return value_of_discr;
}

int solve_linear_equation (const double high_coef, const double constant, double* x1)
{
    assert (x1);
    assert (isfinite(high_coef));
    assert (isfinite(constant));

    if (comparison_on_zero_value (high_coef) == NO_ZERO)
    {
        if (comparison_on_zero_value (constant) == NO_ZERO)
        {
            *x1 = - constant / high_coef;
        }
        else
        {
            *x1 = 0;
        }
        return ONE_SOLUTION;
    }
    else
    {
        if (comparison_on_zero_value (constant) == ZERO)
        {
            return INFINITY_SOLUTIONS;
        }
        else
        {
            return NO_SOLUTIONS;
        }
    }
}

void output_solutions (const int ammount_solutions, const double x1, const double x2)
{
    switch (ammount_solutions)
    {
        case ONE_SOLUTION:
            printf ("%0.2lg\n", x1);
            break;
        case TWO_SOLUTIONS:
            printf ("%0.2lg %0.2lg\n", x1, x2);
            break;
        case NO_SOLUTIONS:
            printf ("No solutions\n");
            break;
        case INFINITY_SOLUTIONS:
            printf ("Infinity solutions\n");
            break;
        default:
            printf ("unexpected ammount solution\n");
            break;
    }
}

int input_three_coef (double* a, double* b, double* c)
{
    int ammount_successfully_scaned_value = scanf ("%lg %lg %lg", a, b, c);

    if (ammount_successfully_scaned_value == 3)
    {
        return CORRECT;
    }
    else
    {
        return UNCORRECT;
    }
}

void output_error (const int input_status)
{
    if (input_status == UNCORRECT)
    {
        printf ("ERROR : uncorrect input\n");
    }
}


void clear_stdin ()
{
    while (getchar() != '\n');
}
