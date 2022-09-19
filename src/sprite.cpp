#include "sprite.h"


void SetSpriteOrigin(Sprite* spr, int x, int y) {
    spr->origin_x = x;
    spr->origin_y = y;
}


void MoveSprite(Sprite *spr, int x, int y) {
    spr->img_pos.x += x;
    spr->img_pos.y += y;
}



int FindSpriteIndex(std::vector<Sprite*> sprites, Sprite* search) {
    std::vector<Sprite*>::iterator it = std::find(sprites.begin(), sprites.end(), search);
    if (it != sprites.end()) {
        return it - sprites.begin();
    }
    std::cout << "Could not find sprite in sprites vector." << std::endl;
    return -1;
}



void DestroyAllSprites(std::vector<Sprite*> *sprites) {
    std::cout << "Deleting every sprite in vector." << std::endl;
    std::cout << " >>> Sprite vector size (before): " << sprites->size() << "." << std::endl;
    for (auto &a : *sprites) {
        delete a;
    }
    sprites->clear();
    std::cout << " >>> Sprite vector size (after):  " << sprites->size() << "." << std::endl;
}




void DestroySprite(std::vector<Sprite*>* sprites, int index) {
    std::cout << "Deleting sprite at index " << index << "." << std::endl;
    std::cout << " >>> Sprite vector size (before): " << sprites->size() << "." << std::endl;
    auto a = &*(*sprites)[index];
    delete a;
    sprites->erase(sprites->begin() + index);
    std::cout << " >>> Sprite vector size (after):  " << sprites->size() << "." << std::endl;
}


