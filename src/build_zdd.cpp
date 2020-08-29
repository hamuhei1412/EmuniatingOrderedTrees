#include <iostream>
#include <vector>
#include <utility>
#include "../../TdZdd/include/tdzdd/DdSpec.hpp"
#include "../../TdZdd/include/tdzdd/DdStructure.hpp"
#include "../../TdZdd/include/tdzdd/DdEval.hpp"

class EmuniatingOrderdTrees: public tdzdd::PodArrayDdSpec<EmuniatingOrderdTrees, short, 2>{
    int const n, k, edge_num;
    std::vector<std::pair<int, int>> Edge;
public:
    EmuniatingOrderdTrees(int n, int k)
    : n(n), k(k), edge_num(n * (n - 1) / 2){
        setArraySize(n);
        for(int i = 0;i < n - 1;i++){
            for(int j = i + 1;j < n;j++){
                Edge.push_back(std::make_pair(i, j));
            }
        }
    }
    
    int getRoot(short* state){
        for(int i = 0;i < n;i++){
            state[i] = -1;
        }
        state[0] = 0;
        return edge_num;
    }

    int getChild(short* state, int level, int take) const {
        std::pair<int, int> e = Edge[edge_num - level];
        if(state[e.first] == -1)return 0;
        
        if(take == 1){
            if(state[e.first] != -1 && state[e.second] != -1) return 0;
            if(state[e.second - 1] == -1)return 0;
            if(state[e.first] == k)return 0;

            state[e.second] = 0;
            state[e.first]++;
            if(e.second == n - 1){
                return -1;
            }
        }
        if(e.second == n - 1){
            state[e.first] = -1;
        }
        return level - 1;
    }
};

class Counting: public tdzdd::DdEval<Counting, long long>{
public:
    void evalTerminal(long long& v, int id){
        v = id;
    }

    void evalNode(long long& v, int level, tdzdd::DdValues<long long, 2> const& values) const{
        v = values.get(0) + values.get(1);
    }
};


int main(){
    int n, k;
    std::cin >> n >> k;
    EmuniatingOrderdTrees Emuniating(n, k);
    tdzdd::DdStructure<2> dd(Emuniating);
    long long ans = dd.evaluate(Counting());
    std::cout<<"answer:"<<ans<<std::endl;
    std::cout<<"zdd size:"<<dd.size()<<std::endl;
    //dd.dumpDot();
}