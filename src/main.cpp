#include "raylib.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> // Necesario para la función srand y rand
#include <ctime>   // Necesario para la función time
#include <vector>
#include <time.h>
#include <algorithm>
#include <string.h>

// TYPEDEF PARA SUBMENUS Y MENUS.

typedef enum jugarscreen
{
    BASICO,
    LETRAS,
    COLORES,
} jugarscreen;

// ESTE TYPEDEF ME AYUDA CON EL ESTADO DEL JUEGO, SI ESTA JUGANDO O SI REGRESA AL MENU ANTERIOR. ------------------------------
typedef enum JuegoEstado
{
    JUEGO_JUGANDO,
    JUEGO_REGRESAR_MENU,
    MUSICA,
    FULLSCREEN,
} JuegoEstado;

// ESTE TYPEDEF ME AYUDA CON LOS ESTADOS DE LA PANTALLA, SI ESTA EN EL MENU, JUGANDO, CREDITOS, ETC. TAMBIEN CON JUGAR_... BASICAMENTE JUNTO EL PRIMER TYPEDEF CON ESTE.
typedef enum GameScreen
{
    INICIO = 0,
    JUGAR,
    CREDITOS,
    JUGAR_BASICO,
    JUGAR_LETRAS,
    JUGAR_COLORES,
} GameScreen;

// STRUCT PARA CARGAR LOS ARCHIVOS DE LAS PANTALLAS. ------------------------------
typedef struct cargas
{
    Texture2D backgroundTexture;
    Texture2D backgroundTexture_creditos;
    Texture2D backgroundTexture_basico;
    Texture2D backgroundTexture_letras;
    Texture2D backgroundTexture_colores;
    Sound buttonSound;
    Sound cardsound;
    Texture2D buttonTextureA;
    Texture2D buttonTextureB;
    Texture2D buttonTextureC;
    Texture2D buttonTextureD;
    Texture2D buttonTextureE;
    Texture2D buttonTexture1;
    Texture2D buttonTexture2;
    Texture2D buttonTexture3;
    Texture2D buttonTexture4;
    Music musica_fondo;
    Font font;
} cargas;

typedef struct cartas
{
    Texture2D carta_back;
    Texture2D carta1;
    Texture2D carta2;
    Texture2D carta3;
    Texture2D carta4;
    Texture2D carta5;
    Texture2D carta6;
    Texture2D carta7;
    Texture2D carta8;
    Texture2D carta9;
    // COLORES
    Texture2D carta10;
    Texture2D carta11;
    Texture2D carta12;
    Texture2D carta13;
    Texture2D carta14;
    Texture2D carta15;
    Texture2D carta16;
    Texture2D carta17;
    Texture2D carta18;
    // LETRAS
} cartas;

typedef struct memorama
{
    bool card_state[3][6];
    int cartas[3][6];
    int cartas_en_estado_1;  // Contador de cartas en estado 1
    int max_cartas_estado_1; // Número máximo de cartas permitidas en estado 1
    // Almacena las coordenadas de la primera carta volteada
    int prim_carta_columna;
    int prim_carta_fila;
    // Almacena las coordenadas de la segunda carta volteada
    int seg_carta_columna;
    int seg_carta_fila;
    int paresEncontrados;
    int prim_carta_filaa;
    int prim_carta_columnaa;
    int card_reveal_number;
    int num_fila = 3;
    int num_columna = 6;
} memorama;

typedef struct _colores
{
    bool card_state[3][6];
    int cartas[3][6];
    int cartas_en_estado_1;  // Contador de cartas en estado 1
    int max_cartas_estado_1; // Número máximo de cartas permitidas en estado 1
    // Almacena las coordenadas de la primera carta volteada
    int prim_carta_columna;
    int prim_carta_fila;
    // Almacena las coordenadas de la segunda carta volteada
    int seg_carta_columna;
    int seg_carta_fila;
    int paresEncontrados;
    int prim_carta_filaa;
    int prim_carta_columnaa;
    int card_reveal_number;
    int num_fila = 3;
    int num_columna = 6;
} _colores;

// AQUI IRA EL NUEVO STRUCT PARA CADA TEXTURA DE LAS CARTAS, HACER UNO PARA CADA CATEGORIA. ------------------------------

// SACAR LA RESOLUCION DEL MONITOR
int screenWidth = GetMonitorWidth(0);
int screenHeight = GetMonitorHeight(0);

// PROTOTIPOS DE FUNCIONES ------------------------------
// >> CARGAS ARCHIVOS Y MENUS.
cargas LoadContent(const char pantalla[], cargas archivos);
int drawcreditos(cargas archivos);
void menudraw(GameScreen currentScreen, cargas archivos, cartas todo, cartas todo2, cartas todo3, memorama estruct, _colores estruct2);
void ToggleFullscreenAndResize();
bool musica(bool musicToggle, bool &musicPaused, Music &musica_fondo);
int drawinicio(cargas archivos);
int drawjugar(GameScreen currentScreen, cargas archivos);
void UnloadContent(cargas archivos, GameScreen currentScreen);
// >> JUEGO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
cartas LoadCartas_b(const char categoria[], cartas todo);
cartas UnloadCartas_b(const char categoria[], cartas todo);
Texture2D GetCartaTexture(cartas todo, int num_carta);
int contarRepeticiones(memorama &estruct, int numero);
void voltearCartas(memorama &estruct, int fila, int columna);
void iniciar_memo(memorama &estruct);
void memoria(cartas todo, memorama &estruct, cargas archivos);
JuegoEstado jugar_basico(GameScreen currentScreen, cargas archivos, cartas todo, memorama &estruct);
JuegoEstado jugar_letras(GameScreen currentScreen, cargas archivos);
Texture2D GetCartaTexture_COLOR(cartas todo3, int num_carta);
void iniciar_memo_COLOR(_colores &estruct2);
JuegoEstado jugar_colores(GameScreen currentScreen, cargas archivos, cartas todo3, _colores &estruct2);

