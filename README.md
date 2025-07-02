# Catalyst ![Libraries.io dependency status for GitHub repo](https://img.shields.io/librariesio/github/Disservin/chess-library)

Catalyst is a chess engine built using C++ with [Chess Library](https://github.com/Disservin/chess-library).

This is more or less a personal project for attempting to create a powerful C++ chess engine.

## Table of Contents
- [Catalyst ](#catalyst-)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [Contributing](#contributing)
  - [Authors](#authors)
  - [Acknowledgements](#acknowledgements)
  - [License](#license)

## Features

> ***Note:*** *Most of these features are either planned or not implemented.*
> *Features that are implemented will have a checkmark next to their names.*

* Engine search
  * [x] Iterative deepening.
  * [x] Fail-soft Negamax with alpha/beta pruning.
  * [x] Transposition tables.
  * [x] Move ordering.
    * [x] TT/Hash-Move.
    * [x] MVV-LVA.
    * [x] Promotions.
    * [x] Killer heuristic.
    * [x] History heuristic.
    * [ ] Countermove heuristic.
  * [x] Quiescence Search.
* Engine evaluation
  * [x] Board material.
  * [x] Piece square tables.
  * [ ] Tapered eval.
  * [ ] Mop-up eval.
  * [ ] Piece mobility.
  * [ ] Piece evaluation.

## Requirements

> Compiler must be a version compatible with C++20

- GCC, Clang, MSVC
- CMake (>= Version 3.13)

## Installation

```sh
git clone --recurse-submodules https://github.com/noodle02846/catalyst.git
cd catalyst
cmake -B build
cmake --build build --config Release --target Catalyst --
```

## Contributing

Open a pull request with a list of changes. If they improve/optimize the engine, I'll add them.

## Authors

* **Noodle** - [@noodle02846](https://github.com/noodle02846)

## Acknowledgements

Libraries used:
* [Chess Library](https://github.com/Disservin/chess-library)

## License

This project should considered open-source software and is licensed with [MIT](https://tlo.mit.edu/understand-ip/exploring-mit-open-source-license-comprehensive-guide) license. See [LICENSE](LICENSE) for more information.