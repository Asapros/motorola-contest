RAYDER
======

Rayder jest grą wyścigową napisaną w C++ bez silnika na potrzeby Motorola Science Cup. Do rysowania grafiki wspomagamy się biblioteką `raylib`.

Architektura
------------

Przed rozpoczęciem zostały rozpatrzone dla projektu różne języki (np. Rust) i systemy zarządzania grą (np. ECS), lecz demokratycznie zadecydowaliśmy programować w C++ obiektowo.
Ze względu na różnice środowisk deweloperów do konfiguracji projektu zastosowany został CMake, dzięki któremu na wielu platformach proces kompilacji wygląda indentycznie i produkuje podobny rezultat.
Jako system kontroli wersji został wybrany `git` ze względu na największą popularność. Do kolaboracji na początku używaliśmy wtyczki `Live Share` edytora Visual Studio Code, lecz wygodniejszym przepływem pracy okazał się dla nas rozwój odmiennych komponentów aplikacji indywidualnie na gałęziach kontroli wersji.

Prace
-----

1. Wczytywanie modeli

Pierwszymi napotkanymi przez nas problemami okazało się wczytywanie modelu do gry. Po dosyć długim czasie debugowania okazało się, że CMake uruchamia skompilowaną grę w katalogu, który nie miał dostępu do folderu z zasobami. Niestety `raylib` nie daje zbyt treściwych ostrzeżeń o takiego rodzaju problemach i zwyczajnie nie pokazuje modelu. Rozwiązaniem okazało się kopiowanie przez CMake katalogu `assets` do wnętrza zbudowanej aplikacji. Głębszym rozszerzeniem tej funkcjonalności jest sprawdzanie też ścieżki wyższego poziomu w poszukiwaniu modeli, gdyż niektóre narzędzia umieszczają plik wykonywalny jeszcze głębiej, w folderze zależnym od profilu budowy np. Debug albo Release.

2. Architektura gry

Podstawowym obiektem w grze jest `Entity`. Jest to zwyczajnie model 3D o ustalonej i zmienialnej pozycji i orientacji. Wirtualna funkcja `Entity::update` jest implementowana dla konkretnego przypadku i jest wywoływana co klatkę w celu aktualizacji jego stanu. Obiekt `World` jest kontenerem trzymającym wszystkie instancje `Entity` z przypisanym do nich unikatowym identyfikatorem. Za jego pomocą można otrzymać wskaźnik do danej instancji. Wywołanie `update` świata jest równoznaczne z aktualizacją każdego z należących do niego obiektów. Klasa `Entity` rozszerzana jest o dodatkowe funkcjonalności w następujący sposób:

    1. Entity -> Collidable: wykrywa kolizje (na płaszczyźnie dwuwymiarowej)
    2. Collidable -> Rigidbody: implementuje fizykę (np. zachowanie po przyłożeniu siły lub momentu siły)
    3. Rigidbody -> Vehicle: najbardziej specyficzny dla gry, oblicza bardziej skomplikowaną fizykę pojazdu

Całością zarządza jeden obiekt `Game` który jest w stanie wczytywać konkretny level z pliku i dodatkowo łączy funkcjonalność świata gry z interfejsem użytkownika (takim jak prędkościomierz).

3. Implementacja dla języka C++

Ważnym aspektem zarządzania takim pogmatwanym zbiorem instancji jest zapobieganie wyciekowi pamięci. Staraliśmy się używać sprytnych wskaźników różnego rodzaju. W potrzebnych miejscach zaimplementowano odpowiednie destruktory. Najlepszym przykładem takiego bezpiecznego zachowania jest `ModelManager`, który pilnuje ładowania i zwalniania modeli, jednocześnie nie tworząc duplikatów i zapewniając, że używane modele cały czas pozostają dostępne. Ogólnie rzecz biorąc sprawa zarządzania pamięcią z niskiego poziomu często stanowiła dla nas wyzwanie ze względu na większe doświadczenie w wysokopoziomowych językach, lecz wszystkie problemy tego typu o których wiemy zostały rozwiązane, wynagradzając nas doświadczeniem.

4. Sterowanie

Samochód posiada obiekt klasy `Controller`, która zajmuje się jego prowadzeniem. W zalezności od implementacji może być to `PlayerController`, który robi to na podstawie wciśniętych klawiszy, lub inna, sztuczna implementacja jak `StraightGuy`, który na podstawie danych o świecie wyznacza swoją trasę po prostych liniach na torze. Z łatwością dodawać można więcej implementacji dla innych typów przeciwników z innym zachowaniem, lecz zabrakło nam czasu.

5. Fizyka samochodu