// MAIN ------------------------------
int main(void)
{
    // CREAR LA VENTANA E INICIAR EL AUDIO, Y CARGAR LOS ARCHIVOS.
    InitWindow(screenWidth, screenHeight, "SIGN MATCH - GAME");
    InitAudioDevice();
    SetTargetFPS(60); // Set desired framerate (frames-per-second)
    Image icon = LoadImage("resources/SG_ICONO.png");
    SetWindowIcon(icon);

    // ASIGNAR LAS PANTALLAS A LAS VARIABLES. Y TAMBIEN CARGAR EL SONIDO Y LA MUSICA.
    GameScreen currentScreen = INICIO;
    GameScreen nextScreen = INICIO;
    cargas archivos;
    archivos.buttonSound = LoadSound("audio/resources/buttonsound.wav");
    archivos.musica_fondo = LoadMusicStream("audio/resources/fondo.mp3");
    archivos = LoadContent("MENU", archivos);
    // BANDERAS DE LA FUNCION PAUSA MUSICA. Y REPRODUCIR LA MUSICA DE FONDO.
    bool musicPaused = false;
    bool musicToggle = false;

    // JUEGO
    cartas todo;
    cartas todo2;
    cartas todo3;
    memorama estruct;
    _colores estruct2;
    srand(time(NULL));
    iniciar_memo(estruct);
    iniciar_memo_COLOR(estruct2);
    // iniciar_memo_COLOR(estruct2);
    todo = LoadCartas_b("BASICO", todo);
    todo2 = LoadCartas_b("LETRAS", todo2);
    todo3 = LoadCartas_b("COLORES", todo3);
    PlayMusicStream(archivos.musica_fondo);
    //--------------------------------------------------------------------------------------

    // MIENTRAS QUE NO SE CIERRE LA VENTANA, SEGUIR EJECUTANDO EL PROGRAMA.
    while (!WindowShouldClose()) // TE VA A SACAR DEL PROGRAMA CON EL "ESC".
    {
        ToggleFullscreenAndResize(); // FUNCION PARA PANTALLA COMPLETA.

        // VARIABLES PARA LOS BOTONES. Y NAVEGAR ENTRE PANTALLAS.
        int buttonClicked = drawinicio(archivos);
        int buttonClicked2 = drawjugar(currentScreen, archivos);
        int buttonClicked3 = drawcreditos(archivos);

        // SWITCH PARA CAMBIAR DE PANTALLAS EN EL JUEGO, Y TAMBIEN PARA CAMBIAR DE PANTALLAS EN EL MENU.
        switch (currentScreen)
        {
        case INICIO: // AQUI EN ESTA OPCION ESTARA EL MENU.
        {
            if (buttonClicked == 1)
            {
                // SI SE PRESIONA EL BOTON 1, CAMBIAR A LA PANTALLA JUGAR. Y DESCARGAR LOS ARCHIVOS DE LA PANTALLA ANTERIOR. Y CARGAR LOS ARCHIVOS DE LA PANTALLA NUEVA.
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, INICIO);
                archivos = LoadContent("JUGAR", archivos);
            }
            if (buttonClicked == 2)
            {
                // SI SE PRESIONA EL BOTON 2, CAMBIAR A LA PANTALLA CREDITOS. Y DESCARGAR LOS ARCHIVOS DE LA PANTALLA ANTERIOR. Y CARGAR LOS ARCHIVOS DE LA PANTALLA NUEVA.
                PlaySound(archivos.buttonSound);
                currentScreen = CREDITOS;
                UnloadContent(archivos, INICIO);
                archivos = LoadContent("CREDITOS", archivos);
            }
            if (IsKeyPressed(KEY_M) || buttonClicked == 4)
            {
                // SI SE PRESIONA LA TECLA "M", PAUSAR LA MUSICA.
                PlaySound(archivos.buttonSound);
                musicToggle = !musicToggle;
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (buttonClicked == 3)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
        }
        break;
        case JUGAR: // AQUI EN ESTA OPCION ESTARA EL SUBMENU DE JUGAR.
        {
            if (buttonClicked2 == 1)
            {
                // SI SE PRESIONA EL BOTON 1, CAMBIAR A LA PANTALLA JUGAR_BASICO. Y DESCARGAR LOS ARCHIVOS DE LA PANTALLA ANTERIOR. Y CARGAR LOS ARCHIVOS DE LA PANTALLA NUEVA.
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_BASICO;
                archivos = LoadContent("JUGAR_BASICO", archivos);
            }
            if (buttonClicked2 == 2)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_LETRAS;
                archivos = LoadContent("JUGAR_LETRAS", archivos);
            }
            if (buttonClicked2 == 3)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR_COLORES;
                archivos = LoadContent("JUGAR_COLORES", archivos);
            }
            if (IsKeyPressed(KEY_DELETE) || buttonClicked2 == 6)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = nextScreen;
                archivos = LoadContent("MENU", archivos);
            }
            if (IsKeyPressed(KEY_M) || buttonClicked2 == 5)
            {
                // SI SE PRESIONA LA TECLA "M", PAUSAR LA MUSICA.
                PlaySound(archivos.buttonSound);
                musicToggle = !musicToggle;
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (buttonClicked2 == 4)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
        }
        break;
        case JUGAR_BASICO:
        {
            // Llama a la función jugar_basico y obtén el estado del juego
            // BOTONES DE CARTAS FINAL -------------------------------------------------
            JuegoEstado estadoJuego = jugar_basico(currentScreen, archivos, todo, estruct);

            // Verifica el estado del juego
            if (estadoJuego == JUEGO_REGRESAR_MENU)
            {
                // Regresa al menú
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, JUGAR_BASICO);
                archivos = LoadContent("JUEGO", archivos);
            }
            if (IsKeyPressed(KEY_M) || estadoJuego == MUSICA)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_F) || estadoJuego == FULLSCREEN)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        case JUGAR_LETRAS:
        {
            JuegoEstado estadoJuego = jugar_letras(currentScreen, archivos);
            if (estadoJuego == JUEGO_REGRESAR_MENU)
            {
                // Regresa al menú
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, JUGAR_LETRAS);
                archivos = LoadContent("JUEGO", archivos);
            }
            if (IsKeyPressed(KEY_M) || estadoJuego == MUSICA)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_F) || estadoJuego == FULLSCREEN)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        case JUGAR_COLORES:
        {
            JuegoEstado estadoJuego = jugar_colores(currentScreen, archivos, todo3, estruct2);
            if (estadoJuego == JUEGO_REGRESAR_MENU)
            {
                // Regresa al menú
                PlaySound(archivos.buttonSound);
                currentScreen = JUGAR;
                UnloadContent(archivos, JUGAR_COLORES);
                archivos = LoadContent("JUEGO", archivos);
            }
            if (IsKeyPressed(KEY_M) || estadoJuego == MUSICA)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_F) || estadoJuego == FULLSCREEN)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        case CREDITOS:
        {
            if (!musicPaused) // Only resume the music if it's not supposed to be paused
            {
                ResumeMusicStream(archivos.musica_fondo);
            }
            if (IsKeyPressed(KEY_DELETE) || buttonClicked3 == 2)
            {
                PlaySound(archivos.buttonSound);
                currentScreen = nextScreen;
                UnloadContent(archivos, CREDITOS);
                archivos = LoadContent("MENU", archivos);
            }
            if (IsKeyPressed(KEY_M) || buttonClicked3 == 3)
            {
                musicToggle = !musicToggle; // Cambiar el estado de musicToggle cada vez que se presiona 'M'
                musicPaused = musica(musicToggle, musicPaused, archivos.musica_fondo);
            }
            if (buttonClicked3 == 1)
            {
                PlaySound(archivos.buttonSound);
                ToggleFullscreen();
            }
            break;
        }
        default:
            break;
        }
        // Draw
        //----------------------------------------------------------------------------------
        menudraw(currentScreen, archivos, todo, todo2, todo3, estruct, estruct2);
        UpdateMusicStream(archivos.musica_fondo);
    }

    // TODO: Unload all loaded data (textures, fonts, audio) here!
    UnloadContent(archivos, currentScreen);
    UnloadContent(archivos, nextScreen);
    UnloadCartas_b("BASICO", todo);
    UnloadCartas_b("LETRAS", todo2);
    UnloadCartas_b("COLORES", todo3);
    UnloadSound(archivos.buttonSound);
    UnloadMusicStream(archivos.musica_fondo);
    UnloadImage(icon);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

