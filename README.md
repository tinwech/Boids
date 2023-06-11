# Boids Simulation

This project is a Boids simulation implemented in C++ using the OpenGL library. The simulation demonstrates the flocking behavior of birds and other animals based on the principles of emergent behavior and local interactions.

![ezgif-5-cb7fd735b6](https://github.com/tinwech/Boids/assets/80531783/2c39fda7-a39b-442a-8f3f-dca8bfe7c1cb)

## For Ubuntu

### Install Dependencies

```
sudo apt-get update
sudo apt-get install cmake -y
sudo apt-get install build-essential -y
sudo apt-get install libgl1-mesa-dev -y
sudo apt-get install libglu1-mesa-dev -y
sudo apt-get install freeglut3-dev -y
sudo apt-get install libglfw3-dev -y
sudo apt-get install libglew-dev -y
sudo apt-get install libglm-dev -y
```

### Build

```
mkdir build && cd build
cmake ..
make
```

### Run

```
./boids
```
