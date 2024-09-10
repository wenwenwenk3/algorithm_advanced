//
// Created by kai.chen on 2021/12/18.
// 字符串匹配算法：
//      1. BF 朴素匹配
//      2. KMP
//
//      3. 前缀树
//       3.1. 单词替换
//       3.2. 模拟设计内存文件系统
//

#ifndef ALGORITHM_ADVANCED_KMP_TRIE_H
#define ALGORITHM_ADVANCED_KMP_TRIE_H
#include <string>
#include <vector>
#include <sstream>
using namespace std;

// 1. BF 朴素匹配
// 题目描述：a是主串，b是模式串
// 思路： BF算法直接对于主串每一个位置进行暴力匹配
//    时间复杂度：最坏O(n*m)，效率不高。但因为实现简单易理解，实际场景中却比较常用。
//    原因有 (1).实际情况string长度都比较小，小数据量复杂度高不一定就差
//          (2).每次匹配的时候遇到不相同的就可以提前终止，虽然最坏情况下复杂度是n*m，但往往不需要这么长
//          (3).思想简单，易于debug，在满足性能要求的前提下。工程中简单易理解是首选。KISS原则 Keep It Simple and Stupid
int bf(char a[], int n, char b[], int m){
    for(int i = 0; i < n-m; i++){
        int j = 0;
        while(j < m){
            if(a[i+j] != b[j]){
                break;
            }
            j++;
        }
        if(j == m) return i;
    }
    return -1;
}

// 2. KMP 算法
// 思路： 算法的核心是在匹配过程中，当遇到不可匹配字符时希望找到一些规律，让模式串往后多滑动几位
//      跳过那些肯定不会匹配的情况
// KMP具体思路：
//      当遇到坏字符时，对于已经比对过的好前缀，找到一种规律，将模式串一次性往后移动多位
//      通过一个数组存放每个前缀的最长可匹配前缀子串的结尾字符下标。称为失效数组next。图文理解参考：https://blog.csdn.net/v_july_v/article/details/7041827
// 如果文本串的长度为n，模式串的长度为m，那么匹配过程的时间复杂度为O(n)，算上计算next的O(m)时间，KMP的整体时间复杂度为O(m + n)。
void GetNext(char* p, int next[]){ // 如 ABCDABD  前缀子串: A,AB,ABC,ABCD... 后缀子串是: D, BD,ABD,DABD...
    int pLen = strlen(p);
    next[0] = -1;
    int k = -1;
    int j = 0;
    // 第一步，寻找 前缀子串 & 后缀子串 公共元素的最大长度 又叫《最大长度表》
    // 失配时，模式串向右移动的位数为：已匹配字符数 - 失配字符的上一位字符所对应的最大长度值
    //  关键理解：next 数组相当于“最大长度值” 整体向右移动一位，然后初始值赋为-1。
    //    最大长度表：0,0,0,0,1,2,0
    //    next数组: -1,0,0,0,0,1,2
    // 从而有 ==> 失配时，模式串向右移动的位数为：失配字符所在位置 - 失配字符对应的next 值

    // // next[j] = k 代表p[j] 之前的模式串子串中，有长度为k 的相同前缀和后缀。
    // 那么已知next [0, ..., j]，如何求出next[j + 1]呢？
    while (j < pLen - 1){
        // 若p[k] == p[j]，则next[j+1] = next[j] +1 = k + 1；
        if (k == -1 || p[j] == p[k]){
            next[j] = k +1 ;
            k++;
            j++;
        }
        // 若p[k ] ≠ p[j]，k跳到next[k]继续查
        else{
            k = next[k];
        }
    }
}
int KMPSearch(string& s, string& p, vector<int>& next){
    int i = 0;
    int j = 0;
    int sLen = s.size();
    int pLen = p.size();
    while (i < sLen && j < pLen){
        //①如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++
        if (j == -1 || s[i] == p[j]){
            i++;
            j++;
        }else{
            //②如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]
            //next[j]即为j所对应的next值
            j = next[j];
        }
    }
    if (j == pLen) // j走到头，说明模式串p都被匹配了
        return i - j;
    else
        return -1;
}






// 3. trie前缀树
//   trie 树也称为字典树、单词查找树, 是一种树形数据结构，用于高效地 存储和检索字符串数据集中的键。
//    可以用于 搜索引擎的单词提示, 敏感词过滤（单词替换）。特点就是共享字符串的公共前缀来达到节省空间的目的
//
//请你实现 Trie 类：
/**
* Your Trie object will be instantiated and called as such:
* Trie* obj = new Trie();
* obj->insert(word);
* bool param_2 = obj->search(word);
* bool param_3 = obj->startsWith(prefix);
*/
class Trie {
private:
    vector<Trie*> children;
    bool isEnd;

    Trie* searchPrefix(const string& prefix) {
        Trie* node = this;
        for (char ch : prefix) {
            ch -= 'a';
            if (node->children[ch] == nullptr) {
                return nullptr;
            }
            node = node->children[ch];
        }
        return node;
    }
public:
    Trie(): children(26), isEnd(false) {}

