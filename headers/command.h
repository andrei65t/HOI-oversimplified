#pragma once
#include "countries.h"
#include "units.h"
#include <iostream>

template<typename CountryType>
void applyMartialLaw(CountryType& country, Army& army) {
    int totalNewSoldiers = 0;

    for (auto& reg : country.GetRegions()) {
        int sold = reg.GetSoldiers();
        if (sold > 0) {
            
            totalNewSoldiers += sold/600;
        }
    }

    
    if (totalNewSoldiers > 0) {
        army.addUnit(UnitFactory::createUnit<SoldierTraits>(totalNewSoldiers));
    }

    country.getPopularity().decrease(20);
}

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};

class MartialLawCommand : public Command {
    Country& country;
    Army& army;
public:
    MartialLawCommand(Country& c, Army& a) : country(c), army(a) {}

    void execute() override {
        applyMartialLaw(country, army);
        std::cout << "Legea marțială a fost aplicată! "
                  << "Soldați noi: " << country.getSoldati() << "\n";
    }
};
