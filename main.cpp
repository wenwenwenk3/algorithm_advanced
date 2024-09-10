#include <iostream>
#include <vector>
using namespace std;
#include "monstack_op.h"
#include "monqueue_op.h"
#include "rb_tree.h"
//#include "kruskal_prim.h"
#include "kmp_trie.h"
#include "greed_algorthm.h"

#include    <iostream>
#include    <fstream>
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;


    // testnextGreaterElementsCircle();
    // testnextGreaterElementI();
    // testmaxSlidingWindow();

//    TestAVLtree();
//    TestRBTree();
//
//    testmaxSubarraySumCircular();

//    testminCostConnectPoints();

    // testTrie();
    // testTriereplaceWords();

//
//    std::cout << removeKdigits("100", 1) << std::endl;
//
//    vector<int> nums1 = {12,24,8,32};
//    vector<int> nums2 = {13,25,32,11};
//    vector<int> res = advantageCount(nums1, nums2);
//    for(const auto & item : res){
//        std::cout << item << ",";
//    }
//    std::cout << std::endl;

    // test_trap();

    ifstream ifs(__FILE__);
    cout << ifs.rdbuf();
    ifs.close();

    return 0;
}
