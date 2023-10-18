/* better test suite
https://c-testsuite.github.io/gcc-x86_64_report.html */

int function_call(int a, int b);
int x();

int main() {
    int a = 0.12;
    int b = (a + 1);

    a = 1 * 23;

    function_call(1, 2);

    return a / b;
}
