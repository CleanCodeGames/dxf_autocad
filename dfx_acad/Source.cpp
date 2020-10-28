/******************************************************************/
/*                     dxf_out                                    */
/*                     =======                                    */
/*    Функции  вывода  графических  примитивов AutoCAD            */
/*              в dxf-файл текстового формата                     */
/******************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include <time.h>
// КОДЫ  ОСНОВНЫХ  ЦВЕТОВ  СИСТЕМЫ  AUTOCAD 

#define  WHITE     7     //  белый
#define	 YELLOW    2     //  желтый
#define	 BROWN    10     //  коричневый
#define	 GREEN     3     //  зеленый
#define	 CYAN      4     //  голубой
#define	 BLUE      5     //  синий
#define	 MAGENTA   6     //  фиолетовый
#define  RED       1     //  красный

/*-------------------------*/
/*  ГЛОБАЛЬНЫЕ  ПЕРЕМЕНЫЕ  */
/*-------------------------*/
int    dxf_color = RED;
char   layer[200] = "0";
FILE* dxf_tr(char*);


// Прототипы функций

void  dxf_mark_value(FILE*, int, int);
void  dxf_mark_value(FILE*, int, double);
void  dxf_mark_value(FILE*, int, char*);

void  dxf_coor(FILE*, int, double[]);

void   dxf_end(FILE*);
void   dxf_layer(char*);
void   dxf_line(FILE*, double[], double[]);
void   dxf_3dface(FILE*, double[], double[], double[], double[]);

//  void   dxf_lwpolyline (FILE *, double [][3], int , double);
void   dxf_text(FILE*, double[], char*, double, double);


/******************************************************************/
/*                     dxf_mark_value                             */
/*                     ==============                             */
/*  Функции  вывода  в  dxf-файл пары  маркер-значение            */
/******************************************************************/
// маркер-целое
void dxf_mark_value(FILE* dxf, int marker, int value)
{
    fprintf(dxf, "%3d\n%d\n", marker, value);
}
// маркер-действительное
void dxf_mark_value(FILE* dxf, int marker, double value)
{
    fprintf(dxf, "%3d\n%lf\n", marker, value);
}
// маркер-строка
void dxf_mark_value(FILE* dxf, int marker, char* value)
{
    fprintf(dxf, "%3d\n%s\n", marker, value);
}
/******************************************************************/
/*                          dxf_init                              */
/*                          ========                              */
/*                   Инициализация    dxf-файла                   */
/******************************************************************/
FILE* dxf_init(const char* dxf_name)
{
    FILE* file_dxf;

    if ((file_dxf = fopen(dxf_name, "wt")) == NULL)
    {
        printf("\nНе  могу открыть файл %s", dxf_name);
        exit(-1);
    }

    char param_section[24] = "SECTION";
    dxf_mark_value(file_dxf, 0, param_section);
    char param_entities[24] = "ENTITIES";
    dxf_mark_value(file_dxf, 2, param_entities);

    return(file_dxf);
}

/******************************************************************/
/*                          dxf_end                               */
/*                          =======                               */
/*                      Закрытие dxf-файла                        */
/******************************************************************/
void  dxf_end(FILE* file_dxf)
{
    char param_endsec[24] = "ENDSEC";
    dxf_mark_value(file_dxf, 0, param_endsec);
    char param_eof[24] = "EOF";
    dxf_mark_value(file_dxf, 0, param_eof);

    fclose(file_dxf);
}

/******************************************************************/
/*                        dxf_layer                               */
/*                        =========                               */
/*                    Установить   слой                           */
/******************************************************************/
void   dxf_layer(char* cur_layer)
{
    sprintf(layer, "%s", cur_layer);
}

/******************************************************************/
/*                        dxf_coor                                */
/*                        ========                                */
/*        Запись  координаты  точки в dxf-файл                    */
/******************************************************************/
void dxf_coor(FILE* file_dxf, int num_coor, double pt[])
{
    dxf_mark_value(file_dxf, 10 + num_coor, pt[0]);
    dxf_mark_value(file_dxf, 20 + num_coor, pt[1]);
    dxf_mark_value(file_dxf, 30 + num_coor, pt[2]);
}

/******************************************************************/
/*                        dxf_line                                */
/*                        ========                                */
/*                Запись  линии в dxf-файл                        */
/******************************************************************/
void   dxf_line(FILE* file_dxf, double pt1[], double pt2[])
{
    char param_line[24] = "LINE";
    dxf_mark_value(file_dxf, 0, param_line);
    dxf_mark_value(file_dxf, 8, layer);
    dxf_mark_value(file_dxf, 62, dxf_color);

    dxf_coor(file_dxf, 0, pt1);
    dxf_coor(file_dxf, 1, pt2);
}

/******************************************************************/
/*                        dxf_3dface                              */
/*                        ==========                              */
/*                Запись  3д грани в dxf-файл                     */
/******************************************************************/
void   dxf_3dface(FILE* file_dxf, double pt1[], double pt2[],
    double pt3[], double pt4[]
)
{
    char param_3dface[24] = "3DFACE";
    dxf_mark_value(file_dxf, 0, param_3dface);
    dxf_mark_value(file_dxf, 8, layer);
    dxf_mark_value(file_dxf, 62, dxf_color);

    dxf_coor(file_dxf, 0, pt1);
    dxf_coor(file_dxf, 1, pt2);
    dxf_coor(file_dxf, 2, pt3);
    dxf_coor(file_dxf, 3, pt4);
}


