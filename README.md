# Grafika Komputerowa i Wizualizacja

Grafika Komputerowa i Wizualizacja 2023/24

Temat projektu: 22. Symulacja rosnącego drzewa

Sterowanie:
===========
W - przód
A - lewo
D - prawo
S - do tyłu
Q - kamera w lewo
E - kamera w prawo
myszka - sterowanie kamerą
G - otwarcie bramy
H - zamknięcie bramy


Źródła:
======
//https://www.urbangreenup.eu/imgpub/1883284/900/0/fences.webp
//https://www.freepik.com/free-photo/background-made-from-bricks_10980125.htm
//https://www.freepik.com/free-photo/top-view-metal-surface-with-pattern_11684381.htm
learnopengl.com


Proces rysowania
================
1. Inicjalizacja i konfiguracja OpenGL: Tworzenie okna, ustawianie kontekstu, włączanie testu głębokości.
2. Ładowanie shaderów: Kompilacja i linkowanie shaderów, ustawienie programów shaderów do użytku.
3. Tworzenie i konfigurowanie buforów wierzchołków: Generowanie VAO, VBO, EBO, ustawienie wskaźników atrybutów wierzchołków.
4. Ładowanie tekstur: Wczytywanie obrazów, tworzenie tekstur OpenGL, ustawienie trybów owijania i filtrowania.
5. Konfiguracja świateł: Inicjalizacja obiektów świateł, przekazanie parametrów do shaderów.
6. Przetwarzanie wejść użytkownika: Obsługa klawiatury i myszy, aktualizacja stanu kamery.
7. Pętla renderująca: Czyszczenie buforów, ustawienie macierzy projekcji i widoku, renderowanie obiektów.
8. Rysowanie obiektów w scenie: Ustawienie macierzy modelu, widoku i projekcji, rysowanie sześcianów, płotu i bramy.
9. Animacja bramy: Aktualizacja kąta obrotu bramy na podstawie stanu gateOpen.
10. Czyszczenie zasobów: Usuwanie VAO, VBO, EBO, kończenie działania GLFW.



main.cpp - główny plik projektu
===============================
SCR_WIDTH - Szerokość okna aplikacji w pikselach.
SCR_HEIGHT - Wysokość okna aplikacji w pikselach.

Rotacja:
rotateAngle_X, rotateAngle_Y, rotateAngle_Z - Kąty rotacji wokół odpowiednich osi.
rotateAxis_X, rotateAxis_Y, rotateAxis_Z - Wektory kierunkowe osi rotacji.

Translacja:
translate_X, translate_Y, translate_Z - Przesunięcie modelu w przestrzeni 3D wzdłuż odpowiednich osi.

Skalowanie:
scale_X, scale_Y, scale_Z - Współczynniki skalowania wzdłuż odpowiednich osi.

Operacje są reprezentowane przez macierze 4x4 i są stosowane do macierzy modelu, aby przekształcić położenie, orientację i rozmiar obiektu w scenie.

Ustawienia kamery:

camera_z - Pozycja kamery na osi Z.
camera - Inicjalizacja obiektu klasy Camera z pozycją początkową (0.0f, 3.0f, -25.0f). Kamera jest odpowiedzialna za przekształcanie przestrzeni widoku.
lastX, lastY - Przechowują ostatnie położenie kursora myszy w celu obliczenia przesunięcia.
firstMouse - Flaga używana do zainicjowania pierwszego położenia kursora myszy.

Ustawienia światła:

glm::vec3 pointLightPositions[]:

Tablica wektorów 3D reprezentujących pozycje punktowych źródeł światła.

PointLight:

Każdy obiekt PointLight jest inicjalizowany z określoną pozycją, kolorem i parametrami zaniku.

Ustawienia czasu:

deltaTime - Różnica czasu pomiędzy aktualną a poprzednią klatką. Używane do płynnego animowania obiektów niezależnie od liczby klatek na sekundę.
lastFrame - Czas ostatniej klatki, służy do obliczenia deltaTime.


int main():

1. Inicjalizacja GLFW:

glfwInit() - Inicjalizuje bibliotekę GLFW.
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3) i glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3) - Ustawia wersję OpenGL na 3.3.
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE) - Ustawia profil OpenGL na rdzeń.
Specjalna konfiguracja dla systemu macOS:
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) - Ustawia kompatybilność wsteczną OpenGL dla systemu macOS.

2. Tworzenie okna:

GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL) - Tworzy okno o rozmiarach SCR_WIDTH x SCR_HEIGHT z tytułem "CSE 4208: Computer Graphics Laboratory".
Sprawdzenie poprawności tworzenia okna:
if (window == NULL) { ... } - Wyświetla komunikat o błędzie i kończy program, jeśli tworzenie okna się nie powiodło.

3. Konfiguracja kontekstu:

glfwMakeContextCurrent(window) - Ustawia kontekst okna jako aktywny.
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback) - Ustawia funkcję callback dla zmiany rozmiaru okna.
glfwSetKeyCallback(window, key_callback) - Ustawia funkcję callback dla obsługi klawiatury.
glfwSetCursorPosCallback(window, mouse_callback) - Ustawia funkcję callback dla obsługi ruchu myszy.
glfwSetScrollCallback(window, scroll_callback) - Ustawia funkcję callback dla obsługi przewijania myszy.
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN) - Ukrywa kursor.

4. Inicjalizacja GLAD:

if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { ... } - Ładuje wskaźniki funkcji OpenGL. Wyświetla komunikat o błędzie i kończy program, jeśli inicjalizacja się nie powiodła.
Włączenie testu głębokości:

glEnable(GL_DEPTH_TEST) - Włącza testowanie głębokości, co zapewnia poprawne renderowanie obiektów 3D w zależności od ich odległości od kamery.

5. Inicjalizacja shaderów:

Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs") - Inicjalizuje shader z plików shaderów dla Phong Shading z teksturą.

6. Inicjalizacja wierzchołków i indeksów sześcianu:

Definicja tablic wierzchołków i indeksów:
float cube_vertices[] = { ... } - Definiuje wierzchołki sześcianu z normalnymi.
unsigned int cube_indices[] = { ... } - Definiuje indeksy wierzchołków sześcianu.
Generowanie i wiązanie buforów:
glGenVertexArrays(1, &cubeVAO), glGenBuffers(1, &cubeVBO), glGenBuffers(1, &cubeEBO) - Generuje bufor VAO, VBO i EBO.
glBindVertexArray(cubeVAO), glBindBuffer(GL_ARRAY_BUFFER, cubeVBO), glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO) - Wiąże bufory VAO, VBO i EBO.
Wypełnianie buforów danymi:
glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW) - Wypełnia bufor wierzchołków.
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW) - Wypełnia bufor indeksów.
Ustawianie atrybutów wierzchołków:
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0) - Ustawia atrybut pozycji wierzchołka.
glEnableVertexAttribArray(0) - Włącza atrybut pozycji wierzchołka.
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12) - Ustawia atrybut normalnej wierzchołka.
glEnableVertexAttribArray(1) - Włącza atrybut normalnej wierzchołka.

7. Inicjalizacja tekstur:

Definicje ścieżek do plików tekstur:
string sandPath = "field.jpg", string brickPath = "brick.jpg", string gatePath = "gate.jpg" - Definiuje ścieżki do plików tekstur.
Ładowanie tekstur:
unsigned int sandMap = loadTexture(sandPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), unsigned int brickMap = loadTexture(brickPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), unsigned int gateMap = loadTexture(gatePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR) - Ładuje tekstury z plików.
Tworzenie obiektów Cube z teksturami:
Cube sand = Cube(sandMap, sandMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f), Cube brick = Cube(brickMap, brickMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f), Cube gate = Cube(gateMap, gateMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f) - Inicjalizuje obiekty Cube z odpowiednimi teksturami.

8. Inicjalizacja modeli 3D:

stbi_set_flip_vertically_on_load(true) - Ustawia flaga, aby obrazy były ładowane do góry nogami (wymagane przez niektóre formaty obrazów).
Shader modelShader("1.model_loading.vs", "1.model_loading.fs") - Inicjalizuje shader do ładowania modeli.
Model tree1("pine_tree/scene.gltf"), Model tree2("pine_tree2/scene.gltf"), Model tree3("pine_tree3/scene.gltf") - Ładuje modele drzew z plików GLTF.

9. Generator liczb losowych:

std::random_device rd - Inicjalizuje urządzenie do generowania losowych liczb.
std::mt19937 gen(rd()) - Inicjalizuje generator liczb losowych.
std::uniform_real_distribution<float> tree1_lim(0.005, 0.04) - Definiuje rozkład losowy dla skali drzewa 1.
std::uniform_real_distribution<float> tree2_lim(1.5, 2.0) - Definiuje rozkład losowy dla skali drzewa 2.
std::uniform_int_distribution<int> lev_lim(1, 2) - Definiuje rozkład losowy dla poziomów.

