#include <stdio.h>
#include <allegro5/allegro.h>
#include <vector>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <omp.h>
#include <algorithm>

using namespace std;
// Variables globales
int  t_X(1080), t_Y(720);
double G(10000);
double K(0.5);
double masa = 10, radio = 200;
double f(500);
bool brillo(TRUE);
bool caras(TRUE);
/////////////

// bolas
typedef struct cuerpo {
	double x, y, z, radio;
	double v_x, v_y, v_z;//, a_x, a_y, a_z;
	int col1, col2, col3;	
	double dluz_x, dluz_y, dluz_z, dist_luz;
} cuerpo;

// col, obs, centro de cubos, vertices de cubo
typedef struct entidad {
	double x, y, z;
} entidad;

//
typedef struct cubo {
	entidad centro,v1,v2,v3,v4,v5,v6,v7,v8;
	double lado;
	double wx, wy, wz;
	int r, g, b;
	double v_x, v_y, v_z;
	int carga;
} cubo;

// crear un  cubo con parámetros aleatorios
cubo definir_cubo()
{
	cubo c;
	entidad centro;
	int area(20000);
	centro.x = ((double(rand()) / double(RAND_MAX)) * (area / 2 - -area / 2)) + -area / 2;
	centro.y = ((double(rand()) / double(RAND_MAX)) * (area / 2 - -area / 2)) + -area / 2;
	centro.z = ((double(rand()) / double(RAND_MAX)) * (area / 2 - -area / 2)) + -area / 2;
	
	c.centro = centro;
	c.lado = rand() % 6000 - 2000;
	double l = c.lado;

	// vertices en plano paralelo

	c.v1.x = centro.x + l / 2;
	c.v1.y = centro.y - l / 2;
	c.v1.z = centro.z - l / 2;

	c.v2.x = centro.x + l / 2;
	c.v2.y = centro.y + l / 2;
	c.v2.z = centro.z - l / 2;

	c.v3.x = centro.x - l / 2;
	c.v3.y = centro.y + l / 2;
	c.v3.z = centro.z - l / 2;

	c.v4.x = centro.x - l / 2;
	c.v4.y = centro.y - l / 2;
	c.v4.z = centro.z - l / 2;

	// vertices en plano paralelo

	c.v5.x = centro.x + l / 2;
	c.v5.y = centro.y - l / 2;
	c.v5.z = centro.z + l / 2;

	c.v6.x = centro.x + l / 2;
	c.v6.y = centro.y + l / 2;
	c.v6.z = centro.z + l / 2;

	c.v7.x = centro.x - l / 2;
	c.v7.y = centro.y + l / 2;
	c.v7.z = centro.z + l / 2;

	c.v8.x = centro.x - l / 2;
	c.v8.y = centro.y - l / 2;
	c.v8.z = centro.z + l / 2;

	// velocidad angular en los 3 ejes

	c.wx =  rand() % 3 + 1;
	c.wy = rand() % 3 + 1;
	c.wz =  rand() % 3 + 1;

	// color 
	c.r = rand() % 204 + 1;
	c.g = rand() % 204 + 1;
	c.b = rand() % 204 + 1;

	
	return c;
}

