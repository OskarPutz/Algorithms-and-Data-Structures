#include <string>
#include <iostream>

using namespace std;


struct ListNode {
    char data;
    ListNode* next;
    explicit ListNode(char val) : data(val), next(nullptr) {}
};

struct ListOfListsNode {
    ListNode* listHead;
    ListOfListsNode* next;
    ListOfListsNode() : listHead(nullptr), next(nullptr) {}
};

struct Stack {
    ListOfListsNode* top = nullptr;

    void push_empty_list() {
        ListOfListsNode* newNode = new ListOfListsNode();
        newNode->next = top;
        top = newNode;
    }
    void prepend_to_top(char c) {
        ListNode* newNode = new ListNode(c);
        newNode->next = top->listHead;
        top->listHead = newNode;
    }
    void pop_top_list() {
        if (top != nullptr) {
            ListOfListsNode* temp = top;
            top = top->next;
            delete temp;
        }
    }
    void negate_helper(ListNode* current, ListNode* prev) {
        if (current->next == nullptr) {
            if (current->data == '-') {
                // remove the last node if it contains '-'
                if (prev != nullptr) {
                    prev->next = nullptr;
                } else {
                    top->listHead = nullptr;
                }
                delete current;
            } else {
                // append '-' to the end of the list
                current->next = new ListNode('-');
            }
            return;
        }
        negate_helper(current->next, current);
    }
    void negate() {
        if (top != nullptr && top->listHead != nullptr) {
            negate_helper(top->listHead, nullptr);
        } else if (top != nullptr) {
            // If the list is empty, add '-' as the first node
            top->listHead = new ListNode('-');
        }
    }
    void absolute_value_helper(ListNode* current, ListNode* prev) {
        if (current->next == nullptr) {
            // last node
            if (current->data == '-') {
                // remove the last node if it contains '-'
                if (prev != nullptr) {
                    prev->next = nullptr;
                } else {
                    top->listHead = nullptr; // if it's the only node
                }
                delete current;
            }
            return;
        }
        absolute_value_helper(current->next, current);
    }
    void absolute_value() {
        // recursively check list for '-'
        if (top != nullptr && top->listHead != nullptr) {
            absolute_value_helper(top->listHead, nullptr);
        }
    }
    void print_list(ListNode* node) {
        if (node == nullptr) {
            return;
        }
        cout << node->data;
        print_list(node->next);
    }
    void print_stack(ListOfListsNode* node, int depth) {
        if (node == nullptr) {
            return;
        }
        print_stack(node->next, depth + 1);

        cout << depth << ": ";
        print_list(node->listHead);
        cout << endl;
    }
    void printer() {
        print_stack(top, 0);
    }

    ListNode* copy_list_helper(ListNode* node){
        if (node == nullptr) {
            return nullptr; // end of the list
        }
        ListNode* newNode = new ListNode(node->data);
        newNode->next = copy_list_helper(node->next); // recurse to copy the next node
        return newNode;
    };

    void copy_list_from_index_to_top() {
        if (top == nullptr || top->next == nullptr) {
            return; // not enough elements on the stack
        }

        // extract the number A from the top of the stack
        ListOfListsNode* numberNode = top;
        top = top->next; // remove the top node

        // helper to calculate the integer value
        auto calculate_index = [](ListNode* node, int multiplier, auto& self) -> int {
            if (node == nullptr) {
                return 0; // end of the list
            }
            int value = (node->data - '0') * multiplier;
            return value + self(node->next, multiplier * 10, self); // Recurse to the next node
        };

        int targetIndex = calculate_index(numberNode->listHead, 1, calculate_index);
        delete numberNode; // Clean up the extracted node

        // helper to traverse to the target index
        auto get_node_at_index = [](ListOfListsNode* node, int index, auto& self) -> ListOfListsNode* {
            if (node == nullptr || index == 0) {
                return node; // reached the target index or end of the stack
            }
            return self(node->next, index - 1, self); // Recurse to the next node
        };

        ListOfListsNode* targetNode = get_node_at_index(top, targetIndex, get_node_at_index);
        if (targetNode == nullptr) {
            return; // Target index is out of bounds
        }


    // create a deep copy of the list at the target index
    ListNode* copiedListHead = copy_list_helper(targetNode->listHead);

    // create a new node for the copied list and push it to the top of the stack
    ListOfListsNode* newListNode = new ListOfListsNode();
    newListNode->listHead = copiedListHead;
    newListNode->next = top;
    top = newListNode;
}