10. Pętla renderująca:

Aktualizacja czasu:
float currentFrame = static_cast<float>(glfwGetTime()) - Pobiera aktualny czas.
deltaTime = currentFrame - lastFrame - Oblicza różnicę czasu między klatkami.
lastFrame = currentFrame - Ustawia czas ostatniej klatki na aktualny czas.
Przetwarzanie wejść użytkownika:
processInput(window) - Wywołuje funkcję przetwarzającą wejścia użytkownika.
Czyszczenie bufora koloru i głębokości:
glClearColor(0.1f, 0.1f, 0.1f, 1.0f) - Ustawia kolor czyszczenia bufora.
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) - Czyści bufor koloru i głębokości.
Ustawienie macierzy projekcji i widoku:
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f) - Tworzy macierz projekcji perspektywicznej.
glm::mat4 view = camera.GetViewMatrix() - Pobiera macierz widoku z kamery.
Obliczanie i ustawianie macierzy modelu:
glm::mat4 identityMatrix = glm::mat4(1.0f) - Tworzy macierz jednostkową.
glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model - Definiuje macierze transformacji.
translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z)) - Tworzy macierz translacji.
rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f)) - Tworzy macierz rotacji wokół osi X.
rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f)) - Tworzy macierz rotacji wokół osi Y.
rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f)) - Tworzy macierz rotacji wokół osi Z.
scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z)) - Tworzy macierz skalowania.
model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix - Kombinuje wszystkie transformacje w jedną macierz modelu.
Rysowanie modeli drzew:
modelShader.use() - Używa shadera do ładowania modeli.
modelShader.setMat4("projection", Mprojection) - Ustawia macierz projekcji w shaderze.
modelShader.setMat4("view", Mview) - Ustawia macierz widoku w shaderze.
Losowe rysowanie drzew:
Jeśli poziom lev wynosi 1:
Inkrementacja skali drzewa 1:
if (s1 < 0.04) s1 += 0.000001
translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0f, 0.0f))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(s1, s1, s1))
glm::mat4 modelForTree = translateMatrix * scaleMatrix
modelShader.setMat4("model", modelForTree)
tree1.Draw(modelShader)
Jeśli poziom lev wynosi 2:
Inkrementacja skali drzewa 2:
if (s2 < 2.0) s2 += 0.000001
translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0f, -.8f))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(s2, s2, s2))
rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0))
glm::mat4 modelForTree = translateMatrix * rotateXMatrix * scaleMatrix
modelShader.setMat4("model", modelForTree)
tree3.Draw(modelShader)
Rysowanie sześcianów z teksturami:
Ustawienia shaderów:
lightingShaderWithTexture.use()
lightingShaderWithTexture.setVec3("viewPos", camera.Position)
lightingShaderWithTexture.setMat4("projection", projection)
lightingShaderWithTexture.setMat4("view", view)
Ustawienia świateł punktowych:
pointlight1.setUpPointLight(lightingShaderWithTexture)
pointlight2.setUpPointLight(lightingShaderWithTexture)
pointlight3.setUpPointLight(lightingShaderWithTexture)
pointlight4.setUpPointLight(lightingShaderWithTexture)
pointlight5.setUpPointLight(lightingShaderWithTexture)

Rysowanie podstawy:
translateMatrix = glm::translate(identityMatrix, glm::vec3(-40.0, 0.0, -20.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(80.0f, 0.1f, 80.0))
glm::mat4 modelForBase = translateMatrix * scaleMatrix
sand.drawCubeWithTexture(lightingShaderWithTexture, modelForBase)
Rysowanie płotu wokół drzewa:
Pierwszy bok płotu:
translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 3.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(8.0, 3.0, 0.5))
glm::mat4 modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Drugi bok płotu:
translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 3.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(-8.0, 3.0, 0.5))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Trzeci bok płotu:
translateMatrix = glm::translate(identityMatrix, glm::vec3(-8.0, 0.0, 3.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5, 3.0, -5.0))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Czwarty bok płotu:
translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0, 0.0, 3.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5, 3.0, -5.0))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Rysowanie ścian bramy:
Pierwsza część ściany:
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5, 0.0, -15.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.5, 5.0, 0.5))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Druga część ściany:
translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0, 0.0, -15.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(7.5, 5.0, 0.5))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Trzecia część ściany:
translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 0.0, -15.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(-7.5, 5.0, 0.5))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Czwarta część ściany:
translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.0, 0.0, -15.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(-7.5, 5.0, 0.5))
modelForBrick = translateMatrix * scaleMatrix
brick.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Rysowanie bramy:
Pierwsze skrzydło bramy:
rotateYMatrix = glm::rotate(identityMatrix, glm::radians(gate_ang), glm::vec3(0.0, 1.0, 0.0))
translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5, 0.0, -15.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(-2.5, 4.8, 0.5))
modelForBrick = translateMatrix * rotateYMatrix * scaleMatrix
gate.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Drugie skrzydło bramy:
rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-gate_ang), glm::vec3(0.0, 1.0, 0.0))
translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 0.0, -15.0))
scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 4.8, 0.5))
modelForBrick = translateMatrix * rotateYMatrix * scaleMatrix
gate.drawCubeWithTexture(lightingShaderWithTexture, modelForBrick)
Animacja bramy:
Otwieranie bramy:
if (gateOpen) { if (gate_ang < 80.0f) gate_ang += 1.0f; }
Zamykanie bramy:
else { if (gate_ang > 0.0f) gate_ang -= 1.0f; }
Aktualizacja buforów:
glfwSwapBuffers(window) - Wymienia bufory, aby zaktualizować zawartość okna.
glfwPollEvents() - Przetwarza zdarzenia.

