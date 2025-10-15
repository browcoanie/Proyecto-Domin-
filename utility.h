
struct mesaNode {
    Ficha ficha;
    mesaNode* prev;
    mesaNode* next;
    mesaNode(const Ficha& f) : ficha(f), prev(nullptr), next(nullptr) {}
};

struct Mesa {
    mesaNode* left;
    mesaNode* right;
    Mesa() : left(nullptr), right(nullptr) {}
};

void initMesa(Mesa &mesa) {
    mesa.left = nullptr;
    mesa.right = nullptr;
}

int valueLeft(const Mesa &mesa) {
    if (mesa.left == nullptr) return -1; // Indica que la mesa está vacía
    return mesa.left->ficha.lado1;
}

int valueRight(const Mesa &mesa) {
    if (mesa.right == nullptr) return -1; // Indica que la mesa está vacía
    return mesa.right->ficha.lado2;
}

void swapSides(Ficha &ficha) {
    int temp = ficha.lado1;
    ficha.lado1 = ficha.lado2;
    ficha.lado2 = temp;
}

bool isValidFicha(const Ficha& ficha) {
    return (ficha.lado1 >= 0 && ficha.lado1 <= 6) && (ficha.lado2 >= 0 && ficha.lado2 <= 6);
}

bool placeLeft(Mesa &mesa, Ficha ficha) {
    if (!isValidFicha(ficha)) {
        return false;
    }
    mesaNode* newNode = new mesaNode(ficha);
    if (newNode == nullptr) {
        return false;
    }
    if (mesa.left == nullptr) {
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }
    if (ficha.lado1 == valueLeft(mesa)) {
        swapSides(ficha);
    }
    if (ficha.lado2 == valueLeft(mesa)) {
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode;
        return true;
    }
    delete newNode;
    return false;
}

bool placeRight(Mesa &mesa, Ficha ficha) {
     if (!isValidFicha(ficha)) {
        return false;
    }
    mesaNode* newNode = new mesaNode(ficha);
    if (newNode == nullptr) {
        return false;
    }
    if (mesa.right == nullptr) {
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }
    if (ficha.lado2 == valueRight(mesa)) {
        swapSides(ficha);
    }
    if (ficha.lado1 == valueRight(mesa)) {
        newNode->prev = mesa.right;
        mesa.right->next = newNode;
        return true;
    }
    delete newNode;
    return false;
}


void clearMesa(Mesa &mesa) {
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        mesaNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    mesa.left = nullptr;
    mesa.right = nullptr;
}