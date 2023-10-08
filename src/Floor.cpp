#include "include/Floor.h"

Floor::Floor(   const std::string label, 
                const int level)
{
    label_ = label;
    level_ = level;

    panel_ = std::make_shared<FloorPanel>(label,level);
}

Floor::~Floor()
{

}  

void 
Floor::SetNumElevators(const int numElevators)
{
    panel_->SetNumberOfElevators(numElevators);
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

