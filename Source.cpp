#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
int cerceve_yatay = 600;
int cerceve_dikey = 250;
int cerceve_x = 90;
int cerceve_y = 90;
int cubuksayisi = 13;
int cubuk_arasi = cerceve_yatay / (cubuksayisi + 1);
//boncuklarin kordinatlari
CircleShape boncuklar[13][5];

//****************************
struct Move {
    int cubuk;
    int boncuk;
    int durum;
};
std::vector<Move> moveQueue;
//****************************

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

class soroban {
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
        for (int i = 0; i < 5; i++)
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
        moveQueue.push_back({ cubuk, boncuk, durum }); //boncuklarin oynama durumunu kaydet.
    }

    void cc2(int carpim, int c1, int c2)
    {
        for (int cubuk = c1; cubuk <= c2; cubuk++)
        {
            int sayi = carpim % 10;
            if (sayi == 0)
                break;
            carpim = carpim / 10;

            if (9 - cubukdegeri(cubuk) < sayi) //bu sayi bu cubukta tasma yapiyor!
            {
                for (int t = 1; t < 13; t++) //en sagdaki cubuk 1 yapilir
                {
                    if (dizi[cubuk + 1][t] == 0)
                    {
                        //dizi[cubuk + 1][t] = 1;
                        recordMove(cubuk + 1, t, 1);
                        break;
                    }
                }
                sayi = 10 - sayi; //bu kadar azalticaz

                if (sayi < 5)
                {
                    int ind = 4; //azaltmaya en dıştan başlar
                    int t = 0;
                    while (t < sayi)
                    {
                        if (dizi[cubuk][ind]) {
                            t++;
                            recordMove(cubuk, ind, 0);
                        }
                        ind--;
                    }
                }
                else
                {
                    recordMove(cubuk, 0, 0);
                    int ind = 4;
                    int t = 0;
                    while (t < sayi - 5)
                    {
                        if (dizi[cubuk][ind]) {
                            t++;
                            recordMove(cubuk, ind, 0);
                        }
                        ind--;
                    }
                }
            }
            else
            {
                if (sayi < 5)
                {
                    if (sayi > 4 - alt_cubukdegeri(cubuk)) //eger sayi alt cubuklarda tasma yapiyorsa ustten borc al alttan dusur.
                    {
                        recordMove(cubuk, 0, 1);
                        sayi = 5 - sayi;
                        int ind = 4;
                        int t = 0;
                        while (t < sayi)
                        {
                            if ((dizi[cubuk][ind]) == 1) {
                                t++;
                                recordMove(cubuk, ind, 0);
                            }
                            ind--;
                        }
                    }
                    else {
                        int ind = 1;
                        int t = 0;
                        while (t < sayi)
                        {
                            if (!(dizi[cubuk][ind])) {
                                t++;
                                recordMove(cubuk, ind, 1);
                            }
                            ind++;
                        }
                    }
                }
                else
                {
                    recordMove(cubuk, 0, 1);
                    int ind = 1;
                    int t = 0;
                    while (t < sayi - 5)
                    {
                        if (!(dizi[cubuk][ind])) {
                            t++;
                            recordMove(cubuk, ind, 1);
                        }
                        ind++;
                    }
                }
            }
        }
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
                //bi soldaki cubuktan oynama yapilip, mevcut cubuk eksiltilir.
                for (int t = 1; t < 13; t++)
                {
                    if (dizi[cubuk + 1][t] == 0)
                    {
                        recordMove(cubuk + 1,t,1);
                        break;
                    }
                }
                sayi = 10 - sayi;

                if (sayi < 5)
                {
                    int ind = 4; //azaltmaya en dıştan başlar
                    int t = 0;
                    while (t < sayi)
                    {
                        if (dizi[cubuk][ind]) {
                            t++;
                            recordMove(cubuk, ind, 0);
                        }
                        ind--;
                    }
                }
                else
                {
                    recordMove(cubuk, 0, 0);
                    int ind = 4;
                    int t = 0;
                    while (t < sayi - 5)
                    {
                        if (dizi[cubuk][ind]) {
                            t++;
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
                        recordMove(cubuk, 0, 1);
                        sayi = 5 - sayi;
                        int ind = 4;
                        int t = 0;
                        while (t < sayi)
                        {
                            if ((dizi[cubuk][ind]) == 1) {
                                t++;
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
                                recordMove(cubuk, ind, 1);
                            }
                            ind++;
                        }
                    }
                }
                else
                {
                    recordMove(cubuk, 0, 1);
                    int ind = 1;
                    int t = 0;
                    while (t < sayi - 5)
                    {
                        if (!(dizi[cubuk][ind])) {
                            t++;
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
        if (sayi2 > sayi1)
        {
            int x = sayi1;
            sayi1 = sayi2;
            sayi2 = x;
        }

        int basamak1 = bassayisi(sayi1); //basamagi uzun olan
        int basamak2 = bassayisi(sayi2);
        std::cout << sayi1 << " ile " << sayi2 << " carpiliyor: " << sayi1*sayi2<<" \n";

        for (int b2 = 1; b2 <= basamak2; b2++)
        {
            int rakam2 = basamakbul(sayi2, b2);
            for (int b1 = 1; b1 <= basamak1; b1++)
            {
                int rakam1 = basamakbul(sayi1, b1);
                int carpim = rakam1 * rakam2;
                std::cout << rakam2 << " " << rakam1 << "->";
                std::cout << "|" << b1 + b2 - 2 << "," << b1 + b2 - 1 << "|\n";
                cc(carpim, b1 + b2 - 2, b1 + b2 - 1);
            }
        }
        printf("Cevap: %d", sayi1 * sayi2);
    }
};

void processNextMove() { //kaydedilen durumlara göre boncukların durumunu günceller.
    if (!moveQueue.empty()) {
        Move m = moveQueue.front();
        moveQueue.erase(moveQueue.begin());
        boncukdurumuguncelle(m.cubuk, m.boncuk, m.durum);
    }
}

int main()
{
    soroban s;
    int x = true;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Soroban");
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
    sf::RectangleShape button;
    button.setSize(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(680, 520);

    s.carp(123, 41);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (button.getGlobalBounds().contains(mousePos))
                {
                    processNextMove();
                }
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

        window.draw(button);

        window.display();
        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
