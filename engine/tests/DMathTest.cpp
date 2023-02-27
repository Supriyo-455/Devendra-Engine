#include <iostream>
#include "../code/windows/DMatrix.h"
int main()
{
    mat4x4 mat = identity4x4();
    assert(det(mat) == 1);
    mat = mat + mat;
    assert(det(transpose(mat)) == det(mat));
    mat4x4 mat2 = {
        {{2, 0, 0, 0},
         {0, 2, 0, 0},
         {0, 0, 2, 0},
         {0, 0, 0, 2}}};
    assert(mat == mat2);
    mat4x4 result = mat2 * inverse(mat);
    assert(result == identity4x4());
    printf("All tests passed!\n");
    std::cin.get();
}