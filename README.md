## The Sudoku Game

A simple sudoku game, made with SFML and C++. Uses openGL underneath.

# Running on Linux
### Install dependencies 
#### Debian/Ubuntu
```bash
sudo apt update &&
sudo apt install \
        libxrandr-dev \
        libxcursor-dev \
        libudev-dev \
        libfreetype-dev \
        libflac-dev \
        libvorbis-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev \
        libfreetype-dev
```
#### Fedora/RHEL
```bash 
sudo dnf in libX11-devel \
        libXrandr-devel \
        libXcursor-devel \
        libXi-devel       
        libgle-devel \
        libgudev-devel \
        freetype-devel \
        libvorbis-devel \
        flac-devel         

```
### Dowloading
```bash
git clone https://github.com/nanda-gopal-sb/sudoku/
```

### Building
```bash
cmake -B build
cmake --build build
```
### Run the binary
from project directory:
```
./build/bin/main
```
