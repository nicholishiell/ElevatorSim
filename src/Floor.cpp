#include "include/Floor.h"

Floor::Floor(   const std::string label, 
                const int level)
{
    label_ = label;
    level_ = level;

    panel_ = nullptr;
}

Floor::~Floor()
{

}  

void 
Floor::UpRequestServiced()
{
    this->GetPanel()->UpRequestServiced();
}

void 
Floor::DownRequestServiced()
{
    this->GetPanel()->DownRequestServiced();
}

