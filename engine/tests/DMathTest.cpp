#include <iostream>
#include "../code/DMatrix.h"
int main()
{
    mat4x4 mat = identity();
    assert(det(mat) == 1);
    mat = mat + mat;
    assert(det(transpose(mat)) == det(mat));
    mat4x4 mat2 = {
        {
            {2, 0, 0, 0},
            {0, 2, 0, 0},
            {0, 0, 2, 0},
            {0, 0, 0, 2}
        }
    };
    assert(mat == mat2);
    printf("All tests passed!\n");
    std::cin.get();
}