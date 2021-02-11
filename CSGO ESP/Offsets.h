//Версия оффсетов 09.02.2021

#pragma once
#define dwEntityList 0x4DA2F44 //статический адрес (лист всех ентити, по нему находят игроков и далее)
#define dwViewMatrix 0x4D94844 //часть класса игрока (кости игрока в матрице)
#define m_iTeamNum 0xF4 //номер команды (кт 2 вродь и т 3)
#define m_iHealth 0x100 //хп игрока
#define m_vecOrigin 0x138 //отвечает за XYZ координаты