    void duplicate_top_list() {
        if (top == nullptr) {
            return; // No list to duplicate
        }

        // create a new node for the duplicated list
        ListOfListsNode* newNode = new ListOfListsNode();
        if (top->listHead != nullptr) {
            newNode->listHead = copy_list_helper(top->listHead);
        }

        // push the duplicated list onto the stack
        newNode->next = top;
        top = newNode;
    }
    void swap_top_two_lists() {
        ListOfListsNode* first = top;
        ListOfListsNode* second = top->next;

        // swap the top two nodes
        first->next = second->next;
        second->next = first;
        top = second;
    }
    void split_top_list() {

        // detach the first node from the top list
        ListNode* detachedNode = top->listHead;
        top->listHead = top->listHead->next;
        push_empty_list();
        prepend_to_top(detachedNode->data);

    }
    void write_and_pop_top_list() {
        if (top != nullptr && top->listHead != nullptr) {
            // write the first character of the list to standard output
            cout << top->listHead->data;

            pop_top_list();
        }
    }
    void convert_top_list_to_ascii_char() {
        if (top != nullptr && top->listHead != nullptr) {
            // extract the list A from the top of the stack
            ListOfListsNode* numberNode = top;
            top = top->next; // remove the top node

            // Recursive helper to calculate the integer value (ASCII code)
            auto calculate_ascii_code = [](ListNode* node, int multiplier, auto& self) -> int {
                if (node == nullptr) {
                    return 0; // Base case: end of the list
                }
                int value = isdigit(node->data) ? (node->data - '0') * multiplier : 0;
                return value + self(node->next, multiplier * 10, self); // Recurse to the next node
            };

            // calculate the ASCII code
            int asciiCode = calculate_ascii_code(numberNode->listHead, 1, calculate_ascii_code);

            delete numberNode; // clean up the extracted node

            // push the character with the ASCII code onto the stack
            push_empty_list();
            prepend_to_top(static_cast<char>(asciiCode));
        }
    }
    void push_ascii_code_of_first_char() {
        if (top != nullptr && top->listHead != nullptr) {
            // extract the list A from the top of the stack
            ListOfListsNode* listNode = top;
            top = top->next; // remove the top node

            // get the ASCII code of the first character in the list
            int asciiCode = listNode->listHead->data;

            // clean up the extracted node
            delete listNode;

            // push the ASCII code as a new list onto the stack
            push_empty_list();

            // Use recursion to prepend digits of the ASCII code
            auto prepend_digits = [this](int num, auto& self) -> void {
                if (num == 0) return; // no more digits to process
                self(num / 10, self); // process the higher-order digits first
                prepend_to_top('0' + (num % 10)); // prepend the current digit
            };
                prepend_digits(asciiCode, prepend_digits); // Start the recursion
        }
    }
    void append_top_list_to_next() {
        if (top == nullptr || top->next == nullptr) {
            return; // not enough elements on the stack
        }

        // pop the top list A off the stack
        ListOfListsNode* listA = top;
        top = top->next;

        // recursive helper to find the end of the list
        auto find_end = [](ListNode* node, auto& self) -> ListNode* {
            if (node == nullptr || node->next == nullptr) {
                return node; // end of the list
            }
            return self(node->next, self);
        };

        // check list B
        if (top->listHead == nullptr) {
            top->listHead = listA->listHead;
        } else {
            // find the end of the next list B and append list A to it
            ListNode* endOfB = find_end(top->listHead, find_end);
            endOfB->next = listA->listHead;
        }

        delete listA;
    }
    void logical_negation() {
        if (top == nullptr) {
            return; // no list
        }

        // check if the top list is empty or contains a single character '0'
        if (top->listHead == nullptr ||
            (top->listHead->data == '0' && top->listHead->next == nullptr)) {
            // replace with '1'
            pop_top_list();
            push_empty_list();
            prepend_to_top('1');
            } else {
                // replace with '0'
                pop_top_list();
                push_empty_list();
                prepend_to_top('0');
            }
    }
    int conditional_jump() {
        // pop the number T off the stack
        ListOfListsNode* numberNode = top;
        top = top->next;

        // helper to calculate the integer value (reversed order)
        auto calculate_integer = [](ListNode* node, int multiplier, auto& self) -> int {
            if (node == nullptr) {
                return 0; // end of the list
            }
            int value = (node->data - '0') * multiplier;
            return value + self(node->next, multiplier * 10, self);
        };

        int targetInstruction = calculate_integer(numberNode->listHead, 1, calculate_integer);
        delete numberNode; // clean up the extracted node

        // pop the list W off the stack
        ListOfListsNode* listWNode = top;
        top = top->next;

        // recursive helper to check if the list is empty or contains only '0'
        auto is_empty_or_zero = [](ListNode* node, auto& self) -> bool {
            if (node == nullptr) {
                return true; // empty list
            }
            if (node->data != '0') {
                return false; // found a non-'0' character
            }
            return self(node->next, self);
        };

        bool isEmptyOrZero = is_empty_or_zero(listWNode->listHead, is_empty_or_zero);
        delete listWNode;

        // if W is not empty and does not contain only '0', jump to T
        if (!isEmptyOrZero) {
            return targetInstruction;
        }

        return -1;
    }

