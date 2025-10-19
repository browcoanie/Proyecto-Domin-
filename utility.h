
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
    
    // Si la mesa está vacía, simplemente añadimos la ficha
    if (mesa.left == nullptr) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }
    
    // Verificamos si la ficha coincide con el lado izquierdo
    int valorIzquierdo = valueLeft(mesa);
    
    // Si lado2 coincide, la ficha va en la orientación correcta
    if (ficha.lado2 == valorIzquierdo) {
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode;
        return true;
    }
    
    // Si lado1 coincide, volteamos la ficha
    if (ficha.lado1 == valorIzquierdo) {
        swapSides(ficha); // Ahora lado2 coincidirá
        mesaNode* newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode->next = mesa.left;
        mesa.left->prev = newNode;
        mesa.left = newNode;
        return true;
    }
    

    return false;
}


bool placeRight(Mesa &mesa, Ficha ficha) {
     if (!isValidFicha(ficha)) {
        return false;
    }

     // Si la mesa está vacía, simplemente añadimos la ficha
    if(mesa.right == nullptr){
        mesaNode *newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        mesa.left = newNode;
        mesa.right = newNode;
        return true;
    }

    // Verificamos si la ficha coincide con el lado derecho
    int valorDerecho = valueRight(mesa);

    // Si lado1 coincide, la ficha va en la orientación correcta
    if(ficha.lado1 == valorDerecho){
        mesaNode *newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode -> prev = mesa.right;
        mesa.right -> next = newNode;
        mesa.right = newNode;
        return true;
    }
    // Si lado2 coincide, volteamos la ficha
    if (ficha.lado2 == valorDerecho)
    {
        swapSides(ficha); // Ahora lado1 coindice 
        mesaNode *newNode = new mesaNode(ficha);
        if (newNode == nullptr) return false;
        newNode -> prev = mesa.right;
        mesa.right -> next = newNode;
        mesa.right = newNode;
        return true;
    }

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