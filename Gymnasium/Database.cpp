// Работа с базой данных
//
// эти файлы нельзя подключить в "Database.h"

#include "sqlite3pp.h"
#include "sqlite3ppext.h"

#include "Database.h"


#include "utils.h"
/*
// =ученик=
struct pupil_view {
  int     id;     // 1-- id
  string  cl_le;  // 2-- цифра-буква
  string  gendr;  // 3-- пол

  string  fname;  // 4-- имя
  string  mname;  // 5-- отч
  string  lname;  // 6-- фам

  string  phone;  // 7-- тел
  string  addrs;  // 8-- адр
  string  email;  // 9-- eml

  int     sclass; // 0-- класс 1..11
  int     letter; // 1-- буква 0..9
  int     gender; // 2-- пол 1-муж, 2-жен
};
*/

// =========================================================

// ---------------------------------------------------------

//** Создание таблицы pupil
string sqlCreateTablePupil(R"(
CREATE TABLE IF NOT EXISTS pupil(
  ppl_id INTEGER PRIMARY KEY AUTOINCREMENT,
  sclass INTEGER NOT NULL,
  letter INTEGER NOT NULL,
  gender INTEGER NOT NULL,
  fname  TEXT NOT NULL,
  mname  TEXT NOT NULL,
  lname  TEXT NOT NULL,
  phone  TEXT NULL,
  addrs  TEXT NULL,
  email  TEXT NULL);
)");
string sqlCreateTableLetter(R"(
CREATE TABLE IF NOT EXISTS cl_letter(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  letter TEXT NOT NULL);
)");
string sqlInsertTableLetter(R"(
INSERT INTO cl_letter (letter)
  VALUES ('А'),('Б'),('В'),('Г'),('Д'),('Е'),('Ж'),('З'),('И'),('К');
)");
string sqlCreateTableGender(R"(
CREATE TABLE IF NOT EXISTS ge_letter(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  letter TEXT NOT NULL);
)");
string sqlInsertTableGender(R"(
INSERT INTO ge_letter (letter)
  VALUES ('М'),('Ж');
)");


// ---------------------------------------------------------
// Выборка всего, Выборка класса (7-А), Поиск по фамилии или телефону
string sqlSelectPupil(R"(
SELECT pupil.*, sclass || '-' || cl_letter.letter
from pupil, cl_letter
WHERE pupil.letter=cl_letter.id %s
ORDER BY sclass, pupil.letter, lname
)");

string sqlInsertPupil(R"(
INSERT INTO pupil (sclass,letter,gender, fname,mname,lname, phone,addrs,email)
  VALUES (%d,%d,%d, '%s','%s','%s', '%s','%s','%s');
)");
string sqlUpdatePupil(R"(
UPDATE pupil SET
sclass = %d,
letter = %d,
gender = %d,
fname = '%s',
mname = '%s',
lname = '%s',
phone = '%s',
addrs = '%s',
email = '%s'
WHERE ppl_id = %d;
)");

string sqlDeleteById(R"(
DELETE FROM pupil
WHERE ppl_id = %d;
)");
string sqlUpdateRow(R"(  // table, col-list, where-list
)");

/**
кол-во учеников параллели:
select count(sclass) from pupil where sclass=1;

кол-во учеников параллели каждого пола:
select count(sclass) from pupil where sclass=1 AND gender=2;

кол-во учеников во всем заведении:
select count(ppl_id) from pupil;

кол-во учеников в каждом классе:
select count(sclass) from pupil where sclass=1 AND letter=1;

кол-во учеников с неполными данными:
select count(sclass) from pupil where phone="" OR addrs="" OR email="" OR phone=NULL OR addrs=NULL OR email=NULL;
*/

/*
struct stats
{
    string name, phone, addrs, email, litera;
    int sclass, letter, gender;
    int total_pupil, boys, girls, parallel, cl_count;
    //vector<string> cl_letters;
};
*/


