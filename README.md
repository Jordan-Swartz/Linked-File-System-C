# Linked-File-System-C Project
A lightweight file system implementation in C with basic file operations, directory management, serialization/deserialization, and unit testing integration. 

---

## 1: About the Project
The Linked-File-System-C project is a file system implementation that supports traditional bash-like operations. These operations include, but are not limited to: file creation, deletion, duplication, navigation, and retrieval. It is designed for learning and practicing data structures, memory management, system-level programming, and algorithmic strategies.

---

## 2: Featured Concepts
1. Recursive Algorithms
   - Implemented for node deletion and duplication
2. DFS Traversal
   - Implemented for searching and retrieving nodes in the file system
3. Directory Traversal and Path Resolution
   - Implemented for custom path parsing for absolute and relative paths 
4. Node Management and Relocation
   - Implemented for moving, renaming, and inserting nodes within the tree hierarchy
5. Memory Management
   - Implemented to use dynamic memory allocation and deallocation for data handling
6. JSON serialization/deserialization
   - Implemented to save and restore file system states
7. Unit Testing
   - Implemented for function validation
      
---

## 3: Running the Project

### Option 1: CMake
1. Configure Project
   - `cmake -B build`
     
2. Build Project
   - `cmake --build build`
     
3. Run Program (cd build)
   - `./LinkedFS`
     
4. Run Unit Tests (cd build)
   - `./test_ui`

### Option 2: build.sh (No CMake Installation)
1. Ensure Script is Executable
   - `chmod +x build.sh `
     
2. Execute Script
   - `./build.sh`
     
3. Run Program (cd build)
   - `./LinkedFS`
     
4. Run Unit Tests (cd build)
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
chmod <permissions> <file>                    - changes permissions for a file (e.g., "chmod 755 myfile")
cp <path/to/source> <path/to/destination>     - copies a file or directory to a new location
find <path/to/start-directory> <target_name>  - searches for a given file or directory name based on start directory
history                                       - displays the command history for the session
exit                                          - exits the system
menu                                          - displays the menu
```
📝 **Note:**  
- The **root directory** is the top-level node in the system, and both `~` and `/` point to this node. Any commands involving absolute paths (`/some/path`) will start from the root. (`~`) is not traversable and will always refer to only the root.


---

## 5: Technologies Used
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
