#pragma once
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "sfmlHex.h"
#include "exceptions.h"
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
    public:
        Region(int ocupant, Resources resources, HexagonShape hexagon, bool playable=false){
            this->playable=playable;
            id=ocupant;
            population=rand()%5000+10000;
            soldiers=population/(rand()%17+7);
            this->resources=resources;
            this->hexagon=hexagon;
            this->hasFactory=false;
        }
        int GetPopulation(){
            return population;
        }
        int GetSoldiers(){
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
        
};

class Country
{
    private:
        string name;
        int population,soldiers;
        int id;
        Resources resources;
        vector<Region> regions;


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
            }
            const vector<Region>& GetRegions() const{
                return regions;
            }
            void Regiuni(int i){
                regions[i].afisareRegiune();
            }
            int getSoldati(){
                return soldiers;
            }
            friend void transferRegion(Country& tara1, Country& tara2, int id_region);
            
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
            
            std::vector<Region>& GetRegions() {
                return regions;
            }
            
            void produceResources();

            ~Country(){
                cerr<<"Tara "<<name<<" a fost distrusa!"<<endl;
                regions.clear();
            }
            
};
