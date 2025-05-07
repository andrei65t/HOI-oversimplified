#include "../headers/countries.h"

void transferRegion(Country& tara1, Country& tara2, int id_region) {
    tara1.regions.push_back(tara2.regions[id_region]);
    tara2.regions.erase(tara2.regions.begin() + id_region);
    tara1.updateInformatii();
    tara2.updateInformatii();
    tara2.checkDestruction();
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
            if (!region.hasFactoryBuilt()) {
                region.addResources(2);
            }
        }
    }
}
