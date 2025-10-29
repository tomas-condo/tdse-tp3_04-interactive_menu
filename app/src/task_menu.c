/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_menu.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"
#include "math.h"
#include "stdio.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_menu_attribute.h"
#include "task_menu_interface.h"
#include "task_menu.h"         // <-- ¡AÑADIR ESTA LÍNEA!
#include "display.h"

/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI			0ul
#define G_TASK_MEN_TICK_CNT_INI		0ul

#define DEL_MEN_XX_MIN				0ul
#define DEL_MEN_XX_MED				50ul
#define DEL_MEN_XX_MAX				500ul

/********************** internal data declaration ****************************/
/*task_menu_dta_t task_menu_dta =
	{DEL_MEN_XX_MIN, ST_MEN_XX_IDLE, EV_MEN_ENT_IDLE, false};*/

task_menu_dta_t task_menu_dta =
{
    /* tick         */ DEL_MEN_XX_MIN,
    /* state        */ ST_MEN_MAIN,          // Usamos ST_MEN_MAIN como estado inicial
    /* event        */ EV_MEN_IDLE,
    /* flag         */ false,

    // CAMPOS DE ÍNDICE AÑADIDOS
    /* index_menu1  */ 0,
    /* index_menu2  */ 0,
    /* index_menu3  */ 0,

};

#define MENU_DTA_QTY	(sizeof(task_menu_dta)/sizeof(task_menu_dta_t))

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_menu 		= "Task Menu (Interactive Menu)";
const char *p_task_menu_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_menu_cnt;
volatile uint32_t g_task_menu_tick_cnt;

/********************** external functions definition ************************/
void task_menu_init(void *parameters)
{
	task_menu_dta_t *p_task_menu_dta;
	task_menu_st_t	state;
	task_menu_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_menu_init), p_task_menu);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_menu), p_task_menu_);

	/* Init & Print out: Task execution counter */
	g_task_menu_cnt = G_TASK_MEN_CNT_INI;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_menu_cnt), g_task_menu_cnt);

	init_queue_event_task_menu();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_menu_dta = &task_menu_dta;

	/* Init & Print out: Task execution FSM */
	state = ST_MEN_XX_IDLE;
	p_task_menu_dta->state = state;

	event = EV_MEN_IDLE;
	p_task_menu_dta->event = event;

	b_event = false;
	p_task_menu_dta->flag = b_event;

	LOGGER_INFO(" ");
	LOGGER_INFO("   %s = %lu   %s = %lu   %s = %s",
				 GET_NAME(state), (uint32_t)state,
				 GET_NAME(event), (uint32_t)event,
				 GET_NAME(b_event), (b_event ? "true" : "false"));

	/* Init & Print out: LCD Display */
	displayInit( DISPLAY_CONNECTION_GPIO_4BITS );

    displayCharPositionWrite(0, 0);
	displayStringWrite("TdSE Bienvenidos");

	displayCharPositionWrite(0, 1);
	displayStringWrite("Test Nro: ");
}