bool musica(bool musicToggle, bool &musicPaused, Music &musica_fondo)
{
    if (musicToggle)
    {
        if (IsMusicStreamPlaying(musica_fondo))
        {
            PauseMusicStream(musica_fondo);
            musicPaused = true;
        }
        else
        {
            ResumeMusicStream(musica_fondo);
            musicPaused = false;
        }
    }

    return musicPaused;
}

cargas LoadContent(const char pantalla[], cargas archivos)
{
    // Cargar texturas según el nombre de la pantalla
    if (strcmp(pantalla, "MENU") == 0)
    {
        archivos.backgroundTexture = LoadTexture("resources/SM_Pantalla.png");
        archivos.buttonTextureA = LoadTexture("resources/SM_BotonJugar_1.png");
        archivos.buttonTextureB = LoadTexture("resources/SM_BotonCreditos.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        SetMusicVolume(archivos.musica_fondo, 1.0f);
    }
    if (strcmp(pantalla, "JUGAR") == 0)
    {
        archivos.backgroundTexture = LoadTexture("resources/SM_PantallaJugar.png");
        archivos.buttonTextureC = LoadTexture("resources/SM_BotonBasicos.png");
        archivos.buttonTextureD = LoadTexture("resources/SM_BotonLetras.png");
        archivos.buttonTextureE = LoadTexture("resources/SM_BotonColores.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
    }

    if (strcmp(pantalla, "CREDITOS") == 0)
    {
        archivos.backgroundTexture_creditos = LoadTexture("resources/SM_PantallaCreditos.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
    }

    if (strcmp(pantalla, "JUGAR_BASICO") == 0)
    {
        archivos.backgroundTexture_basico = LoadTexture("resources/SM_PantallaBasicos.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        archivos.cardsound = LoadSound("audio/resources/SM_cards.wav");
        SetSoundVolume(archivos.cardsound, 1.0f);
        archivos.font = LoadFont("text/resources/Motley Forces.ttf");
        // archivos.buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
    }

    if (strcmp(pantalla, "JUGAR_LETRAS") == 0)
    {
        archivos.backgroundTexture_letras = LoadTexture("resources/SM_PantallaLetras.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        archivos.cardsound = LoadSound("audio/resources/SM_cards.wav");
        SetSoundVolume(archivos.cardsound, 1.0f);
        // archivos.buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
    }

    if (strcmp(pantalla, "JUGAR_COLORES") == 0)
    {
        archivos.backgroundTexture_colores = LoadTexture("resources/SM_PantallaColores.png");
        archivos.buttonTexture2 = LoadTexture("resources/SM_BotonAgrandar.png");
        archivos.buttonTexture3 = LoadTexture("resources/SM_BotonRegresar.png");
        archivos.buttonTexture4 = LoadTexture("resources/SM_BotonVolumen.png");
        archivos.cardsound = LoadSound("audio/resources/SM_cards.wav");
        SetSoundVolume(archivos.cardsound, 1.0f);
        // archivos.buttonTexture1 = LoadTexture("resources/SM_CartaAtras.png");
    }
    // Agrega más condiciones para otras pantallas

    return archivos;
}

void UnloadContent(cargas archivos, GameScreen currentScreen)
{
    // Descargar texturas
    if (currentScreen == INICIO)
    {
        UnloadTexture(archivos.backgroundTexture);
        UnloadTexture(archivos.buttonTextureA);
        UnloadTexture(archivos.buttonTextureB);
        UnloadTexture(archivos.buttonTexture2);
        UnloadTexture(archivos.buttonTexture4);
    }
    if (currentScreen == JUGAR)
    {
        UnloadTexture(archivos.backgroundTexture);
        UnloadTexture(archivos.buttonTextureC);
        UnloadTexture(archivos.buttonTextureD);
        UnloadTexture(archivos.buttonTextureE);
        UnloadTexture(archivos.buttonTexture3);
    }
    if (currentScreen == CREDITOS)
    {
        UnloadTexture(archivos.backgroundTexture_creditos);
    }

    if (currentScreen == JUGAR_BASICO)
    {
        UnloadTexture(archivos.backgroundTexture_basico);
        UnloadSound(archivos.cardsound);
        UnloadFont(archivos.font);
        // UnloadTexture(archivos.buttonTexture1);
    }

    if (currentScreen == JUGAR_LETRAS)
    {
        UnloadTexture(archivos.backgroundTexture_letras);
        UnloadSound(archivos.cardsound);
        // UnloadTexture(archivos.buttonTexture1);
    }

    if (currentScreen == JUGAR_COLORES)
    {
        UnloadTexture(archivos.backgroundTexture_colores);
        UnloadSound(archivos.cardsound);
        // UnloadTexture(archivos.buttonTexture1);
    }
}

cartas LoadCartas_b(const char categoria[], cartas todo)
{
    if (strcmp(categoria, "BASICO") == 0)
    {
        todo.carta_back = LoadTexture("resources/SM_CartaAtras.png");
        todo.carta1 = LoadTexture("resources/SM_CartaBien.png");
        todo.carta2 = LoadTexture("resources/SM_CartaEs.png");
        todo.carta3 = LoadTexture("resources/SM_CartaGracias.png");
        todo.carta4 = LoadTexture("resources/SM_CartaMal.png");
        todo.carta5 = LoadTexture("resources/SM_CartaNo.png");
        todo.carta6 = LoadTexture("resources/SM_CartaPerdon.png");
        todo.carta7 = LoadTexture("resources/SM_CartaPorFavor.png");
        todo.carta8 = LoadTexture("resources/SM_CartaQue.png");
        todo.carta9 = LoadTexture("resources/SM_CartaSi.png");
    }

    if (strcmp(categoria, "COLORES") == 0)
    {
        todo.carta_back = LoadTexture("resources/SM_CartaAtras.png");
        todo.carta10 = LoadTexture("resources/SM_CartaRojo.png");
        todo.carta11 = LoadTexture("resources/SM_CartaAzul.png");
        todo.carta12 = LoadTexture("resources/SM_CartaVerde.png");
        todo.carta13 = LoadTexture("resources/SM_CartaAmarillo.png");
        todo.carta14 = LoadTexture("resources/SM_CartaBlanco.png");
        todo.carta15 = LoadTexture("resources/SM_CartaNegro.png");
        todo.carta16 = LoadTexture("resources/SM_CartaCafe.png");
        todo.carta17 = LoadTexture("resources/SM_CartaColor.png");
        todo.carta18 = LoadTexture("resources/SM_CartaRosa.png");
    }

    return todo;
}

cartas UnloadCartas_b(const char categoria[], cartas todo)
{
    if (strcmp(categoria, "BASICO") == 0)
    {
        UnloadTexture(todo.carta_back);
        UnloadTexture(todo.carta1);
        UnloadTexture(todo.carta2);
        UnloadTexture(todo.carta3);
        UnloadTexture(todo.carta4);
        UnloadTexture(todo.carta5);
        UnloadTexture(todo.carta6);
        UnloadTexture(todo.carta7);
        UnloadTexture(todo.carta8);
        UnloadTexture(todo.carta9);
    }

    if (strcmp(categoria, "COLORES") == 0)
    {
        UnloadTexture(todo.carta_back);
        UnloadTexture(todo.carta10);
        UnloadTexture(todo.carta11);
        UnloadTexture(todo.carta12);
        UnloadTexture(todo.carta13);
        UnloadTexture(todo.carta14);
        UnloadTexture(todo.carta15);
        UnloadTexture(todo.carta16);
        UnloadTexture(todo.carta17);
        UnloadTexture(todo.carta18);
    }

    return todo;
}

int drawinicio(cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture,
        (Rectangle){0, 0, (float)archivos.backgroundTexture.width, (float)archivos.backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    /* DIBUJAR LOS BOTONES */
    // Centrar los botones horizontalmente y ajustar la distancia entre ellos
    float startX = (GetScreenWidth() - archivos.buttonTextureA.width) / 2;
    float startY = (GetScreenHeight() - archivos.buttonTextureA.height) / 2;
    float bstartX = (GetScreenWidth() - archivos.buttonTextureB.width) / 2;

    Rectangle buttonRectA = {startX, startY, (float)archivos.buttonTextureA.width, (float)archivos.buttonTextureA.height};
    Rectangle buttonRectB = {bstartX, GetScreenHeight() - archivos.buttonTextureB.height, (float)archivos.buttonTextureB.width, (float)archivos.buttonTextureB.height};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    /* VERIFICAR SI EL MOUSE ESTÁ SOBRE LOS BOTONES */
    bool isMouseOverButtonA = CheckCollisionPointRec(GetMousePosition(), buttonRectA);
    bool isMouseOverButtonB = CheckCollisionPointRec(GetMousePosition(), buttonRectB);
    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    /* CAMBIAR EL TAMAÑO DEL BOTÓN (A, B) SI EL MOUSE ESTÁ SOBRE DE EL */
    if (isMouseOverButtonA)
    {
        /* AGRANDAR Y REDUCIR (A) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectA.width = archivos.buttonTextureA.width * scale;
        buttonRectA.height = archivos.buttonTextureA.height * scale;
    }

    if (isMouseOverButtonB)
    {
        /* AGRANDAR Y REDUCIR (B) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectB.width = archivos.buttonTextureB.width * scale;
        buttonRectB.height = archivos.buttonTextureB.height * scale;
    }
    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    /* AJUSTAR LA POSICIÓN DEL BOTÓN PARA QUE ESTÉ EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectA.x + buttonRectA.width / 2, buttonRectA.y + buttonRectA.height / 2};
    Vector2 buttonPositionB = {buttonRectB.x + buttonRectB.width / 2, buttonRectB.y + buttonRectB.height / 2};
    DrawTexturePro(archivos.buttonTextureA, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureA.width), static_cast<float>(archivos.buttonTextureA.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectA.width, buttonRectA.height}, (Vector2){buttonRectA.width / 2, buttonRectA.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureB, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureB.width), static_cast<float>(archivos.buttonTextureB.height)}, (Rectangle){buttonPositionB.x, buttonPositionB.y, buttonRectB.width, buttonRectB.height}, (Vector2){buttonRectB.width / 2, buttonRectB.height / 2}, 0.0f, WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonA)
            return 1; // Botón A clickeado
        if (isMouseOverButtonB)
            return 2; // Botón B clickeado
        if (isMouseOverButton2)
            return 3; // Botón 2 clickeado
        if (isMouseOverButton4)
            return 4; // Botón 4 clickeado
    }

    return 0; // Ningún botón
}

Texture2D GetCartaTexture(cartas todo, int num_carta)
{
    switch (num_carta)
    {
    case 1:
        return todo.carta1;
    case 2:
        return todo.carta2;
    case 3:
        return todo.carta3;
    case 4:
        return todo.carta4;
    case 5:
        return todo.carta5;
    case 6:
        return todo.carta6;
    case 7:
        return todo.carta7;
    case 8:
        return todo.carta8;
    case 9:
        return todo.carta9;
    // Agrega más casos según la cantidad de cartas que tengas
    default:
        return todo.carta_back; // Textura por defecto (carta oculta)
    }
}

void iniciar_memo(memorama &estruct)
{
    // Inicializar las coordenadas de las cartas volteadas

    estruct.prim_carta_fila = -1;
    estruct.prim_carta_columna = -1;
    estruct.seg_carta_fila = -1;
    estruct.seg_carta_columna = -1;

    // Inicializar el contador de cartas en estado 1
    estruct.cartas_en_estado_1 = 0;
    // Establecer el número máximo de cartas permitidas en estado 1
    estruct.max_cartas_estado_1 = 2;

    // Inicializar todas las cartas como false
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            estruct.card_state[i][j] = false;
        }
    }

    // Números del 1 al 9
    int numeros_disponibles[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Barajar los números disponibles
    srand((unsigned int)time(NULL));
    for (int i = 17; i > 0; i--)
    {
        int j = rand() % (i + 1);
        // Intercambiar numeros_disponibles[i] y numeros_disponibles[j]
        int temp = numeros_disponibles[i];
        numeros_disponibles[i] = numeros_disponibles[j];
        numeros_disponibles[j] = temp;
    }

    // Asignar aleatoriamente las cartas
    int indice = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            // Asignar el valor a la carta
            estruct.cartas[i][j] = numeros_disponibles[indice];

            // Imprimir las cartas asignadas
            // printf("Carta asignada en (%d, %d): %d\n", i, j, estruct.cartas[i][j]);

            indice++;
        }
    }
}

void memoria(cartas todo, memorama &estruct, cargas archivos)
{
    int ancho = GetScreenWidth();
    int altura = GetScreenHeight();

    int fila = estruct.num_fila;
    int columna = estruct.num_columna;

    float espacioX = 8.0f;
    float espacioY = 8.0f;

    float aspecto = 179.0f / 130.0f;
    float ajustes = 130.0f;
    float ajuste_altura = ajustes * aspecto;

    Vector2 mouse = GetMousePosition();

    // Agregar una matriz para rastrear qué pares ya fueron encontrados
    static bool cartas_pares_encontrados[3][6] = {false};
    static int pares_encontrados = 0;

    for (int i = 0; i < fila; i++)
    {
        for (int j = 0; j < columna; j++)
        {
            float posx = (ancho - columna * (ajustes + espacioX) + espacioX) / 2 + j * (ajustes + espacioX);
            float posy = (altura - fila * (ajuste_altura + espacioY) + espacioY) / 2 + i * (ajuste_altura + espacioY);

            Rectangle carta = {posx, posy, ajustes, ajuste_altura};
            bool isMouseOverCard = CheckCollisionPointRec(mouse, carta);

            // Verificar si la carta ya es parte de un par encontrado
            if (cartas_pares_encontrados[i][j])
            {
                // No permitir interacción con estas cartas
                continue;
            }

            // Chequear clic izquierdo en la carta
            if (isMouseOverCard && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                PlaySound(archivos.cardsound);
                // Verificar el estado actual de la carta
                if (!estruct.card_state[i][j] && estruct.cartas_en_estado_1 < estruct.max_cartas_estado_1)
                {
                    // Si ya hay una carta volteada, verificar si es la misma
                    if (estruct.prim_carta_fila != -1 && estruct.prim_carta_columna != -1 &&
                        (estruct.prim_carta_fila != i || estruct.prim_carta_columna != j))
                    {
                        // Verificar si las cartas son iguales
                        estruct.cartas[estruct.seg_carta_fila][estruct.seg_carta_columna] = estruct.cartas[i][j];
                        int valor = estruct.cartas[estruct.seg_carta_fila][estruct.seg_carta_columna];
                        int valor2 = estruct.cartas[estruct.prim_carta_fila][estruct.prim_carta_columna];

                        printf("Carta volteada UNO en (%d, %d): %d\n", i, j, valor);
                        printf("Carta volteada DOS en (%d, %d): %d\n", estruct.prim_carta_fila, estruct.prim_carta_columna, valor2);

                        if (valor == valor2)
                        {
                            pares_encontrados++;
                            printf("Pares encontrados: %d\n", pares_encontrados);

                            // Marcar el par como encontrado en la matriz
                            cartas_pares_encontrados[i][j] = true;
                            cartas_pares_encontrados[estruct.prim_carta_fila][estruct.prim_carta_columna] = true;

                            // Ambas cartas son iguales, reiniciar las coordenadas de ambas cartas
                            estruct.prim_carta_fila = -1;
                            estruct.prim_carta_columna = -1;
                            estruct.seg_carta_fila = -1;
                            estruct.seg_carta_columna = -1;
                            estruct.cartas_en_estado_1 = 0;
                            estruct.max_cartas_estado_1 = 2;

                            // No permitir interacción con estas cartas
                            continue;
                        }
                        else
                        {
                            // Las cartas no son iguales, esperar para volver a voltearlas
                            SetMouseCursor(MOUSE_CURSOR_ARROW);
                        }
                    }
                    else
                    {
                        // No hay carta volteada previamente, actualizar las coordenadas de la primera carta
                        estruct.prim_carta_fila = i;
                        estruct.prim_carta_columna = j;
                        estruct.seg_carta_fila = -1;
                        estruct.seg_carta_columna = -1;
                    }

                    // Cambiar el estado de la carta a 1
                    estruct.card_state[i][j] = true;
                    estruct.cartas_en_estado_1++;

                    // Verificar si la segunda carta ya está volteada
                    if (estruct.seg_carta_fila != -1 && estruct.seg_carta_columna != -1)
                    {
                        // No permitir interacción con estas cartas
                        continue;
                    }

                    // No permitir interacción con la primera carta ya volteada
                    continue;
                }
                else if (estruct.card_state[i][j])
                {
                    // Si la carta estaba en estado 1, cambiarla a 0
                    estruct.card_state[i][j] = false;
                    estruct.cartas_en_estado_1--;

                    // Reiniciar las coordenadas de la primera y segunda carta
                    estruct.prim_carta_fila = -1;
                    estruct.prim_carta_columna = -1;
                    estruct.seg_carta_fila = -1;
                    estruct.seg_carta_columna = -1;
                }
            }

            // Dibujar cartas
            int num_carta = estruct.cartas[i][j];
            Texture2D currentTexture;
            if (estruct.card_state[i][j])
            {
                currentTexture = GetCartaTexture(todo, num_carta);
            }
            else
            {
                currentTexture = todo.carta_back;
            }

            DrawTexturePro(
                currentTexture,
                (Rectangle){0, 0, (float)currentTexture.width, (float)currentTexture.height},
                carta,
                (Vector2){0, 0},
                0.0f,
                WHITE);

            char texto[30];
            sprintf(texto, "Pares encontrados: %d", pares_encontrados);

            int fontsize = 25;
            int textoAncho = MeasureText(texto, fontsize);
            int textoAlto = fontsize;

            Color micolorpersonalizado = {14, 102, 85, 255};
            DrawTextEx(archivos.font, texto, (Vector2){GetScreenWidth() - textoAncho - 10 - 90, GetScreenHeight() - textoAlto - 10}, fontsize, 2, micolorpersonalizado);
        }
    }
}

JuegoEstado jugar_basico(GameScreen currentScreen, cargas archivos, cartas todo, memorama &estruct)
{
    DrawTexturePro(
        archivos.backgroundTexture_basico,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_basico.width, (float)archivos.backgroundTexture_basico.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (3) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    memoria(todo, estruct, archivos);

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return FULLSCREEN; // Botón 2 clickeado
        if (isMouseOverButton3)
            return JUEGO_REGRESAR_MENU; // Botón 3 clickeado
        if (isMouseOverButton4)
            return MUSICA; // Botón 4 clickeado
    }

    if (IsKeyPressed(KEY_DELETE))
    {
        // Si se presiona DELETE, regresar al menú
        return JUEGO_REGRESAR_MENU;
    }

    // Si no hay cambio de pantalla, continúa jugando
    return JUEGO_JUGANDO;
}

JuegoEstado jugar_letras(GameScreen currentScreen, cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture_letras,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_letras.width, (float)archivos.backgroundTexture_letras.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (3) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    if (IsKeyPressed(KEY_DELETE))
    {
        // Si se presiona DELETE, regresar al menú
        return JUEGO_REGRESAR_MENU;
    }

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return FULLSCREEN; // Botón 2 clickeado
        if (isMouseOverButton3)
            return JUEGO_REGRESAR_MENU; // Botón 3 clickeado
        if (isMouseOverButton4)
            return MUSICA; // Botón 4 clickeado
    }

    // Si no hay cambio de pantalla, continúa jugando
    return JUEGO_JUGANDO;
}

Texture2D GetCartaTexture_COLOR(cartas todo3, int num_carta)
{
    switch (num_carta)
    {
    case 10:
        return todo3.carta10;
    case 11:
        return todo3.carta11;
    case 12:
        return todo3.carta12;
    case 13:
        return todo3.carta13;
    case 14:
        return todo3.carta14;
    case 15:
        return todo3.carta15;
    case 16:
        return todo3.carta16;
    case 17:
        return todo3.carta17;
    case 18:
        return todo3.carta18;
    // Agrega más casos según la cantidad de cartas que tengas
    default:
        return todo3.carta_back; // Textura por defecto (carta oculta)
    }
}

void iniciar_memo_COLOR(_colores &estruct2)
{
    // Inicializar las coordenadas de las cartas volteadas

    estruct2.prim_carta_fila = -1;
    estruct2.prim_carta_columna = -1;
    estruct2.seg_carta_fila = -1;
    estruct2.seg_carta_columna = -1;

    // Inicializar el contador de cartas en estado 1
    estruct2.cartas_en_estado_1 = 0;
    // Establecer el número máximo de cartas permitidas en estado 1
    estruct2.max_cartas_estado_1 = 2;

    // Inicializar todas las cartas como false
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            estruct2.card_state[i][j] = false;
        }
    }

    // Números del 1 al 9
    int numeros_disponibles[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 10, 11, 12, 13, 14, 15, 16, 17, 18};

    // Barajar los números disponibles
    srand((unsigned int)time(NULL));
    for (int i = 17; i > 0; i--)
    {
        int j = rand() % (i + 1);
        // Intercambiar numeros_disponibles[i] y numeros_disponibles[j]
        int temp = numeros_disponibles[i];
        numeros_disponibles[i] = numeros_disponibles[j];
        numeros_disponibles[j] = temp;
    }

    // Asignar aleatoriamente las cartas
    int indice = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            // Asignar el valor a la carta
            estruct2.cartas[i][j] = numeros_disponibles[indice];

            // Imprimir las cartas asignadas
            // printf("Carta asignada en (%d, %d): %d\n", i, j, estruct.cartas[i][j]);

            indice++;
        }
    }
}

void COLOR_memoria(cartas todo3, _colores &estruct2, cargas archivos)
{
    int ancho = GetScreenWidth();
    int altura = GetScreenHeight();

    int fila = estruct2.num_fila;
    int columna = estruct2.num_columna;

    float espacioX = 10.0f;
    float espacioY = 10.0f;

    float aspecto = 179.0f / 130.0f;
    float ajustes = 130.0f;
    float ajuste_altura = ajustes * aspecto;

    Vector2 mouse = GetMousePosition();

    // Agregar una matriz para rastrear qué pares ya fueron encontrados
    static bool cartas_pares_encontrados[3][6] = {false};

    for (int i = 0; i < fila; i++)
    {
        for (int j = 0; j < columna; j++)
        {
            float posx = (ancho - columna * (ajustes + espacioX) + espacioX) / 2 + j * (ajustes + espacioX);
            float posy = (altura - fila * (ajuste_altura + espacioY) + espacioY) / 2 + i * (ajuste_altura + espacioY);

            Rectangle carta = {posx, posy, ajustes, ajuste_altura};
            bool isMouseOverCard = CheckCollisionPointRec(mouse, carta);

            // Verificar si la carta ya es parte de un par encontrado
            if (cartas_pares_encontrados[i][j])
            {
                // No permitir interacción con estas cartas
                continue;
            }

            // Chequear clic izquierdo en la carta
            if (isMouseOverCard && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                PlaySound(archivos.cardsound);
                // Verificar el estado actual de la carta
                if (!estruct2.card_state[i][j] && estruct2.cartas_en_estado_1 < estruct2.max_cartas_estado_1)
                {
                    // Si ya hay una carta volteada, verificar si es la misma
                    if (estruct2.prim_carta_fila != -1 && estruct2.prim_carta_columna != -1 &&
                        (estruct2.prim_carta_fila != i || estruct2.prim_carta_columna != j))
                    {
                        // Verificar si las cartas son iguales
                        estruct2.cartas[estruct2.seg_carta_fila][estruct2.seg_carta_columna] = estruct2.cartas[i][j];
                        int valor = estruct2.cartas[estruct2.seg_carta_fila][estruct2.seg_carta_columna];
                        int valor2 = estruct2.cartas[estruct2.prim_carta_fila][estruct2.prim_carta_columna];

                        printf("Carta volteada UNO en (%d, %d): %d\n", i, j, valor);
                        printf("Carta volteada DOS en (%d, %d): %d\n", estruct2.prim_carta_fila, estruct2.prim_carta_columna, valor2);

                        if (valor == valor2)
                        {

                            // Marcar el par como encontrado en la matriz
                            cartas_pares_encontrados[i][j] = true;
                            cartas_pares_encontrados[estruct2.prim_carta_fila][estruct2.prim_carta_columna] = true;

                            // Ambas cartas son iguales, reiniciar las coordenadas de ambas cartas
                            estruct2.prim_carta_fila = -1;
                            estruct2.prim_carta_columna = -1;
                            estruct2.seg_carta_fila = -1;
                            estruct2.seg_carta_columna = -1;
                            estruct2.cartas_en_estado_1 = 0;
                            estruct2.max_cartas_estado_1 = 2;

                            // No permitir interacción con estas cartas
                            continue;
                        }
                        else
                        {
                            // Las cartas no son iguales, esperar para volver a voltearlas
                            SetMouseCursor(MOUSE_CURSOR_ARROW);
                        }
                    }
                    else
                    {
                        // No hay carta volteada previamente, actualizar las coordenadas de la primera carta
                        estruct2.prim_carta_fila = i;
                        estruct2.prim_carta_columna = j;
                        estruct2.seg_carta_fila = -1;
                        estruct2.seg_carta_columna = -1;
                    }

                    // Cambiar el estado de la carta a 1
                    estruct2.card_state[i][j] = true;
                    estruct2.cartas_en_estado_1++;

                    // Verificar si la segunda carta ya está volteada
                    if (estruct2.seg_carta_fila != -1 && estruct2.seg_carta_columna != -1)
                    {
                        // No permitir interacción con estas cartas
                        continue;
                    }

                    // No permitir interacción con la primera carta ya volteada
                    continue;
                }
                else if (estruct2.card_state[i][j])
                {
                    // Si la carta estaba en estado 1, cambiarla a 0
                    estruct2.card_state[i][j] = false;
                    estruct2.cartas_en_estado_1--;

                    // Reiniciar las coordenadas de la primera y segunda carta
                    estruct2.prim_carta_fila = -1;
                    estruct2.prim_carta_columna = -1;
                    estruct2.seg_carta_fila = -1;
                    estruct2.seg_carta_columna = -1;
                }
            }

            // Dibujar cartas
            int num_carta = estruct2.cartas[i][j];
            Texture2D currentTexture;
            if (estruct2.card_state[i][j])
            {
                currentTexture = GetCartaTexture_COLOR(todo3, num_carta);
            }
            else
            {
                currentTexture = todo3.carta_back;
            }

            DrawTexturePro(
                currentTexture,
                (Rectangle){0, 0, (float)currentTexture.width, (float)currentTexture.height},
                carta,
                (Vector2){0, 0},
                0.0f,
                WHITE);
        }
    }
}

JuegoEstado jugar_colores(GameScreen currentScreen, cargas archivos, cartas todo3, _colores &estruct2)
{
    DrawTexturePro(
        archivos.backgroundTexture_colores,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_colores.width, (float)archivos.backgroundTexture_colores.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (3) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    COLOR_memoria(todo3, estruct2, archivos);
    if (IsKeyPressed(KEY_DELETE))
    {
        // Si se presiona DELETE, regresar al menú
        return JUEGO_REGRESAR_MENU;
    }

    /* VERIFICAR SI SE HIZO CLIC EN EL BOTÓN */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return FULLSCREEN; // Botón 2 clickeado
        if (isMouseOverButton3)
            return JUEGO_REGRESAR_MENU; // Botón 3 clickeado
        if (isMouseOverButton4)
            return MUSICA; // Botón 4 clickeado
    }

    // Si no hay cambio de pantalla, continúa jugando
    return JUEGO_JUGANDO;
}

int drawjugar(GameScreen currentScreen, cargas archivos)
{
    // Resto del código de la función drawjugar...

    DrawTexturePro(
        archivos.backgroundTexture,
        (Rectangle){0, 0, (float)archivos.backgroundTexture.width, (float)archivos.backgroundTexture.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // Usar proporciones relativas para calcular las posiciones de los botones
    float buttonStartY = 0.35f * GetScreenHeight(); // Ajusta el valor 0.35 según tus necesidades
    float buttonSpacing = 0.2f * GetScreenHeight(); // Ajusta el valor 0.2 según tus necesidades

    Rectangle buttonRectC = {(GetScreenWidth() - archivos.buttonTextureC.width) / 2, buttonStartY, (float)archivos.buttonTextureC.width, (float)archivos.buttonTextureC.height};
    Rectangle buttonRectD = {(GetScreenWidth() - archivos.buttonTextureD.width) / 2, buttonStartY + buttonSpacing, (float)archivos.buttonTextureD.width, (float)archivos.buttonTextureD.height};
    Rectangle buttonRectE = {(GetScreenWidth() - archivos.buttonTextureE.width) / 2, buttonStartY + 2 * buttonSpacing, (float)archivos.buttonTextureE.width, (float)archivos.buttonTextureE.height};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    /* VERIFICAR SI EL MOUSE ESTA SOBRE LOS BOTONES */
    bool isMouseOverButtonC = CheckCollisionPointRec(GetMousePosition(), buttonRectC);
    bool isMouseOverButtonD = CheckCollisionPointRec(GetMousePosition(), buttonRectD);
    bool isMouseOverButtonE = CheckCollisionPointRec(GetMousePosition(), buttonRectE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    /* CAMBIAR EL TAMAÑO DEL BOTON (C,D,E) SI EL MOUSE ESTA SOBRE DE EL */
    if (isMouseOverButtonC)
    {
        /* AGRANDAR Y REDUCIR (C) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectC.width = archivos.buttonTextureC.width * scale;
        buttonRectC.height = archivos.buttonTextureC.height * scale;
    }

    if (isMouseOverButtonD)
    {
        /* AGRANDAR Y REDUCIR (D) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectD.width = archivos.buttonTextureD.width * scale;
        buttonRectD.height = archivos.buttonTextureD.height * scale;
    }

    if (isMouseOverButtonE)
    {
        /* AGRANDAR Y REDUCIR (E) */
        float scale = 1.0f + 0.05f * sin(2.0f * GetTime());
        buttonRectE.width = archivos.buttonTextureE.width * scale;
        buttonRectE.height = archivos.buttonTextureE.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    /* AJUSTAR LA POSICION DEL BOTON PARA QUE ESTE EN EL CENTRO */
    Vector2 buttonPosition = {buttonRectC.x + buttonRectC.width / 2, buttonRectC.y + buttonRectE.height / 2};
    Vector2 buttonPositionD = {buttonRectD.x + buttonRectD.width / 2, buttonRectD.y + buttonRectD.height / 2};
    Vector2 buttonPositionE = {buttonRectE.x + buttonRectE.width / 2, buttonRectE.y + buttonRectE.height / 2};
    DrawTexturePro(archivos.buttonTextureC, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureC.width), static_cast<float>(archivos.buttonTextureC.height)}, (Rectangle){buttonPosition.x, buttonPosition.y, buttonRectC.width, buttonRectC.height}, (Vector2){buttonRectC.width / 2, buttonRectC.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureD, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureD.width), static_cast<float>(archivos.buttonTextureD.height)}, (Rectangle){buttonPositionD.x, buttonPositionD.y, buttonRectD.width, buttonRectD.height}, (Vector2){buttonRectD.width / 2, buttonRectD.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTextureE, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTextureE.width), static_cast<float>(archivos.buttonTextureE.height)}, (Rectangle){buttonPositionE.x, buttonPositionE.y, buttonRectE.width, buttonRectE.height}, (Vector2){buttonRectE.width / 2, buttonRectE.height / 2}, 0.0f, WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButtonC)
            return 1; // Botón C clickeado
        if (isMouseOverButtonD)
            return 2; // Botón D clickeado
        if (isMouseOverButtonE)
            return 3; // Botón D clickeado
        if (isMouseOverButton2)
            return 4; // Botón 2 clickeado
        if (isMouseOverButton3)
            return 6; // Botón 3 clickeado
        if (isMouseOverButton4)
            return 5; // Botón 4 clickeado
    }

    return 0; // Ningún botón
}

int drawcreditos(cargas archivos)
{
    DrawTexturePro(
        archivos.backgroundTexture_creditos,
        (Rectangle){0, 0, (float)archivos.backgroundTexture_creditos.width, (float)archivos.backgroundTexture_creditos.height},
        (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        (Vector2){0, 0},
        0.0f,
        WHITE);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    // Ajustar la posición y de los botones para moverlos a la parte superior de la pantalla
    float buttonY = 0;
    // Ajustar la posición x de los botones para moverlos a la derecha de la pantalla
    float button2X = GetScreenWidth() - 2 * archivos.buttonTexture2.width / 4;
    float button3X = 0;
    float button4X = GetScreenWidth() - archivos.buttonTexture4.width / 4;
    Rectangle buttonRect2 = {button2X, buttonY, (float)archivos.buttonTexture2.width / 4, (float)archivos.buttonTexture2.height / 4};
    Rectangle buttonRect3 = {button3X, GetScreenHeight() - archivos.buttonTexture3.height / 4, (float)archivos.buttonTexture3.width / 4, (float)archivos.buttonTexture3.height / 4};
    Rectangle buttonRect4 = {button4X, buttonY, (float)archivos.buttonTexture4.width / 4, (float)archivos.buttonTexture4.height / 4};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    bool isMouseOverButton2 = CheckCollisionPointRec(GetMousePosition(), buttonRect2);
    bool isMouseOverButton3 = CheckCollisionPointRec(GetMousePosition(), buttonRect3);
    bool isMouseOverButton4 = CheckCollisionPointRec(GetMousePosition(), buttonRect4);

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    if (isMouseOverButton2)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect2.width = archivos.buttonTexture2.width * scale;
        buttonRect2.height = archivos.buttonTexture2.height * scale;
    }

    if (isMouseOverButton3)
    {
        /* AGRANDAR Y REDUCIR (2) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect3.width = archivos.buttonTexture3.width * scale;
        buttonRect3.height = archivos.buttonTexture3.height * scale;
    }

    if (isMouseOverButton4)
    {
        /* AGRANDAR Y REDUCIR (4) */
        float scale = 0.2f + 0.02f * sin(2.0f * GetTime());
        buttonRect4.width = archivos.buttonTexture4.width * scale;
        buttonRect4.height = archivos.buttonTexture4.height * scale;
    }

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    Vector2 buttonPosition2 = {buttonRect2.x + buttonRect2.width / 2, buttonRect2.y + buttonRect2.height / 2};
    Vector2 buttonPosition3 = {buttonRect3.x + buttonRect3.width / 2, buttonRect3.y + buttonRect3.height / 2};
    Vector2 buttonPosition4 = {buttonRect4.x + buttonRect4.width / 2, buttonRect4.y + buttonRect4.height / 2};

    // BOTONES DE VOLUMEN Y TAMAÑO DE PANTALLA -------------------------------------
    DrawTexturePro(archivos.buttonTexture2, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture2.width), static_cast<float>(archivos.buttonTexture2.height)}, (Rectangle){buttonPosition2.x, buttonPosition2.y, buttonRect2.width, buttonRect2.height}, (Vector2){buttonRect2.width / 2, buttonRect2.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture3, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture3.width), static_cast<float>(archivos.buttonTexture3.height)}, (Rectangle){buttonPosition3.x, buttonPosition3.y, buttonRect3.width, buttonRect3.height}, (Vector2){buttonRect3.width / 2, buttonRect3.height / 2}, 0.0f, WHITE);
    DrawTexturePro(archivos.buttonTexture4, (Rectangle){0.0f, 0.0f, static_cast<float>(archivos.buttonTexture4.width), static_cast<float>(archivos.buttonTexture4.height)}, (Rectangle){buttonPosition4.x, buttonPosition4.y, buttonRect4.width, buttonRect4.height}, (Vector2){buttonRect4.width / 2, buttonRect4.height / 2}, 0.0f, WHITE);

    /* VERIFICAR SI SE HIZO CLICK EN EL BOTON */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (isMouseOverButton2)
            return 1; // Botón 2 clickeado
        if (isMouseOverButton3)
            return 2; // Botón 3 clickeado
        if (isMouseOverButton4)
            return 3; // Botón 4 clickeado
    }

    return 0; // Ningún botón
}

void menudraw(GameScreen currentScreen, cargas archivos, cartas todo, cartas todo2, cartas todo3, memorama estruct, _colores estruct2)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    switch (currentScreen)
    {
    case INICIO:
    {
        drawinicio(archivos);
        break;
    }
    case JUGAR:
    {
        drawjugar(currentScreen, archivos);
        break;
    }
    case CREDITOS:
    {
        drawcreditos(archivos);
        break;
    }
    case JUGAR_BASICO:
    {
        jugar_basico(currentScreen, archivos, todo, estruct);
        break;
    }
    case JUGAR_LETRAS:
    {
        jugar_letras(currentScreen, archivos);
        break;
    }
    case JUGAR_COLORES:
    {
        jugar_colores(currentScreen, archivos, todo3, estruct2);
        break;
    }
    default:
        break;
    }

    EndDrawing();
}

void ToggleFullscreenAndResize()
{
    // Resto del código de la función ToggleFullscreenAndResize...
    if (IsKeyPressed(KEY_F))
    {
        // Cambiar a pantalla completa al presionar F11
        ToggleFullscreen();

        // Ajustar el tamaño de la ventana al cambiar a pantalla completa
        if (IsWindowFullscreen())
        {
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }
        else
        {
            // Restaurar la resolución original al salir de pantalla completa
            screenWidth = GetMonitorWidth(0);
            screenHeight = GetMonitorHeight(0);
        }

        // Cambiar el tamaño de la ventana
        SetWindowSize(screenWidth, screenHeight);
    }
}