#include <iostream>

using namespace std;

template <typename TC, typename TV>
class DicioAVL {
public:
    struct Noh {
        TC key;
        TV val;

        int height = 0;

        Noh* left   = nullptr;
        Noh* right  = nullptr;
        Noh* father = nullptr;

        Noh* obter_dir() {
            return right;
        }

        Noh* obter_esq() {
            return left;
        }

        Noh* obter_pai() {
            return father;
        }
    };

    Noh* obter_raiz() {
        return root;
    }

private:
    Noh* root;

    Noh* obter_menor(Noh* cur) {
        if (cur == nullptr) {
            return nullptr;
        }

        Noh* aux = cur;
        Noh* aux2 = cur->left;

        while (aux2 != NULL) {
            aux = aux2;
            aux2 = aux2->left;
        }
        return aux;
    }

    int get_height(Noh* no) {
        if (no == NULL) {
            return -1;
        }
        else {
            return no->height;
        }
    }

    int balanced(Noh* no) {
        return abs(get_height(no->left) - get_height(no->right));
    }

    void rotationLL(Noh** root) {
        if ((*root) == nullptr) {
            return;
        }

        Noh* aux = (*root)->left;

        if (aux != nullptr) {
            aux->father = (*root)->father;
        }

        (*root)->father = aux;

        if (aux != nullptr) {
            (*root)->left = aux->right;
            aux->right = (*root);
        }

        if ((*root)->left != nullptr) {
            (*root)->left->father = (*root);
        }

        (*root)->height
            = max(get_height((*root)->left), get_height((*root)->right)) + 1;
        if (aux != nullptr) {
            aux->height
                = max(get_height(aux->left), (*root)->height) + 1;
        }

        (*root) = aux;
    }

    void rotationRR(Noh** root) {
        if ((*root) == nullptr) {
            return;
        }

        Noh* aux = (*root)->right;

        if (aux != nullptr) {
            aux->father = (*root)->father;
        }

        (*root)->father = aux;

        if (aux != nullptr) {
            (*root)->right = aux->left;
            aux->left = (*root);
        }


        if ((*root)->right != nullptr) {
            (*root)->right->father = (*root);
        }

        (*root)->height = max(get_height((*root)->left), get_height((*root)->right)) + 1;

        if (aux != nullptr) {
            aux->height = max(get_height(aux->right), (*root)->height) + 1;
        }

        (*root) = aux;
    }

    void rotationLR(Noh** root) {
        rotationRR(&(*root)->left);
        rotationLL(root);
    }

    void rotationRL(Noh** root) {
        rotationLL(&(*root)->right);
        rotationRR(root);
    }

    Noh* insert(Noh** root, Noh* pai, TC key, TV val) {
        if ((*root) == NULL) {
            Noh* tmp = new Noh();

            tmp->key    = key;
            tmp->val    = val;
            tmp->height = 0;
            tmp->right  = NULL;
            tmp->left   = NULL;
            tmp->father = pai;

            (*root) = tmp;
            return tmp;
        }

        Noh* cur = (*root), * aux = nullptr;
        if (key < cur->key) {
            if ((aux = insert(&(*root)->left, *root, key, val)) != nullptr) {
                if (balanced(cur) >= 2) {
                    if ((*root)->left != nullptr && key < (*root)->left->key) {
                        rotationLL(root);
                    }
                    else {
                        rotationLR(root);
                    }
                }
            }
        }
        else {
            if (key > cur->key) {
                if ((aux = insert(&(*root)->right, *root, key, val)) != nullptr) {
                    if (balanced(cur) >= 2) {
                        if ((*root)->right != nullptr && key > (*root)->right->key) {
                            rotationRR(root);
                        }
                        else {
                            rotationRL(root);
                        }
                    }
                }
            }
            else {
                return nullptr;
            }
        }

        cur->height = max(get_height(cur->left), get_height(cur->right)) + 1;
        return aux;
    }

