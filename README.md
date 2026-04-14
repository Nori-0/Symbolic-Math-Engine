# CAS-C: A Symbolic Computer Algebra System in Pure C

CAS-C is a lightweight, fully modular Computer Algebra System built entirely from scratch in pure C. It features a custom Lexer, an Abstract Syntax Tree (AST) Parser based on the Shutting-Yard algorithm, and a powerful Symbolic Math Enginge capable of exact differentiation, simplification, ad integration.

No external libraries were used (except the standard C `<math.h>`and `<stdio.h>`. This project started as a personal challenge, to test my programming skills, and it gradually evolved into a more advanced idea than I initially thought.

## Features
* **Symbolic Differentiation:** Computes exact derivatives (including product, quotient, and chain rules) for multi-variable expressions;
* **Symbolic Integration:** Solves indefinite integrals for polynomials and basic transcendental functions;
* **Algebraic Optimizer:** Canonizes equations (AST sorting) and prunes redundant branches (e.g., `x/x = 1`, `A - A = 0`);
* **Newton-Raphson Root Finder:** Uses the exact symbolic derivative to find roots of complex non-linear equations with high precision;
* **Taylor Series Generator:** Expands any function into a Maclaurin polynomial of a user-defined degree;
* **ASCII Plotter:** Renders functions directly in the terminal using Bresenham's Line Algorithm for continuous curves.

## Architecture
The project follows a strict separation of concerns, divided into specialized modules:
* `lexer.c/h`: Tokenizes the mathematical string input;
* `parser.c/h`: Builds the AST using Dijkstra's Shunting-Yard algorithm;
* `ast.c/h`: Defines tree structures and unparsing (equation printing) logic;
* `math_engine.c/h`: The core symbolic AI (Derivatives, Integrals, Optimizations);
* `plotter.c/h`: Evaluates and maps the continuous function to a discrete terminal grid;
* `solver.c/h`: Numerical analysis using Newton's method;
* `taylor.c/h`: Recursive derivative application for polynomial approximations.

## Under the Hood (How it Works)
The engine processes mathematical expressions through a classic compiler frontend pipeline:
1. **Lexical Analysis (`lexer.c`):** Scans the raw string and converts characters into a stream of mathematically meaningful Tokens (Numbers, Variables, Functions, Operators);
2. **Parsing (`parser.c`):** Utilizes Edsger W. Dijkstra's **Shunting-Yard algorithm** to resolve operator precedence and associativity, converting the infix token stream into a postfix logic;
3. **AST Generation (`ast.c`):** Constructs a multidimensional Abstract Syntax Tree;
4. **Symbolic Execution (`math_engine.c`):** Recursively traverses the AST to evaluate, derive, integrate, or optimize the mathematical structures.

## Quick Start
### Compilation
The project includes a `Makefile` for easy compilation. Just run:
```bash
make
```
### Exectuion
```bash
./cas
```
### Usage Example
```Plaintext
--- C Symbolic CAS Engine ---
Enter function f(x) : sin(x)

[DEBUG] Starting Abstract Syntax Tree (AST):

[sin]
 └─[x]


Variables found in the expression: [x]
Target variable for calculus: x

=====================================
 FUNCTION: f = sin(x)
 DERIVATIVE: df/dx = cos(x)
 INTEGRAL: INT(f dx) = -1 * cos(x) + C
=====================================

--- NUMERICAL CALCULATION ---
What is the value of 'x'? 15

[RESULTS]
Function value: f = 0.650288
Partial derivative at that point: f' = -0.759688

--- AREA CALCULATION (DEFINITE INTEGRAL) ---
Do you want to calculate the area between two limits for variable 'x'? (y/n): y
Enter the lower limit (a): 1
Enter the upper limit (b): 2

[DEFINITE INTEGRAL RESULT]
Area under the curve between 1 and 2: 0.956449

Do you want to find the function graph ? (y/n): y

--- ASCII PLOTTER ---
Enter lower bound for x (e.g. -10): -10
Enter upper bound for x (e.g. 10): 10

f(x) Graph in [-10.00, 10.00]. Range Y: [-1.00, 1.00]

                  **               |   **                    **
                 *  *              |   * *                  *  *
                *   *              |  *   *                 *   *
                *    *             |  *    *               *    *
               *     *             | *     *               *     *
*              *      *            | *     *              *      *
*              *      *            |*       *             *      *
 *            *       *            |*       *             *       *
 *            *        *           |*       *            *        *
 *            *        *           *         *           *        *
 *           *         *           *         *           *        *
  *          *         *           *         *           *         *
--*----------*----------*----------*---------*----------*----------*--
  *          *          *         *|          *         *          *
   *        *           *         *|          *         *           *
   *        *            *        *|          *        *            *
   *        *            *       * |           *       *            *
    *      *             *       * |           *       *             *
    *      *             *       * |           *      *              *
    *      *              *     *  |            *     *              *
     *    *               *     *  |            *     *
     *    *                *    *  |             *   *
      *   *                *   *   |             *   *
      *  *                  *  *   |              * *
       **                    **    |              **

Do you want to find where the function equals zero (f(x) = 0)? (y/n): y

--- ROOT FINDER (Newton-Raphson) ---
Enter a starting point (initial guess) for x (e.g., 1.0): 1

Searching for root...
Iteraction number  1: x =  -0.557408 | f(x) =   0.841471
Iteraction number  2: x =   0.065936 | f(x) =  -0.528988
Iteraction number  3: x =  -0.000096 | f(x) =   0.065889
Iteraction number  4: x =   0.000000 | f(x) =  -0.000096
Iteraction number  5: x =   0.000000 | f(x) =   0.000000

[SUCCESS] Root found in a flash!
The curve intersects the X-axis exactly at point: x = 0

Do you want to approximate the function with a Taylor (Maclaurin) Series? (y/n): y
Enter the degree of the polynomial (e.g. 5): 5

[TAYLOR POLYNOMIAL OF DEGREE 5]
T(x) = (x + -0.17 * x^3) + 0.01 * x^5

[SYSTEM] Memory cleanup in progress...
[SYSTEM] Memory successfully freed. No leaks detected.
```

## Future Roadmap
While the core engine is fully functional, the modular architecture was designed with scalability in mind. Planned future implementations include:
* **Complex Numbers Support:** Extending the AST and math engine to handle imaginary units (`i`) and complex arithmetic natively.
* **Symbolic Equation Solving:** Implementing algebraic isolation algorithms alongside the current numerical Newton-Raphson solver.
* **GUI / Visual Interface:** Upgrading the ASCII terminal plotter to a fully graphical window using C libraries like SDL2 or Raylib.
* **Linear Algebra:** Introducing matrices, vectors, and symbolic matrix operations (e.g., determinants, dot products).
