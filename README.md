## The Sudoku Game
For now, its just a simple sudoku game with minimal graphics, however I do plan to make it multiplayer/collaborative.

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
### Building & running
```bash
git clone --depth 1 https://github.com/nanda-gopal-sb/sudoku/
cd sudoku
cmake -B build
cmake --build build
./build/bin/main
```
