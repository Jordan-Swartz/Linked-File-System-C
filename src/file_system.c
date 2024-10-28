/**
 * core file system operations:
 *
 * create/delete file
 * create/delete directory
 * move files between directories
 * traverse directory (cd, cd ..)
 *
 * contain nodes/tree struct
 */

#include <stdlib.h>

/*Data Structures*/
//node type
typedef enum {
    Directory,
    File
} NodeType;

//file node
typedef struct {
    char name[100];
    NodeType type;
    int permissions;
    int size;

    struct FSNode* child_head;
    struct FSNode* next;
} FSNode;

/*Global Variables*/
//root
FSNode* root = NULL;

void create_node() {

    ss
}







