#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Body {
    float x;
    float y;
    sf::Sprite sprite;

    Body(float x, float y, sf::Sprite obj) : x(x), y(y), sprite(obj) {}
};

void display(sf::RenderWindow& window,Body & platypus, vector<Body>& upperObs, vector<Body>& lowerObs, sf::Texture& pipeTexture,int &score,sf::Font & font) {
    window.clear(sf::Color(135, 206, 250));

    if ((!upperObs.empty() && upperObs.front().x < 0.0f) || (!lowerObs.empty() && lowerObs.front().x < 0.0f)) {
        upperObs.erase(upperObs.begin());
        lowerObs.erase(lowerObs.begin());

        score++;
    }


    if (upperObs.back().x <= 1200.0f || lowerObs.back().x <= 1200.0f) {
        


        int r = rand() % 3;

        float Uy;
        float Ly;

        switch (r) {

        case 0:
            Uy = -200.0f;
            Ly = 600.0f;
            break;
        case 1:
            Uy = -100.0f;
            Ly = 700.0f;
            break;
        case 2:
            Uy = 0.0f;
            Ly = 800.0f;
            break;
        default:
            Uy = 100.0f;
            Ly = 900.0f;
            break;
        }

        sf::Sprite obj;
        sf::Sprite obj2;

        Body temp(1800.0f, Uy, obj);
        Body temp2(1800.0f, Ly, obj2);

        temp.sprite.setTexture(pipeTexture);
        temp2.sprite.setTexture(pipeTexture);


        temp.sprite.setPosition(temp.x, temp.y);
        temp2.sprite.setPosition(temp2.x,temp2.y);

        upperObs.push_back(temp);
        lowerObs.push_back(temp2);


    }

    for (int i = 0; i < upperObs.size(); i++) {
        upperObs[i].x -= 1.0f;
        upperObs[i].sprite.setPosition(upperObs[i].x, upperObs[i].y);
        window.draw(upperObs[i].sprite);
    }

    for (int i = 0; i < lowerObs.size(); i++) {
        lowerObs[i].x -= 1.0f;
        lowerObs[i].sprite.setPosition(lowerObs[i].x, lowerObs[i].y);
        window.draw(lowerObs[i].sprite);
    }



    window.draw(platypus.sprite);
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(20.0f, 20.0f);
    window.draw(scoreText);

    window.display();
}

void input(bool key,Body &platypus) {
    if (key) {
        platypus.y -= 2.0f;
        platypus.sprite.setPosition(platypus.x, platypus.y);
    }
    else {
        platypus.y += 0.9f;
        platypus.sprite.setPosition(platypus.x, platypus.y);
    }
}

bool checkCollisions(std::vector<Body>& upperObs, std::vector<Body>& lowerObs, Body& platypus) {

    sf::FloatRect platBounds = platypus.sprite.getGlobalBounds();
    
    if (platypus.y <= 0.0f || platypus.y >= 1080.0f) {
        return true;
    }

    for (int i = 0; i < upperObs.size(); i++) {
        sf::FloatRect upperBounds = upperObs[i].sprite.getGlobalBounds();
        
        if (platBounds.intersects(upperBounds)) {

            return true;
        }
    }

    for (int i = 0; i < lowerObs.size(); i++) {
        sf::FloatRect lowerBounds = lowerObs[i].sprite.getGlobalBounds();
        

        if (platBounds.intersects(lowerBounds)) {


            return true;
        }
    }

    return false;
}

int mainScreen(Body & platypus,sf::RenderWindow & window,sf:: Font & f) {
    
    sf::Sprite tempPlat = platypus.sprite;
    tempPlat.setPosition(800.0f, 600.0f);
    tempPlat.setScale(0.5f, 0.5f);
    
    sf::Text text;
    text.setFont(f);
    text.setString("<< FLAPPY PLATY >>\n  press any key");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(700.0f, 350.0f);


    sf::Event event;

    while (true) { 
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return -1;
            }

            if (event.type == sf::Event::KeyPressed) {
                return 0; 
            }
        }


        window.clear(sf::Color(135, 206, 250));
        window.draw(text); 
        window.draw(tempPlat);

        window.display();
    }

    return 0;
}

void gameOver(sf::RenderWindow & window, sf :: Font & font, int & score) {


    sf::Text text;
    text.setFont(font);
    text.setString("<< GameOver >>");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Red);
    text.setPosition(700.0f, 350.0f);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score:     " + std::to_string(score));
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(750.0f, 500.0f);

    

    sf::Text press;
    press.setFont(font);
    press.setString("<<press any key>>");
    press.setCharacterSize(50);
    press.setFillColor(sf::Color::Red);
    press.setPosition(700.0f, 650.0f);

    sf::Event event;

    while (true) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                return;
            }
        }


        window.clear(sf::Color(135, 206, 250));
        window.draw(text);
        window.draw(scoreText);
        window.draw(press);

        window.display();
    }
}

int main() {
    srand(time(0));

    int score = 0;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flappy Bird");

    sf::Texture platypusTexture;
    sf::Texture pipeTexture;
    sf::Font font;
    if (!platypusTexture.loadFromFile("platypus.png") || !pipeTexture.loadFromFile("pipe.png") || !(font.loadFromFile("flappy_font.ttf"))) {
        return -1;
    }

    sf:: Sprite platObj;
    Body platypus(500.0f, 540.0f, platObj);

    platypus.sprite.setTexture(platypusTexture);
    platypus.sprite.setScale(0.14f, 0.14f);
    platypus.sprite.setPosition(platypus.x, platypus.y);

    if (mainScreen(platypus,window,font) == -1) {
        return -1;
    }


    vector<Body> upperObs, lowerObs;

    sf::Sprite obj1;
    Body temp1(1800.0f, -200.0f, obj1);

    temp1.sprite.setTexture(pipeTexture);
    temp1.sprite.setPosition(temp1.x, temp1.y);

    upperObs.push_back(temp1);

    sf::Sprite obj2;
    Body temp2(1800.0f, 600.0f, obj2);

    temp2.sprite.setTexture(pipeTexture);
    temp2.sprite.setPosition(temp2.x, temp2.y);

    lowerObs.push_back(temp2);

    sf::Clock clock;
    sf::Time delay = sf::seconds(0.003f);
    bool delayflag = true;
    bool key = false;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    key = true;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    key = false;
                }
            }
        }
        if (delayflag) {
            input(key, platypus);

            if (checkCollisions(upperObs, lowerObs, platypus)) {
                gameOver(window, font, score);
                break;
            }

            display(window, platypus, upperObs, lowerObs, pipeTexture, score,font);

            delayflag = false;
        }

        if (clock.getElapsedTime() > delay) {
            clock.restart();
            delayflag = true;
        }
    }


    return 0;
}
