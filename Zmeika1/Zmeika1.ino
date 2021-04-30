#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t SmallFont[];

int visota = 56 - 1;
int shirina = 123 - 1;

class Player {
  private:
    int _pin_but0;
    int _pin_but1;
    int _pin_but2;
    int _pin_but3;
  public:
    Player (int pin_but0,
            int pin_but1,
            int pin_but2,
            int pin_but3) {                         // конструктор для подключения кнопок
      _pin_but0 = pin_but0;
      _pin_but1 = pin_but1;
      _pin_but2 = pin_but2;
      _pin_but3 = pin_but3;
    }
    int azimut = 2;
    void set_butt() {
      pinMode(_pin_but0, INPUT_PULLUP);
      pinMode(_pin_but1, INPUT_PULLUP);
      pinMode(_pin_but2, INPUT_PULLUP);
      pinMode(_pin_but3, INPUT_PULLUP);
      if (!digitalRead(_pin_but0)) azimut = 0;      // если кнопка нажата изменяем направление змейки
      if (!digitalRead(_pin_but1)) azimut = 1;
      if (!digitalRead(_pin_but2)) azimut = 2;
      if (!digitalRead(_pin_but3)) azimut = 3;
    }
}; Player player(8, 9, 10, 11);

struct Hvostic {
  int x;
  int y;
  int id;
};

class Hvost {
  public:
    Hvost () {

    }
    int dlinna = 10;
    Hvostic *hvostic = new Hvostic[dlinna];
    Hvostic *oldHvostic = new Hvostic[dlinna];
    void rost(int coord_x1, int coord_x2) {
      hvostic[0] = {coord_x1, coord_x2, 0};
      for (int a = 0; a < dlinna; a++) {
        oldHvostic[a] = hvostic[a];
        hvostic[a + 1] = oldHvostic[a];
      }
    }

    void update_coord() {

    }

    void draw() {
      for (int g = 0; g < dlinna; g++) {
        myOLED.print(".", hvostic[g].x, hvostic[g].y);
      }
      myOLED.update();
    }

}; Hvost hvost;


class Apple {
  public:
    Apple () {

    }
    int cordinata_x = random(shirina);
    int cordinata_y = random(visota);

    void genetate() {
      cordinata_x = random(shirina);
      cordinata_y = random(visota);
    }

    void draw() {
      //myOLED.clrScr();
      myOLED.drawCircle(cordinata_x, cordinata_y, 2);
      //myOLED.update();
    }

}; Apple apple;


class Head {
  public:
    Head() {

    }
    //hvost.mass_x[0] = coord_x;
    //hvost.mass_y[0] = coord_y;
    int coord_x = 0;      // коорднаты для змейки по координте x
    int coord_y = 0;      // коорднаты для змейки по координте y
    bool az_N = false, az_S = false; // направление змейки
    bool az_W = false, az_E = false; // направление змейки
    bool Deatch = false;
    bool Win = false;
    void set_coord() {
      switch (player.azimut) {
        case 1:
          if (0 < coord_y) coord_y--;
          else            coord_y = coord_y + visota;
          break;

        case 3:
          if (coord_y < visota) coord_y++;
          else                 coord_y = coord_y - visota;
          break;

        case 0:
          if (0 < coord_x) coord_x--;
          else           coord_x = coord_x + shirina;
          break;

        case 2:
          if (coord_x < shirina) coord_x++;
          else                  coord_x = coord_x - shirina;
          break;
      }
    }

    void deathWin() {
      if (Deatch) {

      }

      if (Win) {

      }

    }

    void eat () {
      if (coord_x == apple.cordinata_x && coord_y == apple.cordinata_y) {
        hvost.dlinna++;
        hvost.rost(coord_x, coord_y);
        apple.genetate();
      }
    }

    void draw() {
      //  myOLED.clrScr();
      //myOLED.print(".", coord_x, coord_y);
      myOLED.drawCircle(coord_x, coord_y, 1);
      //myOLED.update();
    }

}; Head head;

class Wall {                   // класс стена
  public:
    Wall () {

    }
    void perimetr() {
      /*if (head.coord_y < 0) {
        head.coord_y = 64;
        }
        if (head.coord_y > 64) {
        head.coord_y = 0;
        }
        if (head.coord_x < 0) {
        head.coord_x = 127;
        }
        if (head.coord_x > 127) {
        head.coord_x = 0;
        }*/

      if (head.coord_y < 0 or head.coord_y > 63 or head.coord_x < 0 or head.coord_x > 127) {
        head.Deatch = true;   //проверка на порожение
      }
    }

    void draw() {
      myOLED.clrScr();
      myOLED.drawRect(shirina + 1, 0, 0, visota + 1);
      //myOLED.update();
    }
}; Wall wall;

void setup() {
  myOLED.begin();
  myOLED.setFont(SmallFont);
}

void loop() {
  wall.draw();
  apple.draw();
  head.draw();
  hvost.draw();  
  head.eat();
  hvost.rost(head.coord_x, head.coord_y);
  hvost.draw();
  player.set_butt();
  head.set_coord();
  delay(100);
}
