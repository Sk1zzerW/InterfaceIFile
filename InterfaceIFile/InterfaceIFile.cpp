#include <iostream>
#include <fstream>
#include <memory>
#include <string>

class IFile {
public:
    virtual void Save(std::ofstream& stream) const = 0;
    virtual void Save() const = 0;
    virtual void Load(std::ifstream& stream) = 0;
    virtual void Load() = 0;
    virtual ~IFile() = default;
};

class Weapon : public IFile {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& name = "", int damage = 0) : name(name), damage(damage) {}

    void Save(std::ofstream& stream) const override {
        size_t nameLength = name.size();
        stream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        stream.write(name.c_str(), nameLength);
        stream.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }

    void Save() const override {
        std::ofstream file("Оружие", std::ios::binary);
        if (file.is_open()) {
            Save(file);
            file.close();
            std::cout << "не сохранено\n";
        }
        else {
            std::cerr << "Ошибка загрузки\n";
        }
    }

    void Load(std::ifstream& stream) override {
        size_t nameLength;
        stream.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        name.resize(nameLength);
        stream.read(&name[0], nameLength);
        stream.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    }

    void Load() override {
        std::ifstream file("Оружие", std::ios::binary);
        if (file.is_open()) {
            Load(file);
            file.close();
            std::cout << "Ошбка оружия\n";
        }
        else {
            std::cerr << "ошибка данных\n";
        }
    }

    void display() const {
        std::cout << "название оружия: " << name << ", урон: " << damage << "\n";
    }
};

class Unit : public IFile {
private:
    std::string name;
    int health;
    std::shared_ptr<Weapon> weapon;

public:
    Unit(const std::string& name = "", int health = 100, const std::shared_ptr<Weapon>& weapon = nullptr)
        : name(name), health(health), weapon(weapon) {}

    void Save(std::ofstream& stream) const override {
        size_t nameLength = name.size();
        stream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        stream.write(name.c_str(), nameLength);
        stream.write(reinterpret_cast<const char*>(&health), sizeof(health));

        bool hasWeapon = (weapon != nullptr);
        stream.write(reinterpret_cast<const char*>(&hasWeapon), sizeof(hasWeapon));
        if (hasWeapon) {
            weapon->Save(stream);
        }
    }

    void Save() const override {
        std::ofstream file("данныу", std::ios::binary);
        if (file.is_open()) {
            Save(file);
            file.close();
            std::cout << "сохранено\n";
        }
        else {
            std::cerr << "Ошибка данных\n";
        }
    }

    void Load(std::ifstream& stream) override {
        size_t nameLength;
        stream.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        name.resize(nameLength);
        stream.read(&name[0], nameLength);
        stream.read(reinterpret_cast<char*>(&health), sizeof(health));

        bool hasWeapon;
        stream.read(reinterpret_cast<char*>(&hasWeapon), sizeof(hasWeapon));
        if (hasWeapon) {
            weapon = std::make_shared<Weapon>();
            weapon->Load(stream);
        }
        else {
            weapon = nullptr;
        }
    }

    void Load() override {
        std::ifstream file("Даныые", std::ios::binary);
        if (file.is_open()) {
            Load(file);
            file.close();
            std::cout << "Данный получены\n";
        }
        else {
            std::cerr << "Failed to open unit.dat for reading\n";
        }
    }

    void display() const {
        std::cout << "Имя " << name << ", Хп " << health << "\n";
        if (weapon) {
            std::cout << "У вас оружие ";
            weapon->display();
        }
        else {
            std::cout << "У вас нету оружия.\n";
        }
    }

    void setName(const std::string& name) { this->name = name; }
    void setHealth(int health) { this->health = health; }
    void setWeapon(const std::shared_ptr<Weapon>& weapon) { this->weapon = weapon; }
};

int main() {
    std::shared_ptr<Weapon> sword = std::make_shared<Weapon>("Excalibur", 50);
    Unit hero("Arthur", 100, sword);

    // Сохранение данных
    hero.Save();
    sword->Save();

    Unit loadedHero;
    std::shared_ptr<Weapon> loadedSword = std::make_shared<Weapon>();
    loadedHero.setWeapon(loadedSword);

    loadedHero.Load();
    loadedSword->Load();

    std::cout << "\nЗагруженный герой:\n";
    loadedHero.display();

    return 0;
}
