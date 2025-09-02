#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ENEMY_SPEED 3
#define BULLET_SPEED 5
#define PLAYER_SPEED 5
#define NUM_ENEMIES 5

struct Enemy {
    sf::RectangleShape shape;
    bool active = true;
};

struct Bullet {
    sf::RectangleShape shape;
    bool active = true;
};

int main() {
    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Buzz Bomber Game");
    window.setFramerateLimit(60);

    
    std::srand(static_cast<unsigned>(std::time(0)));

    
    sf::RectangleShape player(sf::Vector2f(50, 10));
    player.setFillColor(sf::Color::Green);
    player.setPosition(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 30);

    
    std::vector<Enemy> enemies(NUM_ENEMIES);
    for (int i = 0; i < NUM_ENEMIES; ++i) {
        enemies[i].shape.setSize(sf::Vector2f(40, 20));
        enemies[i].shape.setFillColor(sf::Color::Red);
        enemies[i].shape.setPosition(std::rand() % (WINDOW_WIDTH - 40), std::rand() % 200);
    }

    
    std::vector<Bullet> bullets;

    
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; 
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER!");
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 50);

    int score = 0;
    bool gameOver = false;

    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!gameOver) {
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player.getPosition().x > 0) {
                player.move(-PLAYER_SPEED, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player.getPosition().x < WINDOW_WIDTH - player.getSize().x) {
                player.move(PLAYER_SPEED, 0);
            }

            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                Bullet newBullet;
                newBullet.shape.setSize(sf::Vector2f(5, 10));
                newBullet.shape.setFillColor(sf::Color::Yellow);
                newBullet.shape.setPosition(player.getPosition().x + player.getSize().x / 2 - 2.5, player.getPosition().y - 10);
                bullets.push_back(newBullet);
            }

           
            for (auto& bullet : bullets) {
                if (bullet.active) {
                    bullet.shape.move(0, -BULLET_SPEED);
                    if (bullet.shape.getPosition().y < 0) {
                        bullet.active = false;
                    }
                }
            }

           
            for (auto& enemy : enemies) {
                if (enemy.active) {
                    enemy.shape.move(0, ENEMY_SPEED);

                    
                    if (enemy.shape.getPosition().y > WINDOW_HEIGHT - 40) {
                        gameOver = true;
                    }
                }
            }

           
            for (auto& bullet : bullets) {
                if (!bullet.active) continue;
                for (auto& enemy : enemies) {
                    if (enemy.active && bullet.shape.getGlobalBounds().intersects(enemy.shape.getGlobalBounds())) {
                        bullet.active = false;
                        enemy.active = false;
                        score += 10;

                        
                        enemy.shape.setPosition(std::rand() % (WINDOW_WIDTH - 40), 0);
                        enemy.active = true;
                    }
                }
            }
        }

       
        window.clear();

        
        if (!gameOver) {
            window.draw(player);
        }

        for (const auto& enemy : enemies) {
            if (enemy.active) {
                window.draw(enemy.shape);
            }
        }

       
        for (const auto& bullet : bullets) {
            if (bullet.active) {
                window.draw(bullet.shape);
            }
        }

        
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        
        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}