Bez wątpienia najbardziej rozbudowaną funkcjonalnością gry jest zachowanie samochodów. Stawialiśmy na realistyczny ruch. Każde koło zachowuje się jako osobny obiekt ze swoją prędkością kątową i orientacją. Silnik na podstawie funkcji *sigmoid* (z pomysłu własnego) przekłada konkretne biegi na moc przyłożoną do kół. Parametry ruchu całego samochodu zależą od tego co się dzieje z kołami. Tarcie wpływające na ruch jest zależne od powierzchni na której znajdują się koła. W ten sposób najwierniej jak mogliśmy symulujemy zachowanie pojazdu.

6. Interfejs użytkownika

Do stworzenia menu przydatna jest zewnętrzna biblioteka `raygui`, która rozszerza funkcjonalność `raylib`a o wszelkiego rodzaju przyciski. Podczas gry, `UiManager` na podstawie danych o świecie i identyfikatora sterowanego pojazdu wyświetla adekwantną wartość na liczniku prędkości i obrotów. Silnik nie jest symulowany z taką dokładnością, że sam w sobie posiada obroty, lecz są liczone sztucznie na podstawie biegu i prędkości.

7. Tory wyścigowe

Prace w kierunku implementacji rzeczywistego wyścigu nie zaszły zbyt daleko, lecz zachowanie AI i checkpointów (obszarów, które trzeba przekroczyć, by zaliczyć okrążenie na torze) jest w plikach gry w początkowej fazie rozwoju.

8. Debugowanie

Do debugowania wykorzystane przede wszystkim są funckja `debugLog(category, text)` i mapa `debugValues`. Log pozwala na wypisywanie wartości w konsoli, jednocześnie zapewniając możliwość filtrowania kategorii logów wybierając w `debug.hpp` co powinno zostac wyświetlone. Za to mapa z wartościami jest dostępna pod klawiszem `F3` i pozwala na monitorowanie zmieniających się wartości takich jak parametry ruchu gracza. Zaczęto robić testy odizolowanych funkcjonalności, lecz zabrakło na to czasu.

9. Czego się nauczyliśmy?

Ten projekt był przede wszystkim okazją do nauki kolaboracji w obrębie jednego repozytorium. Potrzebne było czasami używanie niecodziennych poleceń `git`a lub rozwiązywanie konfliktów połączenia gałęzi, co nie jest spotykane w jednoosobowych projektach. Nauczyliśmy się także korzystania z narzędzia CMake w szerokim zakresie, w celu zautomatyzowania pobierania potrzebnych bibliotek czy kopiowania zasobów do katalogu `build`. Poza aspektem programistycznym warto wspomnieć o zmaganiach nad modelami, dźwiękami, czy shaderami, którym niestety nie udało się ujrzeć światła dziennego ze względu na ograniczenie czasowe.

Kompilacja
-------

Do kompilacji naszego projektu wykorzystaliśmy wcześniej wspomniane narzędzie `CMake`, które pozwoliło nam ujednolicić sposób kompilacji na różnych konfiguracjach systemowych.
- Po sklonowaniu repozytorium, utwórz katalog `build` wykorzystując komendę `mkdir build`
- Przejdź do utworzonego folderu za pomocą `cd build`
- Następnie należy wygenerować konfigurację CMake'a wykorzystując `cmake ..`
- Kolejnym krokiem jest sama kompilacja gry za pomocą komendy `cmake --build .`
- Ostatnim krokiem jest samo uruchomienie gry (znajdującej się zazwyczaj w katalogu `build/Debug` w zależności od konfiguracji), a następnie już sama gra

Obsługa
-------

Po wejściu do gry widoczne jest menu. Sekcja ustawień zawiera przełącznik dodający obsługę trybu pełnoekranowego, dla lepszych wrażeń z gry. `Play` pozwala wejść do gry. Na planszy znajduje się samochód gracza i inne obiekty, lecz kolizja z nimi nie powoduje działania siły reakcji (nie zaimplementowano). Na podłożu znajduje się mapa, która jest podobnie jak położenia wcześniej wspomnianych modeli wczytywana z pliku tekstowego `map_test_...`. W celu sterowania należy użyć klawiszy `A` i `D`: skręcanie w lewo i w prawo, oraz `W` i `S`: jazda w przód i w tył. `Q` i `E` zmienia bieg, a z nim parametry takie jak siła przyspieszająca pojazd, czy maksymalna prędkość. Bieg oraz uzyskana prędkość widoczne są na prędkościomierzu w prawym dolnym rogu ekranu. W zależności od nawierzchni na zmienia się tarcie i pojazd będzie zachowywał się w inny sposób.