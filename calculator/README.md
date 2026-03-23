Polynomial Calculator & Grapher (C++ / SFML)
Project objective

Build a calculator that parses and evaluates arithmetic expressions (with decimals, parentheses, correct precedence) and extends into a polynomial grapher using SFML. Emphasize C++ features: pointers, std::unique_ptr / std::shared_ptr, templates, operator overloading, inheritance (AST), and a lightweight ECS-style UI/graph layer. Provide history with in-memory and on-disk persistence.
Repository layout (recommended)

    calculator/
        main.cpp
        README.md
        core/
            expression.h / expression.cpp — AST node types and evaluation
            parser.h / parser.cpp — tokenizer + shunting-yard -> AST
            calc_api.h / calc_api.cpp — parsing/evaluation API and History
            polynomial.h / polynomial.cpp — templated Polynomial (future)
            utils.h / utils.cpp — helpers (pretty print, number parsing)
        ui/
            entity.h / entity.cpp
            components.h / components.cpp
            systems.h / systems.cpp
        assets/ — fonts, icons, sample data
        build/ — local build output (ignored in VCS)
    CMakeLists.txt — top-level CMake to build calculator and other projects

What we have implemented

    Core parsing pipeline (tokenize -> shunting-yard -> AST):
        Supports integers and decimals
        Operators: + - * / ^ and parentheses with correct precedence and associativity
        AST node types: NumberNode, BinaryOpNode (std::unique_ptr ownership)
        Evaluation via AST::evaluate(EvalContext)
    Calculator API:
        parseExpressionAPI(input) -> AST unique_ptr or error
        evaluateAST(ast, x) -> EvalResult
    History class:
        Stores last N expressions as std::shared_ptr
        In-memory ring-like buffer behavior (drop oldest when full)
        Simple persistence: persist(path) writes expressions line-by-line; restore(path) reparses lines
    Minimal CLI main.cpp demonstrating parse/evaluate/history + pretty output formatting
    Code organizes into core/ files suitable for later expansion

Design decisions & rationale

    AST nodes use std::unique_ptr to enforce exclusive ownership during parse/build; History stores std::shared_ptr for shared access across systems and persistence.
    Parser uses shunting-yard to produce correct precedence and supports exponentiation (right-associative).
    History persistence is plain text for simplicity and portability; easily swapped for JSON later.
    Hybrid architecture: core math (templates, operator overloading, polymorphic AST) kept separate from UI/graph systems; UI will use an ECS-like pattern for renderable entities and interactive systems.

Short-term roadmap (next milestones)

    Refactor & tests
        Add unit tests for parser, AST evaluation, and pretty printing (Catch2 recommended).
        Add better error messages and stricter divide-by-zero checks.
    Polynomial class
        Implement templated Polynomial with operator overloading (+, -, *, scalar ops), evaluate(T), derivative(), and stream output.
        Integrate Polynomial construction from AST where feasible (recognize polynomial patterns).
    Parser improvements
        Add unary minus handling and implicit multiplication (e.g., "2(x+1)").
        Add variables (x) support and function nodes (sin, cos, exp).
    Enhanced History
        Serialize history with explicit meta (timestamp, name) — consider JSON (nlohmann/json).
        Add on-demand saving/auto-save, and import/export.
    Minimal SFML UI & ECS
        Create EntityManager, Components (Transform, Renderable, PlotConfig, InputBuffer), Systems (Input, Parse, Eval, Render, Interaction).
        Implement Plotter system that samples AST/shared function and produces vertex arrays for SFML.
    Advanced features
        Adaptive sampling for plotting (refine where curvature is high).
        Root finding (Newton / bisection), solving linear/quadratic equations from parsed expressions.
        Multiple graphs, color/legend, pan/zoom, save screenshots.
        Memory manager demo: custom allocator or pooled allocator for AST nodes (optional exercise).

Long-term / stretch goals

    Implement a custom memory pool to practice manual memory management (compare with smart pointers).
    Replace simple persistence with an indexed snapshot format for fast restore.
    Add sliders for parameterized polynomials and animated plots.
    Cross-platform builds: finalize CMake with per-target options for Linux/Windows; optionally provide a simple installer.

Integration & build notes

    Current code targets C++17 (g++/clang++ and MSVC with /std:c++17).
    For VS Code debugging, keep launch/tasks configured to calculator/ build directory.
    When ready, add a CMakeLists.txt in calculator/ and top-level CMake to conditionally include this project.
    Dependencies planned: SFML (for UI/graphing). Keep core code dependency-free to ease unit testing.

How to proceed (recommended next actions)

    Add unit tests for parser and AST to lock down behavior.
    Implement Polynomial (core/polynomial.*) and add small CLI commands to create and evaluate polynomials.
    Implement unary minus and implicit multiplication in parser.
    Start minimal SFML window and render a sampled plot from an expression attached to an entity.

Notes & caveats

    Current parser does not yet handle unary minus or implicit multiplication — add these early.
    Division by zero is not specially guarded in evaluation; add runtime checks or safe-evaluate wrappers.
    History restore reparses expressions; if grammar changes later, restoration may fail for older entries — consider versioned persistence.