12. Czyszczenie zasobów:

glDeleteVertexArrays(1, &cubeVAO) - Usuwa VAO sześcianu.
glDeleteVertexArrays(1, &lightCubeVAO) - Usuwa VAO światła.
glDeleteBuffers(1, &cubeVBO) - Usuwa VBO sześcianu.
glDeleteBuffers(1, &cubeEBO) - Usuwa EBO sześcianu.
glfwTerminate() - Kończy działanie GLFW i zwalnia zasoby.

Funkcja główna w skrócie:

1. Inicjalizacja bibliotek i tworzenie okna:

GLFW i GLAD są inicjalizowane.
Tworzone jest okno GLFW z kontekstem OpenGL.

2. Konfiguracja shaderów i buforów:

Shader Phong Shading z teksturą jest ładowany.
Bufory wierzchołków, indeksów i VAO są generowane i konfigurowane.

3. Ładowanie tekstur i modeli:

Ładowane są tekstury piasku, cegieł i bramy.
Modele drzew są ładowane za pomocą klasy Model.

4. Konfiguracja świateł:

Światła punktowe są konfigurowane i ustawiane w shaderze.

5. Pętla renderująca:

Przetwarzane są wejścia użytkownika.
Scena jest czyszczona i renderowana na nowo.
Ustawiane są macierze projekcji, widoku i modelu.
Modele drzew są renderowane w zależności od losowego poziomu i skali.
Sześciany z teksturami są renderowane, w tym podstawa, płot i brama.
Brama jest animowana, otwierana i zamykana na podstawie wejść użytkownika.

6. Czyszczenie zasobów:

Bufory i VAO są usuwane.
GLFW jest kończone, zwalniając wszystkie zasoby.

Funkcje rysujące:

Funkcja rysowania sześcianu - drawCube:

lightingShader.use(): Aktywuje shader do rysowania.
lightingShader.setVec3("material.ambient", glm::vec3(r, g, b)): Ustawia właściwości materiału (ambient, diffuse, specular) na odpowiednie wartości koloru.
lightingShader.setFloat("material.shininess", 22.0f): Ustawia połysk materiału.
lightingShader.setMat4("model", model): Ustawia macierz modelu.
glBindVertexArray(cubeVAO): Wiąże VAO sześcianu.
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0): Rysuje sześcian za pomocą indeksów, tworząc 12 trójkątów (6 ścian sześcianu).

Funkcja rysowania podstawy - func:

glm::mat4 translate = glm::mat4(1.0f): Tworzy macierz translacji.
glm::mat4 scale = glm::scale(model, glm::vec3(width, baseHeight, length)): Tworzy macierz skalowania.
translate = glm::translate(model, glm::vec3(-40.5, 0, -5.5)): Ustawia translację podstawy.
model = translate * scale: Kombinuje translację i skalowanie w jedną macierz modelu.
base.drawCubeWithTexture(lightingShader, model): Rysuje podstawę sześcianu z teksturą za pomocą określonej macierzy modelu.




Przetwarzanie wejść:

processInput:
Obsługuje wejścia z klawiatury.
Aktualizuje stan kamery na podstawie wciśniętych klawiszy (A, D, S, W, Q, E).
Obsługuje otwieranie i zamykanie bramy (klawisze G i H).

Callbacki GLFW:

