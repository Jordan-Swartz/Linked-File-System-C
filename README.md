File and Directory Representation as Nodes
Each file can be a node in the tree, containing information such as:
File name.
File size.
File contents (which could be stored as blocks, simulating disk memory).
File metadata (permissions, timestamps, etc.).
Each directory is also a node, but it will contain pointers (or references) to other nodes (either files or subdirectories). This forms a tree structure, with directories acting as internal nodes and files as leaf nodes.
2. Tree Structure for Directory Hierarchy
   The root node represents the top-level directory (e.g., / or root).
   Each directory node can have multiple child nodes, representing either files or subdirectories.
   For example, if you have a directory /home/user/docs/, it would have a tree structure where:
   The /home directory node points to the user directory node.
   The user directory node points to the docs directory node.
   The docs directory node points to the actual file nodes inside it.
3. Core Operations
   Here’s how the core file system operations could be mapped to the tree structure:

1. Create File
   You’ll create a new node representing the file, and insert it as a child of the current directory node.
   If the file already exists, you might either overwrite it or return an error depending on your implementation.
2. Delete File
   Find the node representing the file, remove it from the parent directory node, and free any associated memory (such as file blocks).
3. Open and Close Files
   Open File: Mark the file as open (set a flag or use a file descriptor) and allow read/write operations.
   Close File: Remove the open flag or file descriptor. Ensure any buffered data is saved before closing.
4. Move Files
   Moving a file involves removing the file node from one directory and inserting it into another directory node, preserving its contents.
5. Read/Write to Files
   Read: Fetch the file contents stored in the file node and return it.
   Write: Modify or append data to the file contents, which might require allocating new memory (or blocks) to store the new data.
4. Node Relationships
   Each file node will have pointers to its parent (the directory it belongs to) and, optionally, metadata about the file (e.g., size, permissions).
   Each directory node will have pointers to both its parent directory and its child nodes (which can be either files or subdirectories).
5. Scheduler
   The scheduler will read commands from a text file (or some input) and perform the required operations sequentially, interacting with the file system tree.
   Commands like CREATE, DELETE, MOVE, OPEN, CLOSE, READ, and WRITE will map to functions that manipulate the tree structure and manage file metadata.