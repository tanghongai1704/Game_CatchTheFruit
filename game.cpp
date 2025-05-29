#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700

// ------------------------- CLASS Entity (đối tượng rơi: enemy hoặc fruit) -------------------------
class Entity
{
public:
    sf::CircleShape shape;
    int type; // 0: enemy (kẻ địch), 1: fruit (trái cây)

    Entity(float x, float y, int type, float radius = 12.f) : type(type)
    {
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(x, y);
        // Gán màu theo loại: enemy màu trắng, fruit màu xanh
        shape.setFillColor(type == 0 ? sf::Color::White : sf::Color::Green);
    }

    void move(float dy)
    {
        shape.move(0, dy); // Di chuyển theo trục y
    }

    bool isOutOfBounds(float bottom)
    {
        // Kiểm tra nếu rơi quá giới hạn đáy
        return shape.getPosition().y - shape.getRadius() > bottom;
    }

    void reset(float x)
    {
        // Đặt lại vị trí về đầu (tọa độ y = 0)
        shape.setPosition(x, 0);
    }
};

// ------------------------- CLASS Player (người chơi) -------------------------
class Player
{
public:
    sf::CircleShape shape;
    float speed;
    float leftBound, rightBound;

    Player(float x, float y, float r, float step, float left, float right)
        : speed(step), leftBound(left), rightBound(right)
    {
        shape.setRadius(r);
        shape.setOrigin(r, r);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Yellow); // Người chơi màu vàng
    }

    void update()
    {
        // Di chuyển trái
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (shape.getPosition().x - shape.getRadius() > leftBound)
                shape.move(-speed, 0);
        }
        // Di chuyển phải
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (shape.getPosition().x + shape.getRadius() < rightBound)
                shape.move(speed, 0);
        }
    }

    // Getter
    float getX() const { return shape.getPosition().x; }
    float getY() const { return shape.getPosition().y; }
    float getRadius() const { return shape.getRadius(); }
};

// ------------------------- CLASS Game (toàn bộ game) -------------------------
class Game
{
private:
    sf::RenderWindow window;
    Player player;
    std::vector<Entity> enemies; // Danh sách các đối tượng rơi

    sf::RectangleShape bar; // Thanh giới hạn vùng di chuyển của player

    sf::Font font;
    sf::Text txtChances;   // Số lần còn lại
    sf::Text txtScore;     // Điểm hiện tại
    sf::Text txtGameOver;  // Thông báo game over
    sf::Text txtHighScore; // Điểm cao nhất

    int chances = 3;         // Số mạng
    int score = 0;           // Điểm hiện tại
    int maxEnemies = 5;      // Số lượng tối đa object trên màn hình
    float enemySpeed = 4.0f; // Tốc độ rơi
    sf::Clock spawnClock;    // Đồng hồ spawn mới
    sf::Clock speedClock;    // Đồng hồ tăng tốc độ

    int highScore = 0; // Lưu điểm cao nhất

public:
    Game() : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Catch the Fruit"),
             player(350, 515, 12, 8, 200, 500)
    {
        window.setFramerateLimit(60); // FPS cố định

        // Vẽ thanh giới hạn vùng chơi
        bar.setSize(sf::Vector2f(300, 30));
        bar.setPosition(200, 500);
        bar.setFillColor(sf::Color::Transparent);
        bar.setOutlineColor(sf::Color::White);
        bar.setOutlineThickness(2);

        // Bắt đầu với 1 enemy đầu tiên
        enemies.emplace_back(SCREEN_WIDTH / 2, 0, 0);

        // Load font
        font.loadFromFile("arial.ttf");

        // Khởi tạo text
        txtChances.setFont(font);
        txtChances.setCharacterSize(30);
        txtChances.setFillColor(sf::Color::White);
        txtChances.setPosition(SCREEN_WIDTH - 250, 100);

        txtScore.setFont(font);
        txtScore.setCharacterSize(30);
        txtScore.setFillColor(sf::Color::White);
        txtScore.setPosition(SCREEN_WIDTH - 250, 200);

        txtHighScore.setFont(font);
        txtHighScore.setCharacterSize(30);
        txtHighScore.setFillColor(sf::Color::Yellow);
        txtHighScore.setPosition(SCREEN_WIDTH - 250, 300);

        txtGameOver.setFont(font);
        txtGameOver.setCharacterSize(50);
        txtGameOver.setFillColor(sf::Color::Red);
        txtGameOver.setPosition(200, 300);
        txtGameOver.setString("Game Over! Press any key...");
    }