//статистика по школе
string sqlTotalPupils(R"(
SELECT
  COUNT(*) AS total_pupils,
  COUNT(*) FILTER (WHERE gender = 1) AS boys_count,
  COUNT(*) FILTER (WHERE gender = 2) AS girls_count
FROM pupil;
)");
/*
total_pupils	boys_count	girls_count
100	51	49
*/


//статистика по параллелям
string sqlParallelPupils(R"(
SELECT
  sclass AS parallel,
  COUNT(DISTINCT letter) AS classes_count,
  COUNT(*) AS pupils_count,
  SUM(CASE WHEN gender = 1 THEN 1 ELSE 0 END) AS boys_count,
  SUM(CASE WHEN gender = 2 THEN 1 ELSE 0 END) AS girls_count,
  GROUP_CONCAT(DISTINCT
    CASE letter
      WHEN 1 THEN 'А'
      WHEN 2 THEN 'Б'
      WHEN 3 THEN 'В'
      WHEN 4 THEN 'Г'
      WHEN 5 THEN 'Д'
      WHEN 6 THEN 'Е'
      WHEN 7 THEN 'Ж'
      WHEN 8 THEN 'З'
      WHEN 9 THEN 'И'
      WHEN 10 THEN 'К'
    END--, ', '
  ) AS class_letters
FROM pupil
GROUP BY sclass
ORDER BY sclass;
)");
/*
parallel	classes_count	pupils_count	boys_count	girls_count	class_letters
1	1	3	2	1	А
2	3	15	9	6	А,Б,В
3	2	12	6	6	А,Б
5	2	12	7	5	А,Б
6	2	8	4	4	А,Б
7	3	16	7	9	А,Б,В
8	2	10	4	6	А,Б
9	2	7	4	3	А,Б
10	1	6	2	4	А
11	3	11	6	5	А,Б,В
*/


//статистика по классам
string sqlClassPupils(R"(
SELECT
  sclass || '-' ||
  CASE letter
      WHEN 1 THEN 'А'
      WHEN 2 THEN 'Б'
      WHEN 3 THEN 'В'
      WHEN 4 THEN 'Г'
      WHEN 5 THEN 'Д'
      WHEN 6 THEN 'Е'
      WHEN 7 THEN 'Ж'
      WHEN 8 THEN 'З'
      WHEN 9 THEN 'И'
      WHEN 10 THEN 'К'
  END AS cl_le,
  COUNT(*) AS pupils_count,
  SUM(CASE WHEN gender = 1 THEN 1 ELSE 0 END) AS boys_count,
  SUM(CASE WHEN gender = 2 THEN 1 ELSE 0 END) AS girls_count
FROM pupil
GROUP BY sclass, letter
ORDER BY sclass, letter;
)");
/*
cl_le	pupils_count	boys_count	girls_count
1-А	3	2	1
2-А	6	4	2
2-Б	6	3	3
2-В	3	2	1
3-А	5	3	2
3-Б	7	3	4
5-А	7	4	3
5-Б	5	3	2
6-А	5	3	2
6-Б	3	1	2
7-А	6	4	2
7-Б	6	1	5
7-В	4	2	2
8-А	7	2	5
8-Б	3	2	1
9-А	3	3	0
9-Б	4	1	3
10-А	6	2	4
11-А	4	4	0
11-Б	3	1	2
11-В	4	1	3

*/


string sqlIncompletePupils(R"(
SELECT
  sclass || '-' ||
  CASE letter
      WHEN 1 THEN 'А'
      WHEN 2 THEN 'Б'
      WHEN 3 THEN 'В'
      WHEN 4 THEN 'Г'
      WHEN 5 THEN 'Д'
      WHEN 6 THEN 'Е'
      WHEN 7 THEN 'Ж'
      WHEN 8 THEN 'З'
      WHEN 9 THEN 'И'
      WHEN 10 THEN 'К'
  END AS cl_le,
  fname||' '||lname||' '||mname AS full_name,
  phone,
  addrs AS address,
  email,
  CASE
    WHEN phone = "" AND addrs = "" AND email = "" THEN 'Все поля пустые'
    WHEN phone = "" AND addrs = "" THEN 'Нет телефона и адреса'
    WHEN phone = "" AND email = "" THEN 'Нет телефона и почты'
    WHEN addrs = "" AND email = "" THEN 'Нет адреса и почты'
    WHEN phone = "" THEN 'Нет телефона'
    WHEN addrs = "" THEN 'Нет адреса'
    WHEN email = "" THEN 'Нет email'
    ELSE 'Частично неполные'
  END AS issue_description
FROM pupil
WHERE phone = ""
   OR addrs = ""
   OR email = ""
ORDER BY sclass, letter, fname;
)");
/*
cl_le	full_name	phone	address	email
1-А	Имя002 Фам002 Отч002	Тел002	Адр002
11-В	Имя100 Фам100 Отч100	Тел100	Адр100

*/

