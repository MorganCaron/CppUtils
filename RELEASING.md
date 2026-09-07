# Release Process

This document describes the step-by-step checklist to release a new version of **CppUtils** and publish it to [xmake-repo](https://github.com/MorganCaron/xmake-repo).

## ⚙️ Initial Setup (One-Time)

To allow GitHub Actions to automatically update the [xmake-repo](https://github.com/MorganCaron/xmake-repo) repository when a new release is published, configure a Personal Access Token (PAT):

1. **Create a Fine-Grained Personal Access Token**:
   - Go to GitHub -> **Settings** -> **Developer Settings** -> **Personal access tokens** -> **Fine-grained tokens** (or [click here](https://github.com/settings/personal-access-tokens)).
   - Click **Generate new token**.
   - **Token name**: `CppUtils-xmake-repo-releaser`
   - **Expiration**: Select your preferred duration (e.g., 90 days or 1 year).
   - **Resource owner**: `MorganCaron`
   - **Repository access**: Select **Only select repositories** -> Choose `MorganCaron/xmake-repo`.
   - **Permissions**: Under **Repository permissions**, find **Contents** and set it to **Read and write**.
   - Click **Generate token** and copy the generated token.

2. **Add the Secret to CppUtils**:
   - In the `CppUtils` repository, go to **Settings** -> **Secrets and variables** -> **Actions** (or [click here](https://github.com/MorganCaron/CppUtils/settings/secrets/actions)).
   - Click **New repository secret**.
   - **Name**: `XMAKE_REPO_TOKEN`
   - **Secret**: Paste the PAT copied in the previous step.
   - Click **Add secret**.

---

## 🚀 Fast Release (Recommended: 1 Single Command)

To release a new version, run the release script from the repository root:

```bash
./scripts/release.sh 0.1.0
```

### What this command does:
1. **Local pre-flight checks**:
   - Verifies that you are on branch `master` and the working tree is clean.
   - Verifies the tag does not already exist locally or remotely.
   - Pulls the latest commits from `origin/master`.
2. **Local test run**:
   - Configures and runs unit tests (`xmake f --toolchain=llvm --runtimes="c++_shared" --enable_tests=y -y -q && xmake run CppUtils-UnitTests`).
3. **Version bumping**:
   - Updates `set_version("0.1.0", ...)` in [`xmake.lua`](xmake.lua).
   - Updates `add_requires("CppUtils 0.1.0")` in [`README.md`](README.md).
   - Prompts for confirmation showing the git diff.
4. **Git operations**:
   - Commits `chore: release v0.1.0`.
   - Creates the annotated Git tag `v0.1.0`.
   - Pushes both the commit and tag to `origin/master`.
5. **Continuous Deployment (GitHub Actions)**:
   - Publishes the GitHub Release with automated categorized release notes (`.github/release.yml`).
   - Downloads the generated tarball and computes its SHA-256 checksum.
   - Checks out `MorganCaron/xmake-repo` (using the `XMAKE_REPO_TOKEN` secret), updates `packages/c/cpputils/xmake.lua`, and commits/pushes to `master`.

---

## 📋 Manual Release Workflow (Fallback)

If you prefer to perform each step manually:

### 1. Pre-release verification
Ensure all unit tests pass locally on your development environment:
```bash
xmake f --toolchain=llvm --runtimes="c++_shared" --enable_tests=y -y
xmake run CppUtils-UnitTests
```
Ensure your working tree is clean (`git status`).

---

### 2. Update Version in CppUtils
1. Edit [`xmake.lua`](xmake.lua) line 2 to set the new version (Semantic Versioning `MAJOR.MINOR.PATCH`):
   ```lua
   set_version("0.1.0", {build = "%Y%m%d%H%M"})
   ```
2. *(Optional)* Update installation examples in [`README.md`](README.md) if referencing the specific version:
   ```lua
   add_requires("CppUtils 0.1.0")
   ```

---

### 3. Commit and Tag
1. Commit the version bump:
   ```bash
   git add xmake.lua README.md
   git commit -m "chore: bump version to 0.1.0"
   git push origin master
   ```

2. Create and push the Git tag:
   ```bash
   git tag v0.1.0
   git push origin v0.1.0
   ```

---

### 4. Update xmake-repo (Tarball & SHA-256 Checksum)
*(Only needed if `XMAKE_REPO_TOKEN` is not configured or CI is skipped)*

1. **Compute the SHA-256 checksum** of the GitHub release tarball:
   ```bash
   curl -sL https://github.com/MorganCaron/CppUtils/archive/refs/tags/v0.1.0.tar.gz | sha256sum | cut -d ' ' -f 1
   ```

2. **Update `packages/c/cpputils/xmake.lua`** in `xmake-repo`:
   ```lua
   add_versions("0.1.0", "<sha256_checksum_here>")
   ```

3. **Verify and push**:
   ```bash
   xrepo install --force CppUtils 0.1.0
   git add packages/c/cpputils/xmake.lua
   git commit -m "add CppUtils 0.1.0"
   git push origin master
   ```

---

## 🛠️ Rectifying a Mistake (Rollback)

If a tag or release was created incorrectly (wrong commit, wrong version number):

1. **Delete local tag**:
   ```bash
   git tag -d v0.1.0
   ```
2. **Delete remote tag on GitHub**:
   ```bash
   git push origin --delete v0.1.0
   ```
3. **Delete GitHub Release**:
   - Via GitHub web UI: go to **Releases** ➔ Select release ➔ Click **Delete release**.
   - Or via GitHub CLI: `gh release delete v0.1.0 -y`
4. Fix the issue, recreate the tag, and push again.
