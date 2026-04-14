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
--- Calcolatore CAS in C ---
Inserisci la funzione f (max 255 caratteri): sin(x)

[DEBUG] Albero Sintattico Astratto (AST) di partenza:

[sin]
 └─[x]


Variabili trovate nell'espressione: [x]
Rispetto a quale variabile vuoi operare? x

=====================================
 FUNZIONE: f = sin(x)
 DERIVATA: df/dx = cos(x)
 INTEGRALE: INT(f dx) = -1 * cos(x) + C
=====================================

--- CALCOLO NUMERICO ---
Quanto vale 'x'? 5

[RISULTATI]
Valore della funzione: f = -0.958924
Derivata parziale in quel punto: f' = 0.283662

--- CALCOLO AREA (INTEGRALE DEFINITO) ---
Vuoi calcolare l'area tra due estremi per la variabile 'x'? (s/n): n

Vuoi visualizzare il grafico della funzione? (s/n): n

Vuoi trovare in quale punto la funzione si azzera (f(x) = 0)? (s/n): s

--- CACCIATORE DI ZERI (Newton-Raphson) ---
Inserisci un punto di partenza (ipotesi iniziale) per x (es. 1.0): 0.5

Ricerca dello zero in corso...
Iterazione  1: x =  -0.046302 | f(x) =   0.479426
Iterazione  2: x =   0.000033 | f(x) =  -0.046286
Iterazione  3: x =  -0.000000 | f(x) =   0.000033
Iterazione  4: x =   0.000000 | f(x) =  -0.000000

[SUCCESSO] Zero trovato in un lampo!
La curva taglia l'asse X esattamente nel punto: x = 0

Vuoi approssimare la funzione con una Serie di Taylor (Maclaurin)? (s/n): s
Inserisci il grado del polinomio (es. 5): 5

[POLINOMIO DI TAYLOR GRADO 5]
T(x) = (x + -0.17 * x^3) + 0.01 * x^5

[SISTEMA] Pulizia della memoria in corso...
[SISTEMA] Memoria liberata con successo. Nessun leak rilevato.

```

## Future Roadmap
While the core engine is fully functional, the modular architecture was designed with scalability in mind. Planned future implementations include:
* **Complex Numbers Support:** Extending the AST and math engine to handle imaginary units (`i`) and complex arithmetic natively.
* **Symbolic Equation Solving:** Implementing algebraic isolation algorithms alongside the current numerical Newton-Raphson solver.
* **GUI / Visual Interface:** Upgrading the ASCII terminal plotter to a fully graphical window using C libraries like SDL2 or Raylib.
* **Linear Algebra:** Introducing matrices, vectors, and symbolic matrix operations (e.g., determinants, dot products).
