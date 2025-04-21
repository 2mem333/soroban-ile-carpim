# Karadeniz Teknik Üniversitesi, Mühendislik Fakültesi  
## Bilgisayar Mühendisliği Bölümü  
### Algoritmalar Dersi Ödev Raporu  

---

## **Soroban Çarpma Algoritması Simülasyonu**  
**Hazırlayan:** Melih Efe Sevinç (434330)  
**Sorumlu Öğretmen:** Prof. Dr. Vasıf NABİYEV  

---

### Ödev İçeriği  
Girilen iki doğal sayının, Soroban abaküsü üzerinde çarpma işleminin adım adım simülasyonunu gerçekleştiren bir program geliştirilmiştir. Kullanıcı, grafiksel arayüz aracılığıyla işlem adımlarını takip edebilir.

---

### Soroban Nedir?  
Soroban, Japonya'da kullanılan bir abaküs türüdür. Yapısı:  
- Her dikey çubuk bir basamağı temsil eder.  
- **1 üst boncuk** (5 değerinde) ve **4 alt boncuk** (1'er değerinde) bulunur.  
- Çubuklar soldan sağa birler, onlar, yüzler... basamaklarını temsil eder.  

---

### Soroban Çarpma Algoritması  
Çarpma işlemi, basamaklara ayrılarak gerçekleştirilir:  
1. Her basamak ayrı ayrı çarpılır (örneğin, `23×5 = 20×5 + 3×5`).  
2. Ara sonuçlar abaküs üzerinde toplanır.  
3. Taşıma işlemleri için **5 ve 10'un tamamlayıcıları** kullanılır.  

---
![Screenshot 2025-04-09 234623](https://github.com/user-attachments/assets/de14f2ec-ae53-4773-86f9-92de9ccf0571)

### Program Algoritması  
#### İlk Fonksiyon (Basamak İşleme)  
1. `A` basamaklı (büyük sayı) ve `B` basamaklı (küçük sayı) için `n = A`, `m = B` başlat.  
2. `n.` ve `m.` basamakları çarp.  
3. `C = (A+B) - (n+m)` formülü ile hedef çubukları belirle.  
4. Çarpım sonucunu `capma_algoritmasi(C, C-1, çarpım)` fonksiyonuna gönder.  
5. `n` ve `m` değerlerini iteratif olarak güncelle.  

#### İkinci Fonksiyon (Çarpma Algoritması - Pseudocode)  
```python  
def carpma_algoritmasi(carpim, c1, c2):  
    for cubuk in range(c1, c2+1):  
        sayi = carpim % 10  
        carpim = carpim // 10  

        if sayi == 0:  
            continue  

        mevcut_deger = cubuk_degeri(cubuk)  
        if (9 - mevcut_deger) < sayi:  
            # Boncuk yetersiz, taşıma yap  
            cubuck_borc = cubuk + 1  
            if alt_cubuk_degeri(cubuck_borc) == 4:  
                # Üst boncuktan taşıma  
                ust_boncuk_artir(cubuck_borc)  
                alt_boncuklari_sifirla(cubuck_borc)  
            else:  
                alt_boncuk_artir(cubuck_borc, 1)  

            sayi = 10 - sayi  
            if sayi < 5:  
                if sayi > alt_cubuk_degeri(cubuk):  
                    ust_boncuk_azalt(cubuk)  
                    sayi = 5 - sayi  
                    alt_boncuk_artir(cubuk, sayi)  
            else:  
                alt_boncuk_azalt(cubuk, sayi)  
        else:  
            if sayi < 5:  
                if sayi > (4 - alt_cubuk_degeri(cubuk)):  
                    ust_boncuk_artir(cubuk)  
                    sayi = 5 - sayi  
                    alt_boncuk_azalt(cubuk, sayi)  
                else:  
                    alt_boncuk_artir(cubuk, sayi)  
            else:  
                ust_boncuk_artir(cubuk)  
                alt_boncuk_artir(cubuk, 5 - sayi)  