framebuffer_size_callback:

Ustawia viewport na nowe wymiary okna, aby dostosować renderowanie do nowego rozmiaru.

mouse_callback:

Przetwarza ruchy myszy.
Oblicza przesunięcie kursora myszy i aktualizuje kamerę.

scroll_callback:

Przetwarza przewijanie myszy.
Aktualizuje zoom kamery.

Ładowanie tekstur- loadTexture():

Generowanie identyfikatora tekstury:

glGenTextures(1, &textureID) - Generuje unikalny identyfikator tekstury.
Ładowanie obrazu:

stbi_set_flip_vertically_on_load(true) - Ustawia flagę, aby obrazy były ładowane do góry nogami.
unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0) - Ładuje obraz z pliku i zwraca wskaźnik do danych pikseli oraz wymiary obrazu i liczbę komponentów.
Sprawdzanie poprawności załadowania obrazu:

if (data) - Sprawdza, czy dane obrazu zostały poprawnie załadowane.
Ustalanie formatu tekstury:
GLenum format; if (nrComponents == 1) format = GL_RED; else if (nrComponents == 3) format = GL_RGB; else if (nrComponents == 4) format = GL_RGBA; - Ustala format obrazu na podstawie liczby komponentów.
Konfigurowanie tekstury:
glBindTexture(GL_TEXTURE_2D, textureID) - Wiąże teksturę 2D z generowanym identyfikatorem.
glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data) - Tworzy teksturę 2D i wczytuje dane obrazu.
glGenerateMipmap(GL_TEXTURE_2D) - Generuje mipmapy dla tekstury.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS) - Ustawia tryb owijania tekstury dla osi S.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT) - Ustawia tryb owijania tekstury dla osi T.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin) - Ustawia tryb filtrowania tekstury przy zmniejszaniu.
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax) - Ustawia tryb filtrowania tekstury przy powiększaniu.
stbi_image_free(data) - Zwalnia pamięć zajmowaną przez dane obrazu.
Obsługa błędu ładowania obrazu:
else { std::cout << "Texture failed to load at path: " << path << std::endl; stbi_image_free(data); } - Wyświetla komunikat o błędzie i zwalnia pamięć w przypadku nieudanego ładowania tekstury.
Zwracanie identyfikatora tekstury:

return textureID - Zwraca identyfikator tekstury.

fragmentShaderForPhongShadingWithTexture:
========================================
Material:

diffuse (sampler2D): Tekstura zawierająca kolory rozproszone materiału.
specular (sampler2D): Tekstura zawierająca kolory odbite (specular) materiału.
shininess (float): Współczynnik połysku materiału, wpływający na rozproszenie światła odbitego.
PointLight:

position (vec3): Pozycja punktowego źródła światła.
k_c, k_l, k_q (float): Współczynniki tłumienia światła (konstanty, liniowy, kwadratowy).
ambient (vec3): Składowa ambient (otoczenia) światła.
diffuse (vec3): Składowa rozproszona światła.
specular (vec3): Składowa odbita światła.
Wejścia i Uniformy
FragPos (vec3): Pozycja fragmentu w przestrzeni świata.
Normal (vec3): Normalna do powierzchni fragmentu.
TexCoords (vec2): Współrzędne tekstury dla fragmentu.
viewPos (vec3): Pozycja kamery/oka w przestrzeni świata.
pointLights (tablica PointLight): Tablica punktowych źródeł światła.
material (Material): Struktura materiału powierzchni.
Funkcje
main:

Normalizuje wektor normalny N i wektor widzenia V.
Inicjalizuje wektor result do sumowania wpływów wszystkich źródeł światła.
Iteruje przez wszystkie punktowe źródła światła (pointLights), obliczając ich wpływ na kolor fragmentu za pomocą funkcji CalcPointLight.
Ustawia końcowy kolor fragmentu w FragColor.
CalcPointLight:

Oblicza wektor L od fragmentu do źródła światła.
Oblicza wektor odbicia R światła od powierzchni.
Oblicza tłumienie światła na podstawie odległości d od źródła światła.
Oblicza składowe ambient, diffuse i specular używając tekstur materiału oraz odpowiednich składowych źródła światła.
Zastosowuje tłumienie do każdej składowej światła.
Zwraca sumę składowych ambient, diffuse i specular jako końcowy kolor fragmentu.
Obliczenia oświetlenia
Ambient: Oświetlenie otoczenia, które nie zależy od kierunku światła.
Diffuse: Oświetlenie rozproszone, które zależy od kąta padania światła na powierzchnię (kąt między N a L).
Specular: Oświetlenie odbite, które tworzy błyski na powierzchni i zależy od kąta odbicia światła (kąt między V a R).

