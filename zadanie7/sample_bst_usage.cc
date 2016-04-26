#include "bst_imp.hh"
#include <iostream>

int main() {
    {
        BST<int> bst({3});
        // 3
    }

    {
        BST<int> bst({3, 4});
        // 3    |
        //  \   |
        //   4  |
    }

    {
        BST<int> bst({3, 4, 1});
        //   3    |
        //  / \   |
        // 1   4  |
    }

    {
        BST<int> bst({3, 4, 1, 2});
        //    3    |
        //   / \   |
        //  1   4  |
        //   \     |
        //    2    |
    }
    {
        BST<int> bst({3,4,1,2,7});
        //    3      |
        //   / \     |
        //  1   4    |
        //   \   \   |
        //    2   7  |
    }

    BST<int> bst({3,4,1,2,7,3});
    //    3          |
    //   / \         |
    //  1   4        |
    //   \   \       |
    //    2   7      |
    //     \         |
    //      3        |

    std::cout << bst << std::endl; // prints 1 2 3 3 4 7
    std::cout << bst.size() << std::endl; //prints 6
    std::cout << bst.min() << std::endl; // prints 1
    std::cout << bst.max() << std::endl; // prints 7
    std::cout << bst.height() << std::endl; // prints 4
    std::cout << spine(bst).height() << std::endl; // prints 6
    std::cout << bst.find(4) << std::endl; // prints 4 7
    std::cout << max_diff(bst) << std::endl; //prints 3
    std::cout << bst.find(11) << std::endl; //throws

    return 0;
}
