# Building boes


```
# Initialize and clone pico sdk
git submodule update --init --recursive

mkdir build
cd build
export PICO_SDK_PATH=../..

# Generate make files and build program
cmake .. -DPICO_BOARD=pico_w
make

# Flash pico
cp timer/*.uf2 /media..TODO

```
