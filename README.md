# Filesystem Simulator — TC4 (Operating Systems Principles)

Simple filesystem written in C for Short Assignment 4 of the course **Operating Systems Principles**. Course offered at the **Costa Rica Institute of Technology (TEC)**, taught by Professor Kenneth Obando Rodríguez.
This project represents the fundamental operations of a real filesystem, including block management, file table, root directory and full CRUD operations.

---

## Key Features

The key features of this filesystem simulator include:

-  **Simulated filesystem of 1 MB**
-  **Fixed blocks of 512 bytes**
-  **Up to 100 files**
-  **Block management (bitmap)**
-  **Simulated root directory**
-  **Operations: CREATE, WRITE, READ, DELETE, LIST**
-  **Professional modular architecture**
-  **Includes intensive test scripts (stress test and fuzz test)**

---

## Project Architecture

The project is organized into the following modules:

```
/src
├── main.c                 # CLI for the system
├── filesystem.c           # Main FS coordinator
├── filesystem.h
│
├── block_manager.c        # Block management: allocation and freeing
├── block_manager.h
│
├── directory.c            # Root directory: search, insert, delete
├── directory.h
│
├── file_operations.c      # CREATE, WRITE, READ, DELETE
├── file_operations.h
│
├── storage.c              # Simulated disk (1MB)
├── storage.h
│
└── Makefile               # Build system
```

---

## Module Descriptions
 
Each module has a specific role, they ensure modularity and a clean separation of responsibilities:
They are described as follows:

### 1. storage.c
Simulates a 1 MB disk using an array of `unsigned char`.

### 2. block_manager.c
Manages block usage via a bitmap (`block_used[]`).  
Features:
- Find free blocks
- Reserve blocks
- Free blocks

### 3. directory.c
System file table.  
Provides:
- Find files by name
- Create entries
- Delete files
- List files

### 4. file_operations.c
Implements the main FS operations:
- CREATE
- WRITE
- READ
- DELETE

### 5. filesystem.c
Integration layer. Coordinates:
- Directory
- Block Manager
- Storage

### 6. main.c
Provides an interactive shell-like interface.

---
## Error Handling

The filesystem simulator implements strict error handling and input validation for common failure cases such as:

- Creating files with duplicate names
- Writing or reading beyond file bounds
- Invalid offsets or arguments
- Deleting non-existent files
- Insufficient space for new files or writes
- Robust boundary checks to prevent data corruption
- Graceful, user-facing error messages
- Proper memory management to avoid leaks

### Error messages

The simulator returns clear error messages for detected conditions:

```
Error: no sufficient space in the filesystem.
Error: file already exists.
Error: file not found.
Error: invalid offset.
Error: operation exceeds file bounds.
Error: invalid argument.
Unknown error (<code>).
```


## Build

Run in terminal:

```bash
make
```

This produces an executable called:

```
./sfs
```

---

## Run and Commands

To run the filesystem simulator, use the following command in your terminal:

```bash
make run
```

Start the simulator:

```bash
./sfs
```

Available commands:

```
CREATE <name> <size>
WRITE <name> <offset> "<data>"
READ  <name> <offset> <size>
DELETE <name>
LIST
EXIT
```

---

## Usage Example

This project provides a simple command-line interface (CLI) to interact with the filesystem. Below is an example session demonstrating the available commands:

```
> CREATE file1.txt 1000
File 'file1.txt' created (1000 bytes).

> WRITE file1.txt 0 "Hello, world"
Wrote 12 bytes to 'file1.txt'.

> READ file1.txt 0 12
Hello, world

> LIST
file1.txt - 1000 bytes

> DELETE file1.txt
File 'file1.txt' deleted.

> LIST
(no files)
```

---

## Included Tests

At the end of the project, several test scripts were created to validate the functionality and robustness of the filesystem simulator. They are automated bash scripts located in the main directory.

### ✔ `test_fs.sh` – Basic functional tests
Creates, writes, reads, lists and deletes multiple files.

### ✔ `test_hs.sh` – 1000-operation stress test
Heavy load testing.

### ✔ `fuzz_fs.sh` – 2000+ operation tests
Thousands of random operations to test robustness.

All tests were run and the system responded correctly without corruption or block leaks.

---


## Technical Details

In reference, the main technical specifications of the filesystem simulator are:

- **FS size:** 1 MB
- **Block size:** 512 bytes
- **Total blocks:** 2048
- **Maximum files:** 100
- **Allocation:** sequential first-fit
- **Strict error validation**
- **Offset and size verified byte-by-byte**

These specifications were provided by the assignments guidelines.
---

## Authors

- Pavel Zamora Araya
- Fabricio Solis Alpizar

## Credits

This project was developed as part of the **Operating Systems Principles** course at the **Costa Rica Institute of Technology (TEC)**, under the guidance of Professor Kenneth Obando Rodríguez.

## License

This project uses the MIT license.  
See the `LICENSE` file for details.

## References

- Course materials from **Operating Systems Principles** at TEC.
- C programming documentation: https://en.cppreference.com/w/c