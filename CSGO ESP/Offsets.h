//Версия оффсетов 09.02.2021

#pragma once
#define dwEntityList 0x4DA2F44 //статический адрес (лист всех ентити, по нему находят игроков и далее)
#define dwViewMatrix 0x4D94844 //Видовая матрица (view matrix), у неё 16 значений типа float в редакторе памяти: https://www.youtube.com/watch?v=VnXvTEH0NK0&list=PLc_QQC_Xnp8ux5c0Fv13QvJy7BJ4qBOKX&index=3 и https://www.youtube.com/watch?v=-WL1Gpe9VRo и это https://www.youtube.com/watch?v=crT9zGviqVs)
#define m_iTeamNum 0xF4 //номер команды (кт 2 вродь и т 3)
#define m_iHealth 0x100 //хп игрока
#define m_vecOrigin 0x138 //отвечает за XYZ координаты