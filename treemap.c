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
  TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  //new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value)
{
  if(tree == NULL)return;
  TreeNode* aux = tree->root;
  TreeNode* parent = NULL;
  TreeNode* new = createTreeNode(key, value);
  
  while(aux != NULL)
  {
    parent = aux;
    if(tree->lower_than(key, aux->pair->key))
    {
      aux = aux->left;
    }
    else if (tree->lower_than(aux->pair->key, key))
    {
      aux = aux->right;
    }
    else {return;}
  }

  if(parent == NULL)
  {
    tree->root = new;
    tree->current = new;
  }
  else if(tree->lower_than(new->pair->key, parent->pair->key))
  {
    new->parent = parent;
    parent->left = new;
    tree->current = new;
  }
  else
  {
    new->parent = parent;
    parent->right = new;
    tree->current = new;
  }
  
  
}

TreeNode * minimum(TreeNode * x){
  while(x->left)
    x = x->left;

  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  TreeNode* parent = node->parent;

  if ((node->left == NULL) && (node->right == NULL))
  {
    if (parent->right == NULL)
      parent->right = NULL;
    else
      parent->left = NULL;
    return;
  }
  else if ((node->left != NULL) && (node->right != NULL))
  {
    TreeNode* son = minimum(node->right);
    node->pair->key = son->pair->key;
    node->pair->value = son->pair->value;
    node = NULL;
    removeNode(tree, son);
    return;
  }
  else
  {
    if (node->left == NULL)
    {
      if (parent->left == NULL)
      {
        parent->left = node->left;
        parent->left->parent = parent;
      }
      else
      {
        parent->right = node->right;
        parent->right->parent = parent;
      }
      node = NULL;
      return;
    }
    else
    {
      if(parent->left == NULL)
      {
        parent->left = node->left;
        parent->left = parent;
      }
      else
      {
        parent->right = node->left;
        parent->right->parent = parent;
      }
      node = NULL;
      return;
    }
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode*  aux = tree->root;
  if(aux == NULL) return(NULL);
  if(aux->pair == NULL) return NULL;
  while(aux)
  {
    if(is_equal(tree,aux->pair->key, key)){
			tree->current = aux;
			return aux->pair;
		}
    else
    {
      if(tree->lower_than(aux->pair->key, key) == 1)
      {
        aux = aux->right;
      }
      else
      {
        aux = aux->left;
      }
    }
  }

  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
  return NULL;
}

Pair * firstTreeMap(TreeMap * tree)
{
  TreeNode* first = minimum(tree->root);
  return first->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
  
   if (tree->current->right)
   {
     tree->current = minimum(tree->current->right);
     return tree->current->pair->value;
   }
  
  TreeNode* aux = tree->current->parent;
  
  while((aux) && (tree->current == aux->right))
  {
    tree->current = aux;
    aux = aux->parent;
  }

  if (aux)
  {
    tree->current = aux;
    return (aux->pair->value);
  }
  return NULL;
}
