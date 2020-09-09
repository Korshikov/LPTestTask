#include <stdio.h>

struct HTWordNode{
    char* word;
    unsigned long counter;
    struct HTWordNode* nextNode;
};

int main(int argc, char** argv) {
    //TODO case-ignore trim
    printf("Hello, World!\n");
    return 0;
}