vertexShaderForPhongShadingWithTexture
======================================

Layout i Wejścia
layout (location = 0) in vec3 aPos: Określa pozycję wierzchołka (aPos) jako trzyelementowy wektor (x, y, z). Dane wejściowe z przypisanej lokalizacji 0.
layout (location = 1) in vec3 aNormal: Określa normalną wierzchołka (aNormal) jako trzyelementowy wektor. Dane wejściowe z przypisanej lokalizacji 1.
layout (location = 2) in vec2 aTexCoords: Określa współrzędne tekstury (aTexCoords) jako dwulementowy wektor (x, y). Dane wejściowe z przypisanej lokalizacji 2.
Wyjścia
out vec3 FragPos: Wyjściowy wektor pozycji fragmentu w przestrzeni świata, przekazywany do fragment shadera.
out vec3 Normal: Wyjściowy wektor normalny w przestrzeni świata, przekazywany do fragment shadera.
out vec2 TexCoords: Wyjściowe współrzędne tekstury, przekazywane do fragment shadera.
Uniforms
uniform mat4 model: Macierz modelu, która przekształca współrzędne wierzchołka z lokalnej przestrzeni modelu do przestrzeni świata.
uniform mat4 view: Macierz widoku, która przekształca współrzędne z przestrzeni świata do przestrzeni widoku (kamery).
uniform mat4 projection: Macierz projekcji, która przekształca współrzędne z przestrzeni widoku do przestrzeni ekranu.
Funkcja główna (main)
gl_Position = projection * view * model * vec4(aPos, 1.0):

Oblicza pozycję wierzchołka w przestrzeni ekranu.
Wektor aPos jest przekształcany najpierw przez macierz modelu (do przestrzeni świata), następnie przez macierz widoku (do przestrzeni kamery), a na końcu przez macierz projekcji (do przestrzeni ekranu). Wynik jest przypisywany do wbudowanej zmiennej gl_Position, która określa ostateczną pozycję wierzchołka na ekranie.
FragPos = vec3(model * vec4(aPos, 1.0)):

Oblicza pozycję wierzchołka w przestrzeni świata i zapisuje ją w FragPos.
Macierz modelu przekształca lokalne współrzędne wierzchołka (aPos) do przestrzeni świata.
Normal = mat3(transpose(inverse(model))) * aNormal:

Oblicza normalny wektor w przestrzeni świata i zapisuje go w Normal.
Macierz normalnych, uzyskana przez transpozycję odwrotnej macierzy modelu, jest stosowana do przekształcenia wektora normalnego wierzchołka (aNormal) do przestrzeni świata.
TexCoords = aTexCoords:

Przekazuje współrzędne tekstury bez zmian do zmiennej TexCoords.

Camera.h - plik do obsługi kamery ze strony learnopengl.com
=================================================================

Definicja klasy 'Camera':

Pozycje i wektory:

Position: Pozycja kamery w przestrzeni 3D.
Front: Wektor kierunkowy wskazujący, w którą stronę kamera patrzy.
Up, Right, WorldUp: Wektory pomocnicze do określania orientacji kamery.

Kąty Eulera:

Yaw: Obrót wokół osi Y.
Pitch: Obrót wokół osi X.
Roll: Obrót wokół osi Z.

Opcje kamery:

MovementSpeed: Prędkość ruchu kamery.
MouseSensitivity: Czułość myszy.
Zoom: Zoom kamery.

Konstruktor: Ustawia początkowe wartości dla atrybutów kamery, a także wywołuje funkcję updateCameraVectors(), która aktualizuje wektory kierunkowe kamery na podstawie kątów Eulera.

GetViewMatrix(): Zwraca macierz widoku używaną przez OpenGL do rysowania sceny z perspektywy kamery.

ProcessKeyboard(): Przetwarza wejście z klawiatury do poruszania kamerą lub obracania jej.

ProcessMouseMovement(): Przetwarza ruch myszy, aktualizując kąty Yaw i Pitch.

ProcessMouseScroll(): Przetwarza ruch kółka myszy, aktualizując wartość Zoom.

updateCameraVectors(): Aktualizuje wektory Front, Right i Up na podstawie aktualnych kątów Eulera.

===================================================================
Shader.h - plik do obsługi shaderów ze strony learnopengl.com
===================================================================

Definicja klasy 'Shader':

ID: Identyfikator programu shaderów w OpenGL

