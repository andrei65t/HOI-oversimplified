#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "sfmlHex.h"
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
};
class Factory{
    private:
        float power;
        int workers;
        public:
        Factory(){
            power=(rand()%100+1.2)/100.0;
            workers=rand()%100+10;
        }
};
class Region : public HexagonShape
{
    private:
        int id;
        int population,soldiers;
        Resources resources;
        HexagonShape hexagon;
    public:
        Region(int ocupant, Resources resources, HexagonShape hexagon){
            id=ocupant;
            population=rand()%5000+10000;
            soldiers=population/(rand()%17+7);
            this->resources=resources;
            this->hexagon=hexagon;
        }
        int GetPopulation(){
            return population;
        }
        int GetSoldiers(){
            return soldiers;
        }
        void afisareRegiune(){
            cerr<<"     Id-ul regiunii "<<id<<" populatie:"<<population<<" soldati:"<<soldiers<<endl;
        }
        friend ostream& operator << (ostream& cout, Region& region){
            cout<<"Id-ul tarii "<<region.id<<" populatie:"<<region.population<<" soldati:"<<region.soldiers<<endl;
            return cout;
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
            vector<Region>& GetRegions(){
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
            
            friend ostream& operator << (ostream& cout, Country& country){
                cout<<"Id-ul tarii "<<country.id<<" care are numele "<<country.name<<" populatie:"<<country.population<<" soldati:"<<country.soldiers<<endl;
                return cout;
            }
};

void transferRegion(Country& tara1, Country& tara2, int id_region){
    tara1.regions.push_back(tara2.regions[id_region]);
    tara2.regions.erase(tara2.regions.begin()+id_region);
    tara1.updateInformatii();
    tara2.updateInformatii();
}
