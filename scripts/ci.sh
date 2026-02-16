#!/usr/bin/env bash
set -euo pipefail

echo "[ci] start"

run() {
  echo "[ci] $*"
  "$@"
}

has_command() {
  command -v "$1" >/dev/null 2>&1
}

file_exists() {
  [[ -f "$1" ]]
}

# 0) Makefile with test target
if file_exists "Makefile"; then
  if grep -Eq '^[a-zA-Z0-9_.-]+:.*$' Makefile && grep -Eq '^test:' Makefile; then
    run make test
    echo "[ci] done (make test)"
    exit 0
  fi
fi

# 1) Node.js
if file_exists "package.json"; then
  echo "[ci] detected: node project"

  if file_exists "pnpm-lock.yaml"; then
    if ! has_command pnpm; then
      run corepack enable
    fi
    run pnpm install --frozen-lockfile
    run pnpm test
    echo "[ci] done (pnpm)"
    exit 0
  fi

  if file_exists "yarn.lock"; then
    if ! has_command yarn; then
      run corepack enable
    fi
    run yarn install --frozen-lockfile
    run yarn test
    echo "[ci] done (yarn)"
    exit 0
  fi

  if file_exists "package-lock.json" || file_exists "npm-shrinkwrap.json"; then
    run npm ci
  else
    echo "[ci] warning: no npm lockfile found; falling back to npm install"
    run npm install
  fi
  run npm test
  echo "[ci] done (npm)"
  exit 0
fi

# 2) Python
if file_exists "pyproject.toml" || file_exists "requirements.txt"; then
  echo "[ci] detected: python project"

  run python -m pip install --upgrade pip

  if file_exists "requirements.txt"; then
    run pip install -r requirements.txt
  else
    run pip install -e .
  fi

  if has_command pytest; then
    run pytest
    echo "[ci] done (pytest)"
    exit 0
  fi

  run python -c "print('python ok')"
  echo "[ci] done (python minimal)"
  exit 0
fi

# 3) Go
if file_exists "go.mod"; then
  echo "[ci] detected: go project"
  run go test ./...
  echo "[ci] done (go test)"
  exit 0
fi

# 4) Rust
if file_exists "Cargo.toml"; then
  echo "[ci] detected: rust project"
  run cargo test
  echo "[ci] done (cargo test)"
  exit 0
fi

# 5) .NET
if file_exists "*.sln"; then
  echo "[ci] detected: dotnet project"
  run dotnet test
  echo "[ci] done (dotnet test)"
  exit 0
fi

# 6) PlatformIO (ESP32/Arduino projects)
if file_exists "platformio.ini"; then
  echo "[ci] detected: platformio project"

  if ! has_command pio; then
    run pip install platformio
  fi

  run pio run
  echo "[ci] done (pio run)"
  exit 0
fi

echo "[ci] ERROR: cannot detect how to run tests."
echo "[ci] Please add a Makefile with a 'test' target OR implement a repo-specific scripts/ci.sh."
exit 1
