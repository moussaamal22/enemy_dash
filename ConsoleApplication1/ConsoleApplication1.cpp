#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
using namespace sf;
 //view
const int M = 35;
const int N = 60;

int grid[M][N] = { 0 };
int ts = 18;

struct Enemy
{
    int x, y, dx, dy;

    Enemy()
    {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }

    void move()
    {
        x += dx;
        if (grid[y / ts][x / ts] == 1)
        {
            dx = -dx;
            x += dx;
        }
        y += dy;
        if (grid[y / ts][x / ts] == 1)
        {
            dy = -dy;
            y += dy;
        }
    }
};

void drop(int y, int x)
{
    if (grid[y][x] == 0)
        grid[y][x] = -1;
    if (grid[y - 1][x] == 0)
        drop(y - 1, x);
    if (grid[y + 1][x] == 0)
        drop(y + 1, x);
    if (grid[y][x - 1] == 0)
        drop(y, x - 1);
    if (grid[y][x + 1] == 0)
        drop(y, x + 1);
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts), "Amal's Game!");
    window.setFramerateLimit(60);
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(270, 200);
    sEnemy.setOrigin(20, 20);

    int enemyCount = 2;
    int enemyCountIncrement = 1;
    Enemy a[10];

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;
    float timer = 0, delay = 0.07;
    Clock clock;
    int score = 0;
    int bestScore = 0;
    int level = 1;
    int bestLevel = 1;


    sf::Font font1;
    font1.loadFromFile("./fast99.ttf");

    sf::Text scoreText;
    scoreText.setFont(font1);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(3, 0);

    sf::Font font2;
    font2.loadFromFile("./fast99.ttf");

    sf::Text bestScoreText;
    bestScoreText.setFont(font2);
    bestScoreText.setCharacterSize(20);
    bestScoreText.setFillColor(sf::Color::Black);
    bestScoreText.setPosition(3, 30);

    sf::Font font3;
    font3.loadFromFile("./stocky.ttf");

    sf::Text gameOverText;
    gameOverText.setFont(font3);
    gameOverText.setCharacterSize(20);
    gameOverText.setFillColor(sf::Color::Black);
    gameOverText.setString("Press ESCAPE to start again !");
    gameOverText.setPosition(350, 500);

    sf::Font font4;
    font4.loadFromFile("./fast99.ttf");

    sf::Text levelText;
    levelText.setFont(font4);
    levelText.setCharacterSize(30);
    levelText.setFillColor(sf::Color::Black);
    levelText.setPosition(935, 0);

    sf::Font font5;
    font5.loadFromFile("./fast99.ttf");

    sf::Text bestLevelText;
    bestLevelText.setFont(font5);
    bestLevelText.setCharacterSize(20);
    bestLevelText.setFillColor(sf::Color::Black);
    bestLevelText.setPosition(935, 30);

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    Music music;
    music.openFromFile("audio/vibes.ogg");
    music.setVolume(2);
    music.play();
    music.setLoop(true);

    Music music1;
    music1.openFromFile("audio/game-over.wav");
    music1.setVolume(8);

    Music music2;
    music2.openFromFile("audio/level-up.ogg");
    music2.setVolume(50);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape)
                {
                    // Reset the game
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;

                    x = 10;
                    y = 0;
                    Game = true;
                    score = 0;
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            dx = -1;
            dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            dx = 1;
            dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            dx = 0;
            dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            dx = 0;
            dy = 1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            music1.stop();
            music.play();
            music.setLoop(true);
        }
        if (!Game)
            continue;

        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0)
                x = 0;
            if (x > N - 1)
                x = N - 1;
            if (y < 0)
                y = 0;
            if (y > M - 1)
                y = M - 1;

            if (grid[y][x] == 2)
            {
                Game = false;

            }
            if (grid[y][x] == 0)
            {
                grid[y][x] = 2;
                score += 1;
                if (score % 50 == 0) {
                    music2.play();
                    level += 1;
                    enemyCount += enemyCountIncrement;

                }
                if (level > bestLevel)
                    bestLevel = level;
            }

            timer = 0;
        }


        for (int i = 0; i < enemyCount; i++)
        {
            a[i].move();
            if (score > bestScore)
                bestScore = score;

        }


        if (grid[y][x] == 1)
        {
            dx = dy = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1)
                        grid[i][j] = 0;
                    else
                        grid[i][j] = 1;
        }

        for (int i = 0; i < enemyCount; i++) {
            if (grid[a[i].y / ts][a[i].x / ts] == 2)
                Game = false;
            if (score % 50 == 0) {
                music2.play();
            }
        }

        /////////draw//////////

        window.clear(sf::Color::White);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0)
                    continue;
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(17.5, 0, ts, ts));
                if (grid[i][j] == 2)
                    sTile.setTextureRect(IntRect(35, 0, ts, ts));
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            }

        sTile.setTextureRect(IntRect(35, 0, ts, ts));
        sTile.setPosition(x * ts, y * ts);
        window.draw(sTile);

        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }

        if (!Game) {
            music.stop();
            music1.play();
            enemyCount = 2;
            level = 1;
            window.draw(sGameover);
            window.draw(gameOverText);

        }


        // Display the score
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        // Display the best score
        bestScoreText.setString("Best Score: " + std::to_string(bestScore));
        window.draw(bestScoreText);

        // Display the level
        levelText.setString("Level: " + std::to_string(level));
        window.draw(levelText);

        // Display the best level
        bestLevelText.setString("Best Level: " + std::to_string(bestLevel));
        window.draw(bestLevelText);

        window.display();
    }

    return 0;
}