Konstruktor:

Ładuje kod źródłowy shaderów z plików, kompiluje je i tworzy program shaderów w OpenGL.

1. Ładowanie kodu shadera z plików:

Pliki z kodem shadera są otwierane i wczytywane do strumieni, a następnie zamieniane na ciągi znaków.

vertexCode, fragmentCode, geometryCode: Przechowują kod źródłowy shaderów.
vShaderFile, fShaderFile, gShaderFile: Strumienie plików do wczytywania shaderów.


2. Kompilacja shaderów:

Kod vertex shadera i fragment shadera jest kompilowany przy użyciu funkcji OpenGL (glCreateShader, glShaderSource, glCompileShader).
Jeśli podano geometry shader, również jest on kompilowany.

3. Tworzenie programu shaderów:

Program shaderów jest tworzony i shader'y są do niego dołączane (glAttachShader), a następnie linkowane (glLinkProgram).
Po linkowaniu niepotrzebne już shadery są usuwane (glDeleteShader).

use(): Aktywuje program shaderów, ustawiając go jako bieżący w OpenGL

checkCompileErrors(): Sprawdza, czy kompilacja shadera lub linkowanie programu shaderów zakończyły się sukcesem. Jeśli nie, wyświetla odpowiedni komunikat błędu.

pointLight.h - plik do obsługi punktowych źródeł światła ze strony learnopengl.com
========================================================================================

position: Pozycja źródła światła w przestrzeni 3D.
ambient: Wkład światła otoczenia (ambient light) - światło rozproszone, które jest wszędzie.
diffuse: Wkład światła rozproszonego (diffuse light) - światło padające bezpośrednio na powierzchnię.
specular: Wkład światła odbitego (specular light) - jasne refleksy świetlne.
k_c: Stała składowa tłumienia światła.
k_l: Liniowa składowa tłumienia światła.
k_q: Kwadratowa składowa tłumienia światła.
lightNumber: Numer identyfikujący źródło światła w scenie.

setUpPointLight: Metoda ustawiająca wartości uniformów w shaderze odpowiedzialnym za oświetlenie

turnOff: Wyłącza wszystkie komponenty światła
turnOn: Włącza wszystkie komponenty światła
turnAmbientOn/Off: Włącza/wyłącza komponent ambient
turnDiffuseOn/Off: Włącza/wyłącza komponent diffuse.
turnSpecularOn/Off: Włącza/wyłącza komponent specular.

ambientOn, diffuseOn, specularOn: Zmienna kontrolująca, czy dany komponent światła jest włączony (1.0) czy wyłączony (0.0).

Mesh.h - plik reprezentujący siatkę 3D do renderowania 
============================================================
Struktura Vertex definiuje pojedynczy wierzchołek siatki zawierający:

Position: pozycję wierzchołka w przestrzeni 3D.
Normal: wektor normalny wierzchołka.
TexCoords: współrzędne tekstury.

Struktura Texture definiuje teksturę używaną przez siatkę:

id: identyfikator tekstury.
type: typ tekstury (np. diffuse, specular).
path: ścieżka do pliku tekstury.

Klasa 'Mesh':

vertices, indices i textures to wektory przechowujące wierzchołki, indeksy i tekstury siatki.
Konstruktor Mesh inicjalizuje pola klasy i wywołuje metodę setupMesh.
Draw to metoda rysująca siatkę za pomocą dostarczonego shadera.
VAO, VBO i EBO to identyfikatory OpenGL dla odpowiednich obiektów (Vertex Array Object, Vertex Buffer Object, Element Buffer Object).
setupMesh to metoda prywatna konfigurująca bufory OpenGL.

Konstruktor: Przyjmuje wektory wierzchołków, indeksów i tekstur, przypisuje je do pól klasy i wywołuje setupMesh w celu konfiguracji siatki.

Metoda Draw:

Rysuje siatkę używając dostarczonego shadera.
Aktywuje odpowiednie jednostki tekstur (glActiveTexture), przypisuje je do shader'a (shader.setInt), a następnie wiąże tekstury (glBindTexture).
Używa glBindVertexArray do związywania VAO, a następnie rysuje elementy (glDrawElements).

Metoda setupMesh:

Metoda setupMesh tworzy i konfiguruje bufory oraz VAO.
Generuje VAO, VBO i EBO (glGenVertexArrays, glGenBuffers).
Wiąże VAO (glBindVertexArray) i bufor wierzchołków (glBindBuffer), a następnie ładuje dane do bufora (glBufferData).
Konfiguruje atrybuty wierzchołków dla pozycji, normalnych i współrzędnych tekstury (glEnableVertexAttribArray, glVertexAttribPointer).
Kończy wiązanie VAO (glBindVertexArray(0)).

