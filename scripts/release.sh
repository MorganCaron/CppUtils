#!/usr/bin/env bash
set -euo pipefail

# Ensure script is run from project root
project_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${project_root}"

# Display usage
function display_usage() {
    echo "Usage: $0 <version> [--yes]"
    echo "Example: $0 0.1.0"
    echo ""
    echo "Options:"
    echo "  --yes, -y    Skip confirmation prompt"
    echo "  --help, -h   Show this help message"
}

auto_confirm=false
target_version=""

for argument in "$@"; do
    case "${argument}" in
        --yes|-y)
            auto_confirm=true
            ;;
        --help|-h)
            display_usage
            exit 0
            ;;
        *)
            if [[ -z "${target_version}" ]]; then
                target_version="${argument#v}"
            else
                echo "Error: Unexpected argument '${argument}'" >&2
                display_usage
                exit 1
            fi
            ;;
    esac
done

if [[ -z "${target_version}" ]]; then
    echo "Error: Missing target version." >&2
    display_usage
    exit 1
fi

# Validate semantic version format (e.g. 0.1.0 or 1.0.0-rc1)
if [[ ! "${target_version}" =~ ^[0-9]+\.[0-9]+\.[0-9]+(-[0-9A-Za-z.-]+)?(\+[0-9A-Za-z.-]+)?$ ]]; then
    echo "Error: Target version '${target_version}' is not a valid Semantic Version (expected X.Y.Z)." >&2
    exit 1
fi

tag_name="v${target_version}"

echo "=================================================="
echo " Preparing release for CppUtils ${tag_name}"
echo "=================================================="

# 1. Git branch check
current_branch="$(git symbolic-ref --short HEAD 2>/dev/null || echo "")"
if [[ "${current_branch}" != "master" ]]; then
    echo "Error: You must be on the 'master' branch to release (current: '${current_branch}')." >&2
    exit 1
fi

# 2. Git working tree clean check
if ! git diff --quiet || ! git diff --cached --quiet; then
    echo "Error: Working directory has uncommitted changes. Please commit or stash them first." >&2
    exit 1
fi

# 3. Check for existing tag
if git rev-parse "${tag_name}" >/dev/null 2>&1; then
    echo "Error: Tag '${tag_name}' already exists locally." >&2
    exit 1
fi
if git ls-remote --tags origin "${tag_name}" | grep -q "${tag_name}"; then
    echo "Error: Tag '${tag_name}' already exists on remote origin." >&2
    exit 1
fi

# 4. Pull latest changes
echo "[1/5] Pulling latest changes from origin/master..."
git pull --ff-only origin master

# 5. Run Unit Tests locally
echo "[2/5] Running unit tests with Clang / libc++..."
xmake f --toolchain=llvm --runtimes="c++_shared" --enable_tests=y -y -q
xmake run CppUtils-UnitTests

# 6. Update version numbers in files
echo "[3/5] Updating version numbers in xmake.lua and README.md..."
python3 - << EOF
import re

# Update xmake.lua
with open("xmake.lua", "r") as file_handle:
    xmake_content = file_handle.read()

updated_xmake = re.sub(
    r'set_version\("[^"]*"',
    f'set_version("${target_version}"',
    xmake_content,
    count=1
)

with open("xmake.lua", "w") as file_handle:
    file_handle.write(updated_xmake)

# Update README.md if present
try:
    with open("README.md", "r") as file_handle:
        readme_content = file_handle.read()

    updated_readme = re.sub(
        r'add_requires\("CppUtils [^"]*"\)',
        f'add_requires("CppUtils ${target_version}")',
        readme_content,
        count=1
    )

    with open("README.md", "w") as file_handle:
        file_handle.write(updated_readme)
except FileNotFoundError:
    pass
EOF

# Show diff
echo "--------------------------------------------------"
git diff
echo "--------------------------------------------------"

# 7. Prompt confirmation
if [[ "${auto_confirm}" != true ]]; then
    read -r -p "Commit, tag (${tag_name}), and push to origin? [y/N] " confirmation_response
    normalized_confirmation_response="$(echo "${confirmation_response}" | tr -d " '\r\t\"" | tr '[:upper:]' '[:lower:]')"
    case "${normalized_confirmation_response}" in
        y|yes)
            ;;
        *)
            echo "Release aborted. Reverting version changes..."
            git checkout -- xmake.lua README.md
            exit 1
            ;;
    esac
fi

# 8. Commit and push
echo "[4/5] Creating release commit and tag..."
git add xmake.lua README.md
git commit -m "chore: release ${tag_name}"
git tag -a "${tag_name}" -m "Release ${tag_name}"

echo "[5/5] Pushing commit and tag to GitHub..."
git push origin master
git push origin "${tag_name}"

echo "=================================================="
echo " ✅ Release ${tag_name} pushed successfully!"
echo " GitHub Actions CD will now:"
echo "   1. Create the GitHub Release with automated notes"
echo "   2. Compute the release tarball SHA-256"
echo "   3. Update MorganCaron/xmake-repo automatically"
echo "=================================================="
