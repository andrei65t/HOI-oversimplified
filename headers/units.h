#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include "countries.h"

class AbstractUnit {
    int unitCount;
    public:
    AbstractUnit(int count) : unitCount(count) {}
    virtual ~AbstractUnit() = default;
    virtual int attackStrength() const = 0;
    virtual int defenseStrength() const = 0;
    virtual Resources totalCost() const = 0;
    virtual std::string getName() const = 0;
    int count() const {
        return unitCount;
    }
    void setCount(int newCount) {
        unitCount = newCount;
    }
    virtual const std::type_info& getType() const = 0;
    virtual std::unique_ptr<AbstractUnit> clone() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const AbstractUnit& unit) {
        os << unit.getName() << " x" << unit.count()
           << " [ATK:" << unit.attackStrength()
           << " DEF:" << unit.defenseStrength() << "]";
        return os;
    }
};
class SoldierTraits {
    public:
        static std::string name() { 
            return "Infanterie"; 
        }
        static int attack() { 
            return 5; 
        }
        static int defense() { 
            return 3; 
        }
        static Resources cost(int n) { 
            return Resources(100*n, 0, 0, 50*n); 
        }
    };
    
class ArmoredSoldierTraits {
public:
    static std::string name() { 
        return "Artilerie"; 
    }
    static int attack() { 
        return 7; 
    }
    static int defense() { 
        return 8; 
    }
    static Resources cost(int n) {
        return Resources(100*n, 60*n, 0, 50*n); 
    }
};

class TankTraits {
public:
    static std::string name() { 
        return "Tanc"; 
    }
    static int attack() { 
        return 20; 
    }
    static int defense() { 
        return 15; 
    }
    static Resources cost(int n) {
        return Resources(200*n, 0, 30*n, 200*n); 
        }
};


template<typename Traits>
class Unit : public AbstractUnit {
    public:
        Unit(int count) : AbstractUnit(count) {}
        int attackStrength() const override {
            return Traits::attack() * count();
        }
        int defenseStrength() const override {
            return Traits::defense() * count();
        }
        Resources totalCost() const override {
            return Traits::cost(count());
        }
        std::string getName() const override {
            return Traits::name();
        }
        const std::type_info& getType() const override {
            return typeid(Traits);
        }
        std::unique_ptr<AbstractUnit> clone() const override {
            return std::make_unique<Unit<Traits>>(*this);
        }
    
};

class UnitFactory {
    public:
        template<typename T>
        static std::unique_ptr<AbstractUnit> createUnit(int count) {
            return std::make_unique<Unit<T>>(count);
        }
};

class Army{
    std::vector<std::unique_ptr<AbstractUnit>> units;
    public:
    Army() = default;

    Army(const Army& other) {
        for (const auto& u : other.units)
            units.push_back(u->clone());
    }
    Army& operator=(const Army& other) {
        if (this != &other) {
            units.clear();
            for (const auto& u : other.units)
                units.push_back(u->clone());
        }
        return *this;
    }
    void addUnit(std::unique_ptr<AbstractUnit> unit) {
        units.push_back(std::move(unit));
    }
    const std::vector<std::unique_ptr<AbstractUnit>>& getUnits() const {
        return units;
    }
    int totalAttackStrength() const {
        int total = 0;
        for (const auto& unit : units) {
            total += unit->attackStrength();
        }
        return total;
    }
    int totalDefenseStrength() const {
        int total = 0;
        for (const auto& unit : units) {
            total += unit->defenseStrength();
        }
        return total;
    }


};

