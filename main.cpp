#include <SFML/Graphics.hpp>
#include <list>
#include <sstream>
#include <iostream>
using namespace sf;
using namespace std;

const int HEIGHT_MAP = 10; 
const int WIDTH_MAP = 120; 
int groundY = 420;

sf::String TileMap[HEIGHT_MAP] = {
	"                                    c                 i                     c                                       c   ",
	"              c           c                   c           c                                  c           c              ",
	"b   c                            c                     h        i                  c               t                   s",
	"b              c        i                           sbss      sbs     c                      b                         b",
	"bc                i   bbs   c   i         t    bb ssbbb      c       h       i      c    bbbbs   sbsbs     s         c s",
	"b       h        sb            sbb   b  ssbbs    c                       sbbbb      ssb         c              bbsbb   s",
	"sb    c    b               sbb   c                                 bssb              b  c                             bs",
	"bb   i     ss    c   s                       iii      c                    iii     bb                  t             Bbb",
	"bbs     bbsb       bbs      t    s  bbb          t       h       c                 sss         h            c       bsbb",
	"gggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg"
};

sf::View view;

void getplayercoordinateforview(float x, float y);

class Basic {
public:
	float dx, dy;
	FloatRect rect;
	bool onGround, life;
	Texture image;
	Sprite sprite;
	int health;

	Basic(const Texture& t, int x, int y) : image(t) {
		sprite.setTexture(image);
		sprite.setPosition(x, y);
		health = 100;
		life = true;
		if (y == 420) onGround = true; 
		else onGround = false;
		dx = 0.2;
		dy = 0.37;
	}

	virtual void update(const float& time) = 0;

	virtual void Collision(const int& dir) = 0;
	
};

class PLAYER : public Basic {

public:
	float currentFrame;
	int score;

	PLAYER(const Texture& t, int x, int y = 420) : Basic(t,x,y)
	{
		rect = FloatRect(120, 85, 70, 111);
		sprite.setTextureRect(IntRect(rect));
		currentFrame = 0;
		score = 0;
	}


	void update(const float& time) override
	{

		rect.left += dx * time; //перем≥щаЇмо координату х на швиду≥сть * час(в≥дстань)
		Collision(0);

		if (!onGround) dy = dy + 0.0005 * time; // €кщо ми не на зeмл≥ падаЇм з прискоренн€м 
		rect.top += dy * time; // у перем≥щаЇмо на швидк≥сть * на час(в≥дстань)
		onGround = false; // поки що
		Collision(1);

		if (rect.top > groundY) { rect.top = groundY; dy = 0; onGround = true; }

		currentFrame += 0.005 * time;
		if (currentFrame > 3) currentFrame = 0;

		if (dx > 0) {

			sprite.setTexture(image);
			if (currentFrame <= 1) {
				sprite.setTextureRect(IntRect(11, 86, 78, 114));
			}
			else if (currentFrame <= 2) {
				sprite.setTextureRect(IntRect(120, 85, 70, 111));
			}
			else if (currentFrame <= 3) {
				sprite.setTextureRect(IntRect(220, 79, 60, 120));
			}
			sprite.move(dx * time, 0);
			getplayercoordinateforview(rect.left, rect.top);
		}
		if (dx < 0) {

			sprite.setTexture(image);
			if (currentFrame <= 1) {
				sprite.setTextureRect(IntRect(89, 86, -78, 114));
			}
			else if (currentFrame <= 2) {
				sprite.setTextureRect(IntRect(190, 85, -70, 111));
			}
			else if (currentFrame <= 3) {
				sprite.setTextureRect(IntRect(280, 79, -60, 120));
			}
			sprite.move(dx * time, 0);//происходит само движение персонажа влево
			getplayercoordinateforview(rect.left, rect.top);
		}


		sprite.setPosition(rect.left, rect.top);

		dx = 0;
		if (health <= 0) life = false;
		if (!life) sprite.setColor(Color::Red);
	}

	void Collision(const int& dir) override
	{
		for (int i = rect.top / 60; i < (rect.top + rect.height) / 60; i++)
			for (int j = rect.left / 60; j < (rect.left + rect.width) / 60; j++)
			{
				if (TileMap[i][j] == 'b' || TileMap[i][j] == 's')
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 60 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 60 + 60;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 60 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 60 + 60;   dy = 0; }
				}

				if (TileMap[i][j] == 't')
				{
					health -= 40;
					TileMap[i][j] = ' ';

				}

				if (TileMap[i][j] == 'i')
				{
					score++;
					TileMap[i][j] = ' ';
				}

				if (TileMap[i][j] == 'h')
				{

					if (health < 80) health += 20;
					else health += 100 - health;
					TileMap[i][j] = ' ';
				}



			}

	}
};


class ENEMY : public Basic{
public:
	ENEMY(const Texture& t, int x, int y=420) : Basic(t,x,y)
	{
		rect = FloatRect(x, y, 93, 114);
		sprite.setTextureRect(IntRect(rect));
	}

	void update(const float& time) override
	{

		rect.left += dx * time; //перем≥щаЇмо координату х на швиду≥сть * час(в≥дстань)
		Collision(0);

		if (!onGround) dy = dy + 0.0005 * time; // €кщо ми не на зeмл≥ падаЇм з прискоренн€м 
		rect.top += dy * time; //перем≥щаЇмо y на швидк≥сть * час(в≥дстань)
		onGround = false; 
		Collision(1);

		if (rect.top > groundY) { rect.top = groundY; dy = 0; onGround = true; }

		if (dx > 0) {
			sprite.setTextureRect(IntRect(170, 91, 93, 114));
			sprite.move(dx * time, 0);
		}
		if (dx < 0) {
			sprite.setTextureRect(IntRect(263, 91, -93, 114));
			sprite.move(dx * time, 0);
		}

		sprite.setPosition(rect.left, rect.top);

		if (health <= 0) life = false;
		if (!life) sprite.setColor(Color::Black);
	}
	void Collision(const int& dir) override
	{
		for (int i = rect.top / 60; i < (rect.top + rect.height) / 60; i++)
			for (int j = rect.left / 60; j < (rect.left + rect.width) / 60; j++)
			{
				if (TileMap[i][j] == 'b' || TileMap[i][j] == 's')
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 60 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 60 + 60;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 60 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 60 + 60;   dy = 0; }
				}

			}
	}
};




