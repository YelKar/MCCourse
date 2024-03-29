namespace ctrl {
    class Motor;
    class Machine;
    typedef Motor* Motors;
    void limit(int * x);  // Функция ограничевает значение переданного указателя до 255
    int limit(int x);  // Функция ограничевает значение переданной переменной до 255 и возвращает полученное значение
    int mod(int a, int b);  // Функция для взятия остатка
}

class ctrl::Motor {
public:
    int currSpeed;
    int8_t fd_pin, bd_pin;
    Motor(const int8_t pwd_fd, const int8_t dig_bd) {
        fd_pin = pwd_fd;
        bd_pin = dig_bd;
        currSpeed = 0;
    }
    void go(int speed) {
        limit(&speed);
        digitalWrite(bd_pin, speed < 0);
        ledcWrite(fd_pin, mod(speed, 256));
        currSpeed = speed;
    }
};

class ctrl::Machine {
public:
    Motor *fl, *fr, *bl, *br;
    int xSpeed   = 0, 
        ySpeed   = 0, 
        rotSpeed = 0;
    Motors all;
    Machine(Motor* _fl, Motor* _fr, Motor* _bl, Motor* _br) {
        Motor all_[] = {
            *(fl = _fl),
            *(fr = _fr),
            *(bl = _bl),
            *(br = _br)
        };
        all = all_;
    }
    void y(int speed) {
        ySpeed = speed;
    }
    void x(int speed) {
        xSpeed = speed;
    }
    void rotate(int speed) {
        rotSpeed = speed;
    }
    void stop() const {
        Motors mtr = all;
        for (int i = 0; i < 4; mtr++, i++) {
            mtr->go(0);
        }
    }
    void update() {
        fl -> go(ySpeed + rotSpeed + xSpeed);
        fr -> go(ySpeed - rotSpeed - xSpeed);
        bl -> go(ySpeed + rotSpeed - xSpeed);
        br -> go(ySpeed - rotSpeed + xSpeed);
        xSpeed = ySpeed = rotSpeed = 0;
    }
};


int ctrl::mod(int a, int b) {
    if (a >= 0) {
      return a % b;
    }
    return b + a % b;
}

void ctrl::limit(int * x) {
    if (!x) {
      return;
    }
    if (abs(*x) >= 256) {
        *x = 255 * (abs(*x) / *x);
    }
}
int ctrl::limit(int x) {
    if (!x) {
      return 0;
    }
    if (abs(x) >= 256) {
        return 255 * (abs(x) / x);
    }
    return x;
}