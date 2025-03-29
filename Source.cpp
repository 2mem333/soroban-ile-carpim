#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "sfml_lib.h"
#include <string>
//76,81
using namespace sf;
int cerceve_yatay = 600;
int cerceve_dikey = 250;
int cerceve_x = 90;
int cerceve_y = 90;
int cubuksayisi = 13;
int cubuk_arasi = cerceve_yatay / (cubuksayisi + 1);
//boncuklarin kordinatlari
CircleShape boncuklar[13][5];

int _sayi1;
int _sayi2;
bool sayilargirildi = false;
//****************************
struct Move {
    int cubuk;
    int boncuk;
    int durum;

    std::string olay;
    std::string olay2;

    int degerler[13];
};
std::vector<Move> moveQueue;
Move lastMove;
//****************************

sf::Text createText(const std::string& content, float x, float y, int size, sf::Color color, sf::Font& font) {
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(color);

    // Doğrudan verilen konuma yerleştir
    text.setPosition(x, y);
    return text;
}

void boncukdurumuguncelle(int cubuk, int sira, bool durum)
{
    cubuk = 12 - cubuk; //tersten, soroban boncuk indeksini soldan baslatmak icin

    int uzaklik = -50;
    int b = cerceve_dikey; //eger ustteki boncugu eski konumuna dondurmek istersek, y'si cerceve_y olmali
    if (!sira)
        uzaklik = uzaklik * -1;
    else {
        sira = 5 - sira;
        b = 0;
    }

    sf::Vector2f pos = boncuklar[cubuk][sira].getPosition();
    if (!durum) {
        boncuklar[cubuk][sira].setPosition(pos.x, cerceve_y + cerceve_dikey - b - 30 * sira); //eski konumuna dondurur.
        boncuklar[cubuk][sira].setFillColor(Color(192, 192, 192));
    }
    else {
        boncuklar[cubuk][sira].setPosition(pos.x, pos.y + uzaklik); //bi üstüne cikarir.
        boncuklar[cubuk][sira].setFillColor(Color(128, 128, 128));
    }
}

void boncuklarisifirla()
{
    for (int cubuk = 0; cubuk < 13; cubuk++)
    {
        boncuklar[cubuk][0].setFillColor(sf::Color(192, 192, 192));
        boncuklar[cubuk][0].setPosition(cerceve_x + cubuk_arasi * (cubuk + 1) - 15, cerceve_y);

        for (int a = 1; a < 5; a++)
        {
            boncuklar[cubuk][a].setFillColor(sf::Color(192, 192, 192));
            boncuklar[cubuk][a].setPosition(cerceve_x + cubuk_arasi * (cubuk + 1) - 15, cerceve_y + cerceve_dikey - 30 * a);
        }
    }
    moveQueue.clear();
}

class soroban {
private:
    std::string olay = "";
    std::string olay2 = "";
public:
    int dizi[13][5];

    int cubukdegeri(int cubuk)
    {
        int max = dizi[cubuk][0] * 5;

        for (int i = 1;i < 5; i++)
        {
            max = max + dizi[cubuk][i] * 1;
        }

        return max;
    }

    int alt_cubukdegeri(int cubuk)
    {
        int max = 0;
        for (int i = 1;i < 5; i++)
        {
            max = max + dizi[cubuk][i] * 1;
        }
        return max;
    }

    int bassayisi(int sayi)
    {
        int count = 0;
        while (sayi)
        {
            count++;
            sayi = sayi / 10;
        }
        return count;
    }
    int basamakbul(int sayi, int bas)
    {
        int temp = 0;
        for (int i = 0; i < bas; i++)
        {
            if (sayi == 0)
                return -1;
            temp = sayi % 10;
            sayi = sayi / 10;
        }
        return temp;
    }

    soroban()
    {
        for (int i = 0; i < 13; i++)
            for (int a = 0; a < 5; a++)
                dizi[i][a] = 0;
    }

