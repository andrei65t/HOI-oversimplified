#include "../headers/countries.h"

void transferRegion(Country& winner, Country& loser, Region* regionPtr)
{
    if (!regionPtr) return; 

    auto it = std::find_if(loser.GetRegions().begin(),
                           loser.GetRegions().end(),
                           [&](const Region& r){ return &r == regionPtr; });

    if (it == loser.GetRegions().end()) return;    


    Region moved = std::move(*it);
    loser.GetRegions().erase(it);

    
    moved.setPlayable(true);                       
    moved.changeOutlineColor(sf::Color::White);    
    moved.changeColor(winner.GetRegions().front().getColor()); 


    winner.GetRegions().push_back(std::move(moved));

    winner.updateInformatii();
    loser.updateInformatii();
    loser.checkDestruction();
}

void Country::produceResources() {
    bool hasFactory = false;
    for (auto& region : regions) {
        if (region.hasFactoryBuilt()) {
            hasFactory = true;
            break;
        }
    }

    if (hasFactory) {
        for (auto& region : regions) {
            //if (!region.hasFactoryBuilt()) {
            region.addResources(2); // de rezolvat daca aveam peste tot fabrici nu generam resurse
            //}
        }
    }
}