    ListNode* normalize(ListNode*& head) {
        if (!head) return nullptr;

        // reverse the list
        head = reverse(head, nullptr);

        auto helper = [](ListNode* node, auto& self) -> ListNode* {
            if (node->next == nullptr || (node->data != '0' && node->data != '-')) {
                return node;
            }
            ListNode* next = node->next;
            delete node; // delete the current node
            return self(next, self);
        };

        head = helper(head, helper);

        return head;
    }
    bool is_negative(ListNode* head){
        if (!head) return false;
        // check if node is a '-'
        if (!head->next) return head->data == '-';
        return is_negative(head->next);
    }
    ListNode* reverse(ListNode* head, ListNode* prev){
        if (!head) return prev;
        ListNode* next = head->next;
        head->next = prev;
        return reverse(next, head);
    };
    void less_than() {

        // helper to compare magnitudes of two lists
        auto compare_magnitude = [](ListNode* a, ListNode* b, auto& self) -> int {
            // both lists are empty
            if (!a && !b) return 0;

            // helper to count digits
            auto count_digits = [](ListNode* node, auto& self) -> int {
                if (!node) return 0;
                return 1 + self(node->next, self);
            };

            int lenA = count_digits(a, count_digits);
            int lenB = count_digits(b, count_digits);

            if (lenA < lenB) return -1;
            if (lenA > lenB) return 1;

            // compare digit by digit
            if (a->data < b->data) return -1;
            if (a->data > b->data) return 1;
            return self(a->next, b->next, self);
        };


        // pop A and B from the stack
        ListOfListsNode* A = top; top = top->next;
        ListOfListsNode* B = top; top = top->next;
        bool aNeg = is_negative(A->listHead);
        bool bNeg = is_negative(B->listHead);

        A->listHead = normalize(A->listHead);
        A->listHead = reverse(A->listHead, nullptr);

        B->listHead = normalize(B->listHead);
        B->listHead = reverse(B->listHead, nullptr);


        // skip signs for magnitude comparison
        auto skip_sign = [](ListNode* head) -> ListNode* {
            return (head && head->data == '-') ? head->next : head;
        };

        ListNode* aMag = skip_sign(A->listHead);
        ListNode* bMag = skip_sign(B->listHead);

        // reverse magnitudes for comparison
        aMag = reverse(aMag, nullptr);
        bMag = reverse(bMag, nullptr);

        int cmp = compare_magnitude(aMag, bMag, compare_magnitude);

        // determine the result based on signs and comparison
        bool result = false;
        if (A->listHead->data == '0' && B->listHead->data == '0') {
            result = true;
        } else if (!aNeg && bNeg) {
            result = false;
        } else if (aNeg && !bNeg) {
            result = true;
        } else if (!aNeg && !bNeg) {
            if (cmp == 0) {
                result = true;
            } else {
                result = cmp < 0;
            }
        } else if (aNeg && bNeg) {
            if (cmp == 0) {
                result = true;
            } else {
                result = cmp > 0;
            }
        }

        // push the result onto the stack
        push_empty_list();
        prepend_to_top(result ? '0' : '1');

        // clean up
        delete A;
        delete B;
    }
    void equal() {
        // compare helper
        auto compare_lists = [](ListNode* a, ListNode* b, auto& self) -> bool {
            // both lists are empty
            if (!a && !b) return true;

            // one is empty, the other is not
            if (!a || !b) return false;

            // compare data
            if (a->data != b->data) return false;

            return self(a->next, b->next, self);
        };


        // pop A and B from stack
        ListOfListsNode* numberNodeA = top;
        top = top->next;

        ListOfListsNode* numberNodeB = top;
        top = top->next;
        bool aMinus = is_negative(numberNodeA->listHead);
        bool bMinus = is_negative(numberNodeB->listHead);
        // normalize both
        normalize(numberNodeA->listHead);
        normalize(numberNodeB->listHead);
        bool result = false;

        if (aMinus != bMinus) {
            // both values are zero
            bool aIsZero = numberNodeA->listHead && numberNodeA->listHead->data == '0' && numberNodeA->listHead->next == nullptr;
            bool bIsZero = numberNodeB->listHead && numberNodeB->listHead->data == '0' && numberNodeB->listHead->next == nullptr;
            result = aIsZero && bIsZero;
        } else {
            // compare normally
            result = compare_lists(numberNodeA->listHead, numberNodeB->listHead, compare_lists);
        }

        // clean up
        delete numberNodeA;
        delete numberNodeB;

        push_empty_list();
        prepend_to_top(result ? '1' : '0');
    }
    int count_zeros(ListNode* head){
        if (!head || head->data != '0') {
            return 0; // no more leading zeros
        }
        return 1 + count_zeros(head->next);
    }
    void addition(ListNode* A, ListNode* B, int carry){
        int a = (A->data - '0');
        int b = (B->data - '0');
        int result = a + b + carry;
        if (result > 9) {
            result -= 10;
            carry = 1;
        } else {
            carry = 0;
        }
        prepend_to_top(result + '0');
        if (A->next != nullptr && B->next != nullptr) {
            addition(A->next, B->next, carry);
        } else {
            if (carry == 1) {
                prepend_to_top('1');
            }
        }
    }
    void subtraction(ListNode* A, ListNode* B, int borrow) {
        // helper to compare magnitudes of two lists
        auto compare_magnitude = [](ListNode* a, ListNode* b, auto& self) -> int {
            if (!a && !b) return 0; // Both lists are empty
            if (!a) return -1;      // A is smaller
            if (!b) return 1;       // B is smaller

            int cmp = self(a->next, b->next, self); // Recurse to the next digit
            if (cmp != 0) return cmp;              // If deeper digits differ, return result

            // Compare current digits
            if (a->data < b->data) return -1;
            if (a->data > b->data) return 1;
            return 0; // Digits are equal
        };

        // check if A is smaller than B
        if (compare_magnitude(A, B, compare_magnitude) < 0) {
            // swap A and B, and prepend a '-' sign to the result
            subtraction(B, A, 0); // subtraction with swapped arguments
            prepend_to_top('-');
            return;
        }

        int a = (A->data - '0');
        int b = (B->data - '0');
        int result = a - b - borrow;

        if (result < 0) {
            result += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        prepend_to_top(result + '0'); // add result to the list

        if (A->next != nullptr || B->next != nullptr) {
            subtraction(A->next ? A->next : nullptr, B->next ? B->next : nullptr, borrow);
        }
    }
    void add() {
        // helper to equalize lengths of both lists
        auto prepend_zeros = [](ListNode* node, int multiplier, auto& self) -> ListNode* {
            if (multiplier <= 0) return node;
            ListNode* newNode = new ListNode('0');
            newNode->next = node;
            return self(newNode, multiplier - 1, self);
        };
        auto count_digits = [](ListNode* node, auto& self) -> int {
            if (!node) return 0;
            return 1 + self(node->next, self);
        };

        auto prepend = [this](ListNode* head, auto& self) -> ListNode* {
            if (head != nullptr) {
                prepend_to_top(head->data);
                self(head->next, self);
            }
            return head;
        };


        ListOfListsNode* numberNodeA = top;
        top = top->next;
        ListOfListsNode* numberNodeB = top;
        top = top->next;
        bool negA = is_negative(numberNodeA->listHead);
        bool negB = is_negative(numberNodeB->listHead);
        // Normalize both
        normalize(numberNodeA->listHead);
        normalize(numberNodeB->listHead);
        numberNodeA->listHead = reverse(numberNodeA->listHead, nullptr);
        numberNodeB->listHead = reverse(numberNodeB->listHead, nullptr);
        int lenA = count_digits(numberNodeA->listHead, count_digits);
        int lenB = count_digits(numberNodeB->listHead, count_digits);
        int diff = lenA - lenB;

        // prepend zeros to equalize the lengths of the two lists
        if (diff > 0) {
            numberNodeB->listHead = reverse(numberNodeB->listHead, nullptr);
            numberNodeB->listHead = prepend_zeros(numberNodeB->listHead, diff, prepend_zeros);
            numberNodeB->listHead = reverse(numberNodeB->listHead, nullptr);
        } else if(diff < 0) {
            numberNodeA->listHead = reverse(numberNodeA->listHead, nullptr);
            numberNodeA->listHead = prepend_zeros(numberNodeA->listHead, -diff, prepend_zeros);
            numberNodeA->listHead = reverse(numberNodeA->listHead, nullptr);
        }
        push_empty_list();
        // both lists are positive
        if (!negA && !negB) {
            addition(numberNodeA->listHead, numberNodeB->listHead, 0);
            ListOfListsNode* final = top;
            top = top->next;
            push_empty_list();
            prepend(final->listHead, prepend);
        } else if (negA && negB) { // both lists are negative
            addition(numberNodeA->listHead, numberNodeB->listHead, 0);
            prepend_to_top('-');
            ListOfListsNode* final = top;
            top = top->next;
            push_empty_list();
            prepend(final->listHead, prepend);
        } else if (!negA && negB) { // B is negative
            subtraction(numberNodeA->listHead, numberNodeB->listHead, 0);
            ListOfListsNode* final = top;
            top = top->next;
            bool sign = false;
            if (final->listHead->data == '-') {
                sign = true;
            }
            final->listHead = reverse(final->listHead, nullptr);
            normalize(final->listHead);
            if (count_digits(final->listHead, count_digits) == 0) {
                final->listHead->data = '0';
            }
            push_empty_list();
            if (sign && final->listHead->data != '0') {
                prepend_to_top('-');
            }
            prepend(final->listHead, prepend);
        } else if (negA && !negB) { // A is negative
            subtraction(numberNodeB->listHead, numberNodeA->listHead, 0);
            ListOfListsNode* final = top;
            top = top->next;
            bool sign = false;
            if (final->listHead->data == '-') {
                sign = true;
            }
            final->listHead = reverse(final->listHead, nullptr);
            normalize(final->listHead);
            if (count_digits(final->listHead, count_digits) == 0) {
                final->listHead->data = '0';
            }
            push_empty_list();
            if (sign && final->listHead->data != '0') {
                prepend_to_top('-');
            }
            prepend(final->listHead, prepend);
        }


    }

    void put_instruction_number(int a) {
        push_empty_list();

        // helper to extract and prepend digits in the correct order
        auto prepend_digits = [this](int num, auto& self) -> void {
            if (num == 0) return; // no more digits to process
            self(num / 10, self); // process the higher-order digits first
            prepend_to_top('0' + (num % 10));
        };

        if (a == 0) {
            prepend_to_top('0'); // case where the number is 0
        } else {
            prepend_digits(a, prepend_digits);
        }
    }
};

struct StackProcessor {
    char program[40000];
    int instruction_pointer = 0;
    int input_pointer;
    int end;
    Stack stack;
    void execute() {
        while (instruction_pointer < end) {
            switch (program[instruction_pointer]) {
                case '\'': stack.push_empty_list(); break;
                case ',': stack.pop_top_list(); break;
                case ':': stack.duplicate_top_list(); break;
                case ';': stack.swap_top_two_lists(); break;
                case '@': stack.copy_list_from_index_to_top(); break;
                case '.': stack.prepend_to_top(program[input_pointer]); input_pointer++; break;
                case '>': stack.write_and_pop_top_list(); break;
                case '!': stack.logical_negation(); break;
                case '<': stack.less_than(); break;
                case '=': stack.equal(); break;
                case '~': stack.put_instruction_number(instruction_pointer); break;
                case '?': {
                    int help = stack.conditional_jump();
                    if (help != -1) {
                        instruction_pointer = help - 1;
                    }
                    break;
                }
                case '-': stack.negate(); break;
                case '^': stack.absolute_value(); break;
                case '$': stack.split_top_list(); break;
                case '#': stack.append_top_list_to_next(); break;
                case '+': stack.add(); break;
                case '&': stack.printer(); break;
                case ']': stack.convert_top_list_to_ascii_char(); break;
                case '[': stack.push_ascii_code_of_first_char(); break;
                default: stack.prepend_to_top(program[instruction_pointer]); break;
            }
            instruction_pointer++;
        }
    }
};

int main() {
    string prog, inp;
    cin >> prog;
    cin.ignore();
    getline(cin, inp);
    StackProcessor processor;
    char program[40000];
    size_t i = 0;

    // copy the program into the program array
    for (; i < prog.length() && i < sizeof(program) - 1; i++) {
        program[i] = prog[i];
    }
    processor.input_pointer = i;
    processor.end = i;
    // append the input to the program array
    for (size_t j = 0; j < inp.length() && i < sizeof(program) - 1; j++, i++) {
        program[i] = inp[j];
    }

    program[i] = '\0'; // null-terminate the program

    strncpy(processor.program, program, sizeof(processor.program) - 1);
    processor.program[sizeof(processor.program) - 1] = '\0';
    processor.execute();
    return 0;
}