void rotar_cubo(cubo &c, double tiempo)
{

	double t(tiempo);
	cubo cn(c);

	// trasladar al origen

	cn.v1.x -= cn.centro.x;
	cn.v1.y -= cn.centro.y;
	cn.v1.z -= cn.centro.z;

	cn.v2.x -= cn.centro.x;
	cn.v2.y -= cn.centro.y;
	cn.v2.z -= cn.centro.z;

	cn.v3.x -= cn.centro.x;
	cn.v3.y -= cn.centro.y;
	cn.v3.z -= cn.centro.z;

	cn.v4.x -= cn.centro.x;
	cn.v4.y -= cn.centro.y;
	cn.v4.z -= cn.centro.z;

	cn.v5.x -= cn.centro.x;
	cn.v5.y -= cn.centro.y;
	cn.v5.z -= cn.centro.z;

	cn.v6.x -= cn.centro.x;
	cn.v6.y -= cn.centro.y;
	cn.v6.z -= cn.centro.z;

	cn.v7.x -= cn.centro.x;
	cn.v7.y -= cn.centro.y;
	cn.v7.z -= cn.centro.z;

	cn.v8.x -= cn.centro.x;
	cn.v8.y -= cn.centro.y;
	cn.v8.z -= cn.centro.z;

	c = cn;

	// eje y
	if (c.wy != 0)
	{
		cn.v1.x = cos(c.wy*t)*c.v1.x - sin(c.wy*t)*c.v1.z;
		cn.v1.z = sin(c.wy*t)*c.v1.x + cos(c.wy*t)*c.v1.z;

		cn.v2.x = cos(c.wy*t)*c.v2.x - sin(c.wy*t)*c.v2.z;
		cn.v2.z = sin(c.wy*t)*c.v2.x + cos(c.wy*t)*c.v2.z;

		cn.v3.x = cos(c.wy*t)*c.v3.x - sin(c.wy*t)*c.v3.z;
		cn.v3.z = sin(c.wy*t)*c.v3.x + cos(c.wy*t)*c.v3.z;

		cn.v4.x = cos(c.wy*t)*c.v4.x - sin(c.wy*t)*c.v4.z;
		cn.v4.z = sin(c.wy*t)*c.v4.x + cos(c.wy*t)*c.v4.z;

		cn.v5.x = cos(c.wy*t)*c.v5.x - sin(c.wy*t)*c.v5.z;
		cn.v5.z = sin(c.wy*t)*c.v5.x + cos(c.wy*t)*c.v5.z;

		cn.v6.x = cos(c.wy*t)*c.v6.x - sin(c.wy*t)*c.v6.z;
		cn.v6.z = sin(c.wy*t)*c.v6.x + cos(c.wy*t)*c.v6.z;

		cn.v7.x = cos(c.wy*t)*c.v7.x - sin(c.wy*t)*c.v7.z;
		cn.v7.z = sin(c.wy*t)*c.v7.x + cos(c.wy*t)*c.v7.z;

		cn.v8.x = cos(c.wy*t)*c.v8.x - sin(c.wy*t)*c.v8.z;
		cn.v8.z = sin(c.wy*t)*c.v8.x + cos(c.wy*t)*c.v8.z;

		
	}
	
	c = cn;

	// eje x

	if (c.wx != 0)
	{
		cn.v1.y = cos(c.wx*t)*c.v1.y - sin(c.wx*t)*c.v1.z;
		cn.v1.z = sin(c.wx*t)*c.v1.y + cos(c.wx*t)*c.v1.z;

		cn.v2.y = cos(c.wx*t)*c.v2.y - sin(c.wx*t)*c.v2.z;
		cn.v2.z = sin(c.wx*t)*c.v2.y + cos(c.wx*t)*c.v2.z;

		cn.v3.y = cos(c.wx*t)*c.v3.y - sin(c.wx*t)*c.v3.z;
		cn.v3.z = sin(c.wx*t)*c.v3.y + cos(c.wx*t)*c.v3.z;

		cn.v4.y = cos(c.wx*t)*c.v4.y - sin(c.wx*t)*c.v4.z;
		cn.v4.z = sin(c.wx*t)*c.v4.y + cos(c.wx*t)*c.v4.z;

		cn.v5.y = cos(c.wx*t)*c.v5.y - sin(c.wx*t)*c.v5.z;
		cn.v5.z = sin(c.wx*t)*c.v5.y + cos(c.wx*t)*c.v5.z;

		cn.v6.y = cos(c.wx*t)*c.v6.y - sin(c.wx*t)*c.v6.z;
		cn.v6.z = sin(c.wx*t)*c.v6.y + cos(c.wx*t)*c.v6.z;

		cn.v7.y = cos(c.wx*t)*c.v7.y - sin(c.wx*t)*c.v7.z;
		cn.v7.z = sin(c.wx*t)*c.v7.y + cos(c.wx*t)*c.v7.z;

		cn.v8.y = cos(c.wx*t)*c.v8.y - sin(c.wx*t)*c.v8.z;
		cn.v8.z = sin(c.wx*t)*c.v8.y + cos(c.wx*t)*c.v8.z;


	}

	c = cn;
	// eje z

	if (c.wz != 0)
	{
		cn.v1.x = cos(c.wz*t)*c.v1.x - sin(c.wz*t)*c.v1.y;
		cn.v1.y = sin(c.wz*t)*c.v1.x + cos(c.wz*t)*c.v1.y;

		cn.v2.x = cos(c.wz*t)*c.v2.x - sin(c.wz*t)*c.v2.y;
		cn.v2.y = sin(c.wz*t)*c.v2.x + cos(c.wz*t)*c.v2.y;

		cn.v3.x = cos(c.wz*t)*c.v3.x - sin(c.wz*t)*c.v3.y;
		cn.v3.y = sin(c.wz*t)*c.v3.x + cos(c.wz*t)*c.v3.y;

		cn.v4.x = cos(c.wz*t)*c.v4.x - sin(c.wz*t)*c.v4.y;
		cn.v4.y = sin(c.wz*t)*c.v4.x + cos(c.wz*t)*c.v4.y;

		cn.v5.x = cos(c.wz*t)*c.v5.x - sin(c.wz*t)*c.v5.y;
		cn.v5.y = sin(c.wz*t)*c.v5.x + cos(c.wz*t)*c.v5.y;

		cn.v6.x = cos(c.wz*t)*c.v6.x - sin(c.wz*t)*c.v6.y;
		cn.v6.y = sin(c.wz*t)*c.v6.x + cos(c.wz*t)*c.v6.y;

		cn.v7.x = cos(c.wz*t)*c.v7.x - sin(c.wz*t)*c.v7.y;
		cn.v7.y = sin(c.wz*t)*c.v7.x + cos(c.wz*t)*c.v7.y;

		cn.v8.x = cos(c.wz*t)*c.v8.x - sin(c.wz*t)*c.v8.y;
		cn.v8.y = sin(c.wz*t)*c.v8.x + cos(c.wz*t)*c.v8.y;


	}

	
	// trasladar a posición original

	cn.v1.x += cn.centro.x;
	cn.v1.y += cn.centro.y;
	cn.v1.z += cn.centro.z;

	cn.v2.x += cn.centro.x;
	cn.v2.y += cn.centro.y;
	cn.v2.z += cn.centro.z;

	cn.v3.x += cn.centro.x;
	cn.v3.y += cn.centro.y;
	cn.v3.z += cn.centro.z;

	cn.v4.x += cn.centro.x;
	cn.v4.y += cn.centro.y;
	cn.v4.z += cn.centro.z;

	cn.v5.x += cn.centro.x;
	cn.v5.y += cn.centro.y;
	cn.v5.z += cn.centro.z;

	cn.v6.x += cn.centro.x;
	cn.v6.y += cn.centro.y;
	cn.v6.z += cn.centro.z;

	cn.v7.x += cn.centro.x;
	cn.v7.y += cn.centro.y;
	cn.v7.z += cn.centro.z;

	cn.v8.x += cn.centro.x;
	cn.v8.y += cn.centro.y;
	cn.v8.z += cn.centro.z;

	c = cn;



}


