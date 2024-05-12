#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    //new->lower_than = lower_than;
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->lower_than = lower_than;
    new->root = NULL;
    new->current = NULL;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) != NULL) return;
    TreeNode * parent = NULL;
    TreeNode * current = tree->root;
    while (current != NULL)
        {
            if (tree->lower_than(key, current->pair->key))
            {
                parent = current;
                current = current->left;
            }
            else
            {
                parent = current;
                current = current->right;
            }
        }

    TreeNode * new = createTreeNode(key, value);
    if (parent == NULL) tree->root = new;
    else
    {
        if (tree->lower_than(key, parent->pair->key) == 1)
            parent->left = new;
        else
            parent->right = new;
        new->parent = parent;    
    }
    tree->current = new;
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    if (node == NULL) return; // If the node is NULL, return
    TreeNode * parent = node->parent; // Create a temporary node to store the parent of the node to be removed

    if (node->left == NULL && node->right == NULL) // If the node to be removed has no children
    {
        if (parent == NULL) // If the node has no parent, set the root to NULL
            tree->root = NULL;
        else
        {
            if (parent->left == node) // If the node to be removed is the left child of the parent
                parent->left = NULL; 
            else // If the node to be removed is the right child of the parent
                parent->right = NULL;
        }
    }
    else if (node->left != NULL && node->right != NULL) // If the node to be removed has two children
    {
        TreeNode * minimumNode = minimum(node->right); // Find the minimum node in the right subtree
        node->pair = minimumNode->pair; // Swap node's pair with the minimum node's pair
        removeNode(tree, minimumNode); // Recursively remove the minimum node
    }
    else // If the node to be removed has one child
    {
        TreeNode * child = (node->left != NULL) ? node->left : node->right; // Create a temporary node to store the child of the node to be removed

        if (parent == NULL) // If the node has no parent, set the root to the child node
            tree->root = child;
        else
        {
            if (parent->left == node) // If the node to be removed is the left child of the parent, replace it with the child
                parent->left = child;
            else // If the node to be removed is the right child of the parent, replace it with the child
                parent->right = child;
            if (child != NULL) // If the child is not NULL, update its parent
                child->parent = parent;
        }
    }

    free(node); // Free the memory of the removed node
    tree->current = parent; // Update the current node to the parent
}
``` // If the node to be removed is the left child of the parent
                parent->left = NULL; 
            else // If the node to be removed is the right child of the parent
                parent->right = NULL;
        }
    }
    else if (node->left != NULL && node->right != NULL) // If the node to be removed has two children
    {
        TreeNode * minimumNode = minimum(node->right); // Find the minimum node in the right subtree
        node->pair = minimumNode->pair; // Swap node's pair with the minimum node's pair
        removeNode(tree, minimumNode); // Recursively remove the minimum node
    }
    else // If the node to be removed has one child
    {
        TreeNode * child = (node->left != NULL) ? node->left : node->right; // Create a temporary node to store the child of the node to be removed

        if (parent == NULL) // If the node has no parent, set the root to the child node
            tree->root = child;
        else
        {
            if (parent->left == node) // If the node to be removed is the left child of the parent, replace it with the child
                parent->left = child;
            else // If the node to be removed is the right child of the parent, replace it with the child
                parent->right = child;
            if (child != NULL) // If the child is not NULL, update its parent
                child->parent = parent;
        }
    }

    free(node); // Free the memory of the removed node
    tree->current = parent; // Update the current node to the parent
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    tree->current = tree->root;
    while (tree->current != NULL)
        {
            if (is_equal(tree,key,tree->current->pair->key))
                return tree->current->pair;
            if (tree->lower_than(key,tree->current->pair->key))
                tree->current = tree->current->left;
            else
                tree->current = tree->current->right;
        }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* ub = NULL;
    tree->current = tree->root;
    while (tree->current != NULL)
        {
            if (is_equal(tree, key, tree->current->pair->key))
            {
                ub = tree->current;
                tree->current = tree->current->right;
                break;
            }
            if (tree->lower_than(key, tree->current->pair->key))
            {
                ub = tree->current;
                tree->current = tree->current->left;
            }
            else
                tree->current = tree->current->right;
        }
    if (ub == NULL) 
        return NULL;
    else 
        return ub->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    tree->current = minimum(tree->root);
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode* next = NULL;
    if (tree->current->right != NULL)
    {
        next = minimum(tree->current->right);
    }
    else{
        TreeNode* parent = tree->current->parent;
        while (parent != NULL && tree->current == parent->right)
            {
                tree->current = parent;
                parent = parent->parent;
            }
        next = parent;
    }
    if (next != NULL)
    {
        tree->current = next;
        return next->pair;
    }
    else
    {
        tree->current = NULL;
        return NULL;
    }
}
