//
// Created by kai.chen on 2021/12/30.
//
//      1. 布隆过滤器
//

#ifndef ALGORITHM_ADVANCED_BOOM_FILTER_H
#define ALGORITHM_ADVANCED_BOOM_FILTER_H

// 1. 布隆过滤器
// 通过多个hash函数映射到同一块位图区间，快速判定元素是否存在。
//  背景：如果想要判断一个元素是不是在一个集合里，一般想到的是将所有元素保存起来，然后通过比较确定。
//      链表，树等等数据结构都是这种思路. 但是随着集合中元素的增加，我们需要的存储空间越来越大，检索速度也越来越慢(O(n),O(logn))

//      可以通过一个Hash函数将一个元素映射成一个位阵列（Bit array）中的一个点。
//      这样一来，我们只要看看这个点是不是1就可以知道集合中有没有它了。这就是布隆过滤器的基本思想
//  Hash面临的问题就是冲突。假设Hash函数是良好的，如果我们的位阵列长度为m个点，那么如果我们想将"冲突率"降低到例如 1%, 这个散列表就只能容纳m / 100个元素
//      这样就不够空间效率了（Space-efficient）了。
//
//  所以就衍生出来了，"布隆过滤器"，就是使用多个Hash，如果它们有一个说元素不在集合中，那肯定就不在。
//


//-----------------------------------------------------------------------------------
//
//          "位图"的实现
//
//-----------------------------------------------------------------------------------

typedef struct BitMap
{
    size_t* _bits;
    size_t _range;
}BitMap;

void BitMapInit(BitMap* bm, size_t range);
void BitMapSet(BitMap* bm, size_t x);
void BitMapReset(BitMap* bm, size_t x);
int BitMapTest(BitMap* bm,size_t x);

void BitMapInit(BitMap* bm, size_t range){
    assert(bm);
    bm->_range = range;
    bm->_bits = (size_t*)malloc(sizeof(size_t)*((range>>5)+1));
    assert(bm->_bits);
    memset(bm->_bits, 0, sizeof(size_t)*((range >> 5) + 1));
}
void BitMapSet(BitMap* bm, size_t x){
    assert(bm);
    size_t index = (x >> 5);
    size_t num = x % 32;
    bm->_bits[index] |= (1 << num);
}

void BitMapReset(BitMap* bm, size_t x){
    assert(bm);
    size_t index = (x >> 5);
    size_t num = x % 32;
    bm->_bits[index] ^= (1 << num);
}
//存在返回0，不存在返回-1
int BitMapTest(BitMap* bm, size_t x){
    assert(bm);
    size_t index = (x >> 5);
    if (bm->_bits[index] & (1 << (x % 32))){
        return 0;
    }
    else
        return -1;
}

void BitMapDestroy(BitMap* bm){
    free(bm->_bits);
}


//-----------------------------------------------------------------------------------
//
//          "布隆过滤器"的实现
//
//-----------------------------------------------------------------------------------