    void sifirla()
    {
        for (int i = 0; i < 13; i++)
            for (int a = 0; a < 5; a++)
                dizi[i][a] = 0;
    }
    void printSoroban() {
        for (int i = 4; i >= 0; i--) {
            std::cout << "Cubuk " << i << ": ";
            std::cout << "[Ust boncuk: " << dizi[i][0] << "]  Alt boncuklar: ";
            for (int j = 1; j < 5; j++) {
                std::cout << dizi[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void boncuk_duzenle(int cubuk, int boncuk, int durum)
    {
        if (dizi[cubuk][boncuk] == durum)
            dizi[cubuk][boncuk] = durum;
        else {
            dizi[cubuk][boncuk] = durum;
            printSoroban();
            std::cout << "-------------------------------------\n";
        }
        boncukdurumuguncelle(cubuk, boncuk, durum);
    }

    void recordMove(int cubuk, int boncuk, int durum) //boncuklarin oynama durumunu duzenle ve kaydet.
    {
        if (dizi[cubuk][boncuk] == durum)
            dizi[cubuk][boncuk] = durum;
        else {
            dizi[cubuk][boncuk] = durum;
            printSoroban();
            std::cout << "-------------------------------------\n";
        }

        Move newMove;
        newMove.cubuk = cubuk;
        newMove.boncuk = boncuk;
        newMove.durum = durum;
        newMove.olay = olay;
        newMove.olay2 = olay2;
        for (int i = 0; i < 13; ++i) {
            newMove.degerler[i] = cubukdegeri(i);  // Örnek olarak her elemanı 2 ile çarpıyoruz
        }
        moveQueue.push_back(newMove); //boncuklarin oynama durumunu kaydet.
    }

    void cc(int carpim, int c1, int c2)
    {
        for (int cubuk = c1; cubuk <= c2; cubuk++)
        {
            int sayi = carpim % 10;
            carpim = carpim / 10;
            if (sayi == 0)
                continue;

            if (9 - cubukdegeri(cubuk) < sayi) //bu sayi bu cubukta tasma yapiyor!
            {
                sayi = 10 - sayi; //geri odenecek borc
                //bir soldaki cubuktan oynama yapilip, mevcut cubuk eksiltilir.
                for (int t = 1; t < 13; t++)
                {
                    if (cubukdegeri(cubuk + 1) < sayi) //iki sonraki cubuktan borc alicaz
                    {
                        if (dizi[cubuk + 2][t] == 0)
                        {
                            sayi = sayi + 100; //odenecek borc artti.
                            olay2 = std::to_string(cubuk + 3) + ". cubuktan 1 alt boncuk borc alindi, fazladan " + std::to_string(sayi) + " boncuk odenecek.";
                            recordMove(cubuk + 2, t, 1);
                            break;
                        }
                    }
                    if (dizi[cubuk + 1][t] == 0)
                    {
                        olay2 = std::to_string(cubuk + 2) + ". cubuktan 1 alt boncuk borc alindi, fazladan " + std::to_string(sayi) + " boncuk odenecek.";
                        recordMove(cubuk + 1, t, 1);
                        break;
                    }
                }

                if (sayi < 5)
                {
                    if (sayi > alt_cubukdegeri(cubuk)) //eger sayi alt cubuklardan borcu odeyemiyorsak
                    {
                        olay2 = "Borc odeniyor, " + std::to_string(cubuk) + ". cubugun ust boncugu 1 azaltildi.";
                        recordMove(cubuk, 0, 0);
                        sayi = 5 - sayi;
                        //fazla verilenleri geri alicaz simdi
                        int ind = 1;
                        int t = 0;
                        while (t < sayi)
                        {
                            if (!(dizi[cubuk][ind])) {
                                t++;
                                olay2 = std::to_string(cubuk + 1) + ". cubugun alt boncugu 1 arttirildi.";
                                recordMove(cubuk, ind, 1);
                            }
                            ind++;
                        }
                    }
                    else {
                        int ind = 4; //azaltmaya en dıştan başlar
                        int t = 0;
                        while (t < sayi)
                        {
                            if (dizi[cubuk][ind]) {
                                t++;
                                olay2 = std::to_string(cubuk + 1) + ". cubugun alt boncugu 1 azaltildi.";
                                recordMove(cubuk, ind, 0);
                            }
                            ind--;
                        }
                    }
                }
                else
                {
                    olay2 = std::to_string(cubuk + 1) + ". cubugun ust boncugu 1 azaltildi.";
                    recordMove(cubuk, 0, 0);
                    int ind = 4;
                    int t = 0;
                    while (t < sayi - 5)
                    {
                        if (dizi[cubuk][ind]) {
                            t++;
                            olay2 = std::to_string(cubuk + 1) + ". cubugun alt boncugu 1 azaltildi.";
                            recordMove(cubuk, ind, 0);
                        }
                        ind--;
                    }
                }
            }
            else
            {
                //sayi kadar deger cubuga eklenecek!.
                if (sayi < 5) //buraya bi kontrol lazim, eger 5'den kucukse fakat alttaki cubuklarla gosterilemiyorsa
                {
                    if (sayi > 4 - alt_cubukdegeri(cubuk)) //eger sayi alt cubuklarda tasma yapiyorsa ustten borc al alttan dusur.
                    {
                        sayi = 5 - sayi;
                        olay2 = std::to_string(cubuk + 1) + ". cubugun ust boncugu arttirildi, fazladan " + std::to_string(sayi) + " kadar boncuk alttan azaltilacak";
                        recordMove(cubuk, 0, 1);
                        int ind = 4;
                        int t = 0;
                        while (t < sayi)
                        {
                            if ((dizi[cubuk][ind]) == 1) {
                                t++;
                                olay2 = std::to_string(cubuk + 1) + ". cubugun alt boncugu 1 azaltildi.";
                                recordMove(cubuk, ind, 0); //aldigimiz borcu oderiz
                            }
                            ind--;
                        }
                    }
                    else { //alt cubuklarda tasma yapmiyorsa, alt cubuklardan oyna
                        int ind = 1;
                        int t = 0;
                        while (t < sayi)
                        {
                            if (!(dizi[cubuk][ind])) {
                                t++;
                                olay2 = std::to_string(cubuk + 1) + ". cubugun alt boncugu 1 arttirildi.";
                                recordMove(cubuk, ind, 1);
                            }
                            ind++;
                        }
                    }
                }
                else
                {
                    olay2 = std::to_string(cubuk + 1) + ". cubugun ust boncugu arttirildi.";
                    recordMove(cubuk, 0, 1);
                    int ind = 1;
                    int t = 0;
                    while (t < sayi - 5)
                    {
                        if (!(dizi[cubuk][ind])) {
                            t++;
                            olay2 = std::to_string(cubuk + 1) + ". cubugun alt boncugu 1 arttirildi.";
                            recordMove(cubuk, ind, 1);
                        }
                        ind++;
                    }
                }
            }
        }
    }

    void carp(int sayi1, int sayi2)
    {
        int basamak1 = bassayisi(sayi1); //basamagi uzun olan
        int basamak2 = bassayisi(sayi2);
        std::cout << sayi1 << " ile " << sayi2 << " carpiliyor: " << sayi1 * sayi2 << " \n";

        for (int b2 = 1; b2 <= basamak2; b2++)
        {
            int rakam2 = basamakbul(sayi2, b2);
            for (int b1 = 1; b1 <= basamak1; b1++)
            {
                int rakam1 = basamakbul(sayi1, b1);
                int carpim = rakam1 * rakam2;
                std::cout << rakam2 << " " << rakam1 << "->";
                std::cout << "|" << b1 + b2 - 2 << "," << b1 + b2 - 1 << "|\n";
                olay = std::to_string(rakam2) + " ile " + std::to_string(rakam1) + " carpiliyor = " + std::to_string(rakam1*rakam2) + " eklenecek, ";
                olay = olay + std::to_string(b1 + b2 - 1) + ". ile " + std::to_string(b1 + b2) + ". cubuklar kullanilacak.";
                cc(carpim, b1 + b2 - 2, b1 + b2 - 1);
            }
        }
        printf("Cevap: %d", sayi1 * sayi2);
    }
};

int main()
{
    soroban s;
    int x = true;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Soroban");

    sf::Font font;
    if (!font.loadFromFile("CenturyGothic.ttf")) {
        return -1; //font yuklenmedi
    }

    window.setFramerateLimit(15);
    window.setVerticalSyncEnabled(true);


    sf::RectangleShape cerceve;
    cerceve.setSize(Vector2f(cerceve_yatay, cerceve_dikey));
    cerceve.setPosition(cerceve_x, cerceve_y);
    cerceve.setOutlineThickness(20);
    cerceve.setOutlineColor(sf::Color(139, 69, 19));
    cerceve.setFillColor(sf::Color::Transparent);

    sf::RectangleShape cubuklar(sf::Vector2f(5, cerceve_dikey));
    cubuklar.setFillColor(sf::Color(212, 175, 55));

    sf::RectangleShape ayrac(sf::Vector2f(cerceve_yatay, 5));
    ayrac.setFillColor(sf::Color(212, 175, 55));

    ayrac.setPosition(cerceve_x, cerceve_y + cerceve_dikey * 32.5 / 100);

    TextBox textBox1(600, 520, 50, 20);
    TextBox textBox2(600, 550, 50, 20);

    sf::Text degerler[13];
    for (int i = 0; i < 13; i++)
    {
        degerler[i] = createText("0", 125 + cubuk_arasi * i, 30, 30, sf::Color::Black, font);
    }
    sf::Text butonyazi = createText("ileri", 690, 510, 20, sf::Color::Black, font);
    sf::Text butonyazi2 = createText("geri", 690, 550, 20, sf::Color::Black, font);

    sf::Text yazi1 = createText("", 70, 360, 20, sf::Color::Black, font);
    sf::Text yazi2 = createText("", 70, 390, 20, sf::Color::Black, font);

    sf::Text islem1 = createText("", 70, 460, 30, sf::Color::Black, font);
    sf::Text islem2 = createText("", 70, 490, 30, sf::Color::Black, font);

    for (int cubuk = 0; cubuk < 13; cubuk++)
    {
        //en ust boncuk ciziliyor
        boncuklar[cubuk][0].setRadius(15);
        boncuklar[cubuk][0].setFillColor(sf::Color(192, 192, 192));
        boncuklar[cubuk][0].setPosition(cerceve_x + cubuk_arasi * (cubuk + 1) - 15, cerceve_y);

        for (int a = 1; a < 5; a++)
        {
            //alt boncuklar
            boncuklar[cubuk][a].setRadius(15);
            boncuklar[cubuk][a].setFillColor(sf::Color(192, 192, 192));
            boncuklar[cubuk][a].setPosition(cerceve_x + cubuk_arasi * (cubuk + 1) - 15, cerceve_y + cerceve_dikey - 30 * a);
        }
    }
    sf::RectangleShape button[2];
    button[0].setSize(sf::Vector2f(100, 25));
    button[0].setFillColor(sf::Color::White);
    button[0].setPosition(680, 510);

    button[1].setSize(sf::Vector2f(100, 25));
    button[1].setFillColor(sf::Color::White);
    button[1].setPosition(680, 550);

    while (window.isOpen())
    {
        if (sayilargirildi == true)
        {
            s.carp(_sayi1, _sayi2);
            sayilargirildi = false;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            textBox1.handleEvent(event);
            textBox2.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (button[0].getGlobalBounds().contains(mousePos))
                {
                    if (!moveQueue.empty()) {
                        Move m = moveQueue.front();
                        moveQueue.erase(moveQueue.begin());
                        boncukdurumuguncelle(m.cubuk, m.boncuk, m.durum);
                        std::cout << "\n" << m.olay << "\n" << m.olay2;
                        yazi1.setString(m.olay);
                        yazi2.setString(m.olay2);
                        for (int i = 0; i < 13; i++)
                        {
                            int sayi = m.degerler[i];
                            degerler[12-i].setString(std::to_string(sayi));
                        }
                        lastMove = m;
                    }
                    else {
                        yazi1.setString("BITTI!");
                        yazi2.setString("");
                    }
                }
                if (button[1].getGlobalBounds().contains(mousePos))
                {
                    if (lastMove.cubuk != 0) {  // lastMove'un geçerli bir değer olup olmadığını kontrol et
                        moveQueue.push_back(lastMove);  // Son hareketi vektöre ekle (geri hamle)

                        if(!lastMove.durum)
                         boncukdurumuguncelle(lastMove.cubuk, lastMove.boncuk, 1);
                        else
                            boncukdurumuguncelle(lastMove.cubuk, lastMove.boncuk, 0);

                        std::cout << "\n" << lastMove.olay << "\n" << lastMove.olay2;

                        yazi1.setString(lastMove.olay);
                        yazi2.setString(lastMove.olay2);

                        for (int i = 0; i < 13; i++) {
                            int sayi = lastMove.degerler[i];
                            degerler[12 - i].setString(std::to_string(sayi));  // Değerleri geri al
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                system("cls");
                s.sifirla();
                for (int i = 0; i < 13; i++)
                {
                    degerler[12 - i].setString(std::to_string(0));
                }
                boncuklarisifirla();
                _sayi1 = textBox1.getValue();
                _sayi2 = textBox2.getValue();
                if (_sayi2 > _sayi1)
                {
                    int x = _sayi1;
                    _sayi1 = _sayi2;
                    _sayi2 = x;
                }
                islem1.setString(std::to_string(_sayi1));
                islem2.setString(std::to_string(_sayi2));
                sayilargirildi = true;
            }
        }

        window.clear(sf::Color(245, 222, 179));

        for (int i = 1; i < cubuksayisi + 1; ++i) {
            cubuklar.setPosition(cerceve_x + i * cubuk_arasi, cerceve_y);
            window.draw(cubuklar);
        }

        window.draw(cerceve);
        window.draw(ayrac);

        for (int i = 0; i < 13; i++)
            for (int a = 0; a < 5; a++)
                window.draw(boncuklar[i][a]);

        window.draw(button[0]);
        window.draw(button[1]);
        textBox1.draw(window);
        textBox2.draw(window);
        window.draw(yazi1);
        window.draw(yazi2);
        window.draw(islem1);
        window.draw(islem2);
        window.draw(butonyazi);
        window.draw(butonyazi2);

        for (int i = 0; i < 13; i++)
            window.draw(degerler[i]);

        window.display();
        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