bool  GymnasiumDatabase::total_pupils(stats_pupil& pv)
{
    try {
        sqlite3pp::database dbase(dbfname.c_str());
        {
            pv={};
            string sql=sqlTotalPupils;

            sqlite3pp::query query(dbase, sql.c_str());

            sqlite3pp::query::iterator i = query.begin();
            (*i).getter()
                >> pv.total >> pv.boys >> pv.girls;
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LogLevel::ERR, string("selectPupil: ") + ex.what());
    }
    return true;
}
/*
SELECT
  COUNT(*) AS total_pupils,
  COUNT(*) FILTER (WHERE gender = 1) AS boys_count,
  COUNT(*) FILTER (WHERE gender = 2) AS girls_count
FROM pupil;
*/

bool  GymnasiumDatabase::paral_pupils(vector<stats_parallel>& vec)
{
    try {
        sqlite3pp::database dbase(dbfname.c_str());
        {
            vec={};
            string sql=sqlParallelPupils;

            sqlite3pp::query query(dbase, sql.c_str());

            for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                stats_parallel pv;
                (*i).getter()
                    >> pv.paral >> pv.classes >> pv.total >> pv.boys >> pv.girls >> pv.letters;
                vec.push_back(pv);
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LogLevel::ERR, string("selectPupil: ") + ex.what());
    }
    return true;
}
/*
SELECT
  sclass AS parallel,
  COUNT(DISTINCT letter) AS classes_count,
  COUNT(*) AS pupils_count,
  SUM(CASE WHEN gender = 1 THEN 1 ELSE 0 END) AS boys_count,
  SUM(CASE WHEN gender = 2 THEN 1 ELSE 0 END) AS girls_count,
  GROUP_CONCAT(DISTINCT
    CASE letter
      WHEN 1 THEN 'А'
      WHEN 2 THEN 'Б'
      WHEN 3 THEN 'В'
      WHEN 4 THEN 'Г'
      WHEN 5 THEN 'Д'
      WHEN 6 THEN 'Е'
      WHEN 7 THEN 'Ж'
      WHEN 8 THEN 'З'
      WHEN 9 THEN 'И'
      WHEN 10 THEN 'К'
    END--, ', '
  ) AS class_letters
FROM pupil
GROUP BY sclass
ORDER BY sclass;
*/

bool  GymnasiumDatabase::class_pupils(vector<stats_classes>& vec)
{
    try {
        sqlite3pp::database dbase(dbfname.c_str());
        {
            vec={};
            string sql=sqlClassPupils;

            sqlite3pp::query query(dbase, sql.c_str());

            for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                stats_classes pv;
                (*i).getter()
                    >> pv.cl_le >> pv.total >> pv.boys >> pv.girls;
                vec.push_back(pv);
            }

//            sqlite3pp::query::iterator i = query.begin();
//            (*i).getter()
//                >> pv.cl_le >> pv.total >> pv.boys >> pv.girls;
    }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LogLevel::ERR, string("selectPupil: ") + ex.what());
    }
    return true;
}
/*
SELECT
  sclass || '-' ||
  CASE letter
      WHEN 1 THEN 'А'
      WHEN 2 THEN 'Б'
      WHEN 3 THEN 'В'
      WHEN 4 THEN 'Г'
      WHEN 5 THEN 'Д'
      WHEN 6 THEN 'Е'
      WHEN 7 THEN 'Ж'
      WHEN 8 THEN 'З'
      WHEN 9 THEN 'И'
      WHEN 10 THEN 'К'
  END AS cl_le,
  COUNT(*) AS pupils_count,
  SUM(CASE WHEN gender = 1 THEN 1 ELSE 0 END) AS boys_count,
  SUM(CASE WHEN gender = 2 THEN 1 ELSE 0 END) AS girls_count
FROM pupil
GROUP BY sclass, letter
ORDER BY sclass, letter;
*/

