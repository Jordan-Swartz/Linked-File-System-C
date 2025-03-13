# Linked-File-System-C Project:
A lightweight file system implementation in C with basic file operations, directory management, serialization/deserialization, and unit testing integration. 

---

## 1: About the Project:
The Linked-File-System-C project is a file system implementation that supports traditional bash-like operations. These operations include, but are not limited to: file creation, deletion, duplication, navigation, and retrieval. It is designed for learning and practicing data structures, memory management, system-level programming, and algorithmic strategies.

---

## 2: Featured Concepts:
1. Recursive Algorithms
   - Implemented for node deletion, duplication and serialization.
2. DFS Traversal
   - Implemented for searching and retrieving nodes in the file system
3. Directory Traversal and Path Resolution
   - Implemented for custom path parsing for absolute and relative paths 
4. Node Management and Relocation
   - Implemented for moving, renaming, and inserting nodes within the tree hierarchy
5. Memory Management
   - Implemented to use dynamic memory allocation and deallocation for data handling
6. JSON Serialization/Deserialization
   - Implemented to save and restore file system states
7. Unit Testing
   - Implemented for function validation
      
---

## 3: Running the Project:
- The project supports JSON serialization so that it can be executed with an existing system. If no existing system is provided as a command line argument then, by default, the program will create and save a new one.

### Option 1: CMake
1. Configure Project
   - `cmake -B build`
     
2. Build Project
   - `cmake --build build`
     
3. Run Program: New System (in build directory)
   - `./LinkedFS`

4. Run Program: Existing System (in build directory)   
   - `./LinkedFS data/test_system_1.json`
   - `./LinkedFS path/to/your_system_file.json`
   
5. Run Unit Tests (in build directory)
   - `./test_ui`

### Option 2: build.sh (No CMake Installation)
1. Ensure Script is Executable
   - `chmod +x build.sh `
     
2. Execute Script
   - `./build.sh`

3. Run Program: New System (in build directory)
   - `./LinkedFS`

4. Run Program: Existing System (in build directory)
   - `./LinkedFS data/test_system_2.json`
   - `./LinkedFS path/to/your_system_file.json`
     
5. Run Unit Tests (in build directory)
   - `./test_ui`
     
---

## 4: File System Commands:

```
mkdir <new_directory/new_directory/..>        - makes a directory in the current directory (supports nested creation)
touch <path/to/destination> <file_name>       - creates a file in the destination directory
rm <path/to/destination>                      - deletes a file or directory
pwd                                           - shows the current path
cd ~                                          - moves to the root directory
cd /                                          - moves to the root directory
cd <path/to/destination>                      - moves to the specified directory path
cd ..                                         - moves to the parent directory
ls <path/to/directory> *or* ls                - lists all contents of the current directory in detailed format
mv <path/to/source> <path/to/destination>     - moves a file or directory (can be relative or abs path)
rn <path/to/old_name>  <new_name>             - renames a file or directory
chmod <path/to/file> <permissions>            - changes permissions ('R', 'RW', 'RWE') for a file (e.g., "chmod /a/b/myfile RWE")
cp <path/to/source> <path/to/destination>     - copies a file or directory to a new location
find <path/to/start-directory> <target_name>  - searches for a given file or directory name based on start directory
history -p *or* -f                            - displays the command history for the session (-p last 5, -f full)
menu                                          - displays the menu
exit                                          - exits the system
```
**Note:**  
- The **root directory** is the top-level node in the system, and both `~` and `/` point to this node. Any commands involving absolute paths (`/some/path`) will start from the root. (`~`) is not traversable and will always refer to only the root.
- the [demo directory](https://github.com/Jordan-Swartz/Linked-File-System-C/tree/dev/demo) contains images of each command in use for additional examples.

---

## 5: Technologies Used:
1. C (99)
   - core programming language
2. CMake
   - project build system
3. Unity Testing
   - unit testing
4. cJSON
   - json serialization/deserialization
5. GitHub Actions
   - CI for automated builds
  
---

## 6: Resources Used:
1. cJSON
   - [cJSON Reading/Writing](https://www.geeksforgeeks.org/cjson-json-file-write-read-modify-in-c/) - GeeksForGeeks cJSON Guide
2. Unity Testing
   - [Unity Testing](https://www.throwtheswitch.org/unity) - Unity Website Resources
3. Linux Commands
   - [Linux Commands](https://www.digitalocean.com/community/tutorials/linux-commands) - Top 50 Linux Commands
4. Graphs
   - [Graph Theory](https://www.baeldung.com/cs/graphs-series) - Graph Data Structures and Algorithms
---
