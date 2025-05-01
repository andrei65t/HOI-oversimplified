#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "sfmlHex.h"
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
            ~Country(){
                cerr<<"Tara "<<name<<" a fost distrusa!"<<endl;
                regions.clear();
            }
};

void transferRegion(Country& tara1, Country& tara2, int id_region){
    tara1.regions.push_back(tara2.regions[id_region]);
    tara2.regions.erase(tara2.regions.begin()+id_region);
    tara1.updateInformatii();
    tara2.updateInformatii();
    tara2.checkDestruction();
}
