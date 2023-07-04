//
// Created by Russ Sobti on 5/3/23.
//

#ifndef CSC357_ASSIGNMENT3_REALRUSSSOBTI_ASSIGNMENT3_H
#define CSC357_ASSIGNMENT3_REALRUSSSOBTI_ASSIGNMENT3_H

#endif //CSC357_ASSIGNMENT3_REALRUSSSOBTI_ASSIGNMENT3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// add_children: recursively adds children to a given entry
// Parameters: entry *e, char *path, int a, int s
// Tree command: mimics the tree command in linux
// Parameters: char *path
// flags: -a, list all files including hidden files
//        -s, list all file sizes

void tree(char *path, int a, int s);

// Main: parses arguments and calls tree
int main(int argc, char *argv[]);


