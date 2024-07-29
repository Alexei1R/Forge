# Forge



> Point cloud viewver

Components:

- `src`.
- `sandbox`.
- `libs/*`.


## Getting Started

#Build 
```
sudo pacman -S 
```



<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with 

```bash
git clone --recursive https://github.com/Alexei1R/Forge
```


If the repository was cloned non-recursively previously, use 
```bash
git submodule update --init
```
to clone the necessary submodules.


#Build 
```
sudo pacman -S 
```

Create the `build` directory, configure the build system and run make:

```bash
mkdir build & cd build & cmake .. & make -j$(nproc)
```

To run the project cd `build` and run execurable


