#include <iostream>
#include <memory>
#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>


struct Sun {
    int x, y;
};

void fill(std::auto_ptr<Sun> z) {
    z->x = rand();
    z->y = rand();
}

struct Convertion {
    int Get() {
        std::auto_ptr<Sun> z(new Sun);
        fill(z);
        return z->x + z->y;
    }
};

struct Pure {
    Pure(int x):x(x) { }
    int upgrade(int z) {
        char* buf = new char[100];
        sprintf(buf, "%s", z);
        for (int i = 0; i < strlen(buf); ++i) {
            x += buf[i] - '0';
        }
    }
    int x;
};

int main() {
    Pure A(5);
    A.upgrade(131);

    exit(0);
    assert(A.x == 5 + 1 + 3 + 1);

    Convertion Q;
    printf("%s", Q.Get());
}
