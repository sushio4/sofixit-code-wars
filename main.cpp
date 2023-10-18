/*
MIT License

Copyright (c) 2023 Maciej Suski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#include <iostream>
#include <vector>

//  This algorithm exploits the fact that when going towards the highest stack of boxes
//  the height of the material pool inside will be equal to previous max_stack
//  and the pool ends when it encounters new max_stack.

//  I was wondering whether it's better to do 
//
//  sum += pool_size * max_stack
//
//  at the end of a pool or rather
//
//  sum += max_stack
//
//  when looping through it.
//  For greater pools (more than 6 wide) it's better to multiply
//  since multiplication takes around 6 CPU cycles
//  while addition takes one cycle, so it would be better for pools smaller than 6.

//  The overhead from using lambda is zero as the g++ compiler with -O3 flag will generate almost the same assembly
//  with only difference being registers used and subtle differences in conditional jumps.

int material(std::vector<int> vec) {
    int max_stack = 0;
    int sum = 0;
    int pool_size = 0;
    auto max_index = vec.begin();

    //find max element
    for(auto i = vec.begin(); i < vec.end(); i++) {
        if(*i > max_stack)
            max_stack = *i, 
            max_index = i;
    }

    //DRY
    auto loop_body = [&](int current_stack) {
        if(current_stack < max_stack) {
            pool_size++;
            sum -= current_stack;
            return;
        }
        sum += pool_size * max_stack;
        pool_size = 0;
        max_stack = current_stack;
    };

    //go from the beginning to the highest stack of boxes
    //I reused max_stack to store maximum number until vec[i]
    max_stack = vec[0];
    for(auto i = vec.begin() + 1; i <= max_index; i++) {
        loop_body(*i);
    }

    //go from the end to the highest stack
    //once again reused max_stack
    max_stack = vec[vec.size() - 1];
    for(auto i = vec.end() - 2; i >= max_index; i--) {
        loop_body(*i);
    }

    return sum;
}

#define TEST(function_call, expected_output) { \
    if(function_call == expected_output) \
        std::cout << "\033[0;32mTest passed!\033[0m\n"; \
    else  \
        std::cout << "\033[0;31mTest failed on line " << __LINE__  << "!\033[0m\n"; \
}

void test_all() {
    TEST(material({0,1,0,2,1,0,3,1,2,0}), 5);
    TEST(material({0,3,2,0,3,2,0,4,2,0}), 8);

    TEST(material({0,1,0,2,1,0,1,3,2,1,2,1}), 6);
    TEST(material({0,1,0,2,1,0,3,1,0,1,2}), 8);
    TEST(material({4,2,0,3,2,5}), 9);

    TEST(material({6,4,2,0,3,2,0,3,1,4,5,3,2,7,5,3,0,1,2,1,3,4,6,8,1,3}), 83);
    TEST(material({6,2,1,1,8,0,5,5,0,1,8,9,6,9,4,8,0,0}), 50);
}

int main() {
    test_all();
    return 0;
}