bool  GymnasiumDatabase::incpl_pupils(vector<stats_incomplete>& vec)
{
    try {
        sqlite3pp::database dbase(dbfname.c_str());
        {
            vec={};
            string sql=sqlIncompletePupils;

            sqlite3pp::query query(dbase, sql.c_str());

            for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                stats_incomplete pv;
                (*i).getter()
                    >> pv.cl_letter >> pv.name >> pv.phone >> pv.addrs >> pv.email >> pv.desc;
                vec.push_back(pv);
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LogLevel::ERR, string("selectPupil: ") + ex.what());
    }
    return true;
}
/*
SELECT
  sclass || '-' ||
  CASE letter
      WHEN 1 THEN 'А'
      WHEN 2 THEN 'Б'
      WHEN 3 THEN 'В'
      WHEN 4 THEN 'Г'
      WHEN 5 THEN 'Д'
      WHEN 6 THEN 'Е'
      WHEN 7 THEN 'Ж'
      WHEN 8 THEN 'З'
      WHEN 9 THEN 'И'
      WHEN 10 THEN 'К'
  END AS cl_le,
  fname||' '||lname||' '||mname AS full_name,
  phone,
  addrs AS address,
  email
FROM pupil
WHERE phone = ""
   OR addrs = ""
   OR email = ""
ORDER BY sclass, letter, fname;
*/



extern std::ostream & operator << (std::ostream& orr , const pupil_view& p);

// ===================================================================


/// //////////////////////////////////////////////////////////////////
// Создание всех рабочих таблиц базы.
// Запись расшифровок символов клаасов и пола
// Использовать: rc = dbase.execute(
//    "DROP TABLE IF EXISTS pupil;"
//    sqlCreateTableLetter
//    sqlInsertTableLetter
//    sqlCreateTableGender
//    sqlInsertTableGender
//    sqlCreateTablePupil

bool  GymnasiumDatabase::createTables()
{
    Logger logger;
    cout << "createTables: " << dbfname.c_str() << endl;
    try
    {
        sqlite3pp::database dbase(dbfname.c_str());
        createTables((void*)&dbase);
        dbase.disconnect();
    }
    catch (exception & ex) {
        logger.log(LogLevel::ERR, string("createTables: ") + ex.what());
        return false;
    }
    return true;
}

void GymnasiumDatabase::createTables(void* vdb)
{
    sqlite3pp::database* dbase=(sqlite3pp::database*)vdb;

    int rc;
    rc=dbase->execute("DROP TABLE IF EXISTS pupil;");
    rc=dbase->execute("DROP TABLE IF EXISTS cl_letter;");
    rc=dbase->execute("DROP TABLE IF EXISTS ge_letter;");

    rc=dbase->execute(sqlCreateTablePupil.c_str());
    rc=dbase->execute(sqlCreateTableLetter.c_str());
    rc=dbase->execute(sqlCreateTableGender.c_str());

    rc=dbase->execute(sqlInsertTableGender.c_str());
    rc=dbase->execute(sqlInsertTableLetter.c_str());
}

/// //////////////////////////////////////////////////////////////////
// Запись в базу всех данных из списка учеников
bool  GymnasiumDatabase::savePupils(vector<pupil_view> &vec)
{
    Logger logger(logfile);

    sqlite3pp::database dbase(dbfname.c_str());

    createTables((void*)&dbase);

    for (auto p : vec)
    {
        string sql = sformat(sqlInsertPupil,
                     p.sclass, p.letter, p.gender,
                     p.fname.c_str(), p.mname.c_str(), p.lname.c_str(),
                     p.phone.c_str(), p.addrs.c_str(), p.email.c_str());
        int rc = dbase.execute(sql.c_str());
    }

    dbase.disconnect();

    return true;
}
/*INSERT INTO pupil (sclass,letter,gender, fname,mname,lname, phone,addrs,email)
  VALUES (%s,%s,%s, %s,%s,%s, %s,%s,%s);*/
