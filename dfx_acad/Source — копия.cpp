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
void   dxf_text(FILE*,
    double[],
    char*,
    double,
    double
);


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
/*                Запись  текста в dxf-файл                     */
/******************************************************************/
void   dxf_text(FILE* file_dxf,
    double pt[],
    char* text,
    double h,
    double ang
)
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

/******************************************************************/
/*                        dxf_lwpolyline                          */
/*                        ==============                          */
/*                Запись  полилинии линии в dxf-файл              */
/******************************************************************/
void   dxf_lwpolyline(FILE* file_dxf, double mas_pt[][3], int all_pt, double  w_pline)
{
    int i;
    char param_lwpolyline[24] = "LWPOLYLINE";
    dxf_mark_value(file_dxf, 0, param_lwpolyline);
    char param_1d[24] = "1D";
    dxf_mark_value(file_dxf, 5, param_1d);
    char param_acdbentity[24] = "AcDbEntity";
    dxf_mark_value(file_dxf, 100, param_acdbentity);
    dxf_mark_value(file_dxf, 8, layer);
    char param_acdbpolyline[24] = "AcDbPolyline";
    dxf_mark_value(file_dxf, 100, param_acdbpolyline);
    dxf_mark_value(file_dxf, 62, dxf_color);


    dxf_mark_value(file_dxf, 90, all_pt);
    dxf_mark_value(file_dxf, 70, 0);
    dxf_mark_value(file_dxf, 43, w_pline);

    for (i = 0; i < all_pt; i++)
        dxf_coor(file_dxf, 0, &mas_pt[i][0]);

}


/******************************************************************/
/*               Пример работы с dxf-файлом                       */
/*                  (Вывод таблицы чисел)                         */
/******************************************************************/
#define DIM_KV   10.0
#define ALL_CEL  6
//    1 0 0 0 0 0 
//    0 2 0 0 0 0
//    0 0 3 0 0 0
//    0 0 0 4 0 0 
//    0 0 0 0 5 0 
//    0 0 0 0 0 6

int main(void)
{
    FILE* dxf;

    double  d_cel, x_cur = 0.0, y_cur = 0.0, gab;
    char    buff[200];

    static double pt1[3], pt2[3];

    int     i, j, mas[ALL_CEL][ALL_CEL], num;

    // Заполнение массива
    for (i = 0; i < ALL_CEL; i++)
        for (j = 0; j < ALL_CEL; j++)
        {
            mas[i][j] = 0;
            if (i == j)
                mas[i][j] = i + 1;
        }

    dxf = dxf_init("test.dxf");
    char param_layer_line[24] = "LAYER_LINE";
    dxf_layer(param_layer_line);
    dxf_color = RED;

    pt1[0] = 10;  pt1[1] = 10;
    pt2[0] = 20;  pt2[1] = 20;
    dxf_line(dxf, pt1, pt2);

    pt1[0] = 10;  pt1[1] = 10;
    pt2[0] = 20;  pt2[1] = 10;
    dxf_line(dxf, pt1, pt2);


    // Текст
    char param_layer_text[24] = "LAYER_TEXT";
    dxf_layer(param_layer_text);
    dxf_color = GREEN;
    char param_sapr[24] = "SAPR";
    dxf_text(dxf, pt1, param_sapr, 2, 0);
    char param_smt[24] = "SMT";
    dxf_text(dxf, pt2, param_smt, 1, 45);


    d_cel = DIM_KV / ALL_CEL;
    gab = d_cel * 0.1;

    // Горизонтальные линии
    for (i = 0; i <= ALL_CEL; i++)
    {
        y_cur = i * d_cel;
        pt1[0] = x_cur;   pt1[1] = y_cur;
        pt2[0] = DIM_KV;  pt2[1] = y_cur;
        dxf_line(dxf, pt1, pt2);
    }

    // Вертикальные линии
    y_cur = 0.0;
    for (i = 0; i <= ALL_CEL; i++)
    {
        x_cur = i * d_cel;
        pt1[0] = x_cur;  pt1[1] = y_cur;
        pt2[0] = x_cur;  pt2[1] = DIM_KV;
        dxf_line(dxf, pt1, pt2);
    }

    // Текст
    dxf_layer(param_layer_text);
    dxf_color = GREEN;

    y_cur = DIM_KV - d_cel;

    for (i = 0; i < ALL_CEL; i++, y_cur -= d_cel)
        for (j = 0, x_cur = 0.0; j < ALL_CEL; j++, x_cur += d_cel)
        {
            pt1[0] = x_cur + gab;
            pt1[1] = y_cur + gab;
            num = mas[i][j];

            if (num != 0)
            {
                sprintf(buff, "%d", num);
                dxf_text(dxf, pt1, buff, d_cel / num, 0);
            }
        }

    dxf_end(dxf);
    return 0;
}

