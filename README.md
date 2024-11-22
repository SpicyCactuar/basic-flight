# Basic Flight

Simple flight simulation application showcasing basic arcball-like physics and sphere collisions. Built using Qt and rendered with OpenGL.
The flight is simulated using adhoc linear algebra primitives and calculations (e.g.: matrix transformations).
The plane can collide against the floor or lava bombs erupting from a small volcano.

![basic-flight](https://github.com/user-attachments/assets/1e457f28-ac3e-4172-8f80-ef44ef4a60af)

## Project Structure

```plaintext
basic-flight/
├── src/                 # Source code
├── assets/              # Static assets (.tri and .dem files)
├── basic-flight.pro     # QMake project
└── README.md            # Project README
```

## Build

```bash
qmake
make
```

## Run

```bash
bin/basic-flight <initial (x, y, z)>
```

Example:

```shell
bin/basic-flight -33000 3000 2000
```

## Controls

| Key(s)                  | Action                                |
|-------------------------|---------------------------------------|
| `A` / `S`               | Pitch down and up by 3°               |
| `Q` / `E`               | Roll left and right by 3°             |
| `W` / `D`               | Yaw left and right by 3°              |
| `+` / `-`               | Increase and decrease                 |
| `X`                     | Close the application                 |

## Technologies

* **C++**: `>= C++17`
* **Qt**: `5.12.x`
* **OpenGL**: `>= 4.0`

Newer versions of Qt might work correctly, if no breaking changes that affect the application were introduced.