/////////////////////////////////////////////////////////////////////
// Запись ученика в БД
bool GymnasiumDatabase::insertPupil (const pupil_view& p)
{
    sqlite3pp::database dbase(dbfname.c_str());

    string sql = sformat(sqlInsertPupil,
                         p.sclass, p.letter, p.gender,
                         p.fname.c_str(), p.mname.c_str(), p.lname.c_str(),
                         p.phone.c_str(), p.addrs.c_str(), p.email.c_str());
    int rc = dbase.execute(sql.c_str());

    dbase.disconnect();
    return rc==SQLITE_OK;
}

//////////////////////////////////////////////////////////////////////
// Удаление ученика по id
bool GymnasiumDatabase::deletePupil (int ppl_id)
{
    sqlite3pp::database dbase(dbfname.c_str());

    string sql = sformat(sqlDeleteById, ppl_id);
    int rc = dbase.execute(sql.c_str());

    dbase.disconnect();
    return rc==SQLITE_OK;
}


/// //////////////////////////////////////////////////////////////////
// Изменить данные ученика в БД
bool GymnasiumDatabase::updatePupil (const pupil_view& p)
{
    sqlite3pp::database dbase(dbfname.c_str());

    string sql = sformat(sqlUpdatePupil,
                         p.sclass, p.letter, p.gender,
                         p.fname.c_str(), p.mname.c_str(), p.lname.c_str(),
                         p.phone.c_str(), p.addrs.c_str(), p.email.c_str(), p.id);
    int rc = dbase.execute(sql.c_str());

    dbase.disconnect();
    return rc==SQLITE_OK;
}

//////////////////////////////////////////////////////////////////////
// получить данные ученика по id
bool GymnasiumDatabase::selectPupil(int id, pupil_view& pv)
{
    try {
        sqlite3pp::database dbase(dbfname.c_str());
        {
            pv={};
            string sql=sformat(sqlSelectPupil, "AND pupil.ppl_id="+to_string(id));

            sqlite3pp::query query(dbase, sql.c_str());

            for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                (*i).getter()
                    >> pv.id >> pv.sclass >> pv.letter >> pv.gender
                    >> pv.fname >> pv.mname >> pv.lname
                    >> pv.phone >> pv.addrs >> pv.email >> pv.cl_le;
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LogLevel::ERR, string("selectPupil: ") + ex.what());
    }
    return true;
}

/*
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
*/
//////////////////////////////////////////////////////////////////////
// получить список учеников
int  GymnasiumDatabase::getPupilList (vector<pupil_view> &vec, search_data sch)
{
    Logger logger(logfile);

    string sql;
    if (sch.mode==1)
    {
        if (sch.sclass) sql+=" AND sclass = "+to_string(sch.sclass);
        if (sch.letter) sql+=" AND pupil.letter = "+to_string(sch.letter);
        if (sch.gender) sql+=" AND gender = "+to_string(sch.gender);
    }
    else if (sch.mode==2)
    {
        sql+=" AND (phone LIKE '%"+sch.word+"%' OR lname LIKE '%"+sch.word+"%')";
    }

    string Sql=sformat(sqlSelectPupil, sql.c_str());
    try {
        sqlite3pp::database dbase(dbfname.c_str());
        {
                vec.clear();

                sqlite3pp::query query(dbase, Sql.c_str());

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    pupil_view pv;
                    (*i).getter()
                        >> pv.id >> pv.sclass >> pv.letter >> pv.gender
                        >> pv.fname >> pv.mname >> pv.lname
                        >> pv.phone >> pv.addrs >> pv.email >> pv.cl_le;
//                        cout << pv << endl;
                    vec.push_back(pv);
                }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        logger.log(LogLevel::ERR, string("createTables: ") + ex.what());
    }

    return vec.size();
}
// pupil(
//  ppl_id INTEGER PRIMARY KEY AUTOINCREMENT,
//  sclass INTEGER NOT NULL,
//  letter INTEGER NOT NULL,
//  gender INTEGER NOT NULL,
//  fname  TEXT NOT NULL,
//  mname  TEXT NOT NULL,
//  lname  TEXT NOT NULL,
//  phone  TEXT NULL,
//  addrs  TEXT NULL,
//  email  TEXT NULL);