    Noh* remove(Noh** root, TC key) {
        if ((*root) == NULL) {
            return nullptr;
        }

        Noh* aux = nullptr;

        if (key < (*root)->key) {
            if ((aux = remove(&(*root)->left, key)) != nullptr) {
                if (balanced(*root) >= 2) {
                    if (get_height((*root)->right->left) <= get_height((*root)->right->right)) {
                        rotationRR(root);
                    }
                    else {
                        rotationRL(root);
                    }
                }
            }
        }
        if ((*root)->key < key) {
            if ((aux = remove(&(*root)->right, key)) != nullptr) {
                if (balanced(*root) >= 2) {
                    if ((*root)->left != nullptr && get_height((*root)->left->right) <= get_height((*root)->left->left)) {
                        rotationLL(root);
                    }
                    else {
                        rotationLR(root);
                    }
                }
            }
        }

        if ((*root)->key == key) {
            if ((*root)->left == nullptr || (*root)->right == nullptr) {
                Noh* aux = (*root);
                if ((*root)->left != nullptr) {
                    (*root)->left->father = (*root)->father;
                    (*root) = (*root)->left;
                }
                else {
                    if ((*root)->right != nullptr) {
                        (*root)->right->father = (*root)->father;
                    }
                    (*root) = (*root)->right;
                }

                delete[] aux;
            }
            else {
                Noh* temp = obter_menor((*root)->right);

                Noh* aux = (*root);

                if (aux->right == temp) {
                    aux->right = temp->right;
                    temp->right = aux;
                }
                else {
                    temp->father->left = aux;
                    swap(aux->right, temp->right);
                }

                swap(temp->father, aux->father);

                swap(temp->height, aux->height);

                swap(temp->left, aux->left);

                if (temp->left != nullptr) {
                    temp->left->father = temp;
                }

                if (temp->right != nullptr) {
                    temp->right->father = temp;
                }

                if (aux->left != nullptr) {
                    aux->left->father = aux;
                }

                if (aux->right != nullptr) {
                    aux->right->father = aux;
                }

                (*root) = temp;

                aux = remove(&temp->right, aux->key);
                if (balanced(*root) >= 2) {
                    if (get_height((*root)->left->right) <= get_height((*root)->left->left)) {
                        rotationLL(root);
                    }
                    else {
                        rotationLR(root);
                    }
                }
            }
            if ((*root) != nullptr) {
                (*root)->height = max(get_height((*root)->left), get_height((*root)->right)) + 1;
            }
            return (Noh*)0xBEEF;
        }

        (*root)->height = max(get_height((*root)->left), get_height((*root)->right)) + 1;
        return aux;
    }

    Noh* buscar(Noh* root, TC key) {
        if (root == nullptr) return 0;
        Noh* cur = root;
        while (cur != nullptr) {
            if (key == cur->key) {
                return cur;
            }
            if (key > cur->key) {
                cur = cur->obter_dir();
            }
            else {
                cur = cur->obter_esq();
            }
        }
        return nullptr;
    }

public:

    void free_memory(Noh* subtree) {
        if (subtree == NULL)
            return;
        if (subtree->obter_esq() == nullptr && subtree->obter_dir() == nullptr) {
            delete[] subtree;
            subtree = nullptr;
            return;
        }
        free_memory(subtree->obter_esq());
        free_memory(subtree->obter_dir());
    }

    DicioAVL() {
        root = nullptr;
    }

    ~DicioAVL() {
        free_memory(this->obter_raiz());
    }

    class Iterador {
    private:
        Noh* ptr = nullptr;

    public:
        void mudar_no(Noh* newNode) {
            this->ptr = newNode;
        }

        bool operator!=(Iterador j) {
            return this->ptr != j.ptr;
        }

        bool operator==(Iterador j) {
            return this->ptr == j.ptr;
        }

        Noh* getNode() {
            return this->ptr;
        }

        TC chave() {
            return this->ptr->key;
        }

        TV valor() {
            return this->ptr->val;
        }

        void operator++() {
            if (ptr->obter_pai() != nullptr && ptr->obter_pai()->key > ptr->key) {
                if (ptr->obter_dir() == nullptr) {
                    ptr = ptr->obter_pai();
                }
                else {
                    Noh* aux = ptr->obter_dir();
                    while (aux->obter_esq() != nullptr) {
                        aux = aux->obter_esq();
                    }

                    ptr = aux;
                }
            }
            else {
                if (ptr->obter_dir() != nullptr) {
                    Noh* aux = ptr->obter_dir();
                    while (aux->obter_esq() != nullptr) {
                        aux = aux->obter_esq();
                    }

                    ptr = aux;
                }
                else {
                    Noh* aux = ptr;
                    while (aux->obter_pai() != nullptr && aux->key > aux->obter_pai()->key) {
                        aux = aux->obter_pai();
                    }

                    if (aux->obter_pai() == nullptr) {
                        ptr = nullptr;
                    }
                    else {
                        ptr = aux->obter_pai();
                    }
                }
            }
        }
    };

    void remover(Iterador it) {
        this->remove(&this->root, it.chave());
    }

    Iterador inicio() {
        Iterador iter;
        iter.mudar_no(obter_menor(obter_raiz()));
        return iter;
    }


    Iterador fim() {
        Iterador iter;
        iter.mudar_no(nullptr);
        return iter;
    }

    Iterador inserir(TC c, TV v) {
        Iterador it_aux;
        Noh* node = this->insert(&root, nullptr, c, v);
        if (node != nullptr) {
            it_aux.mudar_no(node);
        }

        return it_aux;
    }

    Iterador buscar(TC c) {
        Iterador iter;
        Noh* aux = buscar(root, c);
        iter.mudar_no(aux);
        return iter;
    }
};
