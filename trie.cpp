#include <cstdio>
#include <cstdlib>

struct Node {
    int key;
    Node **children;
    int num_children;
    bool has_key;

    Node(int size) : key(0), has_key(false), num_children(size), children(nullptr) {}

    ~Node() {
        if (children) {
            for (int i = 0; i < num_children; ++i)
                if (children[i]) delete children[i];
            free(children);
        }
    }

    void ensure_children(int count) {
        if (!children) {
            num_children = count;
            children = new Node*[count]();
        }
    }
};

Node *root;
int N, K;

Node* insert(int x) {
    if (!root->has_key) {
        root->key = x;
        root->has_key = true;
        return 0;
    }

    int div = x % N;
    int quotient = x / N;
    root->ensure_children(N);

    Node* current = root->children[div];
    if (!current) current = root->children[div] = new Node(N);

    if (!current->has_key) {
        current->key = x;
        current->has_key = true;
        return 0;
    }

    while (true) {
        int nextDiv = quotient % K;
        quotient /= K;

        current->ensure_children(K);
        if (!current->children[nextDiv])
            current->children[nextDiv] = new Node(K);

        current = current->children[nextDiv];

        if (!current->has_key) {
            current->key = x;
            current->has_key = true;
            return 0;
        }
    }
}

Node* search(Node *node, int x, int depth, int quotient) {
    if (!node) return nullptr;
    if (node->has_key && node->key == x)
        return node;
    int div = depth == 0 ? x % N : quotient % K;
    quotient = quotient / (depth == 0 ? N : K);
    if (!node->children) return nullptr;
    return search(node->children[div], x, depth + 1, quotient);
}

bool is_leaf(Node *node) {
    if (!node->children) return true;
    for (int i = 0; i < node->num_children; ++i) {
        if (node->children[i] != nullptr && node->children[i]->has_key) return false;
    }
    return true;
}

Node* find_leaf(Node *node) {
    while (!is_leaf(node)) {
        for (int i = 0; i < node->num_children; ++i) {
            if (node->children[i] && node->children[i]->has_key) {
                node = node->children[i];
                break;
            }
        }
    }
    node->has_key = false;
    return node;
}
bool remove(int x) {
    Node* to_delete = search(root, x, 0, x);
    if (!to_delete) return false;
    if (is_leaf(to_delete)) {
        to_delete->has_key = false;
        return true;
    } else {
        to_delete->key = find_leaf(to_delete)->key;
        return true;
    }
}

void inorder(Node* node) {
    if (!node) return;
    if (node->has_key) printf("%d ", node->key);
    if (node->children) {
        for (int i = 0; i < node->num_children; ++i)
            inorder(node->children[i]);
    }
}

int main() {
    int t;
    scanf("%d", &t);

    int minVal, maxVal;
    scanf("%d %d", &minVal, &maxVal);

    scanf("%d %d", &N, &K);
    getchar(); // consume newline
    getchar(); // consume empty line

    root = new Node(N);
    char cmd;
    int x;

    for (int i = 0; i < t; ++i) {
        cmd = getchar();
        if (cmd == 'P') {
            inorder(root);
            printf("\n");
            getchar();
        } else {
            scanf("%d", &x);
            getchar();
            if (cmd == 'I') {
                if (search(root, x, 0, x))
                    printf("%d exist\n", x);
                else
                    insert(x);
            } else if (cmd == 'L') {
                if (search(root, x, 0, x))
                    printf("%d exist\n", x);
                else
                    printf("%d not exist\n", x);
            } else if (cmd == 'D') {
                if (!remove(x))
                    printf("%d not exist\n", x);
            }
        }
    }

    delete root;
    return 0;
}
