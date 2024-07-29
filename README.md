
---

# Forge

> **Point Cloud Viewer**: An application designed to visualize data in multiple formats.

## Components

- `src/*`
- `sandbox/*`
- `libs/*`

## Getting Started

### 1. Downloading the Repository

Clone the repository using:

```bash
git clone --recursive https://github.com/Alexei1R/Forge
```

If you previously cloned the repository without the `--recursive` flag, update the submodules with:

```bash
git submodule update --init
```

### 2. Build Instructions

#### Prerequisites

Ensure the following packages are installed:

```bash
sudo pacman -S base-devel cmake gdb clang ninja
```

#### Building on Linux

Run the build script:

```bash
./build.sh
```

---

