# Autonomous robot

## Table of Contents

1. [Description](#description)
2. [Repository Structure](#repository-structure)
3. [Usage](#usage)
    - [Compiling the Library](#compiling-the-library)
    - [Compiling the App and Installing it in the Robot](#compiling-the-app-and-installing-it-in-the-robot)

## Description

This repository contains all the files developed during the integrative project [INF1900](https://www.polymtl.ca/programmes/cours/projet-initial-de-systeme-embarque) on Embedded Systems.

The goal of this project is to program an autonomous line-following robot equipped with sensors to detect obstacles and intelligently select the unobstructed path when presented with two possible routes.

## Repository Structure

-   **tps**: Contains all the lab work done during the semester to learn the necessary material to complete the project.
-   **projet**: Contains the code of the project.
    -   **projet/lib**: Contains the library files.
    -   **projet/app**: Contains the main application code that define the robot's behavior. It makes use of the library functions.

## Usage

### Compiling the Library

1. Navigate to the `lib/` directory:

```bash
cd projet/lib
```

2. Compile the library:

```bash
make
```

### Compiling the App and Installing it in the Robot

1. Navigate to the `app` directory:

```bash
cd projet/app
```

2. Compile and install the application in the robot:

```bash
make install
```

3. If you want to simply compile the app and not install, you can execute:

```bash
make
```

### Cleaning up

To remove all generated files, run the following command:

```bash
make clean
```
