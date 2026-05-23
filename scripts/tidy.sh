#!/usr/bin/env bash

set -euo pipefail

# ── Defaults ──────────────────────────────────────────────────────────────────
TARGET_DIR="."
EXTENSIONS=("cpp" "ixx")   # TUs only; headers checked transitively
CLANG_TIDY="${CLANG_TIDY:-clang-tidy}"
COMPILE_COMMANDS=""
CONFIG_FILE=""
EXTRA_ARGS=""
JOBS=$(nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 4)
DRY_RUN=false
VERBOSE=false

# ── Usage ─────────────────────────────────────────────────────────────────────
usage() {
    cat <<EOF
Usage: $0 [directory] [options]

Options:
  -p, --compile-commands <path>  Path to compile_commands.json or its directory
  -c, --config-file <path>       Path to a .clang-tidy config file
  -j, --jobs <n>                 Parallel jobs (default: nproc → $JOBS)
  -n, --dry-run                  Diagnose only; do not apply any fixes
  -v, --verbose                  Print each file as it is processed
  --extra-args <flags>           Extra compiler flags (quoted, e.g. "-Imy/path")
  -h, --help                     Show this help

Environment:
  CLANG_TIDY   Path to the clang-tidy binary (default: clang-tidy)

Notes:
  Applies --fix and --fix-errors so clang-tidy writes all safe fixits.
  Without -c, clang-tidy walks up the tree from each TU to find .clang-tidy.
  With -c, that file is used for every TU regardless of location.
  A compile_commands.json is strongly recommended; without one include paths
  may be wrong. Generate one with: cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
  The date shown in the header is read from a comment of the form:
    # Date: YYYY-MM-DD
  at the top of the .clang-tidy file.
EOF
    exit 0
}

# ── Argument parsing ──────────────────────────────────────────────────────────
POSITIONAL_DONE=false
i=1
while [[ $i -le $# ]]; do
    arg="${!i}"
    case "$arg" in
        -h|--help)             usage ;;
        -n|--dry-run)          DRY_RUN=true ;;
        -v|--verbose)          VERBOSE=true ;;
        -p|--compile-commands) i=$(( i+1 )); COMPILE_COMMANDS="${!i}" ;;
        -c|--config-file)      i=$(( i+1 )); CONFIG_FILE="${!i}" ;;
        -j|--jobs)             i=$(( i+1 )); JOBS="${!i}" ;;
        --extra-args)          i=$(( i+1 )); EXTRA_ARGS="${!i}" ;;
        -*)  echo "Error: unknown option '$arg'" >&2; exit 1 ;;
        *)
            if ! $POSITIONAL_DONE; then
                TARGET_DIR="$arg"
                POSITIONAL_DONE=true
            fi ;;
    esac
    i=$(( i+1 ))
done

# ── Validate inputs ───────────────────────────────────────────────────────────
[[ -d "$TARGET_DIR" ]] || { echo "Error: '$TARGET_DIR' is not a directory." >&2; exit 1; }

command -v "$CLANG_TIDY" &>/dev/null \
    || { echo "Error: '$CLANG_TIDY' not found. Set CLANG_TIDY or install clang-tidy." >&2; exit 1; }

# ── Helpers ───────────────────────────────────────────────────────────────────

# Extract the value of a  # Date: <value>  comment from a .clang-tidy file.
# Matching is case-insensitive and tolerates arbitrary surrounding whitespace.
# Returns the trimmed value, or empty string if the comment is absent.
extract_config_date() {
    local file="$1"
    grep -m1 -iE '^[[:space:]]*#[[:space:]]*Date[[:space:]]*:' "$file" 2>/dev/null \
        | sed -E 's/^[[:space:]]*#[[:space:]]*[Dd][Aa][Tt][Ee][[:space:]]*:[[:space:]]*//' \
        | sed 's/[[:space:]]*$//'
}

# Walk up from TARGET_DIR to find the .clang-tidy clang-tidy would discover
# on its own (used only when -c is not given, purely for display).
find_nearest_clang_tidy() {
    local dir
    dir="$(realpath "$TARGET_DIR")"
    while [[ "$dir" != "/" ]]; do
        [[ -f "$dir/.clang-tidy" ]] && { echo "$dir/.clang-tidy"; return 0; }
        dir="$(dirname "$dir")"
    done
    return 1
}

# ── Resolve config file ───────────────────────────────────────────────────────
CONFIG_DATE=""

if [[ -n "$CONFIG_FILE" ]]; then
    [[ -f "$CONFIG_FILE" ]] \
        || { echo "Error: config file '$CONFIG_FILE' not found." >&2; exit 1; }
    CONFIG_FILE="$(realpath "$CONFIG_FILE")"
    CONFIG_DATE="$(extract_config_date "$CONFIG_FILE")"
else
    # Not passed explicitly — find the one clang-tidy would naturally use,
    # just so we can show its date in the header.
    DISCOVERED=""
    DISCOVERED="$(find_nearest_clang_tidy)" || true
    if [[ -n "$DISCOVERED" ]]; then
        CONFIG_DATE="$(extract_config_date "$DISCOVERED")"
    fi
fi

