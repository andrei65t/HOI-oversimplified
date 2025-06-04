#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "sfmlHex.h"
#include "exceptions.h"
#include "popularity.h"
#include <sstream>
using namespace std;

class Resources{
    int gold,stone,wood,iron;
    public:
        Resources(int gold=0, int stone=0, int wood=0, int iron=0){
            this->gold=gold;
            this->stone=stone;
            this->wood=wood;
            this->iron=iron;
        }
        void setBoost(){
            gold=200;
            stone=1000;
            wood=1000;
            iron=1000;
        }
        void adderResouces(int amount){
            gold+=amount;
            stone+=amount;
            wood+=amount;
            iron+=amount;
        }
        int getIron() const{
            return iron;
        }
        int getGold() const{
            return gold;
        }
        int getStone() const{
            return stone;
        }
        int getWood() const{
            return wood;
        }
        void consume(int ironUsed, int woodUsed, int goldUsed, int stoneUsed) {
            iron -= ironUsed;
            wood -= woodUsed;
            gold -= goldUsed;
            stone -= stoneUsed;
        }
        
};
class Construction {
    public:
        virtual void build(Resources& res) = 0;
        virtual ~Construction(){}
    };
    
class Factory : public Construction{
    private:
        float power;
        int workers;

    public:
        Factory(){
            power=(rand()%100+1.2)/100.0;
            workers=rand()%100+10;
        }
        void build(Resources& res) override{
            if(res.getIron()<750||res.getWood()<750||res.getStone()<750)
                throw FactoryBuildException("Resurse insuficiente pentru\na construi o fabrica.");
            res.consume(750, 750, 0, 750);   
        }
};
class Region : public HexagonShape
{
    private:
        int id;
        int population,soldiers;
        Resources resources;
        HexagonShape hexagon;
        bool hasFactory;
        bool playable;
        Factory* factory=nullptr;
        int tanks;
        
    public:
        Region(int ocupant, Resources resources, HexagonShape hexagon, bool playable=false){
            this->playable=playable;
            id=ocupant;
            population=rand()%5000+10000;
            soldiers=population/(rand()%17+7);
            this->resources=resources;
            this->hexagon=hexagon;
            this->hasFactory=false;
            tanks=rand()%2;
        }
        void setBoost(){
            resources.setBoost();
        }
        int GetPopulation(){
            return population;
        }
        int GetSoldiers() const {
            return soldiers;
        }
        void setPlayable(bool value) {
            playable = value;
        }
        bool isPlayable() const {
            return playable;
        }
        void checkAccess() const {
            if (!playable)
                throw AccessDeniedException("Nu ai voie să accesezi această regiune!");
        }
        Color getColor() const {
            return hexagon.getFillColor();
        }
        void changeColor(Color color) {
            hexagon.setFillColor(color);
        }

        int GetTanks() const { return tanks; }
        void addSoldiers(int delta) { soldiers += delta; }
        void addTanks(int delta) { tanks += delta; }
        string afisareRegiune() const{
            ostringstream ss;
            ss << "Id-ul regiunii " << id << " populatie:" << population << " soldati:" << soldiers;
            return ss.str();
        }
        friend ostream& operator << (ostream& cout, Region& region){
            cout<<"Id-ul tarii "<<region.id<<" populatie:"<<region.population<<" soldati:"<<region.soldiers<<endl;
            return cout;
        }
        const HexagonShape& getHexShape() const {
            return hexagon;
        }
        bool contains(Vector2f point) const {
            return hexagon.contains(point);
        }
        void changeOutlineColor(Color color) {
            hexagon.OutlineColor(color);
        }
        int GetID() const {
            return id;
        }
        const Resources& getResources() const {
            return resources;
        }
        bool hasFactoryBuilt() const {
            return hasFactory;
        }        
        void buildFactory(){
            if(hasFactory)
                throw FactoryBuildException("Regiunea are deja fabrica");
            Factory* f=new Factory();
            f->build(resources);
            hasFactory=true;
            factory=f;
        }
        void addResources(int amount) {
            resources.adderResouces(amount);
        }
        void consumeResources(int ironUsed, int woodUsed, int goldUsed, int stoneUsed) {
            if(ironUsed <= resources.getIron() &&
               woodUsed <= resources.getWood() &&
               goldUsed <= resources.getGold() &&
               stoneUsed <= resources.getStone()) {
                resources.consume(ironUsed, woodUsed, goldUsed, stoneUsed);
            } else {
                throw std::runtime_error("Resurse insuficiente pentru consum.");
            }
        }
        
};

class Country
{
    private:
        string name;
        int population,soldiers;
        int id;
        bool pc;
        bool labour;
        Resources resources;
        vector<Region> regions;
        Popularity<int> popularity;

