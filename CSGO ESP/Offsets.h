//¬ерси€ оффсетов 09.02.2021

#pragma once
#define dwEntityList 0x4DA2F44 //статический адрес (лист всех ентити, по нему наход€т игроков и далее)
#define dwViewMatrix 0x4D94844 //часть класса игрока (кости игрока в матрице) —уд€ по Cheat Engine это положение камеры прицела (16 значений типо float, видова€ матрица view matrix: https://www.youtube.com/watch?v=VnXvTEH0NK0&list=PLc_QQC_Xnp8ux5c0Fv13QvJy7BJ4qBOKX&index=3 и https://www.youtube.com/watch?v=-WL1Gpe9VRo и это https://www.youtube.com/watch?v=crT9zGviqVs)
#define m_iTeamNum 0xF4 //номер команды (кт 2 вродь и т 3)
#define m_iHealth 0x100 //хп игрока
#define m_vecOrigin 0x138 //отвечает за XYZ координаты