// rotaciónd de todos los cubos del vector de cubos

void actualizar_cubos(vector<cubo> &vi, double tiempo)
{
		double t = tiempo / 1000;
	
	omp_set_num_threads(8);
#pragma omp parallel for schedule(dynamic) 
	for (int i = 0; i < vi.size(); ++i)
	{
			rotar_cubo(vi[i], t);
	}

}


// proyección del vector normal a un triángulo con vector al observador o fuente de luz 


double proy_vec(entidad p1, entidad p2, entidad centro, entidad sol_o_obs)
{
	entidad v1, v2, normal;
	v1.x = p1.x - centro.x;
	v1.y = p1.y - centro.y;
	v1.z = p1.z - centro.z;

	v2.x = p2.x - centro.x;
	v2.y = p2.y - centro.y;
	v2.z = p2.z - centro.z;

	// n = v1 x v2
	normal.x = v1.y*v2.z - v2.y*v1.z;
	normal.y = -v1.x*v2.z + v2.x*v1.z;
	normal.z = v1.x*v2.y - v2.x*v1.y;

	

	double mod = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));

	normal.x /= mod;
	normal.y /= mod;
	normal.z /= mod;

	// proy sobre vector hasta luz u observador
	entidad vluz;
	entidad centro_cara;

	centro_cara.x = centro.x + v1.x / 2 + v2.x / 2;
	centro_cara.y = centro.y + v1.y / 2 + v2.y / 2;
	centro_cara.z = centro.z + v1.z / 2 + v2.z / 2;

	
	vluz.x = sol_o_obs.x - centro_cara.x;
	vluz.y = sol_o_obs.y - centro_cara.y;
	vluz.z = sol_o_obs.z - centro_cara.z;

	
	mod = sqrt(pow(vluz.x, 2) + pow(vluz.y, 2) + pow(vluz.z, 2));


	vluz.x /= mod;
	vluz.y /= mod;
	vluz.z /= mod;

	double proy;

	// producto escalar
	proy = normal.x*vluz.x + normal.y*vluz.y + normal.z*vluz.z;

		
	return proy;

}



