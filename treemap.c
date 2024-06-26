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
    if (node == NULL) return; //Si el nodo es nulo retorna
    if(node->parent == NULL) //Si el nodo es la raiz
    {
        if(node->left == NULL && node->right == NULL) //Si el nodo no tiene hijos
        {
            tree->root = NULL;
        }
        else
        {
            if(node->left == NULL) //Si el nodo tiene un hijo derecho
            {
                tree->root = node->right;
                tree->root->parent = NULL;
            }
            else
            {
                if(node->right == NULL) //Si el nodo tiene un hijo izquierdo
                {
                    tree->root = node->left;
                    tree->root->parent = NULL;
                }
            }
        }
    }
    TreeNode * parent = node->parent; //Creamos un nodo auxiliar para guardar el padre del nodo a eliminar
    if (node->left == NULL && node->right == NULL) //Si el nodo a eliminar no tiene hijos
    {
        if (parent->left == node) //Si el nodo a eliminar es el hijo izquierdo del padre
            parent->left = NULL; 
        else  //Si el nodo a eliminar es el hijo derecho del padre
            parent->right = NULL;
    }
    else if (node->left != NULL && node->right != NULL) //Si el nodo a eliminar tiene dos hijos
    {
        TreeNode * aux = node->right; //Buscamos el nodo más pequeño en el subárbol derecho
        aux = minimum(aux);
        node->pair->key = aux->pair->key;
        node->pair->value = aux->pair->value;
        removeNode(tree,aux); // Corrected recursive call
    }
    else //Si el nodo a eliminar tiene un hijo
    {
        TreeNode * child = (node->left != NULL) ? node->left : node->right; //Creamos un nodo auxiliar para guardar el hijo del nodo a eliminar
        if (parent->left == node) parent->left = child; //Si el nodo a eliminar es el hijo izquierdo del padre, lo reemplazamos por el hijo
        else parent->right = child; //Si el nodo a eliminar es el hijo derecho del padre, lo reemplazamos por el hijo
        if (child != NULL) child->parent = parent; //Si el hijo no es nulo, actualizamos su padre
    }
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