class BattleStrategy{
    public:
        virtual ~BattleStrategy() = default;
        virtual std::string decide(const Army& attacker, const Army& defender) = 0;
};
class ClassicBattleStrategy : public BattleStrategy {
    public:
        std::string decide(const Army& attacker, const Army& defender) override {
            int attackerStrength = attacker.totalAttackStrength();
            int defenderStrength = defender.totalDefenseStrength();
            std::cerr << "[DEBUG] ClassicBattleStrategy: ATK=" 
                  << attackerStrength 
                  << "  DEF=" 
                  << defenderStrength 
                  << "\n";

            if (attackerStrength > defenderStrength) {
                return "Attacker wins!";
            } else {
                return "Defender wins!";
            }
        }
};
class RandomizedStrategy : public BattleStrategy {
    public:
        std::string decide(const Army& attacker, const Army& defender) override {
            int attackerStrength = attacker.totalAttackStrength();
            int defenderStrength = defender.totalDefenseStrength();
            int totalStrength = attackerStrength + defenderStrength;

            std::cerr << "[DEBUG] RandomizedStrategy: ATK=" 
                  << attackerStrength 
                  << "  DEF=" 
                  << defenderStrength 
                  << "  TOTAL=" 
                  << totalStrength 
                  << "\n";
            double probability = static_cast<double>(attackerStrength) / totalStrength;
            double randomValue = static_cast<double>(rand()) / RAND_MAX;
            std::cerr << "[DEBUG] RandomizedStrategy: Probability=" 
                  << probability 
                  << "  RandomValue="
                    << randomValue
                    << "\n";
                    
            if (randomValue < probability) {
                return "Attackers Win! (Random)";
            } else {
                return "Defenders Win! (Random)";
            }
        }
};

using BattleObserver = std::function<void(const std::string&)>;


class BattleManager {
    std::vector<Army> attackers_, defenders_;
    std::vector<BattleObserver> observers_;
    std::unique_ptr<BattleStrategy> strategy_;

public:
    BattleManager() : strategy_(std::make_unique<ClassicBattleStrategy>()) {}
    ~BattleManager() = default;

    
    void setStrategy(std::unique_ptr<BattleStrategy> s) { strategy_ = std::move(s); }
    void addArmy(const Army& army, bool isAttacker) {
        if (isAttacker) attackers_.push_back(army);
        else defenders_.push_back(army);
    }
    void attach(BattleObserver obs) { observers_.push_back(obs); }

    
    template<typename Traits>
    void recruitFromCountry(Country& country, int count, std::vector<Region*>& regions, bool isAttacker) {
        int remain = count;
        for (Region* reg : regions) {
            int available = 0;
            if constexpr (std::is_same<Traits, SoldierTraits>::value || std::is_same<Traits, ArmoredSoldierTraits>::value) {
                available = reg->GetSoldiers();
                int take = std::min(remain, available);
                reg->addSoldiers(-take);
                remain -= take;
            } else if constexpr (std::is_same<Traits, TankTraits>::value) {
                available = reg->GetTanks();
                int take = std::min(remain, available);
                reg->addTanks(-take);
                remain -= take;
            }
            if (remain == 0) break;
        }
        if (remain > 0)
            throw std::runtime_error("Not enough units in selected regions!");
        country.updateInformatii();
        Resources need = Traits::cost(count);
        Resources& res = country.getResources();
        if (res.getGold() < need.getGold() || res.getIron() < need.getIron() || res.getWood() < need.getWood())
            throw std::runtime_error("Not enough resources!");
        res.consume(need.getIron(), need.getWood(), need.getGold(), need.getStone());

        Army a;
        a.addUnit(UnitFactory::createUnit<Traits>(count));
        addArmy(a, isAttacker);
    }

    
    void fight() {
        Army combinedAttackers, combinedDefenders;
        for (const auto& a : attackers_) {
            for (const auto& unit : a.getUnits()) {
                combinedAttackers.addUnit(unit->clone());
            }
        }
        for (const auto& a : defenders_) {
            for (const auto& unit : a.getUnits()) {
                combinedDefenders.addUnit(unit->clone());
            }
        }
        std::string res = strategy_ ? strategy_->decide(combinedAttackers, combinedDefenders) : "No strategy!";
        for (auto& obs : observers_) obs(res);
    }

    void clear() { attackers_.clear(); defenders_.clear(); observers_.clear(); }
};