/// BKDR Hash Function
/// 本算法由于在Brian Kernighan与Dennis Ritchie的《The C Programming Language》一书被展示而得名，
///         是一种简单快捷的hash算法，也是Java目前采用的字符串的Hash算法（累乘因子为31）。
size_t BKDRHash(KeyType str){
    size_t hash = 0;
    size_t ch;
    while ( ch = (size_t)*str++){
        hash = hash * 131 + ch;   // 也可以乘以31、131、1313、13131、131313.
    }
    return hash;
}
// SDBM Hash Function
// 本算法是由于在开源项目SDBM（一种简单的数据库引擎）中被应用而得名
//          它与BKDRHash思想一致，只是种子不同而已。
size_t SDBMHash(KeyType str){
    size_t hash = 0;
    size_t ch;
    while (ch = (size_t)*str++){
        hash = 65599 * hash + ch;
        //hash = (size_t)ch + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

/// RS Hash Function
/// 因Robert Sedgwicks在其《Algorithms in C》一书中展示而得名。
size_t RSHash(KeyType str){
    size_t hash = 0;
    size_t magic = 63689;
    size_t ch;
    while (ch = (size_t)*str++){
        hash = hash * magic + ch;
        magic *= 378551;
    }
    return hash;
}

typedef char* KeyType;
typedef size_t(*HASH_FUNC)(KeyType str);

typedef struct BloomFilter{
    BitMap _bm;

    HASH_FUNC hashfunc1;
    HASH_FUNC hashfunc2;
    HASH_FUNC hashfunc3;
}BloomFilter;

void BloomFilterInit(BloomFilter* bf,size_t range){
    assert(bf);
    BitMapInit(&bf->_bm,range);

    bf->hashfunc1 = BKDRHash;
    bf->hashfunc2 = SDBMHash;
    bf->hashfunc3 = RSHash;
}
void BloomFilterSet(BloomFilter* bf, KeyType key){
    assert(bf);

    size_t range = bf->_bm._range;
    size_t hash1 = bf->hashfunc1(key);
    size_t hash2 = bf->hashfunc2(key);
    size_t hash3 = bf->hashfunc3(key);

    BitMapSet(&bf->_bm,hash1%range);
    BitMapSet(&bf->_bm,hash2%range);
    BitMapSet(&bf->_bm,hash3%range);
}
//存在返回0，不存在返回-1
int BloomFilterTest(BloomFilter* bf, KeyType key){
    assert(bf);

    size_t range = bf->_bm._range;

    if (BitMapTest(&bf->_bm, bf->hashfunc1(key) % range) == -1)
        return -1;

    if (BitMapTest(&bf->_bm, bf->hashfunc2(key) % range) == -1)
        return -1;

    if (BitMapTest(&bf->_bm, bf->hashfunc3(key) % range) == -1)
        return -1;
    return 0;
}

void BloomFilterDestroy(BloomFilter* bf){
    BitMapDestroy(&bf->_bm);
}

void TestBloomFilter(){
    BloomFilter bf;
    BloomFilterInit(&bf, 10000);
    BloomFilterSet(&bf, "sort");

    BloomFilterSet(&bf, "insert");
    size_t x = 0;
    printf("%d\n",BloomFilterTest(&bf,"sort"));
}





//-----------------------------------------------------------------------------------
//
//          "哈希表"的实现
//
//-----------------------------------------------------------------------------------
class MyHashMap {
private:
    vector<list<pair<int, int>>> data;
    static const int base = 769;
    static int hash(int key) {
        return key % base;
    }
public:
    /** Initialize your data structure here. */
    MyHashMap(): data(base) {}

    /** value will always be non-negative. */
    void put(int key, int value) {
        int h = hash(key);
        for (auto it = data[h].begin(); it != data[h].end(); it++) {
            if ((*it).first == key) {
                (*it).second = value;
                return;
            }
        }
        data[h].push_back(make_pair(key, value));
    }

    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        int h = hash(key);
        for (auto it = data[h].begin(); it != data[h].end(); it++) {
            if ((*it).first == key) {
                return (*it).second;
            }
        }
        return -1;
    }

    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) {
        int h = hash(key);
        for (auto it = data[h].begin(); it != data[h].end(); it++) {
            if ((*it).first == key) {
                data[h].erase(it);
                return;
            }
        }
    }
};


//-----------------------------------------------------------------------------------
//
//          "RandomSet"的实现 - 插入，删除和随机访问都是 O(1)的容器
//
//
//-----------------------------------------------------------------------------------

class RandomizedSet {
private:
    vector<int> nums;
    unordered_map<int, int> indices;
public:
    RandomizedSet() {
        srand((unsigned)time(NULL));
    }

    bool insert(int val) {
        if (indices.count(val)) {
            return false;
        }
        int index = nums.size();
        nums.emplace_back(val);
        indices[val] = index;
        return true;
    }

    bool remove(int val) {
        if (!indices.count(val)) {
            return false;
        }
        int index = indices[val];
        int last = nums.back();
        nums[index] = last;
        indices[last] = index;
        nums.pop_back();
        indices.erase(val);
        return true;
    }

    int getRandom() {
        int randomIndex = rand()%nums.size();
        return nums[randomIndex];
    }
};



#endif //ALGORITHM_ADVANCED_BOOM_FILTER_H
