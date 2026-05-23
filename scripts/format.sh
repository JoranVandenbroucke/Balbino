#!/usr/bin/env bash

set -euo pipefail

TARGET_DIR="${1:-.}"
EXTENSIONS=("cpp" "hpp" "ixx")
CLANG_FORMAT="${CLANG_FORMAT:-clang-format}"
DRY_RUN=false
VERBOSE=false

usage() {
    echo "Usage: $0 [directory] [options]"
    echo ""
    echo "Options:"
    echo "  -n, --dry-run    Show files that would be formatted without modifying them"
    echo "  -v, --verbose    Print each file as it's formatted"
    echo "  -h, --help       Show this help message"
    echo ""
    echo "Environment:"
    echo "  CLANG_FORMAT     Path to clang-format binary (default: clang-format)"
    exit 0
}

# Parse args (skip first positional if it's a directory)
ARGS=("$@")
for i in "${!ARGS[@]}"; do
    case "${ARGS[$i]}" in
        -n|--dry-run) DRY_RUN=true ;;
        -v|--verbose) VERBOSE=true ;;
        -h|--help) usage ;;
    esac
done

if [[ ! -d "$TARGET_DIR" ]]; then
    echo "Error: '$TARGET_DIR' is not a directory." >&2
    exit 1
fi

if ! command -v "$CLANG_FORMAT" &>/dev/null; then
    echo "Error: '$CLANG_FORMAT' not found. Install clang-format or set CLANG_FORMAT env var." >&2
    exit 1
fi

CLANG_FORMAT_VERSION=$("$CLANG_FORMAT" --version 2>&1 | head -n1)

# Build find expression for all extensions
FIND_ARGS=()
for i in "${!EXTENSIONS[@]}"; do
    [[ $i -gt 0 ]] && FIND_ARGS+=(-o)
    FIND_ARGS+=(-name "*.${EXTENSIONS[$i]}")
done

mapfile -t FILES < <(find "$TARGET_DIR" \( "${FIND_ARGS[@]}" \) -type f | sort)

FILE_COUNT=${#FILES[@]}

if [[ $FILE_COUNT -eq 0 ]]; then
    echo "No matching source files found in '$TARGET_DIR'."
    exit 0
fi

echo "clang-format: $CLANG_FORMAT_VERSION"
echo "Directory:    $TARGET_DIR"
echo "Files found:  $FILE_COUNT"
$DRY_RUN && echo "(dry-run mode — no files will be modified)"
echo ""

FORMATTED=0
SKIPPED=0

for FILE in "${FILES[@]}"; do
    if $DRY_RUN; then
        echo "  [dry-run] $FILE"
        (( FORMATTED++ )) || true
    else
        $VERBOSE && echo "  Formatting: $FILE"
        if "$CLANG_FORMAT" -i "$FILE"; then
            (( FORMATTED++ )) || true
        else
            echo "  Warning: clang-format failed on '$FILE'" >&2
            (( SKIPPED++ )) || true
        fi
    fi
done

echo ""
echo "Done. Formatted: $FORMATTED  |  Skipped/failed: $SKIPPED"
