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

2. **Add the Secret to the `release` environment** (not as a repository secret, so only approved release jobs can read it):
   - In the `CppUtils` repository, go to **Settings** -> **Environments** -> `release` -> **Add environment secret**.
   - **Name**: `XMAKE_REPO_TOKEN`
   - **Value**: Paste the PAT copied in the previous step.

---

## 🚀 Releasing a Version

The **Git tag is the single source of truth** for the version: no file contains the version number, and **no commit is created by a release**.

1. Open the [**CD: Release** workflow page](https://github.com/MorganCaron/CppUtils/actions/workflows/cd-cpp.yml), click **Run workflow** (on branch `master`), and enter the version (e.g. `0.2.0`).
   Or from the command line:
   ```bash
   gh workflow run cd-cpp.yml --ref master -f version=0.2.0
   ```
2. Approve the `release` environment deployment when GitHub asks for it.

### What the workflow does
1. **Checks**: it must run from `master`, the version must be a valid Semantic Version (`X.Y.Z`), and the tag `vX.Y.Z` must not exist yet.
2. **Tests**: CI Linux, macOS and Windows run on the current head of `master`.
3. **Tag and Release**: after approval, it creates the tag `vX.Y.Z` on that commit and publishes the GitHub Release with categorized notes (`.github/release.yml`).
4. **xmake-repo**: it downloads the tag tarball, computes its SHA-256, and updates `packages/c/cpputils/xmake.lua` in `MorganCaron/xmake-repo` (using `XMAKE_REPO_TOKEN`).