Model.h - plik odpowiedzialny za obsługę modeli 3D
==================================================

Klasa 'Model':
textures_loaded: Wektor przechowujący załadowane tekstury, aby uniknąć ich wielokrotnego ładowania.
meshes: Wektor przechowujący wszystkie siatki modelu
directory: Katalog, w którym znajduje się plik modelu

Draw: Metoda rysująca model przy użyciu podanego shadera:

Parametr: shader - Obiekt klasy Shader, który ma być używany do renderowania modelu.
Działanie: Iteruje przez wszystkie siatki w wektorze meshes i wywołuje metodę Draw dla każdej z nich, przekazując shader jako argument.

loadModel: Metoda ładująca model z pliku:

Parametr: path - Ścieżka do pliku modelu.
Działanie:
Tworzy obiekt Assimp::Importer do zaimportowania modelu.
Wywołuje ReadFile z odpowiednimi flagami przetwarzania, aby wczytać model i przetworzyć go na trójkąty oraz odwrócić współrzędne UV.
Sprawdza, czy scena została poprawnie załadowana.
Ustawia katalog na podstawie ścieżki do pliku modelu.
Wywołuje metodę processNode, aby przetworzyć węzeł główny sceny.

processNode: Metoda przetwarzająca węzeł sceny:

Parametr:
node - Wskaźnik do węzła Assimp.
scene - Wskaźnik do sceny Assimp.
Działanie:
Iteruje przez wszystkie siatki w węźle, wywołując processMesh dla każdej z nich i dodając wyniki do wektora meshes.
Rekurencyjnie wywołuje siebie dla wszystkich dzieci węzła.


processMesh: Metoda przetwarzająca pojedynczą siatkę:

Parametry:
mesh - Wskaźnik do siatki Assimp.
scene - Wskaźnik do sceny Assimp.
Działanie:
Przetwarzanie wierzchołków: Iteruje przez wszystkie wierzchołki siatki, kopiując pozycje, normale (jeśli dostępne) i współrzędne tekstury (jeśli dostępne) do odpowiednich struktur Vertex.
Przetwarzanie indeksów: Iteruje przez wszystkie twarze (faces) siatki, kopiując indeksy wierzchołków do wektora indices.
Przetwarzanie materiałów: Pobiera materiał siatki i ładuje tekstury materiału (diffuse, specular, normal, height), korzystając z metody loadMaterialTextures.
Tworzenie obiektu Mesh: Tworzy obiekt Mesh z przetworzonych danych (wierzchołków, indeksów, tekstur) i zwraca go.

loadMaterialTexture: Metoda ładująca tekstury materiału

Parametry:
mat - Wskaźnik do materiału Assimp.
type - Typ tekstury Assimp (np. aiTextureType_DIFFUSE).
typeName - Nazwa typu tekstury używana w shaderach (np. "texture_diffuse").
Działanie:
Iteruje przez wszystkie tekstury danego typu w materiale.
Sprawdza, czy tekstura była już załadowana wcześniej, aby uniknąć duplikacji.
Jeśli tekstura nie była jeszcze załadowana, ładuje ją z pliku przy użyciu funkcji TextureFromFile i dodaje do wektora tekstur.
Zwraca wektor załadowanych tekstur.

TextureFromFile: ładuje teksturę z pliku i zwraca jej ID

Parametry:
path - Ścieżka do pliku tekstury.
directory - Katalog, w którym znajduje się plik tekstury.
Działanie:
Generuje nowy identyfikator tekstury OpenGL (glGenTextures).
Łączy katalog i nazwę pliku, tworząc pełną ścieżkę do pliku tekstury.
Ładuje dane obrazu z pliku za pomocą stbi_load.
Ustala format tekstury na podstawie liczby komponentów (1 - GL_RED, 3 - GL_RGB, 4 - GL_RGBA).
Wiąże teksturę (glBindTexture) i ładuje dane obrazu do tekstury (glTexImage2D).
Generuje mipmapy dla tekstury (glGenerateMipmap).
Ustawia parametry tekstury (glTexParameteri).
Zwalnia pamięć obrazu (stbi_image_free).
Zwraca identyfikator tekstury OpenGL. Jeśli ładowanie się nie powiedzie, wypisuje komunikat o błędzie i również zwalnia pamięć obrazu.