        public:
            Country(int id, string name, vector<Region> regions){
                this->id=id; 
                this->regions=regions;
                this->population=0;
                this->soldiers=0;
                for(int i=0;i<regions.size();i++){
                    this->population+=regions[i].GetPopulation();
                    this->soldiers+=regions[i].GetSoldiers();
                }
                this->name=name;
                popularity = Popularity<int>();
                pc=false;
                labour=false;
            }
            void setPc(bool pc){
                this->pc=pc;
            }
            const vector<Region>& GetRegions() const{
                return regions;
            }
            bool isMyCountry() const {
                return pc;
            }
            void Regiuni(int i){
                regions[i].afisareRegiune();
            }
            void setlabour(bool value) {
                labour = value;
            }
            int getLabour() const {
                return labour;
            }
            int getSoldati(){
                int total = 0;                               
                for (const auto& r : regions)               
                    total += r.GetSoldiers();                
                return total;                                
            }
            friend void transferRegion(Country& tara1, Country& tara2, Region* regionPtr);
            const Resources& getResources() const {
                return resources;
            }
            void updateInformatii(){
                this->population=0;
                this->soldiers=0;
                for(int i=0;i<regions.size();i++){
                    this->population+=regions[i].GetPopulation();
                    this->soldiers+=regions[i].GetSoldiers();
                }
            }
            void outline(Color color){
                for(int i=0;i<regions.size();i++){
                    regions[i].changeOutlineColor(color);
                }
            }
            friend ostream& operator << (ostream& cout, Country& country){
                cout<<"Id-ul tarii "<<country.id<<" care are numele "<<country.name<<" populatie:"<<country.population<<" soldati:"<<country.soldiers<<endl;
                return cout;
            }
            void checkDestruction(){
                if(regions.size()==0){
                    cerr<<"Tara "<<name<<" a fost distrusa!"<<endl;
                }
            }
            string GetName() const {
                return name;
            }
            const Region& getRegionByClick(const sf::Vector2f& clickPos, int playerId) const {
                if (playerId != id) {
                    throw AccessDeniedException("Nu poti accesa această regiune!");
                }
            
                for (const auto& region : regions) {
                    if (region.getHexShape().getGlobalBounds().contains(clickPos)) {
                        return region;
                    }
                }
            
                throw std::runtime_error("Regiune invalidă.");
            }
            Popularity<int>& getPopularity() {
                return popularity;
            }
            const Popularity<int>& getPopularity() const {
                return popularity;
            }
            void increasePopularity(int amount) {
                popularity.increase(amount);
            }
            void decreasePopularity(int amount) {
                popularity.decrease(amount);
                cerr<< "Popularitate scazuta pentru tara " << name << ": " << amount << endl;
            }
            std::vector<Region>& GetRegions() {
                return regions;
            }
            void produceResources();
            // countries.h (în clasa Country, după celelalte metode)
            Resources getTotalResources() const {
                Resources total{0, 0, 0, 0};
                for (const auto& r : regions) {
                    const Resources& rRes = r.getResources();
                    // Observă ordinea parametrilor din constructorul Resources:
                    // Resources(int gold, int stone, int wood, int iron);
                    total = Resources{
                        total.getGold()  + rRes.getGold(),   // gold
                        total.getStone() + rRes.getStone(),  // stone
                        total.getWood()  + rRes.getWood(),   // wood
                        total.getIron()  + rRes.getIron()    // iron
                    };
                }
                return total;
            }
            

    /// Scade din resursele tuturor regiunilor costul dat,
    /// împărţind suma echilibrat pe fiecare regiune.
    void consumeResourcesFromRegions(const Resources& cost) {
        // 1) Verificăm totalul resurselor disponibile în toate regiunile
        Resources total{0, 0, 0, 0};
        for (const auto& r : regions) {
            const Resources& rRes = r.getResources();
            total = Resources{
                total.getGold()  + rRes.getGold(),
                total.getStone() + rRes.getStone(),
                total.getWood()  + rRes.getWood(),
                total.getIron()  + rRes.getIron()
            };
        }
        if (total.getIron()  < cost.getIron()  ||
            total.getWood()  < cost.getWood()  ||
            total.getGold()  < cost.getGold()  ||
            total.getStone() < cost.getStone())
        {
            throw std::runtime_error("Resurse insuficiente în toate regiunile combinate!");
        }

        
        int totalRegions = regions.size();
        int remIron  = cost.getIron();
        int remWood  = cost.getWood();
        int remGold  = cost.getGold();
        int remStone = cost.getStone();

        for (auto& r : regions) {
            const Resources& rRes = r.getResources();

            
            int takeIron  = std::min(rRes.getIron(), remIron / totalRegions);
            int takeWood  = std::min(rRes.getWood(), remWood / totalRegions);
            int takeGold  = std::min(rRes.getGold(), remGold / totalRegions);
            int takeStone = std::min(rRes.getStone(), remStone / totalRegions);

            r.consumeResources(takeIron, takeWood, takeGold, takeStone);

            remIron  -= takeIron;
            remWood  -= takeWood;
            remGold  -= takeGold;
            remStone -= takeStone;

            totalRegions--; 

           
            if (remIron == 0 && remWood == 0 && remGold == 0 && remStone == 0) {
                break;
            }
        }
    }

            ~Country(){
                cerr<<"Tara "<<name<<" a fost distrusa!"<<endl;
                regions.clear();
            }
            
};
