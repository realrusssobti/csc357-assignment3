//
// Created by Russ Sobti on 5/3/23.
//
#include <sys/stat.h>
#include "tree.h"
#include "dirent.h"
#include "stdlib.h"
#include "unistd.h"
#include "array_list.h"
#include "string.h"

void free_entry(entry *e) {
    if (e->children) {
        for (int i = 0; i < e->children->size; i++) {
            free_entry(e->children->array[i]);
        }
    }
    array_list_free(e->children);
    free(e);


}

void print_n_dirs_files(entry *root, int *dirs, int *files) {
    // iterate through all children, and increment count
    for (int i = 0; i < root->children->size; i++) {
        if (root->children->array[i]->type == 'd') {
            (*dirs)++;
            print_n_dirs_files(root->children->array[i], dirs, files);
        } else {
            (*files)++;
        }
    }

}

void print_tree(entry *e, int depth, int a, int s, int last) {
    // Print the indentation based on depth
    if (depth == -1) {
        if (s) {
            printf("[%10d]", e->size);
        }
        printf("%s", e->name);

    } else {
        for (int i = 0; i < depth; i++) {
            printf("|   ");
        }
        if (last) {
            printf("`-- ");
        } else if (depth == 0) {
            printf("|-- ");
        } else {
            printf("|-- ");
        }
        if (s) {
            printf("[%10d]", e->size);
        }

        // Print the name of the entry
        printf("%s", e->name);
    }


    printf("\n");

    // Recursively print the children of the entry, if any
    if (e->children) {
        // alphabetize the children
        array_list_alphabetize(e->children);
        for (int i = 0; i < e->children->size; i++) {
            // if last child, print last flag
            if (i == e->children->size - 1) {
                last = 1;
            } else {
                last = 0;
            }
            print_tree(e->children->array[i], depth + 1, a, s, last);
        }
    }
}

// add_children: recursively adds children to a given entry
void add_children(char *root, entry *e, int a, int s) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    dir = opendir(root);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }
    // Iterate through all entries in the directory
    while (1) {
        if ((entry = readdir(dir)) == NULL) {

            break;
        }
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Skip hidden files if the -a flag is not set
        if (!a && entry->d_name[0] == '.') {
            continue;
        }
        // Create a new entry for the child
        struct entry *child = malloc(sizeof(struct entry));
//        child->name = entry->d_name;
        // copy the name of the child into the entry
        child->name = strdup(entry->d_name);
        child->visited = 0;
        child->children = array_list_new();
        child->size = 0;
        // Get the full path of the child
        char path[1024];
        int status = snprintf(path, sizeof(path) - 1, "%s/%s", root, entry->d_name);
        if (status < 0) {
            perror("snprintf");
            exit(1);
        }
        // Get the type of the child
        if (lstat(path, &info) == 0) {
            // check if dir, link, or file
            if (S_ISDIR(info.st_mode)) {
                child->type = 'd';
                child->size = info.st_size;
            } else if (S_ISLNK(info.st_mode)) {
                child->type = 'l';
                // get the size of the link
                child->size = info.st_size;
            } else {
                child->type = 'f';
                child->size = info.st_size;
            }
        } else {
            child->type = 'u';
        }
        // Add the child to the entry's children array_list
        array_list_add_to_end(e->children, child);
        // if the child is a directory, recursively add its children
        if (child->type == 'd') {

            add_children(path, child, a, s);
        }
        // also if the child is a link, recursively add its children
        if (child->type == 'l') {
            // read the link
            char link[1024];
            readlink(path, link, sizeof(link));
            // get the full path of the link
            char link_path[1024];
            int status = snprintf(link_path, sizeof(link_path) - 1, "%s/%s", root, link);
            if (status < 0) {
                perror("snprintf");
                exit(1);
            }
            // recursively add the link's children
            add_children(link_path, child, a, s);
        }
    }
    // Close the directory
    closedir(dir);
}

void tree(char *path, int a, int s) {

    // Create the root entry
    entry *root = calloc(1, sizeof(entry));
    root->type = 'd';
    root->hidden = 0;
    root->name = path;
    root->visited = 1;
    root->children = array_list_new();
//    root->size = 0;
// call lstat on root
    struct stat info;
    if (lstat(path, &info) == 0) {
        root->size = info.st_size;
    } else {
        root->size = 0;
    }


    // Recursively add children to the root entry
    add_children(root->name, root, a, s);


    print_tree(root, -1, a, s, 0);
    printf("\n");
    // print the number of directories and files
    int directories = 0;
    int files = 0;
    print_n_dirs_files(root, &directories, &files);
    printf("%d directories, %d files\n", directories, files);
    // free the root
    // free the array list
//    free_entry(root);
    free(root->children);
    free(root);
// set the root to null
    root = NULL;


}

int main(int argv, char *argc[]) {
    // check for -s and -a flags
    int a = 0;
    int s = 0;
    for (int i = 0; i < argv; i++) {
        if (strcmp(argc[i], "-a") == 0) {
            a = 1;
            argc[i] = "\0";
        }
        if (strcmp(argc[i], "-s") == 0) {
            s = 1;
            argc[i] = "\0";
        }
    }
    // check for path
    char *path = argc[1];
    if (path == NULL || strcmp(path, "\0") == 0) {
        path = ".";
    }

    tree(path, a, s);


    return 0;
}
