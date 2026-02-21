#include <Arduino.h>
#include <mathematics.hpp>
#include <M5Cardputer.h>
#include <M5Unified.h>
#include <M5GFX.h>
#include <list>

#define m M5Cardputer

int numofballs = 3;
float gravity = 0.0;
float damp = 1;
float gravitych = 0;

std::list<Ball> balls;

int randomColor() {
    return random(0, 255);
}

void setup() {
    const auto cfg = M5.config();
    m.begin(cfg);
    Serial.begin(115200);

    for (auto i = 0; i < numofballs; ++i) {
        float x  = random(20, 200);
        float y  = random(20, 100);
        float velx = random(-1, 2);
        float vely = random(-1, 2);
        int mass  = random(1, 3);
        int radius  = mass * 10;
        balls.emplace_back(Ball(x, y, {velx, vely}, radius, mass, randomColor(), randomColor(), randomColor()));
    }
    Serial.println("Balls created");

    m.Display.setTextSize(2);
    m.Display.setCursor(m.Lcd.width()/2 - 65, m.Lcd.height()/2 - 5);
    m.Display.setTextColor(TFT_YELLOW);
    m.Display.setCursor(0, 0);
    m.Display.drawRect(0, 0, m.Lcd.width(), m.Lcd.height(), TFT_BLUE);
}

bool start = false;

float beforeTime = 0;
float timedelay = 1000;

void loop() {
    float currentTime = millis();
    M5Canvas c(&m.Lcd);
    m.update();

    int h = m.Lcd.height();
    int w = m.Lcd.width();

    if (!start) {
        m.Display.setCursor(m.Lcd.width()/2 - 65, m.Lcd.height()/2 - 5);
        m.Display.setTextColor(TFT_YELLOW);
        m.Display.print("Press ENTER");
        m.Display.setCursor(0, 0);
        m.Display.drawRect(0, 0, m.Lcd.width(), m.Lcd.height(), TFT_BLUE);
        if (currentTime - beforeTime >= timedelay && m.Keyboard.isChange()) {
            if (m.Keyboard.isKeyPressed(KEY_ENTER)) {
                start = true;
                m.Speaker.tone(600, 100);
            }
        }
    } else {
        c.createSprite(w, h);
        c.fillSprite(c.color565(100,235,200));
        for (auto& ball : balls) {
            c.fillCircle(ball.x, ball.y, ball.radius, c.color565(ball.colorR, ball.colorG, ball.colorB));
        }
        c.drawRect(0, 0, m.Lcd.width(), m.Lcd.height(), TFT_BLUE);
        c.pushSprite(0, 0);
        gravity += gravitych;
        for (auto& ball : balls){
            ball.vel.y += gravity;

            ball.x += ball.vel.x;
            ball.y += ball.vel.y;

            if (ball.x + ball.radius >= w) {
                ball.x = w - ball.radius;
                if(dotProduct(ball.vel, Vec2{-1, 0}) < 0){
                    ball.vel.x -= 2.0 * dotProduct(ball.vel, Vec2{-1,0}) * -1;
                }
                ball.vel.x *= damp;
                m.Speaker.tone(700, 50);
            }
            if (ball.y + ball.radius >= h) {
                ball.y = h - ball.radius;
                if(dotProduct(ball.vel, Vec2{0, -1}) < 0){
                    ball.vel.y -= 2.0 * dotProduct(ball.vel, Vec2{0, -1}) * -1;
                }
                ball.vel.y *= damp;
                m.Speaker.tone(700, 50);
            }
            if (ball.x  <= ball.radius) {
                ball.x = ball.radius;
                if(dotProduct(ball.vel, Vec2{1, 0}) < 0){
                    ball.vel.x -= 2.0 * dotProduct(ball.vel, Vec2{1, 0}) * 1;
                }
                ball.vel.x *= damp;
                m.Speaker.tone(700, 50);
            }
            if (ball.y <= ball.radius) {
                ball.y = ball.radius;
                if(dotProduct(ball.vel, Vec2{0, 1}) < 0){
                    ball.vel.y -= 2.0 * dotProduct(ball.vel, Vec2{0, 1}) * 1;
                }
                ball.vel.y *= damp;
                m.Speaker.tone(700, 50);

            }


            //keybinding
            if (m.Keyboard.isKeyPressed('a')){
                if (ball.radius < 1) {
                    ball.radius = 1;
                }else {
                    ball.radius -= 1;
                }
            }
            if (m.Keyboard.isKeyPressed('d')){
                ball.radius += 1;
            }
            if (m.Keyboard.isChange()) {
                if (m.Keyboard.isKeyPressed('r')) {
                    m.Speaker.tone(700, 100);
                    ball.mass = random(1, 100);
                    m.Display.fillScreen(TFT_BLACK);
                    m.Display.setCursor(20, m.Lcd.height()/2 - 5);
                    m.Display.print("Masses randomized!");
                    m.Display.setCursor(0,0);
                    delay(500);
                }
                if (m.Keyboard.isKeyPressed(KEY_ENTER)) {
                    if (start){
                        start = false;
                        m.Speaker.tone(600, 100);
                    }
                }
                if (m.Keyboard.isKeyPressed('m')) {
                    m.Speaker.tone(700, 100);
                    if (m.Speaker.getVolume() == 0) {
                        m.Speaker.setVolume(50);
                    }else {
                        m.Speaker.setVolume(0);
                    }
                }
                if (m.Keyboard.isKeyPressed('f')) {
                    m.Speaker.tone(700, 100);
                    m.Display.fillScreen(TFT_BLACK);
                    m.Display.setCursor(20, m.Lcd.height()/2 - 5);
                    m.Display.print("Added velocities");
                    for (auto& ball : balls) {
                        ball.vel.y = 1;
                        ball.vel.x = 1;
                    }
                    m.Display.setCursor(0,0);
                    delay(500);
                }
                if (m.Keyboard.isKeyPressed('h')) {
                    m.Speaker.tone(700, 100);
                    m.Display.fillScreen(TFT_BLACK);
                    m.Display.setCursor(20, m.Lcd.height()/2 - 5);
                    if (damp >= 1.0f) {
                        m.Display.print("Friction on!");
                        damp = 0.9f;
                    }else {
                        m.Display.print("Friction off!");
                        damp = 1.0f;
                    }
                    m.Display.setCursor(0,0);
                    delay(500);
                }
                if (m.Keyboard.isKeyPressed('b')) {
                    numofballs++;
                    balls.emplace_back(Ball(random(20, 200), random(20, 200), {1, 1}, 10, 1, randomColor(), randomColor(), randomColor()));
                }
                if (m.Keyboard.isKeyPressed('x')) {
                    balls.pop_back();
                }
            }
        }
        //calculating ball-ball ellastic collisions
        for (auto i = balls.begin(); i != balls.end(); i++) {
            auto j = i;
            j++;
            for (; j != balls.end(); j++) {
                Ball& ball = *i;
                Ball& ball2 = *j;
                if (ball.radius + ball2.radius >= sqrt((ball.x - ball2.x) * (ball.x - ball2.x) + (ball.y - ball2.y) * (ball.y - ball2.y))) {
                    calcEllasticCollsion(ball, ball2, damp);
                    m.Speaker.tone(700, 100);
                }
            }
        }
    }
}

