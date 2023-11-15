int function_call(int a, int b);
int x();

int main() {
    int a = 0.12;
    int b = (a + 1);

    // a = 1 * 23;

    function_call(1, 2);

    int y = function_call(3, 4);

    return a / b;
}
