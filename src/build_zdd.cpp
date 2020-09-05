#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include "../../TdZdd/include/tdzdd/DdSpec.hpp"
#include "../../TdZdd/include/tdzdd/DdStructure.hpp"
#include "../../TdZdd/include/tdzdd/DdEval.hpp"

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>


bool is_reduction = true;

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
            return level - 1;
        }else{
            if(is_reduction){
                do {
                    if(e.second == n - 1){
                        state[e.first] = -1;
                    }
                    level--;
                    if(level == 0)return 0;
                    e = Edge[edge_num - level];
                }while((state[e.first] != -1 && state[e.second] != -1 || state[e.second - 1] == -1 || state[e.first] == k));
                return level;
            }
        }
        if(e.second == n - 1){
            state[e.first] = -1;
        }
        return level - 1;
    }
};

namespace mp = boost::multiprecision;
std::vector<int> size_of_level;

class Counting: public tdzdd::DdEval<Counting, mp::cpp_int>{
public:
    void evalTerminal(mp::cpp_int& v, int id){
        v = id;
    }

    void evalNode(mp::cpp_int& v, int level, tdzdd::DdValues<mp::cpp_int, 2> const& values) const{
        size_of_level[level]++;
        v = values.get(0) + values.get(1);
    }
};

void solve(int n, int k){
    clock_t start = clock();
    EmuniatingOrderdTrees Emuniating(n, k);
    tdzdd::DdStructure<2> dd(Emuniating);
    dd.zddReduce();
    clock_t stop = clock();

    size_of_level = std::vector<int>(n * (n - 1) / 2 + 1);

    mp::cpp_int ans = dd.evaluate(Counting());

    int max = *std::max_element(size_of_level.begin(), size_of_level.end());
    
    if(ans == 0)ans = 1;
    mp::cpp_dec_float_100 compressibility = (mp::cpp_dec_float_100)max / (mp::cpp_dec_float_100)ans;

    std::cout<<compressibility<<std::endl;
    
    //std::cout<<"n = "<<n<<" k = "<<k<<std::endl;
    //std::cout<<n<<":"<<ans<<std::endl;
    //std::cout<<n<<":"<<dd.size()<<std::endl;
    //std::cout<<n<<" "<<static_cast<double>(stop - start) / CLOCKS_PER_SEC * 1000.0<<std::endl;;
    //dd.dumpDot();
}


int main(){
    int k;
    std::cin >> k;
    for(int i = 1;i <= 150;i++){
        solve(i, k);
    }
}