/******************************************************************/
/*                        dxf_text                                */
/*                        ========                                */
/*                Запись  текста в dxf-файл                       */
/******************************************************************/
void   dxf_text(FILE* file_dxf, double pt[], char* text, double h, double ang)
{
    char param_text[24] = "TEXT";
    dxf_mark_value(file_dxf, 0, param_text);
    dxf_mark_value(file_dxf, 8, layer);
    dxf_mark_value(file_dxf, 62, dxf_color);

    dxf_coor(file_dxf, 0, pt);

    dxf_mark_value(file_dxf, 40, h);
    dxf_mark_value(file_dxf, 1, text);
    dxf_mark_value(file_dxf, 50, ang);
}

void dxf_circle(FILE* file_dxf, double center[], double radius)
{
    char param_circle[24] = "CIRCLE";
    dxf_mark_value(file_dxf, 0, param_circle);
    dxf_mark_value(file_dxf, 8, layer);
    dxf_mark_value(file_dxf, 40, radius);
    dxf_mark_value(file_dxf, 10, center[0]);
    dxf_mark_value(file_dxf, 20, center[1]);
    dxf_mark_value(file_dxf, 30, center[2]);
    dxf_mark_value(file_dxf, 62, dxf_color);
}

/******************************************************************/
/*               Пример работы с dxf-файлом                       */
/*                  (Вывод таблицы чисел)                         */
/******************************************************************/
#define DIM_KV   10.0
#define ALL_CEL  6

int main(void)
{
    srand(time(0));
    FILE* dxf;

    double  d_cel, x_cur = 0.0, y_cur = 0.0, gab;
    char    buff[200];

    static double pt1[3], pt2[3];

    int     i, j, mas[ALL_CEL][ALL_CEL], num;

    // Заполнение массива
    mas[0][0] = 1,  mas[1][0] = 2,   mas[2][0] = 3,   mas[3][0] = 4,   mas[4][0] = 5,   mas[5][0] = 6;
    mas[0][1] = 20, mas[1][1] = 21,  mas[2][1] = 22,  mas[3][1] = 23,  mas[4][1] = 24,  mas[5][1] = 7;
    mas[0][2] = 19, mas[1][2] = 32,  mas[2][2] = 33,  mas[3][2] = 34,  mas[4][2] = 25,  mas[5][2] = 8;
    mas[0][3] = 18, mas[1][3] = 31,  mas[2][3] = 36,  mas[3][3] = 35,  mas[4][3] = 25,  mas[5][3] = 9;
    mas[0][4] = 17, mas[1][4] = 30,  mas[2][4] = 29,  mas[3][4] = 28,  mas[4][4] = 27,  mas[5][4] = 10;
    mas[0][5] = 16, mas[1][5] = 15,  mas[2][5] = 14,  mas[3][5] = 13,  mas[4][5] = 12,  mas[5][5] = 11;

    dxf = dxf_init("test.dxf");
    char param_layer_line[24] = "LAYER_LINE";
    dxf_layer(param_layer_line); // Задаём слой для линий

    d_cel = DIM_KV / ALL_CEL;
    gab = d_cel * 0.1;

    // Горизонтальные линии
    for (i = 0; i <= ALL_CEL; i++)
    {
        dxf_color = rand() % 10 + 1;
        y_cur = i * d_cel;
        pt1[0] = x_cur;   pt1[1] = y_cur;
        pt2[0] = DIM_KV;  pt2[1] = y_cur;
        dxf_line(dxf, pt1, pt2);
    }

    // Вертикальные линии
    y_cur = 0.0;
    for (i = 0; i <= ALL_CEL; i++)
    {
        dxf_color = rand() % 10 + 1;
        x_cur = i * d_cel;
        pt1[0] = x_cur;  pt1[1] = y_cur;
        pt2[0] = x_cur;  pt2[1] = DIM_KV;
        dxf_line(dxf, pt1, pt2);
    }

    
    y_cur = DIM_KV - d_cel;
    char param_layer_text[24] = "LAYER_TEXT";
    dxf_layer(param_layer_text); // Задаём слой для текста
    for (i = 0; i < ALL_CEL; i++, y_cur -= d_cel) 
    {
        for (j = 0, x_cur = 0.0; j < ALL_CEL; j++, x_cur += d_cel)
        {
            pt1[0] = x_cur + gab;
            pt1[1] = y_cur + gab;
            num = mas[j][i];
            sprintf(buff, "%d", num);
            dxf_color = rand() % 10 + 1;
            dxf_text(dxf, pt1, buff, d_cel/2.f, 0);

            
        }
    }
    y_cur = DIM_KV - d_cel;
    char param_layer_circle[24] = "LAYER_CIRCLE";
    dxf_layer(param_layer_circle); // Задаём слой для кругов
    for (i = 0; i < ALL_CEL; i++, y_cur -= d_cel)
    {
        for (j = 0, x_cur = 0.0; j < ALL_CEL; j++, x_cur += d_cel)
        {
            num = mas[j][i];
            dxf_color = rand() % 10 + 1;
            pt1[0] = x_cur;
            pt1[1] = y_cur;
            pt1[0] += d_cel / 2;
            pt1[1] += d_cel / 2;
            dxf_circle(dxf, pt1, (d_cel / 2) / num);
        }
    }

    dxf_end(dxf);
    return 0;
}

