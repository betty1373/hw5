#include "lib.h"
template<typename Container>
void printContainter(const Container& values) {
    for (auto v : values)
        std::cout << v << ' ';
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, const std::pair<const int, int>& pair) {
    os << pair.first << " " << pair.second << '\n';
    return os;
}  
using namespace libSrc;
int main(int, char *[]) {
    const int num_els = 10;
    auto std_map = std::map<int,int,std::less<int>,std::allocator<std::pair<const int,int>>>{};  

    for (auto i=0;i<num_els;++i) {
        std_map.emplace(std::pair<int,int>(i,fact(i)));
    }
 
    auto cust_map = std::map<int,int,std::less<int>,logging_allocator<std::pair<const int,int>,num_els>>{};  

    for (auto i=0;i<num_els;++i) {
        cust_map.emplace(std::pair<int,int>(i,fact(i)));
    }
    printContainter(cust_map);
    
    auto cust_list=ForwardList<int,std::allocator<int>>{}; 
    for (auto i=0;i<num_els;++i) {
         cust_list.push_back(i);
    }
    std::cout << std::boolalpha;
    std::cout <<"Custom list with std::allocator is empty ? "<< cust_list.empty()<<std::endl;

    auto cust_list1 = ForwardList<int,logging_allocator<int,num_els>>{};
    std::cout <<"Custom list with logging_allocator is empty ? "<< cust_list1.empty()<<std::endl;
    for (auto i=0;i<num_els;++i) {
        cust_list1.push_back(i);
    }

    std::cout <<"Custom list with logging_allocator' size "<< cust_list1.size()<<std::endl;
    for (auto iter=cust_list1.cbegin(), iterF(cust_list1.cend());iter!=iterF;++iter) {
        std::cout<< *iter << " ";
    }
    std::cout<<std::endl;
    
    return 0;
}
