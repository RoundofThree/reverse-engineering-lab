Exercise 1: mystery1

```c
void mystery1(char* arg1, int* arg2) {
    char r3 = *arg1; 
    int r6;
    if (r3 == "-") { // 0x2d
        r3 = arg1[1];
        arg1++;
        r6 = 1;
    } else {
        r6 = 0;
        if (r3 == '+') { // 0x2b
            r3 = arg1[1];
            arg1++;
        }
    }
    while (arg1[0] == '0') {
        ++arg1;
    }
    int r4 = 0; 
    while ('0' <= arg1[0] <= '9') {
        r4 = r4*10 + arg1[0] - '0';
    }
    if (abs(r4) > 2^31) {
        return false;
    } else {
        *arg2 = r4;
        return true;
    }
}
```