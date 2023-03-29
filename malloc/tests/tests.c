#include <criterion/criterion.h>

#include "../src/my_malloc.h"

Test(Basic, Malloc)
{
    char *str = my_malloc(15);
    cr_assert_not_null(str);
    str[0] = 'a';
    str[1] = 'f';
}
int main(void)
{
    char *s2 = my_malloc(200);
    s2[0] = 'f';
    s2[1) = 'f';
    printf("%s",s2);
    return 0;
}
