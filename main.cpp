#include <iostream>
#include "countries.h"
using namespace std;
vector<Region> regions;
vector<Country> countries;
string names[4]={"Romania","Bulgaria","Serbia","Ungaria"};
void initializare(){
    srand(time(0));
    for (int i=0;i<4;i++){
        for (int j=0;j<6;j++){
            regions.push_back(Region(i*6+j,Resources(rand()%1000+100,rand()%1000+100,rand()%1000+100,rand()%1000+100)));
        }
        countries.push_back(Country(i,names[i],regions));
        regions.clear();
    }
}
int main() {
    initializare();
    while(1==1)
    {
        int op;
        cout<<"1. Afisare tari si regiuni"<<endl;
        cout<<"2. Cucerire"<<endl;
        cin>>op;
        switch (op)
        {
        case 1:
            for (int i=0;i<4;i++){
                cout<<countries[i];
                for(int j=0;j<countries[i].GetRegions().size();j++)
                    countries[i].Regiuni(j);
                cout<<endl;
            }
            break;
        case 2:
            int id1,id2,reg;
            cout<<"ID-ul tarii care cucereste: ";
            cin>>id1;
            cout<<"ID-ul tarii din care e cucerita regiunea: ";
            cin>>id2;
            cout<<"ID-ul regiunii care e cucerita: ";
            cin>>reg;
            if(countries[id1].getSoldati()<countries[id2].getSoldati()){
                cout<<"Cucerirea a esuat"<<endl;
                break;
            }
            else{
                transferRegion(countries[id1],countries[id2],reg);
                cout<<"Cucerirea a reusit"<<endl;
            }
            break;
        default:
            break;
        }
    }

    return 0;
}