// ===================================================================
/**
# класс;литера;пол; Имя;Отч;Фам; тел;адр;eml

11;7;2;Имя848;Отч848;Фам848;тел848;адр848;eml848;
11;7;1;Имя849;Отч849;Фам849;тел849;адр849;eml849;

11;7;2;Имя850;Отч850;Фам850;тел850;адр850;eml850;
*/

void GymnasiumDatabase::exportCsv(vector<pupil_view> & vecp)
{
    ofstream of(csvfile);
    of << "# класс;литера;пол; Имя;Отч;Фам; тел;адр;eml\n";
    int op=0;
    for(auto &pv : vecp)
    {
        if (pv.letter!=op) of << "\n";
        of << pv.sclass << ";" << pv.letter << ";" << pv.gender << ";" <<
           pv.fname << ";" << pv.mname << ";" << pv.lname << ";" <<
           pv.phone << ";" << pv.addrs << ";" << pv.email << ";\n";
        op=pv.letter;
    }
    of.close();
}

// -------------------------------------------------------------------
void GymnasiumDatabase::importCsv(vector<pupil_view> & vp)
{
    ifstream is(csvfile);
    string sv[10];
    string line, word;

    sqlite3pp::database dbase(dbfname.c_str());

    while (getline(is, line)) {
        if (line.empty()||line[0]=='#') continue;
        stringstream lineStream(line);
        int idx=0;
        while (getline(lineStream, word, ';')) {
            sv[idx++]=word;
        }
        string sql = sformat(sqlInsertPupil,
                             atoi(sv[0].c_str()),
                             atoi(sv[1].c_str()),
                             atoi(sv[2].c_str()),
                             sv[3].c_str(),sv[4].c_str(),sv[5].c_str(),
                             sv[6].c_str(),sv[7].c_str(),sv[8].c_str());
        int rc = dbase.execute(sql.c_str());
    }
    getPupilList(vp);
    dbase.disconnect();
    is.close();
}

// ===================================================================

// -------------------------------------------------------------------
// отладка: Создание БД с нуля:
//    формирование и заполнение таблиц тестовыми данными
//    Кодировка UTF8
void GymnasiumDatabase::makeTestDatabase()
{
    Logger logger(logfile);
    try
    {
        sqlite3pp::database dbase(dbfname.c_str());
        {
            createTables((void*)&dbase);

            int pupil=1;
            for(int sclass=1; sclass<=11; sclass++)
            {
                int l=genint(0,3);
                for(int letter=1; letter<=l;letter++)
                {
                    int pupils=genint(3,7);
                    for(int p=0;p<pupils;p++)
                    {
                        int gender=genint(1,2);
                        string sql=sformat(sqlInsertPupil,sclass,letter,gender,
                            sformat("Имя%03d", pupil).c_str(),
                            sformat("Отч%03d", pupil).c_str(),
                            sformat("Фам%03d", pupil).c_str(),
                            sformat(genint(0,2)==0 ? "" : "Тел%03d", pupil).c_str(),
                            sformat(genint(0,2)==0 ? "" : "Адр%03d", pupil).c_str(),
                            sformat(genint(0,2)==0 ? "" : "Пчт%03d", pupil).c_str());
                        pupil++;
                        cout << to_local(sql);
                        dbase.execute(sql.c_str());
                    }
                }
            }
        }
        dbase.disconnect();
    }
    catch (exception & ex) {
        logger.log(LogLevel::ERR, string("createTables: ") + ex.what());
    }
}

//////////////////////////////////////////////////////////////////////
