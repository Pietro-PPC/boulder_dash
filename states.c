#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include "states.h"
#include "map.h"
#include "error_msg.h"
#include "sprites.h"
#include "draw.h"

ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_EVENT event;

map_t map;
sprites_t sprites;

bool redraw = true;
bool done = false;

void state_initialize()
// Inicializa todas as variáveis e structs necessárias para o jogo
{
    // inicializa allegro
    test_init(al_init(), "Allegro");

    // inicializa teclado
    test_init(al_install_keyboard(), "teclado");

    // inicializa addon de imagens
    test_init(al_init_image_addon(), "addon de imagem");

    // inicializa timer de 1/FRAMERATE segundos
    timer = al_create_timer(FRAMERATE);
    test_init(timer, "temporizador");

    // inicializa display com altura e largura especificadas nas macros
    disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    test_init(disp, "tela de jogo");

    // inicializa fonte built-in
    font = al_create_builtin_font();
    test_init(font, "fonte");
    // inicializa fila de eventos
    queue = al_create_event_queue();
    test_init(queue, "fila de eventos");

    // registra teclado, display e timer como fontes de eventos da fila queue
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // inicializa variáveis de controle
    al_start_timer(timer);

    // le mapa do jogo
    read_map(&map);

    // inicializa sprites
    initialize_sprites(&sprites);
    init_sprites(&sprites);
}

void state_play()
{
    // loop principal
    while(true)
    {
        al_wait_for_event(queue, &event);
        
        // age conforme evento ocorrido
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
            case ALLEGRO_EVENT_KEY_DOWN:
                done = true;
                break;
        }

        if (done)
            break;
        
        // desenhar na tela
        if (redraw)
        {
            al_clear_to_color(al_map_rgb(255, 255, 255));
//          al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello World!");

            draw_map(&sprites, &map);
//          al_draw_bitmap(sprites.border, 20, 20, 0);

            al_flip_display();
        }
    }
}

void state_end()
{
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroy_sprites(&sprites);

    destroy_map(&map);

    exit(0);
}