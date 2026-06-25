#include <iostream>
#include <kotone/retroactive_heap>

int heap_sum = 0;
void on_add(int x) { heap_sum += x; }
void on_del(int x) { heap_sum -= x; }

int main() {
    // Construction
    kotone::retroactive_heap<int, std::greater<>, on_add, on_del> heap(5);
    assert(heap.age() == 5);
    assert(heap.empty());
    assert(heap.size() == 0);

    // set pop / set push
    heap.set_pop(4);
    heap.set_pop(3);
    heap.set_push(2, 20);
    heap.set_push(1, 30);
    heap.set_push(0, 10);
    assert(heap.size() == 1);
    assert(heap.top() == 10);
    assert(heap_sum == 10);

    // reset pop
    heap.reset(4);
    assert(heap.size() == 2);
    assert(heap.top() == 20);
    assert(heap_sum == 30);

    // reset push
    heap.reset(2);
    assert(heap.size() == 1);
    assert(heap.top() == 10);
    assert(heap_sum == 10);
    heap.reset(1);
    assert(heap.empty());
    assert(heap_sum == 0);

    // empty pop
    heap.set_push(0, 100);
    for (int k = 1; k < 5; k++) heap.set_pop(k);
    assert(heap.empty());
    assert(heap_sum == 0);
    for (int k = 0; k < 4; k++) heap.reset(k);
    assert(heap.empty());
    assert(heap_sum == 0);

    // overwrite
    for (int k = 0; k < 5; k++) heap.set_push(k, k * 100);
    assert(heap_sum == 1000);
    heap.set_pop(1);
    heap.set_pop(2);
    assert(heap_sum == 700);
}
