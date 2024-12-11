#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <fstream>

using namespace std;

string path = "List.txt";

struct Car {
    string name;
    int year;
    double engineVolume;
    double price;

    void display() const {
        cout << "Name: " << name << ", Year: " << year
            << ", Engine Volume: " << engineVolume
            << ", Price: " << price << " USD" << endl;
    }
};

class CarSalon {
private:
    vector<Car> cars;

public:
    void addCar(const Car& car) {
        cars.push_back(car);
    }

    void removeCar(const string& name) {
        auto it = remove_if(cars.begin(), cars.end(), [&](const Car& car) {
            return car.name == name;
            });
        if (it != cars.end()) {
            cars.erase(it, cars.end());
            cout << "Car(s) removed.\n";
        }
        else {
            cout << "No car with the specified name found.\n";
        }
    }

    void displayCars() const {
        if (cars.empty()) {
            cout << "No cars in the salon.\n";
            return;
        }
        for (const auto& car : cars) {
            car.display();
        }
    }

    void sortCars(function<bool(const Car&, const Car&)> comparator) {
        sort(cars.begin(), cars.end(), comparator);
    }

    void findCars(function<bool(const Car&)> predicate) const {
        auto results = count_if(cars.begin(), cars.end(), predicate);
        if (results == 0) {
            cout << "No cars found matching the criteria.\n";
            return;
        }

        for (const auto& car : cars) {
            if (predicate(car)) {
                car.display();
            }
        }
    }

    void saveToFile() {
        ofstream fout;
        fout.open(path);
        for (auto el : cars) {
            fout << el.name << "\n";
            fout << el.year << "\n";
            fout << el.engineVolume << "\n";
            fout << el.price << "\n";
        }
        fout.close();
    }

    void loadFromFile() {
        ifstream fin;
        fin.open(path);
        if (!fin.is_open()) {
            cout << "Error: Could not open file " << path << endl;
            return;
        }

        cars.clear(); 

        while (!fin.eof()) {
            Car car;
            getline(fin, car.name);
            if (car.name.empty()) break; 
            fin >> car.year >> car.engineVolume >> car.price;
            fin.ignore(); 
            cars.push_back(car);
        }

        fin.close();
        cout << "Cars loaded from file successfully.\n";
    }

};

int main() {
    CarSalon salon;

    salon.addCar({ "Toyota Corolla", 2020, 1.8, 20000 });
    salon.addCar({ "Honda Civic", 2018, 2.0, 18000 });
    salon.addCar({ "Ford Mustang", 2021, 5.0, 55000 });

    int choice;
    do {
        cout << "\n1. Add Car\n2. Remove Car\n3. Display Cars\n4. Sort Cars\n5. Find Cars\n6. Save to File\n7. Load from File\n0. Exit\nChoice: ";

        cin >> choice;

        switch (choice) {
        case 1: {
            Car car;
            cout << "Enter name, year, engine volume, price: ";
            cin >> ws;
            getline(cin, car.name);
            cin >> car.year >> car.engineVolume >> car.price;
            salon.addCar(car);
            break;
        }
        case 2: {
            string name;
            cout << "Enter the name of the car to remove: ";
            cin >> ws;
            getline(cin, name);
            salon.removeCar(name);
            break;
        }
        case 3:
            salon.displayCars();
            break;
        case 4: {
            cout << "Sort by:\n1. Name\n2. Year\n3. Engine Volume\n4. Price\nChoice: ";
            int sortChoice;
            cin >> sortChoice;
            if (sortChoice == 1) {
                salon.sortCars([](const Car& a, const Car& b) {
                    return a.name < b.name;
                    });
            }
            else if (sortChoice == 2) {
                salon.sortCars([](const Car& a, const Car& b) {
                    return a.year < b.year;
                    });
            }
            else if (sortChoice == 3) {
                salon.sortCars([](const Car& a, const Car& b) {
                    return a.engineVolume < b.engineVolume;
                    });
            }
            else if (sortChoice == 4) {
                salon.sortCars([](const Car& a, const Car& b) {
                    return a.price < b.price;
                    });
            }
            else {
                cout << "Invalid choice.\n";
            }
            break;
        }
        case 5: {
            cout << "Find by:\n1. Name\n2. Year\n3. Engine Volume\n4. Price\nChoice: ";
            int findChoice;
            cin >> findChoice;
            if (findChoice == 1) {
                string name;
                cout << "Enter name: ";
                cin >> ws;
                getline(cin, name);
                salon.findCars([&](const Car& car) {
                    return car.name == name;
                    });
            }
            else if (findChoice == 2) {
                int year;
                cout << "Enter year: ";
                cin >> year;
                salon.findCars([&](const Car& car) {
                    return car.year == year;
                    });
            }
            else if (findChoice == 3) {
                double engineVolume;
                cout << "Enter engine volume: ";
                cin >> engineVolume;
                salon.findCars([&](const Car& car) {
                    return car.engineVolume == engineVolume;
                    });
            }
            else if (findChoice == 4) {
                double price;
                cout << "Enter price: ";
                cin >> price;
                salon.findCars([&](const Car& car) {
                    return car.price == price;
                    });
            }
            else {
                cout << "Invalid choice.\n";
            }
            break;
        }
        case 6:
            salon.saveToFile();
            cout << "Cars saved to file.\n";
            break;
        case 7:
            salon.loadFromFile();
            break;

        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

}
