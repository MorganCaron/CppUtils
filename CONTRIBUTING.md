# [CppUtils](README.md) / Contributing

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](README.md)

The project becomes better for everyone when people like you help make it better!

Have any questions or concerns? Did I forget something? Feel free to submit an issue or pull request.

Thanks for taking the time to contribute :)

---

## Development Setup

### Build Prerequisites

- Modern C++26 compliant toolchain with C++26 Standard Library Module support (LLVM / Clang ≥ 19)
- [XMake](https://xmake.io/) (v2.8+)

Always configure the project using LLVM and shared C++ runtime:
```console
xmake f --toolchain=llvm --runtimes="c++_shared"
```

To configure with unit tests enabled:
```console
xmake f --toolchain=llvm --runtimes="c++_shared" --enable_tests=y
```

---

## Development Workflow

1. Create a new branch from `master`.
2. Format branch names in `kebab-case`. Prefixing with a type (such as `feat/`, `fix/`, `ci/`, etc. See [Commit Messages](#commit-messages--pull-request-titles) below) is recommended, but optional (e.g., `feat/thread-pool`, `fix/null-pointer`).
3. Follow the [Conventional Commits](https://www.conventionalcommits.org/) convention for all commits and Pull Request titles (e.g., `feat: add thread pool`, `fix(logger): resolve deadlock`).
4. Push your branch and open a Pull Request targeting `master`.
5. Ensure all CI checks pass before merging.

### Commit Messages & Pull Request Titles

Both commit messages and Pull Request titles must follow the [Conventional Commits](https://www.conventionalcommits.org/) specification format: `<type>[optional scope]: <description>` (e.g., `feat: add thread pool`, `fix(logger): resolve deadlock`).

Pull Request titles are automatically parsed by the CI to assign repository labels and categorize entries in the release changelog.

Supported and common types:

- `feat`: A new feature.
- `fix`: A bug fix.
- `docs`: Documentation updates only (e.g., README, code comments).
- `refactor`: Code refactoring that neither fixes a bug nor adds a feature.
- `perf`: A code change that improves performance.
- `test`: Adding missing tests or correcting existing tests.
- `build`: Changes that affect the build system or dependencies (e.g., `xmake.lua`).
- `ci`: Changes to CI/CD configurations and scripts (e.g., GitHub Actions workflows).
- `style`: Formatting or style adjustments that do not affect code logic (e.g., `.clang-format`).
- `chore`: Miscellaneous maintenance tasks, tooling updates, or release preparation.
- `revert`: Reverting a previous commit.

For breaking changes, append a `!` after the type/scope (e.g., `feat!: change function signature` or `feat(api)!: change function signature`).

---

## Testing & Validation

Before submitting a pull request, ensure that the project builds and all unit tests pass:

1. Configure the build with test suite enabled:
   ```console
   xmake f --toolchain=llvm --runtimes="c++_shared" --enable_tests=y -y
   ```
2. Run the test suite:
   ```console
   xmake run CppUtils-UnitTests
   ```
3. (Optional) Run tests continuously in watch mode during development:
   ```console
   xmake watch -r CppUtils-UnitTests
   ```

---

## Maintainers

For the release and versioning process, see [RELEASING.md](RELEASING.md).
