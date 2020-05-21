#include "block.h"

/**
 * Returns the width of the block.
 */
int Block::width() const{
    return data.size();
}

/**
 * Returns the height of the block.
 */
int Block::height() const{
    return data[0].size();
}

/**
 * Default Block constructor.
 */
Block::Block() {}

/**
 * Useful Block constructor.
 * Makes a block from the rectangle of width by height pixels in im
 * whose upper-left corner is at position (x,y).
 */
Block::Block(PNG & im, int x, int y, int width, int height) {
    int originalY = y;
    for (int i = 0; i < width; i++, x++) {
        //data[i].resize(width);
        vector<HSLAPixel> tempVector;
        for (int j = 0, y = originalY; j < height; j++, y++) {
            //data[i][j] = *(im.getPixel(x, y)); // get the image's pixels

            HSLAPixel pixel = *(im.getPixel(x,y));
            tempVector.push_back(pixel);
        }
        data.push_back(tempVector);
    }
}

/**
 * Draws the block at position (x,y) in the image im
 */
void Block::render(PNG & im, int x, int y) const {
    int originalY = y;
    for (int i = 0; i < width(); i++, x++) {
        for (int j = 0, y = originalY; j < height(); j++, y++) {
            // replace with our block's pixels
            HSLAPixel dataPixel = data[i][j];
            HSLAPixel *imgPixel = im.getPixel(x,y);
            imgPixel->h = dataPixel.h;
            imgPixel->s = dataPixel.s;
            imgPixel->l = dataPixel.l;
            imgPixel->a = dataPixel.a;
        }
    }
}

/**
 * Changes the saturation of every pixel in the block to 0,
 * which removes the color, leaving grey.
 */
void Block::greyscale() {
    for (int x = 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            data[x][y].s = 0;
        }
    }
}