void dibujar_cubo(cubo c, entidad obs, entidad sol)
{
	double proy;
	int min_col(20);
	entidad v1p, v2p, v3p, v4p, v5p, v6p, v7p, v8p;
	
	v1p.x = int(f*(c.v1.x + obs.x) / (c.v1.z - obs.z) + t_X / 2);
	v1p.y = t_Y - int(f*(c.v1.y + obs.y) / (c.v1.z - obs.z) + t_Y / 2);

	v2p.x = int(f*(c.v2.x + obs.x) / (c.v2.z - obs.z) + t_X / 2);
	v2p.y = t_Y - int(f*(c.v2.y + obs.y) / (c.v2.z - obs.z) + t_Y / 2);

	v3p.x = int(f*(c.v3.x + obs.x) / (c.v3.z - obs.z) + t_X / 2);
	v3p.y = t_Y - int(f*(c.v3.y + obs.y) / (c.v3.z - obs.z) + t_Y / 2);

	v4p.x = int(f*(c.v4.x + obs.x) / (c.v4.z - obs.z) + t_X / 2);
	v4p.y = t_Y - int(f*(c.v4.y + obs.y) / (c.v4.z - obs.z) + t_Y / 2);

	v5p.x = int(f*(c.v5.x + obs.x) / (c.v5.z - obs.z) + t_X / 2);
	v5p.y = t_Y - int(f*(c.v5.y + obs.y) / (c.v5.z - obs.z) + t_Y / 2);

	v6p.x = int(f*(c.v6.x + obs.x) / (c.v6.z - obs.z) + t_X / 2);
	v6p.y = t_Y - int(f*(c.v6.y + obs.y) / (c.v6.z - obs.z) + t_Y / 2);

	v7p.x = int(f*(c.v7.x + obs.x) / (c.v7.z - obs.z) + t_X / 2);
	v7p.y = t_Y - int(f*(c.v7.y + obs.y) / (c.v7.z - obs.z) + t_Y / 2);

	v8p.x = int(f*(c.v8.x + obs.x) / (c.v8.z - obs.z) + t_X / 2);
	v8p.y = t_Y - int(f*(c.v8.y + obs.y) / (c.v8.z - obs.z) + t_Y / 2);



	if (c.centro.z > obs.z + f){

		// si la opción de colorear las caras del cubo está activada con Z
		if (caras)

		{			

			if (proy_vec(c.v4, c.v2, c.v1, obs) > 0)
			{
				proy = abs(proy_vec(c.v4, c.v2, c.v1, sol));
				al_draw_filled_triangle(v1p.x, v1p.y, v2p.x, v2p.y,	v4p.x, v4p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));
			}


			if (proy_vec(c.v2, c.v5, c.v1, obs) > 0)
			{
				proy = abs(proy_vec(c.v2, c.v5, c.v1, sol));
				al_draw_filled_triangle(v1p.x, v1p.y, v2p.x, v2p.y, v5p.x, v5p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}


			if (proy_vec(c.v5, c.v4, c.v1, obs) > 0)
			{
				proy = abs(proy_vec(c.v5, c.v4, c.v1, sol));
				al_draw_filled_triangle(v1p.x, v1p.y, v5p.x, v5p.y, v4p.x, v4p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));
			}


			//
			

			if (proy_vec(c.v5, c.v2, c.v6, obs) > 0)
			{
				proy = abs(proy_vec(c.v5, c.v2, c.v6, sol));
				al_draw_filled_triangle(v6p.x, v6p.y, v2p.x, v2p.y, v5p.x, v5p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}

			if (proy_vec(c.v2, c.v7, c.v6, obs) > 0)
			{
				proy = abs(proy_vec(c.v2, c.v7, c.v6, sol));
				al_draw_filled_triangle(v6p.x, v6p.y, v2p.x, v2p.y, v7p.x, v7p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}


			if (proy_vec(c.v7, c.v5, c.v6, obs) > 0)
			{
				proy = abs(proy_vec(c.v7, c.v5, c.v6, sol));
				al_draw_filled_triangle(v6p.x, v6p.y, v7p.x, v7p.y, v5p.x, v5p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}



			//
			
			if (proy_vec(c.v7, c.v2, c.v3, obs) > 0)
			{
				proy = abs(proy_vec(c.v7, c.v2, c.v3, sol));
				al_draw_filled_triangle(v3p.x, v3p.y, v2p.x, v2p.y, v7p.x, v7p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}
			if (proy_vec(c.v2, c.v4, c.v3, obs) > 0)
			{
				proy = abs(proy_vec(c.v2, c.v4, c.v3, sol));
				al_draw_filled_triangle(v3p.x, v3p.y, v2p.x, v2p.y, v4p.x, v4p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}

			if (proy_vec(c.v4, c.v7, c.v3, obs) > 0)
			{
				proy = abs(proy_vec(c.v4, c.v7, c.v3, sol));
				al_draw_filled_triangle(v3p.x, v3p.y, v7p.x, v7p.y, v4p.x, v4p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}

			//
			

			if (proy_vec(c.v7, c.v4, c.v8, obs) > 0)
			{
				proy = abs(proy_vec(c.v7, c.v4, c.v8, sol));
				al_draw_filled_triangle(v8p.x, v8p.y, v7p.x, v7p.y, v4p.x, v4p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			
			}
			if (proy_vec(c.v4, c.v5, c.v8, obs) > 0)
			{
				proy = abs(proy_vec(c.v4, c.v5, c.v8, sol));
				al_draw_filled_triangle(v8p.x, v8p.y, v5p.x, v5p.y, v4p.x, v4p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}

			if (proy_vec(c.v5, c.v7, c.v8, obs) > 0)
			{
				proy = abs(proy_vec(c.v5, c.v7, c.v8, sol));
				al_draw_filled_triangle(v8p.x, v8p.y, v7p.x, v7p.y, v5p.x, v5p.y, al_map_rgb(c.r*proy + min_col, c.g*proy + min_col, c.b*proy + min_col));

			}
		}

// dibuja las aristas del cubo si la opción de dibujar las caras del cubo está desactivada Z
		else
		{

			
			al_draw_line(v1p.x, v1p.y, v2p.x, v2p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v1p.x, v1p.y, v4p.x, v4p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v1p.x, v1p.y, v5p.x, v5p.y, al_map_rgb(c.r, c.g, c.b), 2);

						
			al_draw_line(v6p.x, v6p.y, v5p.x, v5p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v6p.x, v6p.y, v2p.x, v2p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v6p.x, v6p.y, v7p.x, v7p.y, al_map_rgb(c.r, c.g, c.b), 2);
			
			al_draw_line(v3p.x, v3p.y, v4p.x, v4p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v3p.x, v3p.y, v2p.x, v2p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v3p.x, v3p.y, v7p.x, v7p.y, al_map_rgb(c.r, c.g, c.b), 2);

			al_draw_line(v8p.x, v8p.y, v4p.x, v4p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v8p.x, v8p.y, v5p.x, v5p.y, al_map_rgb(c.r, c.g, c.b), 2);
			al_draw_line(v8p.x, v8p.y, v7p.x, v7p.y, al_map_rgb(c.r, c.g, c.b), 2);

		}
	
	}


	
	
}


// dibuja todos los cubos del vector de cubos

void dibujar_todos_cubos(vector<cubo> &v, entidad obs, entidad sol)
{
	for (int i = 0; i < v.size(); ++i)
	{
		dibujar_cubo(v[i], obs, sol);
	}
}


void dibujar_bolas(vector<cuerpo> v, double e_t, int y_plano_rebote, entidad obs, entidad sol)
{
	double pant_x, pant_y;
	double dluz_x, dluz_y, dluz_z, dist_luz;
	

	al_clear_to_color(al_map_rgb(0, 0, 0));


	// lineas suelo

	int c_gran(6000);
	
	al_draw_line(int(f*(-c_gran + obs.x) / (-c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (-c_gran - obs.z) + t_Y / 2), int(f*(c_gran + obs.x) / (-c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (-c_gran - obs.z) + t_Y / 2), al_map_rgb(255, 255, 255), 2);
	al_draw_line(int(f*(-c_gran + obs.x) / (c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (c_gran - obs.z) + t_Y / 2), int(f*(c_gran + obs.x) / (c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (c_gran - obs.z) + t_Y / 2), al_map_rgb(255, 255, 255), 2);

	al_draw_line(int(f*(-c_gran + obs.x) / (-c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (-c_gran - obs.z) + t_Y / 2), int(f*(-c_gran + obs.x) / (c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (c_gran - obs.z) + t_Y / 2), al_map_rgb(255, 255, 255), 2);
	al_draw_line(int(f*(c_gran + obs.x) / (-c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (-c_gran - obs.z) + t_Y / 2), int(f*(c_gran + obs.x) / (c_gran - obs.z) + t_X / 2), t_Y - int(f*(y_plano_rebote + obs.y) / (c_gran - obs.z) + t_Y / 2), al_map_rgb(255, 255, 255), 2);
	
	
	al_draw_line(0, int(t_Y / 2), t_X, int(t_Y / 2), al_map_rgb(255, 255, 255), 1);

	for (int i = 0; i < v.size(); ++i)
	{

		
		if (v[i].z > obs.z + f)
		{
			pant_x = f*(v[i].x + obs.x) / (v[i].z - obs.z) + t_X / 2;
			pant_y = f*(v[i].y + obs.y) / (v[i].z - obs.z) + t_Y / 2;

			al_draw_filled_circle(int(pant_x), t_Y - int(pant_y), int(f*radio / (v[i].z - obs.z)) + 1, al_map_rgb(v[i].col1, v[i].col2, v[i].col3));

			
			// dibuja el brillo de las bolas si esta activado con B
			if (brillo && sol.z < v[i].z)
			{
						

				pant_x = f*(v[i].x + obs.x + 0.5*radio*v[i].dluz_x) / (v[i].z - obs.z + 0.5*radio*v[i].dluz_z) + t_X / 2;
				pant_y = f*(v[i].y + obs.y + 0.5*radio*v[i].dluz_y) / (v[i].z - obs.z + 0.5*radio*v[i].dluz_z) + t_Y / 2;

				al_draw_filled_circle(int(pant_x), t_Y - int(pant_y), int(f*0.4*radio / (v[i].z - obs.z)) + 1, al_map_rgb(v[i].col1 + 50, v[i].col2 + 50, v[i].col3 + 50));
				

			}
		}


	}
	
}



// criterio del algoritmo sort para ordenar los elementos por distancia a la pantalla
bool criterio_particulas(cuerpo i, cuerpo j)
{
	return i.z > j.z;
}

bool criterio_cubos(cubo i, cubo j)
{
	return i.centro.z > j.centro.z;
}


// para entrada por teclado

bool getInput(char *c)
{
	if (_kbhit())
	{
		*c = _getch();
		return true; // Key Was Hit
	}
	return false; // No keys were pressed
}


// calcular el desplazamiento de las bolas

void calcular_desplazamiento(vector<cuerpo> &vi, double e_t, int y_plano_rebote, entidad obs, entidad sol)
{
	int tam(vi.size()), i, j;
	vector<cuerpo> v = vi;
	double tiempo = e_t / 1000.0f;
	double dq;
	double a, b, c, d, e, f, pe;

	omp_set_num_threads(8);
#pragma omp parallel for schedule(dynamic) private(j, dq, a, b, c , d, e, f, pe)
	for (i = 0; i < tam; ++i){

		// desplazamiento
#pragma omp atomic
		v[i].x += v[i].v_x*(tiempo);
#pragma omp atomic
		v[i].y += v[i].v_y*(tiempo);
#pragma omp atomic
		v[i].z += v[i].v_z*(tiempo);

		// aceleración de la gravedad hasta el plano de rebote
		if (vi[i].y > y_plano_rebote + radio)
		{
#pragma omp atomic
			v[i].v_y -= G*(tiempo);
		}

		// "rozamiento" con el plano de rebote
		if (vi[i].y < y_plano_rebote + radio)

		{

#pragma omp atomic
			v[i].v_x -= 0.005*vi[i].v_x;
#pragma omp atomic
			v[i].v_z -= 0.005*vi[i].v_z;
			
			// rebote

			if (vi[i].v_y < 0)
			{
#pragma omp atomic
				v[i].v_y *= -K;

			}
		}

		// rebote con paredes en adicionales 

		/*if (abs(vi[i].x) > 600 - radio)
		{
		#pragma omp atomic
		v[i].v_x *= -K;
		#pragma omp atomic
		v[i].v_y -= 0.005*vi[i].v_y;
		#pragma omp atomic
		v[i].v_z -= 0.005*vi[i].v_z;
		}
		if (abs(vi[i].z) > 600 - radio)
		{
		#pragma omp atomic
		v[i].v_z *= -K;
		#pragma omp atomic
		v[i].v_x -= 0.005*vi[i].v_x;
		#pragma omp atomic
		v[i].v_y -= 0.005*vi[i].v_y;
		}*/


		// colisiones entre partículas

		for (j = i + 1; j < tam; ++j){
			if (tam != 1 && j == i) continue;
			dq = pow(vi[i].x - vi[j].x, 2) + pow(vi[i].y - vi[j].y, 2) + pow(vi[i].z - vi[j].z, 2);
			if (dq < 4.1 * radio*radio)
			{

				a = vi[i].v_x - vi[j].v_x;
				b = vi[i].v_y - vi[j].v_y;
				c = vi[i].v_z - vi[j].v_z;

				d = vi[i].x - vi[j].x;
				e = vi[i].y - vi[j].y;
				f = vi[i].z - vi[j].z;

				pe = a*d + b*e + c*f;


#pragma omp atomic
				v[i].v_x += -K*d*pe / dq;
#pragma omp atomic
				v[i].v_y += -K*e*pe / dq;
#pragma omp atomic
				v[i].v_z += -K*f*pe / dq;

#pragma omp atomic
				v[j].v_x += +K*d*pe / dq;
#pragma omp atomic
				v[j].v_y += +K*e*pe / dq;
#pragma omp atomic
				v[j].v_z += +K*f*pe / dq;


				
			}

		}

		// calcular brillo


		if (brillo && sol.z < v[i].z)
		{


			v[i].dluz_x = -sol.x + v[i].x;
			v[i].dluz_y = -sol.y + v[i].y;
			v[i].dluz_z = -sol.z + v[i].z;

			v[i].dist_luz = sqrt(pow(v[i].dluz_x, 2) + pow(v[i].dluz_y, 2) + pow(v[i].dluz_z, 2));

			v[i].dluz_x /= v[i].dist_luz;
			v[i].dluz_y /= v[i].dist_luz;
			v[i].dluz_z /= v[i].dist_luz;
		}

	}

	vi = v;

}


// crear un numero de bolas nuevas

void crear(vector<cuerpo> &v, int num)
{
	cuerpo c;
	double lado(pow(4 * 3.14*pow(radio, 3)*v.size(), 1 / 3) * 4000);
	for (int i = 0; i < num; ++i)	{

		c.x = ((double(rand()) / double(RAND_MAX)) * (lado / 2 - -lado / 2)) + -lado / 2;
		c.y = ((double(rand()) / double(RAND_MAX)) * (8 * lado - 5 * lado)) + 5 * lado;
		c.z = ((double(rand()) / double(RAND_MAX)) * (lado / 2 - -lado / 2)) + -lado / 2;
		c.v_x = 0;
		c.v_y = rand() % 25 - 12;;
		c.v_z = 0;

		c.col1 = rand() % 205 + 1;
		c.col2 = rand() % 205 + 1;
		c.col3 = rand() % 205 + 1;


		v.push_back(c);

	}
}

// eliminar el exceso de bolas

void eliminar(vector<cuerpo> &v)
{
	vector<cuerpo> vf{};
	int radio(100);

	for (int i = 0; i < v.size(); ++i)	{
		if (pow(v[i].x, 2) + pow(v[i].z, 2) < 500000000 && abs(v[i].v_y) + abs(v[i].v_x) + abs(v[i].v_z) > 500 || v[i].y > 2 * radio)
			vf.push_back(v[i]);
	}

	v = vf;
}


// darle valor a las coordenadas de una entidad

void definir_entidad(entidad &obs, double x, double y, double z)
{
	obs.x = x;
	obs.y = y;
	obs.z = z;
}


int main(int argc, char **argv)
{
	srand(time(NULL));
	unsigned int numero(0);
	cout << "--------------------------------" << endl;
	cout << "Introduce el numero de particulas: " << endl;
	cout << "--------------------------------" << endl;
	cin >> numero;
	cout << "--------------------------------" << endl;
	cout << " W / S - eje Z || Q / E - eje Y || A / D - eje X" << endl;
	cout << "--------------------------------" << endl;
	cout << "--------------------------------" << endl;

	ALLEGRO_DISPLAY *display = NULL;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(t_X, t_Y);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	al_init_primitives_addon();
	al_clear_to_color(al_map_rgb(0, 0, 0));


	///////


	// observador y fuente de luz
	entidad sol, observador;
	definir_entidad(observador, 0, -10000, -25000);
	definir_entidad(sol, observador.x, observador.y, observador.z);


	// contenedores de particulas y cubos
	vector<cuerpo> particulas(numero);
	vector<cubo> cubos;
	crear(particulas, numero);


	
	double t1, t2, e_t(1);
		
	int contador = 1;
	char key = ' ';
	
	int desp(100);
	int y_plano_rebote(0);
	int n_eliminados;


	// bucle principal

	while (key != 'o')
	{

		if (getInput(&key))
		{
			if (key == 'w')
			{
			
				observador.z += desp;
				cout << "z " << observador.z << endl;
			}
			else if (key == 's')
			{
			
				observador.z -= desp;
				cout << "z " << observador.z << endl;
			}
			else if (key == 'd')
			{
			
				observador.x -= desp;
				cout << "x " << observador.x << endl;
			}
			else if (key == 'a')
			{
			
				observador.x += desp;
				cout << "x " << observador.x << endl;
			}
			else if (key == 'e')
			{
				observador.y += desp;
				cout << "y " << observador.y << endl;
			}
			else if (key == 'q')
			{
			
				observador.y -= desp;
				cout << "y " << observador.y << endl;
			}
			else if (key == 'm')
			{
				numero += 50;
				cout << "mas particulas: " << numero << endl;
			}
			else if (key == 'n')
			{
				if (numero >= 50)
					numero -= 50;
				cout << "menos particulas: " << numero << endl;
			}
			else if (key == 'b')
			{
				brillo = !brillo;
				cout << "brillo  " << brillo << endl;
			}
			else if (key == 'z')
			{
				caras = !caras;
				cout << "caras de cubos  " << caras << endl;
			}
			else if (key == 'j')
			{
				if (K > 0.2)
					K -= 0.1;
				cout << "K disminuye  " << K << endl;
			}
			else if (key == 'l')
			{
				if (K < 0.9)
					K += 0.1;
				cout << "K aumenta  " << K << endl;
			}
			else if (key == 'c')
			{
				cubos.push_back(definir_cubo());
				observador.y = 0;
				observador.x = 0;
				cout << "un cubo mas  " << cubos.size() << endl;
			}
			else if (key == 'x')
			{
				if (cubos.size() > 0)
					cubos.pop_back();
				cout << "un cubo menos  " << cubos.size() << endl;
			}



		}

		t1 = clock();

		std::sort(particulas.begin(), particulas.end(), criterio_particulas);
		std::sort(cubos.begin(), cubos.end(), criterio_cubos);

		dibujar_bolas(particulas, e_t, y_plano_rebote, observador, sol);
		dibujar_todos_cubos(cubos, observador, sol);
	
		al_flip_display();

		actualizar_cubos(cubos, e_t);	
		calcular_desplazamiento(particulas, e_t, y_plano_rebote, observador, sol);

		eliminar(particulas);
		n_eliminados = numero - particulas.size();

		if (particulas.size() < numero)
			crear(particulas, n_eliminados);

		definir_entidad(sol, observador.x, observador.y, observador.z);
		
		t2 = clock();
		e_t = t2 - t1;

		if (!(contador % 399))
		{
			cout << "------------------------------------------------------------------" << endl;
			cout << "tiempo " << e_t << " num particulas " << particulas.size() << " brillo " << brillo << " K " << K << endl;
			cout << " num cubos " << cubos.size() << " caras " << caras <<  endl;
			cout << "x obs " << observador.x << " y obs " << observador.y << " z obs " << observador.z << endl;
			cout << "x sol " << sol.x << " y sol " << sol.y << " z sol " << sol.z << endl;
			cout << "------------------------------------------------------------------" << endl;
		}
		++contador;
	}
	
	al_destroy_display(display);

	return 0;
}
