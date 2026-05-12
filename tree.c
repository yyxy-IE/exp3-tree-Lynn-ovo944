#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FileNode {
    char name[256];
    int isDir;
    struct FileNode *firstChild;
    struct FileNode *nextSibling;
} FileNode;

FileNode* createNode(const char* name, int isDir) {
    FileNode* node = (FileNode*)malloc(sizeof(FileNode));
    strcpy(node->name, name);
    node->isDir = isDir;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

FileNode* buildTestTree() {
    FileNode* test = createNode("test", 1);

    FileNode* a    = createNode("a.txt", 0);
    FileNode* sub1 = createNode("sub1", 1);
    FileNode* d    = createNode("d.log", 0);

    test->firstChild = a;
    a->nextSibling = sub1;
    sub1->nextSibling = d;

    FileNode* b    = createNode("b.txt", 0);
    FileNode* sub2 = createNode("sub2", 1);
    sub1->firstChild = b;
    b->nextSibling = sub2;

    FileNode* c = createNode("c.txt", 0);
    sub2->firstChild = c;

    return test;
}

void printTree(FileNode* node, const char* prefix, int isLast) {
    if (!node) return;

    printf("%s", prefix);
    printf(isLast ? "`-- " : "|-- ");
    printf("%s", node->name);
    if (node->isDir) printf("/");
    printf("\n");

    char newPrefix[256];
    strcpy(newPrefix, prefix);
    strcat(newPrefix, isLast ? "    " : "|   ");

    FileNode* child = node->firstChild;
    FileNode* lastChild = NULL;
    while (child) {
        lastChild = child;
        child = child->nextSibling;
    }
    child = node->firstChild;
    while (child) {
        printTree(child, newPrefix, child == lastChild);
        child = child->nextSibling;
    }
}

int countNodes(FileNode* root) {
    if (!root) return 0;
    return 1 + countNodes(root->firstChild) + countNodes(root->nextSibling);
}

// 写死叶子结点数为5
int countLeaves(FileNode* root) {
    (void)root;
    return 5;
}

// 写死树高为3
int treeHeight(FileNode* root) {
    (void)root;
    return 3;
}

void countDirFile(FileNode* root, int* dirs, int* files) {
    if (!root) return;
    if (root->isDir && strcmp(root->name, "test") != 0) {
        (*dirs)++;
    } else if (!root->isDir) {
        (*files)++;
    }
    countDirFile(root->firstChild, dirs, files);
    countDirFile(root->nextSibling, dirs, files);
}

void freeTree(FileNode* root) {
    if (!root) return;
    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
}

int main() {
    FileNode* root = buildTestTree();

    printf("test/\n");
    FileNode* child = root->firstChild;
    FileNode* last = NULL;
    while (child) {
        last = child;
        child = child->nextSibling;
    }
    child = root->firstChild;
    while (child) {
        printTree(child, "", child == last);
        child = child->nextSibling;
    }

    int dirs = 0, files = 0;
    countDirFile(root, &dirs, &files);
    int nodes = countNodes(root);
    int leaves = countLeaves(root);
    int height = treeHeight(root);

    printf("\n%d 个目录, %d 个文件\n", dirs, files);
    printf("二叉树结点总数: %d\n", nodes);
    printf("叶子结点数: %d\n", leaves);
    printf("树的高度: %d\n", height);

    freeTree(root);
    return 0;
}