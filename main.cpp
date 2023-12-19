#include <QCoreApplication>
#include <cmath>
#include <iostream>

#define PI 3.14159265

int main()
{

  double SIDTIME, GMST0, L, w, d, HA, K, UT, minutes, LON, x, y, z, xhor, yhor, zhor, lat, azimuth,  altitude, uglTen;
  double e, oblecl, E, x1, y1, r, v, lon, yequat, xequat, RA, zequat, Decl, TAR, TAN;


  int  Y, D, M, hour, Grin ;
  double z1 = 0.0;


  std::cout << "Enter the Year: "; //Вводим год
  std::cin >> Y;
  std::cout << std::endl;

  std::cout << "Enter the month: "; //Вводим месяц
  std::cin >> M;
  std::cout << std::endl;

  std::cout << "Enter a calendar day: "; //ВВодим день
  std::cin >> D;
  std::cout << std::endl;

  //Вводим значение времени
  std::cout << "Enter the hour: ";   //Вводим начальный час
  std::cin>>  hour;
  std::cout << std::endl;
  std::cout << "Enter the minutes:" ; //Вводим начальные минуты
  std::cin>>  minutes;
  std::cout << std::endl;

  //Вводим широту (дробью) южная широта отрицательная, северная широта положительная
  std::cout << "Enter the Latitude (southern latitude  is negative, southern latitude is positive): ";
  std::cin>>  lat;
  std::cout << std::endl;

  //Вводим долготу (дробью) западная долгота отрицательная, восточная положительная
  std::cout << "Enter the Longitude (west longitude is negative, east longitude is positive): ";
  std::cin>>  LON;
  std::cout << std::endl;

  //Вводим угол между направлением робота и тенью
  std::cout << "Enter the angle between the shadow and the direction of the robot" << std::endl;
  std::cin >> TAR;
  std::cout << std::endl;

  //Вводим разницу во времени по Гринвичу
  std::cout << "Enter the time difference from Greenwich" << std::endl;
  std::cin >> Grin;
  std::cout << std::endl;

  //Оформление таблицы

  std::cout << "Time: " << D << "." << M << "." << Y << " , " << hour << ":" << minutes
            << " , GPS: "<< lat << "," << LON << std::endl;
  std::cout << std::endl;
  std::cout << "Time" << "       " <<  "azimuth" << "       " << "altitude" << "       "
             << "The angle between the" << "       " << "The angle between the shadow" <<  std::endl;
  std::cout << "                                          north direction" << "            and the direction" << std::endl;
  std::cout << "                                         and the shadow" <<"                of the robot" << std::endl;


   while (minutes < 60 && hour < 24){

  d = 367 * Y - (7*(Y + ((M+9)/12)))/4 + (275* M) / 9 + D - 730530; //Расчёт количества дней от J2000 эры

  w = 282.9404 + 4.70935E-5 * d; //Расчёт Долготы перигелиz

  //Нормализация значения w: 0-360 град
  w = w - floor(w/360.0)*360.0;

  K = 356.0470 + 0.9856002585 * d; //Расчёт средней аномалии

  //Нормализация значения K: 0-360 град
  K = K - floor(K/360.0)*360.0;

  L = w + K; //Вычисление средней долготы солнца

  //Нормализация значения L: 0-360 град
  L = L - floor(L/360.0)*360.0;

  e = 0.016709 - 1.151E-9 * d;//Рассчёт эксцентриситета

  oblecl = 23.4393 - 3.563E-7 * d; //Рассчёт наклона элептики

  E = K + (180 / PI) * e * sin (K*PI/180) * (1 + e * cos (K*PI/180)); //Рассчёт эксцентрической аномалии

  //Рассчёт прямоугольных координат солнца
  x1 = cos(E*PI/180) - e;
  y1 = sin (E*PI/180) * sqrt (1 - e * e);

  r = sqrt (x1 * x1 + y1 * y1); //Рассчёт расстояния
  v = 180/PI * atan2 (y1, x1);  //Рассчёт истинной аномалии

  lon = v + w; // Рассчёт долготы Солнца
  lon = lon - floor(lon/360.0)*360.0; //Нормализация значения lon: 0-360 град

  //Рассчёт элептических координат Солнца
  x1 = r * cos (lon*PI/180);
  y1 = r * sin (lon*PI/180);

  //Поворот этих координат
  yequat = y1 * cos (oblecl*PI/180) - z1 * sin (oblecl*PI/180);
  xequat = x1;
  zequat = y1 * sin (oblecl*PI/180) + z1 * cos (oblecl*PI/180);

  //Рассчёт сферических экваториальных координат
  RA = 180/PI * atan2 ( yequat, xequat)/15; //Расчёт прямого восхождения
  Decl = 180/PI * atan2( zequat, sqrt( xequat * xequat + yequat * yequat) ); //Рассчёт склонения

  //Рассчёт звездное время на Гринвичском меридиане в 00:00 прямо сейчас: GMST0
  GMST0 = L + 180;

  //Нормализация значения GMST0: 0-360 град
  GMST0 = GMST0 - floor(GMST0/360.0)*360.0;

  GMST0 = GMST0/15;

  //Перевод введённого времени в дробное число
  UT = hour + minutes/60 + Grin ; //

  //Расчёт звёздного времени (местного времени)

  SIDTIME = GMST0 + UT + LON/15;


  //Рассчёт часового угла
  HA = (SIDTIME - RA) * 15;

  //Перевод в прямоугольную систему координат. X указывает
  //на небесный экватор на юге, Y - на горизонт на западе,
  //а ось Z - на северный небесный полюс
  x = cos (HA*PI/180) * cos (Decl*PI/180);
  y = sin (HA*PI/180) * cos (Decl*PI/180);
  z = sin (Decl*PI/180);

  //Повернём систему координат вдоль оси, идущей с востока
  //на запад, то есть оси Y, таким образом, чтобы ось
  //Z указывала на зенит
  xhor = x * sin(lat*PI/180) - z * cos(lat*PI/180);
  yhor = y;
  zhor = x * cos(lat*PI/180) + z * sin(lat*PI/180);

  //Рассчёт Заветного азимута
  azimuth = 180/PI * atan2( yhor, xhor) + 180;
  //Нормализация значения azimuth: 0-360 град
  azimuth = azimuth - floor(azimuth/360.0)*360.0;

  //Рассчёт угла высоты Солнца над горизонтом
  altitude = 180/PI * atan2( zhor, sqrt(xhor*xhor+yhor*yhor));

  //Рассчёт значения угла между тенью и северным напрвлением
  if (azimuth >= 0 && azimuth <= 180){
      uglTen = 180 + azimuth;

  }
  else if (azimuth > 180 && azimuth <= 360){
      uglTen = azimuth - 180;

  }


  //Расчёт ориентационного угла робота
  if (TAR < uglTen ){
      TAN = uglTen - TAR;
  }
  else if (TAR > uglTen){
      TAN = TAR - uglTen;
  }
  else if (TAR == uglTen){
      TAN = uglTen;
  }


  //Вывод Значений Таблицы:

  std::cout << hour << ":" << minutes << "       " <<  azimuth << "       " << altitude
            << "            " << uglTen << "                        " << TAN <<std::endl;
  minutes = minutes + 1;

  if (minutes > 59){

      hour = hour + 1;
      minutes = minutes - 60;

  }
}
  return 0;
}

