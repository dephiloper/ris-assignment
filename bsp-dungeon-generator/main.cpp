#include <iostream>
#include <vector>
#include <memory>

float randf() {
    return (rand() % 100) / 100.f;
}

class Rectangle {
public:
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) { }

    int x;
    int y;
    int width;
    int height;
};

class Leaf: public Rectangle {
private:
    const unsigned int MIN_LEAF_SIZE = 6;
public:
    Leaf(int x, int y, int width, int height) : Rectangle(x, y, width, height) { }

    bool split() {
        // check if already splitted
        if (left != nullptr || right != nullptr)
            return false;
        
        // determine direction of split
        // if the width is > 25% larger than height, we split vertically
        // if the height is > 25% larger than the width, we split horizontally
        // otherwise we split randomly
        bool splitH = randf() > 0.5f;
        if (width > height && width / height >= 1.25)
            splitH = false;
        else if (height > width && height / width >= 1.25)
            splitH = true;

        // determine the maximum height or width
        unsigned int max = (splitH ? height : width) - MIN_LEAF_SIZE;
        
        if (max <= MIN_LEAF_SIZE)
            return false; // the area is too small to split any more

        unsigned int split = (rand() % max) + MIN_LEAF_SIZE; // determine where we're going to split

        // create our left and right children based on the direction of the split
        if (splitH)
        {
            left = std::make_shared<Leaf>(x, y, width, split);
            right = std::make_shared<Leaf>(x, y + split, width, height - split);
        }
        else
        {
            left = std::make_shared<Leaf>(x, y, split, height);
            right = std::make_shared<Leaf>(x + split, y, width - split, height);
        }
        return true; // split successful!
    }

    std::shared_ptr<Leaf> left;
    std::shared_ptr<Leaf> right;
    std::shared_ptr<Rectangle> room;
    std::vector<Rectangle> halls;
};

int main() {
    std::cout << "hey" << std::endl;
    return 0;
}