    void reset()
    {
        // Cập nhật điểm cao nếu điểm hiện tại cao hơn
        if (score > highScore)
            highScore = score;

        chances = 3;
        score = 0;
        enemySpeed = 4.0f;
        enemies.clear();
        enemies.emplace_back(SCREEN_WIDTH / 2, 0, 0); // Thêm lại enemy đầu tiên
    }

    void run()
    {
        while (window.isOpen())
        {
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                    window.close();
            }

            update();
            render();
        }
    }

    void update()
    {
        player.update();

        // Tăng tốc độ sau mỗi 10 giây
        if (speedClock.getElapsedTime().asSeconds() > 10.f)
        {
            enemySpeed += 0.5f;
            speedClock.restart();
        }

        // Spawn thêm entity nếu chưa đủ
        if (enemies.size() < maxEnemies && spawnClock.getElapsedTime().asMilliseconds() > 700)
        {
            float randX = 200 + rand() % 300;
            // 1/3 khả năng là fruit
            int type = (rand() % 3 == 0) ? 1 : 0;
            enemies.emplace_back(randX, 0, type);
            spawnClock.restart();
        }

        // Cập nhật từng entity
        for (auto &e : enemies)
        {
            e.move(enemySpeed);

            // Nếu rơi ra ngoài màn hình thì reset
            if (e.isOutOfBounds(530))
            {
                e.reset(200 + rand() % 300);
                // Giữ lại loại của entity
                if (e.type == 0)
                    e.shape.setFillColor(sf::Color::White);
                else
                    e.shape.setFillColor(sf::Color::Green);
            }

            // Kiểm tra va chạm với player
            float dx = std::abs(player.getX() - e.shape.getPosition().x);
            float dy = std::abs(player.getY() - e.shape.getPosition().y);
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < player.getRadius() + e.shape.getRadius())
            {
                if (e.type == 0)
                {
                    // Va chạm enemy: mất mạng
                    chances--;
                    e.reset(200 + rand() % 300);
                    e.shape.setFillColor(sf::Color::White);

                    if (chances <= 0)
                    {
                        renderGameOver();
                        reset();
                    }
                }
                else
                {
                    // Bắt được fruit: cộng điểm
                    score += 5;
                    e.reset(200 + rand() % 300);
                    e.shape.setFillColor(sf::Color::Green);
                }
            }
        }

        // Cập nhật thông tin text
        txtChances.setString("Chances: " + std::to_string(chances));
        txtScore.setString("Score: " + std::to_string(score));
        txtHighScore.setString("High Score: " + std::to_string(highScore));
    }

    void renderGameOver()
    {
        // Hiển thị game over
        window.clear();
        window.draw(txtGameOver);
        window.display();
        sf::sleep(sf::seconds(1.5));

        sf::Event event;
        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::KeyPressed)
                    return; // Chờ người chơi nhấn phím để chơi lại
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
    }

    void render()
    {
        window.clear();

        // Vẽ các đối tượng
        window.draw(bar);
        window.draw(player.shape);
        for (auto &e : enemies)
            window.draw(e.shape);

        window.draw(txtChances);
        window.draw(txtScore);
        window.draw(txtHighScore);

        window.display();
    }
};

// --------------------------- HÀM MAIN ---------------------------
int main()
{
    srand(static_cast<unsigned>(time(0))); // Seed random

    Game game;
    game.run();

    return 0;
}