    void insert(const string& word) {
        Trie* node = this;
        for (char ch : word) {
            ch -= 'a';
            if (node->children[ch] == nullptr) {
                node->children[ch] = new Trie();
            }
            node = node->children[ch];
        }
        node->isEnd = true;
    }

    bool search(const string& word) {
        Trie* node = this->searchPrefix(word);
        return node != nullptr && node->isEnd;
    }

    bool startsWith(string prefix) {
        return this->searchPrefix(prefix) != nullptr;
    }

    string find(const string& word){ // 找出字典中word的最短词根,不存在返回""
        string ans;
        Trie* node = this;
        for(char ch : word){
            if(node->children[ch-'a'] != nullptr){
                ans += ch;
                node = node->children[ch-'a'];
                if(node->isEnd) {
                    return ans;
                }
            }else{
                break;
            }
        }
        return node->isEnd ? ans : "";
    }

    ~Trie() {
        for (auto child: children) {
            delete child;
        }
    }
};
// 时间复杂度：初始化为 O(1)，其余操作为 O(|S|)，其中 |S|是每次插入或查询的字符串的长度。
// 空间复杂度：O(∣T∣⋅Σ)，其中 ∣T∣ 为所有插入字符串的长度之和，Σ 为字符集的大小，本题 Σ=26。

void testTrie(){
    string word = "helloworld";
    string prefix = "hello";
    Trie* obj = new Trie();

    obj->insert(word);
    bool param_2 = obj->search(word);
    bool param_3 = obj->startsWith(prefix);

    cout<<"search success:"<<param_2<<"startwith success:"<<param_3<<endl;
    delete obj;
}


// 3.2 单词替换
//  用来解决 前缀匹配问题  可以用于 搜索引擎的单词提示
// https://my.oschina.net/labuladong/blog/4792860
// https://leetcode-cn.com/problems/UhWRSj/solution/zui-yi-yu-li-jie-de-javajie-fa-shi-jian-vcx4e/
//
// 题目描述：用最短的词根替换句子中的单词
// 思路：用单词前缀树，只需要实现 insert 插入树，和 find 找字典中word的最短词根
string replaceWords(vector<string>& dictionary, string& sentence) {
    stringstream ss;
    ss << sentence;

    Trie root;
    // 将所有单词放进 前缀树
    for(string &temp : dictionary){
        root.insert(temp);
    }

    string str;
    string ans;
    while(ss >> str){
        string res = root.find(str);
        if(!res.empty()){
            ans += res;
        }else {
            ans += str;
        }
        ans += ' ';
    }

    return ans.substr(0,ans.size()-1); // [)
}


void testTriereplaceWords(){
    string word = "helloworld";
    string prefix = "hello";
    Trie* obj = new Trie();

    obj->insert(word);
    bool param_2 = obj->search(word);
    bool param_3 = obj->startsWith(prefix);

    cout<<"search success:"<<param_2<<endl<<"startwith success:"<<param_3<<endl;


    delete obj;
}

// 3.3. 设计内存文件系统
// 输入:
//  ["FileSystem","ls","mkdir","addContentToFile","ls","readContentFromFile"]
//  [[],["/"],["/a/b/c"],["/a/b/c/d","hello"],["/"],["/a/b/c/d"]]
// 输出:
// [null,[],null,null,["a"],"hello"]
struct trie {
    bool isFile;
    string content;
    unordered_map<string, trie*> next;
    trie() : isFile(false) {}
};
class FileSystem {
public:
    FileSystem() {
        root = new trie();
    }
    vector<string> ls(string &path) {
        vector<string> pathArray = pathSplit(path);
        trie* cur = root;
        for (const auto &each : pathArray) {
            cur = cur->next[each];
        }
        vector<string> result;
        if (cur->isFile) {
            result.push_back(pathArray.back());
        } else {
            for (const auto &each : cur->next) {
                result.push_back(each.first);
            }
        }
        return result;
    }
    void mkdir(string &path) {
        vector<string> pathArray = pathSplit(path);
        trie *cur = root;
        for (const auto &each : pathArray) {
            if (cur->next.count(each) == 0) {
                cur->next[each] = new trie();
            }
            cur = cur->next[each];
        }
    }
    void addContentToFile(string &filePath, const string& content) {
        vector<string> fileArray = pathSplit(filePath);
        trie *cur = root;
        for (const auto &each : fileArray) {
            if (cur->next.count(each) == 0) {
                cur->next[each] = new trie();
            }
            cur = cur->next[each];
        }
        cur->isFile = true;
        cur->content += content;
    }
    string readContentFromFile(const string& filePath) {
        vector<string> fileArray = pathSplit(filePath);
        trie* cur = root;
        for (const auto &each : fileArray) {
            cur = cur->next[each];
        }
        return cur->content;
    }
private:
    trie *root;

    static vector<string> pathSplit(const string& path){
        vector<string> answer;
        if (path.empty()) {
            return answer;
        }
        stringstream ss(path);
        string s;
        while (getline(ss, s, '/')) {
            answer.push_back(s);
        }
        return vector<string>(answer.begin() + 1, answer.end()); //这里注意开始可能会有一个空字符
    }
};


#endif //ALGORITHM_ADVANCED_KMP_TRIE_H