int main()
{
	RenderWindow window(sf::VideoMode(1500, 600), "Miner"); 
	view.reset(sf::FloatRect(0, 0, 1500, 600));
	
	Texture herotexture;
	herotexture.loadFromFile("hero4.png");
	Texture enemytexture;
	enemytexture.loadFromFile("enemy.png");


	PLAYER p(herotexture, 140);

	/*list<ENEMY> EN = { (ENEMY(enemytexture, 1700 * 1)) ,(ENEMY(enemytexture, 1700 * 2)) ,(ENEMY(enemytexture, 1700 * 3)) ,(ENEMY(enemytexture, 1700 * 4)) };
	list<ENEMY>::iterator e;
	e = EN.begin();
	for (e = EN.begin(); e != EN.end(); ++e) {
		cout << e->rect.left << '\t';
	}*/

	Clock clock;
	Clock gameTimeClock; //час гри 
	int gameTime = 0;

	Texture sky;
	sky.loadFromFile("sky2.jpg");
	Texture cloud;
	cloud.loadFromFile("sky.jpg");
	Texture box1;
	box1.loadFromFile("box1.jpg");
	Texture box2;
	box2.loadFromFile("box2.jpg");
	Texture land;
	land.loadFromFile("land.jpg");
	Texture TNT;
	TNT.loadFromFile("TNT.jpg");
	Texture heart;
	heart.loadFromFile("heart.png");
	Texture coin;
	coin.loadFromFile("coin.png");

	Sprite map;
	map.setTexture(sky);

	Font font;//шрифт 
	font.loadFromFile("font.ttf");
	Text text("", font, 40);
	text.setStyle(sf::Text::Bold);
	Text text2("", font, 40);
	text.setStyle(sf::Text::Bold);
	Text textDeath("", font, 170);
	textDeath.setStyle(sf::Text::Bold);
	
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asSeconds();
	
		clock.restart();
		time = time / 800;
		
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.draw(p.sprite);
		if (p.life) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) p.dx = -0.3;
			if (Keyboard::isKeyPressed(Keyboard::Right))  p.dx = 0.3;
			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				getplayercoordinateforview(p.rect.left, p.rect.top);
				if (p.onGround) { p.dy = -0.37; p.onGround = false; }
			}
		}
		p.update(time);

		/*cout << endl;
		e = EN.begin();
		for (e = EN.begin(); e != EN.end(); ++e) {
			if (gameTime % 3 == 0) e->dx = -0.15;
			if (gameTime % 6 == 0) e->dx = 0.15;
			cout << gameTime<<" ";
		}

		

		p.update(time);
		e = EN.begin();
		for (e = EN.begin(); e != EN.end(); ++e) {
			e->update(time);
			cout <<  ".";
		}*/
		

		window.setView(view);
		window.clear();

	/////////////////////////////Kартa/////////////////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ') { map.setTexture(sky); if (!p.life) map.setColor(Color::Red);}
					 
				if (TileMap[i][j] == 'b') map.setTexture(box1);
					
				if (TileMap[i][j] == 's') map.setTexture(box2);
					
				if (TileMap[i][j] == 'c') { map.setTexture(cloud); if (!p.life) map.setColor(Color::Red); }
					
				if (TileMap[i][j] == 'g') map.setTexture(land);
					
				if (TileMap[i][j] == 'h') map.setTexture(heart);
					
				if (TileMap[i][j] == 't') map.setTexture(TNT);
					
				if (TileMap[i][j] == 'i') map.setTexture(coin);
					
				map.setTextureRect(IntRect(0, 0, 60, 60));
				map.setPosition(j * 60, i * 60);//розкидуЇ квадратики

				window.draw(map);
			}

		std::ostringstream playerScoreString;   
		playerScoreString << p.score;		
		text.setString("Coin: " + playerScoreString.str()); 
		text.setPosition(view.getCenter().x -750, view.getCenter().y-290);
		window.draw(text);

		std::ostringstream playerHealthString, gameTimeString;   
		playerHealthString << p.health; gameTimeString << gameTime;		
		text2.setString("Health: " + playerHealthString.str() + "\nTime: " + gameTimeString.str());

		text2.setPosition(view.getCenter().x - 750, view.getCenter().y - 230);
		window.draw(text2);

		textDeath.setString("!!! GAME OVER !!!");
		textDeath.setPosition(view.getCenter().x-720, view.getCenter().y-120);

		//if (!p.life) for(auto i : EN) i.sprite.setColor(Color::Red);
			
		
		if(!p.life) window.draw(textDeath);
		//for (auto i : EN) window.draw(i.sprite);
		window.draw(p.sprite);
		
		window.display();
	}
	
	return 0;
}

void getplayercoordinateforview(float x, float y) { //функц≥€ считуванн€ координат гравц€ (750 п≥ксел≥в в≥д ст≥н)
	float tempX = x; float tempY = y;
	tempY = 300;
	if (x < 750) tempX = 750;
	if (x > 6450) tempX = 6450;
	view.setCenter(tempX, tempY);
}
