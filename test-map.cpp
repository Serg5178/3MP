#include "btree_map.h"
#include <iostream>
#include <ctime>
int main(int argc, char* argv[]){ 
    srand(time(NULL));
    Btree<int, int> test(5); 
    test.insert(30, 1);
    test.insert(70, 2);
    test.insert(8, 3);
    test.insert(25, 4);
    test.insert(40, 5);
    test.insert(50, 6);
    test.insert(76, 7);
    test.insert(88, 8);
    test.insert(1, 9);
    test.insert(3, 10);
    test.insert(7, 11);
    test.insert(15, 12);
    test.insert(21, 13);
    test.insert(23, 14);
    test.insert(26, 15);
    test.insert(28, 16);
    test.insert(35, 17);
    test.insert(38, 18);
    test.insert(42, 19);
    test.insert(49, 20);
    test.insert(56, 21);
    test.insert(67, 22);
    test.insert(71, 23);
    test.insert(73, 24);
    test.insert(75, 25);
    test.insert(77, 26);
    test.insert(85, 27);
    test.insert(89, 28);
    test.insert(97, 29);

    for(int i = 0; i < 500; i++){
        test.insert(rand()%1000, rand()%1000);
        
    }
    test.test();
    std::cout << test.isEmpty() << std::endl;
    std::cout << test[1] << std::endl; 
    Btree<int, int> test1(5); 
    test1 = test; 
    test.clear();
    std::cout << test.isEmpty() << std::endl;
    test1.test();
    std::cout << test1.isEmpty() << std::endl;
    std::cout << test1[1] << std::endl; 
    return 0; 
}