#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <ctime>

class HLL{
    public:
        unsigned int p; // precision
        unsigned int m; // registers count
        float alpha; // hash collision bias correction constant
        std::vector<unsigned int> registers;

        HLL(unsigned int precision):
            p(precision),
            m(pow(2, p)),
            registers(m)
        {
            alpha = 0.7213 / (1 + 1.079 / m);
        }

        void add(std::string input){
            std::hash<std::string> h; // hash function
            uint64_t hash = h(input);

            unsigned int index = hash >> (64 - p) & ((1ULL << p) - 1);

            uint64_t remaining_bits = hash & ((1ULL << (64 - p)) - 1);
            unsigned int rank = 0;
            while (remaining_bits != 0) {
                if (remaining_bits & (1ULL << (64 - p))) {
                    break;
                }
                remaining_bits <<= 1;
                rank++;
            }
            registers[index] = std::max(registers[index], rank);
        }

        unsigned int estimate(){
            float z = 0.0;
            for (int i = 0; i < m; i++){
                z += 1/pow(2, registers[i]);
            }
            return floor(alpha * m * m / z);
        }
};


int main() {
    HLL hll(4);
    for (int i = 0; i < 8; i++){
        std::string input;
        std::cin >> input;
        hll.add(input);
    }
    std::cout << hll.estimate() << std::endl;
    return 0;
}