# ── Resolve compile_commands ──────────────────────────────────────────────────
if [[ -z "$COMPILE_COMMANDS" ]]; then
    for candidate in \
        "$TARGET_DIR/compile_commands.json" \
        "build/compile_commands.json" \
        "out/compile_commands.json" \
        "cmake-build-debug/compile_commands.json" \
        "cmake-build-release/compile_commands.json"
    do
        if [[ -f "$candidate" ]]; then
            COMPILE_COMMANDS="$(realpath "$(dirname "$candidate")")"
            echo "Info: Found compile_commands.json → $COMPILE_COMMANDS"
            break
        fi
    done
fi

if [[ -n "$COMPILE_COMMANDS" ]]; then
    [[ -f "$COMPILE_COMMANDS" ]] && COMPILE_COMMANDS="$(realpath "$(dirname "$COMPILE_COMMANDS")")"
    [[ -d "$COMPILE_COMMANDS" ]] \
        || { echo "Error: compile_commands path '$COMPILE_COMMANDS' not found." >&2; exit 1; }
    COMPILE_COMMANDS="$(realpath "$COMPILE_COMMANDS")"
else
    echo "Warning: No compile_commands.json found — results may be incomplete." >&2
    echo "         Use -p, or generate one with: cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON" >&2
fi

# ── Collect source files ──────────────────────────────────────────────────────
FIND_ARGS=()
for idx in "${!EXTENSIONS[@]}"; do
    [[ $idx -gt 0 ]] && FIND_ARGS+=(-o)
    FIND_ARGS+=(-name "*.${EXTENSIONS[$idx]}")
done

mapfile -t FILES < <(find "$TARGET_DIR" \( "${FIND_ARGS[@]}" \) -type f | sort)
FILE_COUNT=${#FILES[@]}

if [[ $FILE_COUNT -eq 0 ]]; then
    echo "No matching source files found in '$TARGET_DIR'."
    exit 0
fi

# ── Print summary header ──────────────────────────────────────────────────────
CLANG_TIDY_VERSION=$("$CLANG_TIDY" --version 2>&1 | head -n1)
echo "clang-tidy:  $CLANG_TIDY_VERSION"
echo "Directory:   $TARGET_DIR"
if [[ -n "$CONFIG_FILE" ]]; then
    if [[ -n "$CONFIG_DATE" ]]; then
        echo "Config:      $CONFIG_FILE  (date: $CONFIG_DATE)"
    else
        echo "Config:      $CONFIG_FILE"
    fi
elif [[ -n "$CONFIG_DATE" ]]; then
    echo "Config date: $CONFIG_DATE"
fi
[[ -n "$COMPILE_COMMANDS" ]] && echo "Build dir:   $COMPILE_COMMANDS"
echo "Files found: $FILE_COUNT"
echo "Jobs:        $JOBS"
$DRY_RUN && echo "(dry-run — no fixes will be written)"
echo ""

# ── Worker function (runs in subshell via xargs) ──────────────────────────────
parallel_tidy() {
    local file="$1"
    local outfile="$TMPDIR_RESULTS/$(printf '%s' "$file" | tr '/' '_').out"
    local rcfile="$TMPDIR_RESULTS/$(printf '%s' "$file" | tr '/' '_').rc"

    [[ "$VERBOSE" == "true" ]] && echo "  Processing: $file"

    local args=()
    [[ -n "$COMPILE_COMMANDS" ]] && args+=(-p "$COMPILE_COMMANDS")
    [[ -n "$CONFIG_FILE"      ]] && args+=(--config-file "$CONFIG_FILE")
    [[ -n "$EXTRA_ARGS"       ]] && args+=(--extra-arg "$EXTRA_ARGS")
    [[ "$DRY_RUN" != "true"   ]] && args+=(--fix --fix-errors)
    args+=("$file")

    echo "  CMD: $CLANG_TIDY ${args[*]}" >&2
    "$CLANG_TIDY" "${args[@]}" > "$outfile" 2>&1
    echo $? > "$rcfile"
}

export -f parallel_tidy
export CLANG_TIDY COMPILE_COMMANDS CONFIG_FILE EXTRA_ARGS DRY_RUN VERBOSE

TMPDIR_RESULTS=$(mktemp -d)
export TMPDIR_RESULTS
trap 'rm -rf "$TMPDIR_RESULTS"' EXIT

# ── Dispatch ──────────────────────────────────────────────────────────────────
printf '%s\n' "${FILES[@]}" \
    | xargs -P "$JOBS" -I{} bash -c 'parallel_tidy "$@"' _ {}

# ── Collect and print results ─────────────────────────────────────────────────
SUCCEEDED=0
FAILED=0

for FILE in "${FILES[@]}"; do
    KEY=$(printf '%s' "$FILE" | tr '/' '_')
    RC_FILE="$TMPDIR_RESULTS/${KEY}.rc"
    OUT_FILE="$TMPDIR_RESULTS/${KEY}.out"

    rc=0
    [[ -f "$RC_FILE" ]] && rc=$(< "$RC_FILE")

    [[ -s "$OUT_FILE" ]] && cat "$OUT_FILE"

    if [[ "$rc" -eq 0 ]]; then
        (( SUCCEEDED++ )) || true
    else
        (( FAILED++ )) || true
    fi
done

echo ""
if $DRY_RUN; then
    echo "Done (dry-run). Clean: $SUCCEEDED  |  With diagnostics: $FAILED"
else
    echo "Done. Fixed: $SUCCEEDED  |  Failed/warnings: $FAILED"
fi

[[ $FAILED -gt 0 ]] && exit 1 || exit 0
