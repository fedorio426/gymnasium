#ifndef Gymnasium_H_INCLUDED
#define Gymnasium_H_INCLUDED

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

#include "logger.h"

using namespace std;

// ---------------------------------------------------------

struct stats_pupil
{
    int total,boys,girls;
};

struct stats_parallel
{
    int paral,classes,total,boys,girls;
    string letters;
};

struct stats_classes
{
    int total,boys,girls;
    string cl_le;
};

struct stats_incomplete
{
    string name,phone,addrs,email,cl_letter,desc;
};

struct pupil_view
{
    int     id{0};  // 1-- id
    string  cl_le;  // 2-- цифра-буква
    string  gendr;  // 3-- пол

    string  fname;  // 4-- имя
    string  mname;  // 5-- отч
    string  lname;  // 6-- фам

    string  phone;  // 7-- тел
    string  addrs;  // 8-- адр
    string  email;  // 9-- eml

    int     sclass {0}; // 0-- класс 1..11
    int     letter {0}; // 1-- буква 1..13
    int     gender {0}; // 2-- пол 1-муж, 2-жен
};

struct search_data
{
    int     mode   {0}; // 0-- выбор 1-класса / 2-ученика по фамилии .-по телефону
    int     sclass {0}; // 1-- класс 1..11
    int     letter {0}; // 2-- буква 1..13
    int     gender {0}; // 3-- пол 1-муж, 2-жен
    string  word;       // 4-- имя / тел
};


// ---------------------------------------------------------
class GymnasiumDatabase
{
private:
    string dbfname;
    string csvfile;
    string logfile;

public:
    GymnasiumDatabase()
    {
        srand(time(NULL));
        dbfname = "gymnasium.db";
        csvfile = "gymnasium.csv";
        logfile = "gymnasium.log";
    };

    string & getDbname() { return dbfname; }
    string & getCsv()    { return csvfile; }

    // Создание всех рабочих таблиц базы.
    // Запись расшифровок символов клаасов и пола
    bool  createTables ();
    void  createTables(void* dbase);
    // Запись в базу всех данных из списка учеников
    bool  savePupils (vector<pupil_view> &vec);

    // импорт / экспорт CSV и создание базы
    void  importCsv(vector<pupil_view> & vecPupil);
    void  exportCsv(vector<pupil_view> & vecPupil);

    // поиск по фамилии, номеру телефона, классу, полу [ sad ]
    int   getPupilList(vector<pupil_view> &vec, search_data sch = {});

    bool  total_pupils(stats_pupil& pv);
    bool  paral_pupils(vector<stats_parallel>& vec);
    bool  class_pupils(vector<stats_classes>& vec);
    bool  incpl_pupils(vector<stats_incomplete>& vec);
    // CRUD операции. Все виды поиска реализованы в getPupilList
    bool  selectPupil (int id, pupil_view& prj);
    bool  insertPupil (const pupil_view& prj);
    bool  updatePupil (const pupil_view& prj);
    bool  deletePupil (int id);

    // отладка: непосредственное создание БД
    void  makeTestDatabase ();

    // отладка: генерация целого числа в диапазоне
    int   genint(int min, int max) { return min + rand() % (max-min+1); }

};



#endif // Gymnasium_H_INCLUDED
