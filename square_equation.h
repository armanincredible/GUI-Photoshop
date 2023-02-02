#ifndef SQAURE_EQUATION_H
#define SQAURE_EQUATION_H

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/**
 * @brief constant value of epsilon
 *
 */
static const double EPS = 10e-6;

/**
 * @brief
 *
 */
enum correct_uncorrect
{
    UNCORRECT,
    CORRECT
};

/**
 * @brief zero or not zero value
 *
 */
enum zero_value
{
    NO_ZERO,
    ZERO
};

/**
 * @brief ammoont sollutions of equation
 *
 */
enum number_solutions
{
    NO_SOLUTIONS,
    ONE_SOLUTION,
    TWO_SOLUTIONS,
    INFINITY_SOLUTIONS
};

/**
 * @brief compare on zero value
 *
 * @param value argument of function
 * @return int ZERO or NO_ZERO
 */
int comparison_on_zero_value (const double value);

/**
 * @brief solve quadratic equation
 *
 * @param a coefficient of x^2
 * @param b coefficien of x
 * @param c free coefficient
 * @param first_root first root of equation
 * @param second_root second root of equation
 * @return int ammount sollutions
 */
int solve_quadratic_equation (const double a, const double b, const double c,
                              double* first_root, double* second_root);

/**
 * @brief solve linear equation
 * solve linear equation if high coefficient has zero value
 *
 * @param high_coef
 * @param constant
 * @param first_root
 * @return int ammount sollutions
 */
int solve_linear_equation (const double high_coef, const double constant, double* first_root);

/**
 * @brief count three coefficients
 * count three coefficients and check correct of input. If input uncorrect you can corect input. If input uncorrect function return it.
 *
 * @param first_coef coefficient of x^2
 * @param second_coef coefficient of x
 * @param third_coef free coefficient
 * @return int correct or uncorrect input
 */
int input_three_coef (double* first_coef, double* second_coef, double* third_coef);

/**
 * @brief ouput solutions
 *
 * @param ammount_solutions ammount solutions of equation
 * @param first_root
 * @param second_root
 */
void output_solutions (const int ammount_solutions, const double first_root, const double second_root);

/**
 * @brief calculate value of discriminant
 *
 * @param a
 * @param b
 * @param c
 * @return double value of discriminant
 */
double calc_discr (const double a, const double b, const double c);

/**
 * @brief output error
 * if program has error function output it
 *
 * @param type_error
 */
void output_error (const int type_error);

/**
 * @brief unit test
 *
 * @param a
 * @param b
 * @param c
 * @param rl_n_roots correct ammount solutions
 * @param rl_first_root coreect value of first root
 * @param rl_second_root corrext value of second root
 * @param ammount number of test
 * @return int correct or uncorrect values of roots
 */
int unit_test (const double a, const double b, const double c,
               int rl_n_roots, const double rl_first_root, const double rl_second_root,
               const int ammount);

/**
 * @brief unit test run
 * function
 *
 * @return int
 */
int run_unit_test (void);

/**
 * @brief clear stdin
 *
 */
void clear_stdin (void);

#endif // SQAURE_EQUATION_H
