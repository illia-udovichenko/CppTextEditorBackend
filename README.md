# BSTTextEditorBackend

Efficient backend implementation for a line-based text editor in **C++23**, supporting fast character and line operations with logarithmic complexity.

---

## Features

- Stores text efficiently using a balanced binary search tree  
- Constant-time queries for text size and line count  
- Logarithmic-time insertions, deletions, and edits  
- Safe exception handling for invalid indices  
- Fully covered by unit tests  

---

## Functionality Overview

- **Initialization** — load text into the editor  
- **Access** — retrieve a specific character or line  
- **Edit** — replace, insert, or delete characters  
- **Navigation** — get line start index, line length, or map characters to lines  
- **Print** — visualize the tree structure for debugging  

---

## Example Usage

```cpp
TextEditorBackend editor("Hello\nWorld");

cout << "Initial text:" << endl;
editor.print();
cout << "\nLines: " << editor.lines()
     << ", Size: " << editor.size() << endl;

editor.insert(5, '!');     // Insert '!' after "Hello"
editor.edit(0, 'h');       // Edit first char
editor.erase(6);           // Delete '\n'

cout << "\nModified text:" << endl;
editor.print();
cout << "\nLines: " << editor.lines()
     << ", Size: " << editor.size();
````

**Output:**

```
Initial text:
Hello
World
Lines: 2, Size: 11

Modified text:
hello!World
Lines: 1, Size: 11
```
---

## Build & Run

### Requirements

* `g++` with C++23 support
* `make`

### Commands

```bash
make          # Build and run main demo
make test     # Build and run tests
make clean    # Remove build artifacts

```