void task_menu_statechart(void) {
    task_menu_dta_t *p_task_menu_dta;
    /* Update Task Menu Data Pointer */
    p_task_menu_dta = &task_menu_dta;

    // ---------------------------------------------------------------------
    // 1. Manejo de Eventos
    // ---------------------------------------------------------------------
    if (true == any_event_task_menu()) {
        p_task_menu_dta -> flag = true;
        p_task_menu_dta -> event = get_event_task_menu();
    }

    // ---------------------------------------------------------------------
    // 2. Máquina de Estados (solo si hay un evento para procesar)
    // ---------------------------------------------------------------------
    if (true == p_task_menu_dta -> flag) {
        switch (p_task_menu_dta -> state) {
            // =============================================================
            // ESTADO MAIN
            // =============================================================
            case ST_MEN_MAIN:
                if (p_task_menu_dta -> event == EV_MEN_ENTER) {
                    // Transición MAIN -> MENU 1
                	displayCharPositionWrite(0, 0);
					displayStringWrite("Bienvenido al Menu 1");
					for(int i = 1; i < 4; i++){
						displayCharPositionWrite(0, i);
						displayStringWrite("                    ");
					};
					displayCharPositionWrite(0, 2);
					displayStringWrite(">Motor 1     Motor 2");
                    p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                    p_task_menu_dta -> index_menu1 = 0; //reset
                }
                else if (p_task_menu_dta->event == EV_MEN_NEXT || p_task_menu_dta->event == EV_MEN_ESC)
                    // Transición MAIN -> MAIN
                    p_task_menu_dta->state = ST_MEN_MAIN;
                break;
            // =============================================================
            // ESTADO MENU 1
            // =============================================================
            case ST_MEN_MENU1:
                if (p_task_menu_dta -> event == EV_MEN_NEXT){
                    if (p_task_menu_dta -> index_menu1 < 1){ // [guard] index_menu1 < 2
                        // Acción: index_menu1 ++
                    	displayCharPositionWrite(0, 2);
                    	displayStringWrite(" Motor 1    >Motor 2");
                        p_task_menu_dta -> index_menu1++;
                    }
                    else if (p_task_menu_dta->index_menu1 == 1){ // [guard] index_menu1 == 2
                        // Acción: index_menu1 = 0 (Wrap-around)
                        p_task_menu_dta -> index_menu1 = 0;
                        displayCharPositionWrite(0, 2);
                        displayStringWrite(">Motor 1     Motor 2");
                    }
                    // La transición es a MENU 1 en ambos casos
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                }
                else if (p_task_menu_dta -> event == EV_MEN_ESC){
                    // Transición MENU 1 -> MAIN
                    p_task_menu_dta -> state = ST_MEN_MAIN;
                    p_task_menu_dta -> index_menu1 = 0; // Reset
                }
                else if (p_task_menu_dta -> event == EV_MEN_ENTER){
                    // Transición MENU 1 -> MENU 2
                	displayCharPositionWrite(0, 0);
                	displayStringWrite("Bienvenido al Menu 2");
                	displayCharPositionWrite(0, 2);
                	displayStringWrite(">Power  Speed  Spin ");

                	p_task_menu_dta -> flag = false;
                    p_task_menu_dta -> state = ST_MEN_MENU2;
                    p_task_menu_dta -> index_menu2 = 0; // Entramos a MENU 2, iniciamos su índice
                }
                break;

            // =============================================================
            // ESTADO MENU 2
            // =============================================================

            case ST_MEN_MENU2:
                if (p_task_menu_dta -> event == EV_MEN_NEXT)
                {
                    if (p_task_menu_dta -> index_menu2 == 0){ // [guard] index_menu2 < 3
                        // Acción: index_menu2 ++
                    	displayCharPositionWrite(0, 2);
                    	displayStringWrite(" Power >Speed  Spin ");
                        p_task_menu_dta -> index_menu2++;
                    }
                    else if (p_task_menu_dta -> index_menu2 == 1){ // [guard] index_menu2 < 3
						// Acción: index_menu2 ++
                        displayCharPositionWrite(0, 2);
                        displayStringWrite(" Power  Speed >Spin");

						p_task_menu_dta -> index_menu2++;
					}
                    else if (p_task_menu_dta -> index_menu2 == 2){ // [guard] index_menu2 == 3
                        // Acción: index_menu2 = 0 (Wrap-around)
                        displayCharPositionWrite(0, 2);
                        displayStringWrite(">Power  Speed  Spin ");
                        p_task_menu_dta -> index_menu2 = 0;
                    }
                    // La transición es a MENU 2 en ambos casos
                    p_task_menu_dta -> state = ST_MEN_MENU2;
                }
                else if (p_task_menu_dta -> event == EV_MEN_ESC) {
                    // Transición MENU 2 -> MENU 1
					displayCharPositionWrite(0, 2);
					displayStringWrite(">Motor 1     Motor 2");
                    p_task_menu_dta -> state = ST_MEN_MENU1;
                    p_task_menu_dta->index_menu1 = 0;
                }
                else if (p_task_menu_dta->event == EV_MEN_ENTER) {
                    // Transición MENU 2 -> MENU 3
                	displayCharPositionWrite(0, 0);
					displayStringWrite("Bienvenido al Menu 3");

                	p_task_menu_dta -> flag = false;
                    p_task_menu_dta->state = ST_MEN_MENU3;
                    p_task_menu_dta->index_menu3 = 0; // Entramos a MENU 3, iniciamos su índice
                    if (p_task_menu_dta -> index_menu2 == 0){
                    	displayCharPositionWrite(0, 2);
                    	displayStringWrite(">ON             OFF");
                    }
                    else if (p_task_menu_dta -> index_menu2 == 1){
                    	displayCharPositionWrite(0, 2);
                    	displayStringWrite(">0 1 2 3 4 5 6 7 8 9");
                    }
                    else if (p_task_menu_dta -> index_menu2 == 2){
                    	displayCharPositionWrite(0, 2);
                    	displayStringWrite(">LEFT          RIGHT");
                    }
                }
                break;

            // =============================================================
            // ESTADO MENU 3
            // =============================================================

            case ST_MEN_MENU3:
            	if (p_task_menu_dta -> index_menu2 == 0){
            		if (p_task_menu_dta->event == EV_MEN_NEXT){
            			if(p_task_menu_dta -> index_menu3 == 0){
							p_task_menu_dta->index_menu3++; // Acción: index_menu3++
							displayCharPositionWrite(0, 2);
							displayStringWrite(" ON            >OFF");
            			}
            			else if(p_task_menu_dta -> index_menu3 == 1){
							p_task_menu_dta->index_menu3--; // Acción: index_menu3--
							displayCharPositionWrite(0, 2);
							displayStringWrite(">ON             OFF");
            			}
            		}
            		//power -> enter (cami dice: no me acuerdo xq escribimos esto con f)

				}
				else if (p_task_menu_dta -> index_menu2 == 1){
					if (p_task_menu_dta->event == EV_MEN_NEXT){
						if(p_task_menu_dta -> index_menu3 == 0){
							p_task_menu_dta->index_menu3++; // Acción: index_menu3++
							displayCharPositionWrite(0, 2);
							displayStringWrite(">0 1 2 3 4 5 6 7 8 9"); //agregar cada > a cada numero
						}
					}
				}
				else if (p_task_menu_dta -> index_menu2 == 2){
					if (p_task_menu_dta->event == EV_MEN_NEXT){
						if(p_task_menu_dta -> index_menu3 == 0){
							p_task_menu_dta->index_menu3++; // Acción: index_menu3++
							displayCharPositionWrite(0, 2);
							displayStringWrite(" LEFT         >RIGHT");
						}
						else if(p_task_menu_dta -> index_menu3 == 1){
							p_task_menu_dta->index_menu3--; // Acción: index_menu3--
							displayCharPositionWrite(0, 2);
							displayStringWrite(">LEFT          RIGHT");
						}
					}
				}





                if (p_task_menu_dta->event == EV_MEN_NEXT){
                	if ((p_task_menu_dta->index_menu2 == 0) && (p_task_menu_dta->index_menu3 < 3)){

                	}
                    else if ((p_task_menu_dta->index_menu2 == 1) && (p_task_menu_dta->index_menu3 < 10)){
                        // Transición MENU 3 -> MENU 3
                        p_task_menu_dta->index_menu3++; // Acción: index_menu3++
                		for(int i = 0; i < 20; i++){
							displayCharPositionWrite(i, 2);
							displayStringWrite("%d0 1 2 3 4 5 6 7 8 9");
						}
					}
                    else if ((p_task_menu_dta->index_menu2 == 1) && (p_task_menu_dta->index_menu3 == 10)){
                        // Acción: Ninguna (si el wrap-around es 0, va index_menu3 = 0.
                        // Según la tabla, la acción está VACÍA, sigue en 10).
                    }
                    // NOTA: Las últimas dos guardas (index_menu3 < 2 y index_menu3 == 2) no tienen acciones y son redundantes con la lógica de menú2.

                    p_task_menu_dta -> state = ST_MEN_MENU3;
                }
                else if (p_task_menu_dta->event == EV_MEN_ESC) {
                    // Transición MENU 3 -> MENU 2
                    p_task_menu_dta->state = ST_MEN_MENU2;
                    p_task_menu_dta->index_menu2 = 0;
                }
                else if (p_task_menu_dta->event == EV_MEN_ENTER)
                {
                    // Transición MENU 3 -> MENU 3 (No hay acción)
                	displayCharPositionWrite(0, 0);
					displayStringWrite("Seguimos en Menu 3  ");
					for(int i = 1; i < 4; i++){
						displayCharPositionWrite(0, i);
						displayStringWrite("                    ");
					};
                	p_task_menu_dta -> flag = false;
                    p_task_menu_dta->state = ST_MEN_MENU3;
                }
                break;

            default:
                p_task_menu_dta->state = ST_MEN_MAIN;
                break;
        }

        p_task_menu_dta->flag = false;
    }
    // ---------------------------------------------------------------------
    // 3. Lógica de Display (para actualizar el LCD)
    // ---------------------------------------------------------------------

}

void task_menu_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_menu_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_menu_cnt++;

		/* Run Task Menu Statechart */
    	task_menu_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_menu_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
	}
}

/********************** end of file ******************************************/
