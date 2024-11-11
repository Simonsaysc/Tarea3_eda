#include "trees/rb.hpp"
#include <iostream>

namespace trees {

RB::RB() : root(nullptr) {}

/* void RB::insert(int val) {
    if (root == nullptr) {
        root = new RBNode(val);
        root->setColor(NodeColor::BLACK);  //la raíz siempre debe ser negra
    } else {
        insert(val, root);
    }
} 

void RB::insert(int val, RBNode* node) {
    if (val < node->getData()) {
        if (node->getLeft() == nullptr) {
            RBNode* new_node = new RBNode(val, node);
            new_node->setColor(NodeColor::RED);  //los nuevos nodos son rojos por defecto
            node->setLeft(new_node);
            balance(new_node);  //llama a balance después de insertar
        } else {
            insert(val, node->getLeft());
        }
    } else {
        if (node->getRight() == nullptr) {
            RBNode* new_node = new RBNode(val, node);
            new_node->setColor(NodeColor::RED); //los nuevos nodos son rojos por defecto
            node->setRight(new_node);
            balance(new_node);  //llama a balance después de insertar
        } else {
            insert(val, node->getRight());
        }
    }
}
 */

void RB::insert(int val) {
    RBNode* newNode = new RBNode(val);
    newNode->setColor(NodeColor::RED);  //los nuevos nodos son rojos por defecto

    if (root == nullptr) {
        root = newNode;
        root->setColor(NodeColor::BLACK);  //la raíz siempre es negra
        return;
    }

    RBNode* current = root;
    RBNode* parent = nullptr;
    RBNode* grandparent = nullptr;

    //iterativamente buscar la posición de inserción
    while (current != nullptr) {
        grandparent = parent;
        parent = current;
        if (val < current->getData()) {
            current = current->getLeft();
        } else {
            current = current->getRight();
        }
    }

    //establece el nuevo nodo en la posición encontrada
    newNode->setParent(parent);
    if (val < parent->getData()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }

    balance(parent);  //llama a balance después de la inserción
}
void RB::balance(RBNode* node) {
    RotationType rType = getRotationType(node);
    if (rType == RotationType::case_1) {
        do_case_1(node);
    } else if (rType == RotationType::case_2) {
        do_case_2(node);
    } else if (rType == RotationType::case_3) {
        do_case_3(node);
    }
}

RotationType RB::getRotationType(RBNode* node) {
    RotationType rType = RotationType::case_1;
    RBNode* leftChild = node->getLeft();
    RBNode* rightChild = node->getRight();

    if (leftChild != nullptr && rightChild != nullptr &&
        leftChild->getColor() == 'R' && rightChild->getColor() == 'R') {
        rType = RotationType::case_1;
    } else if ((leftChild != nullptr && leftChild->getColor() == 'R' &&
               leftChild->getLeft() != nullptr && leftChild->getLeft()->getColor() == 'R') ||
               (rightChild != nullptr && rightChild->getColor() == 'R' &&
                rightChild->getRight() != nullptr && rightChild->getRight()->getColor() == 'R')) {
        rType = RotationType::case_2;
    }
    else if((leftChild != nullptr && leftChild->getColor() == 'R' &&
               leftChild->getRight() != nullptr && leftChild->getRight()->getColor() == 'R') ||
               (rightChild != nullptr && rightChild->getColor() == 'R' &&
                rightChild->getLeft() != nullptr && rightChild->getLeft()->getColor() == 'R')){
        rType = RotationType::case_3;
    }

    return rType;
}

void RB::do_case_1(RBNode* node) {
    RBNode* parent = node->getParent();
    RBNode* leftChild = node->getLeft();
    RBNode* rightChild = node->getRight();
    node->setColor(NodeColor::RED);
    if (parent == nullptr) {
        node->setColor(NodeColor::BLACK);
    }
    if (leftChild) leftChild->setColor(NodeColor::BLACK);
    if (rightChild) rightChild->setColor(NodeColor::BLACK);
}

void RB::do_case_2(RBNode* node) {
    RBNode* parent = node->getParent();
    RBNode* leftChild = node->getLeft();
    RBNode* rightChild = node->getRight();
    bool isLeftChild = node->isLeft();
    if (leftChild && leftChild->getColor() == 'R' && leftChild->getLeft() && leftChild->getLeft()->getColor() == 'R') {
        node->setLeft(leftChild->getRight());
        leftChild->setRight(node);
        if (node == root) {
            root = leftChild;
        } else {
            if (isLeftChild) {
                parent->setLeft(leftChild);
            } else {
                parent->setRight(leftChild);
            }
        }
        node->setColor(NodeColor::RED);
        leftChild->setColor(NodeColor::BLACK);
    } else if (rightChild && rightChild->getColor() == 'R' && rightChild->getRight() && rightChild->getRight()->getColor() == 'R') {
        node->setRight(rightChild->getLeft());
        rightChild->setLeft(node);
        if (node == root) {
            root = rightChild;
        } else {
            if (isLeftChild) {
                parent->setLeft(rightChild);
            } else {
                parent->setRight(rightChild);
            }
        }
        node->setColor(NodeColor::RED);
        rightChild->setColor(NodeColor::BLACK);
    }
}

void RB::do_case_3(RBNode* node) {
    RBNode* parent = node->getParent();
    RBNode* grandparent = parent ? parent->getParent() : nullptr;
    if (parent && parent->isRight() && node->isLeft()) {
        rotateRight(parent);
        node = parent;
    } else if (parent && parent->isLeft() && node->isRight()) {
        rotateLeft(parent);
        node = parent;
    }
    do_case_2(node);
}

void RB::rotateLeft(RBNode* node) {
    RBNode* rightChild = node->getRight();
    RBNode* parent = node->getParent();
    node->setRight(rightChild->getLeft());
    rightChild->setLeft(node);
    if (parent != nullptr) {
        if (node->isLeft()) {
            parent->setLeft(rightChild);
        } else {
            parent->setRight(rightChild);
        }
    } else {
        root = rightChild;
    }
    rightChild->setParent(parent);
    node->setParent(rightChild);
}

void RB::rotateRight(RBNode* node) {
    RBNode* leftChild = node->getLeft();
    RBNode* parent = node->getParent();
    node->setLeft(leftChild->getRight());
    leftChild->setRight(node);
    if (parent != nullptr) {
        if (node->isLeft()) {
            parent->setLeft(leftChild);
        } else {
            parent->setRight(leftChild);
        }
    } else {
        root = leftChild;
    }
    leftChild->setParent(parent);
    node->setParent(leftChild);
}

void RB::traverse(RBNode* node, int label) {
    if (node != nullptr) {
        std::cout<<node->getData()<<std::endl;
        std::cout<<"es izquierdo: "<< node->isLeft()<<std::endl;
        traverse(node->getLeft(), label + 1);
        traverse(node->getRight(), label + 1);
    }
}

void RB::traverse() {
    traverse(root, 1);
}

void RB::clear(RBNode* node) {
    if (node != nullptr) {
        clear(node->getLeft());
        clear(node->getRight());
        delete node;
    }
}

RB::~RB() {
    clear(root);
    root = nullptr;
}

RBNode* RB::find(int val, RBNode* node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (val == node->getData()) {
        return node;
    }
    if (val < node->getData()) {
        return find(val, node->getLeft());
    } else {
        return find(val, node->getRight());
    }
}

RBNode* RB::find(int key) {
    return find(key, root);
}

}  // namespace trees