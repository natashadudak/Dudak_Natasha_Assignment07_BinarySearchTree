// CS 210 - Assignment 07: Binary Search Tree Operations
 // Natasha Dudak
 // C++17

 #include <algorithm>
 #include <iostream>
 #include <string>
 #include <vector>

 class BinarySearchTree {
 private:
     struct Node {
         int key;
         Node* left;
         Node* right;

         explicit Node(int value) : key(value), left(nullptr), right(nullptr) {}
     };

     Node* root = nullptr;

     static Node* insertNode(Node* node, int key, bool& inserted) {
         if (node == nullptr) {
             inserted = true;
             return new Node(key);
         }

         if (key < node->key) {
             node->left = insertNode(node->left, key, inserted);
         } else if (key > node->key) {
             node->right = insertNode(node->right, key, inserted);
         }
         // Equal keys are ignored to keep every value unique.
         return node;
     }

     static Node* removeNode(Node* node, int key, bool& removed) {
         if (node == nullptr) {
             return nullptr;
         }

         if (key < node->key) {
             node->left = removeNode(node->left, key, removed);
         } else if (key > node->key) {
             node->right = removeNode(node->right, key, removed);
         } else {
             removed = true;

             // Case 1 and Case 2: zero or one child.
             if (node->left == nullptr) {
                 Node* next = node->right;
                 delete node;
                 return next;
             }
             if (node->right == nullptr) {
                 Node* next = node->left;
                 delete node;
                 return next;
             }

             // Case 3: two children. Use the inorder successor.
             Node* successor = node->right;
             while (successor->left != nullptr) {
                 successor = successor->left;
             }
             node->key = successor->key;

             bool successorRemoved = false;
             node->right = removeNode(node->right, successor->key,
                                      successorRemoved);
         }
         return node;
     }

     static void inorderNode(const Node* node, std::vector<int>& values) {
         if (node == nullptr) return;
         inorderNode(node->left, values);
         values.push_back(node->key);
         inorderNode(node->right, values);
     }

     static void preorderNode(const Node* node, std::vector<int>& values) {
         if (node == nullptr) return;
         values.push_back(node->key);
         preorderNode(node->left, values);
         preorderNode(node->right, values);
     }

     static void postorderNode(const Node* node, std::vector<int>& values) {
         if (node == nullptr) return;
         postorderNode(node->left, values);
         postorderNode(node->right, values);
         values.push_back(node->key);
     }

     static int heightNode(const Node* node) {
         if (node == nullptr) return -1;
         return 1 + std::max(heightNode(node->left), heightNode(node->right));
     }

     static void clearNode(Node* node) {
         if (node == nullptr) return;
         clearNode(node->left);
         clearNode(node->right);
         delete node;
     }

 public:
     ~BinarySearchTree() {
         clearNode(root);
     }

     // Disable copying to avoid two trees owning the same nodes.
     BinarySearchTree() = default;
     BinarySearchTree(const BinarySearchTree&) = delete;
     BinarySearchTree& operator=(const BinarySearchTree&) = delete;

     bool insert(int key) {
         bool inserted = false;
         root = insertNode(root, key, inserted);
         return inserted;
     }

     bool contains(int key) const {
         const Node* current = root;
         while (current != nullptr) {
             if (key == current->key) return true;
             current = (key < current->key) ? current->left : current->right;
         }
         return false;
     }

     bool remove(int key) {
         bool removed = false;
         root = removeNode(root, key, removed);
         return removed;
     }

     std::vector<int> inorder() const {
         std::vector<int> values;
         inorderNode(root, values);
         return values;
     }

     std::vector<int> preorder() const {
         std::vector<int> values;
         preorderNode(root, values);
         return values;
     }

     std::vector<int> postorder() const {
         std::vector<int> values;
         postorderNode(root, values);
         return values;
     }

     int height() const {
         return heightNode(root);
     }
 };

 void printValues(const std::vector<int>& values) {
     for (int value : values) {
         std::cout << value << ' ';
     }
     std::cout << '\n';
 }

 bool strictlyIncreasing(const std::vector<int>& values) {
     for (std::size_t i = 1; i < values.size(); ++i) {
         if (values[i - 1] >= values[i]) return false;
     }
     return true;
 }

 void showInorderTest(const BinarySearchTree& tree) {
     std::vector<int> values = tree.inorder();
     std::cout << "Inorder: ";
     printValues(values);
     std::cout << "Strictly sorted: "
               << (strictlyIncreasing(values) ? "PASS" : "FAIL") << "\n";
 }

 int main() {
     BinarySearchTree tree;
     const int keys[] = {50, 30, 70, 20, 40, 60, 80,
                         10, 35, 45, 65, 75, 85};

     for (int key : keys) {
         tree.insert(key);
     }

     std::cout << "CS 210 - Binary Search Tree Operations\n";
     std::cout << "Inserted 13 distinct integers.\n\n";

     std::cout << "INITIAL TREE\n";
     showInorderTest(tree);
     std::cout << "Preorder: ";
     printValues(tree.preorder());
     std::cout << "Postorder: ";
     printValues(tree.postorder());
     std::cout << "Height (edges): " << tree.height() << "\n\n";

     std::cout << "SEARCH TESTS\n";
     std::cout << "Search 40 (present): "
               << (tree.contains(40) ? "FOUND" : "NOT FOUND") << '\n';
     std::cout << "Search 99 (absent): "
               << (tree.contains(99) ? "FOUND" : "NOT FOUND") << "\n\n";

     std::cout << "REMOVAL TESTS\n";
     const int removalKeys[] = {45, 60, 30};
     const std::string cases[] = {"leaf", "one child", "two children"};

     for (int i = 0; i < 3; ++i) {
         int key = removalKeys[i];
         bool removed = tree.remove(key);
         std::cout << "Remove " << key << " (" << cases[i] << "): "
                   << (removed && !tree.contains(key) ? "PASS" : "FAIL")
                   << '\n';
         showInorderTest(tree);
         std::cout << '\n';
     }

     std::cout << "EDGE CASES\n";
     std::cout << "Insert duplicate 70: "
               << (!tree.insert(70) ? "PASS (ignored)" : "FAIL") << '\n';
     std::cout << "Remove absent 999: "
               << (!tree.remove(999) ? "PASS (not found)" : "FAIL") << '\n';
     std::cout << "Successor 35 is still present: "
               << (tree.contains(35) ? "PASS" : "FAIL") << '\n';
     std::cout << "Child 65 is still present: "
               << (tree.contains(65) ? "PASS" : "FAIL") << '\n';
     std::cout << "Final height (edges): " << tree.height() << '\n';
     return 0;
 }