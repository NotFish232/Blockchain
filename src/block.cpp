#include "block.hpp"

Block::Block() {

}
Block::~Block() {
    delete last_block;
}
string Block::get_hash() {
    return "placeholder text";
}