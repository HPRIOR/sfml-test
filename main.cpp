#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;


vector<string> split(string &s, char delim) {
    vector<string> result;
    stringstream sStream(s);
    string item;

    while (getline(sStream, item, delim)) {
        result.push_back(item);
    }
    return result;
}


class Colour {
public:
    int r, b, g;

    Colour() = default;;

    Colour(int r, int b, int g) : r(r), b(b), g(g) {};

};

class FontCfg {
public:
    string file;
    int size;
    Colour colour;

    FontCfg() = default;;

    explicit FontCfg(string &s) {
        vector<string> configSplit = split(s, ' ');
        // make a string copied
        string thisStr = configSplit[1];
        this->file = thisStr;
        this->size = stoi(configSplit[2]);
        this->colour = Colour(stoi(configSplit[3]), stoi(configSplit[4]), stoi(configSplit[5]));
    };


};


class Shape {
public:
    virtual void render() = 0;

    virtual void move(int maxW, int maxH) = 0;

    virtual shared_ptr<sf::Shape> getShape() = 0;

};

class Rectangle : public Shape {
public:

    string name;
    float x, y, sx, sy, w, h;
    Colour colour;
    sf::RectangleShape shape;

    Rectangle() = default;;

    Rectangle(string &s) {
        vector<string> configSplit = split(s, ' ');
        string nameStr = configSplit[1];
        this->name = nameStr;
        this->x = stof(configSplit[2]);
        this->y = stof(configSplit[3]);
        this->sx = stof(configSplit[4]);
        this->sy = stof(configSplit[5]);
        this->colour = Colour(stoi(configSplit[6]), stoi(configSplit[7]), stoi(configSplit[8]));
        this->w = stof(configSplit[9]);
        this->h = stof(configSplit[10]);
        sf::Vector2f vec2(w, h);
        this->shape = sf::RectangleShape(vec2);
        this->shape.setFillColor(sf::Color(colour.r, colour.g, colour.b));
        this->shape.setPosition(x, y);

    };


    void render() override {
        cout << "implements render" << endl;
    }

    void move(int maxW, int maxH) override {
        if (this->x <= 0) {
            this->sx = -(this->sx);
        }
        if (this->y <= 0) {
            this->sy = -(this->sy);
        }
        if (this->x + this->w >= maxW) {
            this->sx = -(this->sx);
        }
        if (this->y + this->h >= maxH) {
            this->sy = -(this->sy);
        }

        this->x = this->x + this->sx;
        this->y = this->y + this->sy;
        this->shape.setPosition(this->x, this->y);
    }

    shared_ptr<sf::Shape> getShape() override {
        shared_ptr<sf::Shape> rect = make_shared<sf::RectangleShape>(this->shape);
        return rect;
    }
};

class Circle : public Shape {
public:
    string name;
    float x, y, sx, sy, r;
    Colour colour;
    sf::CircleShape shape;

    Circle() = default;;

    explicit Circle(string &s) {
        vector<string> configSplit = split(s, ' ');
        string nameStr = configSplit[1];
        this->name = nameStr;
        this->x = stof(configSplit[2]);
        this->y = stof(configSplit[3]);
        this->sx = stof(configSplit[4]);
        this->sy = stof(configSplit[5]);
        this->colour = Colour(stoi(configSplit[6]), stoi(configSplit[7]), stoi(configSplit[8]));
        this->r = stof(configSplit[9]);
        this->shape = sf::CircleShape(r);
        this->shape.setFillColor(sf::Color(colour.r, colour.g, colour.b));
        this->shape.setPosition(x, y);
    };

    void render() override {
        cout << "override render" << endl;
    }


    void move(int maxW, int maxH) override {
        if (this->x <= 0) {
            this->sx = -(this->sx);
        }
        if (this->y <= 0) {
            this->sy = -(this->sy);
        }
        if (this->x + (this->r * 2) >= maxW) {
            this->sx = -(this->sx);
        }
        if (this->y + (this->r * 2) >= maxH) {
            this->sy = -(this->sy);
        }

        this->x = this->x + this->sx;
        this->y = this->y + this->sy;
        this->shape.setPosition(this->x, this->y);
    }

    shared_ptr<sf::Shape> getShape() override {
        shared_ptr<sf::Shape> circ = make_shared<sf::CircleShape>(this->shape);
        return circ;
    }


};


class WindowCfg {
public:
    int width, height;

    WindowCfg() = default;;


    explicit WindowCfg(string &configLine) {
        vector<string> configSplit = split(configLine, ' ');
        int w = stoi(configSplit[1]);
        int h = stoi(configSplit[2]);
        this->width = w;
        this->height = h;
    };
};

class Config {
public:
    WindowCfg windowCfg;
    FontCfg fontCfg;
    vector<shared_ptr<Shape>> shapes;

    Config(WindowCfg windowCfg, FontCfg fontCfg, vector<shared_ptr<Shape>> shapes) : windowCfg(windowCfg),
                                                                                     fontCfg(std::move(fontCfg)),
                                                                                     shapes(std::move(shapes)) {};

};


Config getConfig() {
    ifstream file;
    file.open("/home/harry/Code/sfml-test/resources/config.txt");
    if (!file.is_open()) {
        cout << "Unable to open the file." << endl;
        throw exception();
    }

    WindowCfg windowConf{};
    FontCfg fontConf;
    vector<shared_ptr<Shape>> shapes;
    string line;
    while (getline(file, line)) {
        if (line.contains("Window")) {
            windowConf = WindowCfg(line);
        }
        if (line.contains("Font")) {
            fontConf = FontCfg(line);
        }
        if (line.contains("Rectangle")) {
            // push config to rect config
            Rectangle r = Rectangle(line);
            shared_ptr<Shape> shape = make_shared<Rectangle>(r);
            shapes.push_back(shape);
        }
        if (line.contains("Circle")) {
            // push config to circle config
            Circle c = Circle(line);
            shared_ptr<Shape> shape = make_shared<Circle>(c);
            shapes.push_back(shape);
        }
    }
    file.close();

    return {windowConf, fontConf, shapes};
}


int main() {

    Config config = getConfig();


    sf::RenderWindow window(
            sf::VideoMode(config.windowCfg.width, config.windowCfg.height),
            "shapes");

    window.setFramerateLimit(1000);

    vector<shared_ptr<Shape>> shapes = config.shapes;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        // draw things here
        for (auto &s: shapes) {
            s->move(config.windowCfg.width, config.windowCfg.height);
            shared_ptr<sf::Shape> shape = s->getShape();
            window.draw(*shape);
        }

        window.display();
    }

    return 0;
}
 