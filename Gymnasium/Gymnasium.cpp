// ===================================================================
// Gymnasium Students Management
//
// ===================================================================

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/menubar.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/filebox.hpp>
#include <nana/gui/widgets/picture.hpp>

#include <iostream>
#include <sstream>
//#include <fstream>
#include <string>
#include <vector>
#include <functional>   // для std::function
#include <Windows.h>

#include "Database.h"
#include "search.hpp"
#include "utils.h"

using namespace std;

// преобразователь выходных данных записывает данные из объекта `pupil_view` в поток ostream
std::ostream & operator << (std::ostream& orr , const pupil_view& p)
{
    orr << "pupil_view: ";
    orr << "id=" << p.id << "; ";

    // для кодировки в консоли 1521
    orr << "class="  << to_local(p.cl_le)  << "; ";
    orr << "fname="  << to_local(p.fname)  << "; ";
    orr << "mname="  << to_local(p.mname)  << "; ";
    orr << "lname="  << to_local(p.lname)  << "; ";

    // для кодировки в консоли UTF-8
//    orr << "class="  << p.cl_le << "; ";
//    orr << "fname="  << p.fname << "; ";
//    orr << "mname="  << p.mname << "; ";
//    orr << "lname="  << p.lname << "; ";

    // полный / сокращённый вывод
//    orr << "phone="  << to_local(p.phone)  << "; ";
//    orr << "addrs="  << to_local(p.addrs)  << "; ";
//    orr << "email="  << to_local(p.email)  << "; ";

    orr << "sclass=" << p.sclass << "; ";
    orr << "letter=" << p.letter << "; ";
    orr << "gender=" << p.gender << "; ";

	return orr ;
}

/*
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


struct search_data {
  int     mode   {0}; // 0-- выбор 1-класса / 2-ученика по фамилии / 3-по телефону
  int     sclass {0}; // 1-- класс 1..11
  int     letter {0}; // 2-- буква 1..13
  int     gender {0}; // 3-- пол 1-муж, 2-жен
  string  word;       // 4-- имя / тел
};

*/

// преобразователь выходных данных записывает данные из объекта `pupil_view` в элемент списка
nana::listbox::oresolver& operator << ( nana::listbox::oresolver& orr , const pupil_view& p)
{
    orr << p.id; orr << p.cl_le;
    orr << p.fname; orr << p.mname; orr << p.lname;
    orr << p.phone; orr << p.addrs; orr << p.email;
    orr << p.sclass;orr << p.letter;orr << p.gender;
    return orr ;
}

// преобразователь входных данных записывает данные из элемента списка в объект `pupil_view`
nana::listbox::iresolver& operator >> ( nana::listbox::iresolver& irr , pupil_view& p)
{
    irr >> p.id; irr >> p.cl_le;
    irr >> p.fname; irr >> p.mname; irr >> p.lname;
    irr >> p.phone; irr >> p.addrs; irr >> p.email;
    irr >> p.sclass;irr >> p.letter;irr >> p.gender;
    return irr ;
}

// преобразователь выходных данных записывает данные из объекта `stats_parallel` в элемент списка
nana::listbox::oresolver& operator << ( nana::listbox::oresolver& orr , const stats_parallel& p)
{
    orr << p.paral; orr << p.classes;
    orr << p.total; orr << p.boys; orr << p.girls;
    orr << p.letters;
    return orr ;
}

// преобразователь входных данных записывает данные из элемента списка в объект `stats_parallel`
nana::listbox::iresolver& operator >> ( nana::listbox::iresolver& irr , stats_parallel& p)
{
    irr >> p.paral; irr >> p.classes;
    irr >> p.total; irr >> p.boys; irr >> p.girls;
    irr >> p.letters;
    return irr ;
}

// преобразователь выходных данных записывает данные из объекта `stats_classes` в элемент списка
nana::listbox::oresolver& operator << ( nana::listbox::oresolver& orr , const stats_classes& p)
{
    orr << p.cl_le; orr << p.total;
    orr << p.boys; orr << p.girls;
    return orr ;
}

// преобразователь входных данных записывает данные из элемента списка в объект `stats_classes`
nana::listbox::iresolver& operator >> ( nana::listbox::iresolver& irr , stats_classes& p)
{
    irr >> p.cl_le; irr >> p.total;
    irr >> p.boys; irr >> p.girls;
    return irr ;
}

// преобразователь выходных данных записывает данные из объекта `stats_incomplete` в элемент списка
nana::listbox::oresolver& operator << ( nana::listbox::oresolver& orr , const stats_incomplete& p)
{
    orr << p.cl_letter << p.name;
    orr << p.desc;
    return orr ;
}

// преобразователь входных данных записывает данные из элемента списка в объект `stats_incomplete`
nana::listbox::iresolver& operator >> ( nana::listbox::iresolver& irr , stats_incomplete& p)
{
    irr >> p.cl_letter >> p.name;
    irr >> p.desc;
    return irr ;
}



// ===================================================================
//
namespace Gymnasium
{
    using namespace nana;

    /// Идентификаторы кнопок, на которые нажимал пользователь
    enum pick_t{pick_yes, pick_no, pick_ok, pick_cancel} ret_code;

    string litera[] = {" ", "А","Б","В","Г","Д", "Е","Ж","З","И","К"};
    string gender[] = {" ", "М","Ж"};
    search_data sch_data;

    ///
    detail::font_style fntstyle = detail::font_style{400};
    paint::font afont = paint::font{"Arial", 12.0, fntstyle};

    GymnasiumDatabase gdb;


    // подготовка виджетов textbox к использованию
    void setTextbox (window wnd, textbox & control, bool edit=true, bool multi=false){
        control.create(wnd, true);
        control.text_align(align::left);
        control.typeface( afont );
        control.multi_lines(multi);
        control.editable(edit);
    }

    // ===============================================================


	class tab_page_listbox 	: public panel<false>
	{
		place       tplace{*this};
	public:
		listbox lstbox {*this};
		tab_page_listbox(window wd)	: panel<false>(wd)
		{
		    tplace.div("<list>");
            // the list field --------------------------------------
            lstbox.enable_single(true, false);// выбор только одной строки
            lstbox.append_header( ("#"),        30); // ID=0
            lstbox.append_header( ("Класс"),    80); // ID=1
            lstbox.append_header( ("Имя"),     100); // ID=4
            lstbox.append_header( ("Отчество"),120); // ID=5
            lstbox.append_header( ("Фамилия"), 120); // ID=6
            lstbox.append_header( ("Телефон"),  80); // ID=7
            lstbox.append_header( ("Адрес"),   130); // ID=8
            lstbox.append_header( ("Еmail"),   100); // ID=9

            lstbox.append_header( ("К"),         0); // ID=10
            lstbox.append_header( ("Л"),         0); // ID=11
            lstbox.append_header( ("П"),         0); // ID=12

			tplace.field("list")<<lstbox;
		}
//		pupil_view resolve(unsigned& cat, unsigned& pos, pupil_view &item)
//		{
//		    lstbox.at(cat).at(pos).resolve_to(item);
//		    //return item;
//		}
	};

    // ===============================================================
//-  реализовать полный функционал PupilDataDialog:
//    + вводить можно несколько учеников за 1 вызов диалога
//    + редактировать можно только одного ученика
//    + введенные данные сразу созранять в базе
//    + предусмотреть выход из диалога с подтверждением и без




    /////////////////////////////////////////////
    // добавить / изменить данные ученика
    //
    class PupilDataDialog : public form
    {
        place   fplace;
        textbox fname, mname, lname, phone, addrs, email;
        button  btn1  {*this, true},
                btn2  {*this, true},
                btn3  {*this, true};
        combox  cb_cl {*this, ("Класс")} ,
                cb_le {*this, ("Литера")} ,
                cb_ge {*this, ("Пол")} ;
        string  captions[2] = {"Создание записи ученика", "Редактирование записи ученика"};

    public:

        PupilDataDialog(window wd, pupil_view& pppl) : form(wd, {450, 250}, appear::optional<true>{} )
        {
            fplace.bind(*this);
            caption(pppl.id==0 ? captions[0] : captions[1]);

            cb_cl.push_back( (" ") );
            for(int c=1; c<=11; c++)
            cb_cl.push_back( (to_string(c)));
            cb_cl.events().selected ( [this, pppl](const nana::arg_combox& acmb) mutable
            {
                pppl.sclass = acmb.widget.option();
            });

            cb_le.push_back( (" ") );
            for(int c=1; c<=10; c++)
            cb_le.push_back( (litera[c]));
            cb_le.events().selected ( [this, pppl](const nana::arg_combox& acmb) mutable
            {
                pppl.letter = acmb.widget.option();
            });

            cb_ge.push_back( (" ") );
            for(int c=1; c<=2; c++)
            cb_ge.push_back( (gender[c]));
            cb_ge.events().selected ( [this, pppl](const nana::arg_combox& acmb) mutable
            {
                pppl.gender = acmb.widget.option();
            });

            setTextbox(*this, lname);
            setTextbox(*this, fname);
            setTextbox(*this, mname);
            setTextbox(*this, phone);
            setTextbox(*this, addrs);
            setTextbox(*this, email);

            btn1.caption("Выход");
            btn1.events().click([this, &pppl](const nana::arg_click & ei) {
                getPupilData(pppl);
                if (checkPupilData(pppl)) {
                    cout << pppl << endl;
                    msgbox mb(this->handle(), "Подтверждение", msgbox::yes_no);
                    mb.icon(mb.icon_question);
                    mb << ("Сохранить запись?");
                    if (mb() == mb.pick_yes) {
                        savePupilData(gdb, pppl);
                    }
                    API::close_window(*this);
                }
                else
                {
                    msgbox mb(this->handle(), "Внимание", msgbox::ok);
                    mb.icon(mb.icon_warning);
                    mb << ("Не все данные введены.");
                    mb();
                }
            });

            btn2.caption("Очистить");
            btn2.events().click([this](const nana::arg_click & ei) {\
                cb_cl.option(0);
                cb_le.option(0);
                cb_ge.option(0);
                fname.reset("");
                mname.reset("");
                lname.reset("");
                phone.reset("");
                addrs.reset("");
                email.reset("");
            });

            btn3.caption("Сохранить");
            btn3.events().click([this, &pppl](const nana::arg_click & ei) {
                getPupilData(pppl);

                //проверка заполнения данных
                //если мы в режиме создания записи, не закрывать окно
                //иначе, закрыть окно
                if (checkPupilData(pppl))
                {
                    savePupilData(gdb, pppl);
                    if (pppl.id!=0) API::close_window(*this);
                }
                else
                {
                    msgbox mb(this->handle(), "Внимание", msgbox::ok);
                    mb.icon(mb.icon_warning);
                    mb << ("Не все данные введены.");
                    mb();
                }
            });

            // создание и заполнение виджетов данными ученика
            setPupilData(pppl);

            fplace.div(R"(vert margin=[10,5,5,5]
                     <weight=55 margin=[15,5,10,5] gap=10 arrange=[70,70,70,variable] field1>
                     <weight=55 margin=[15,5,10,5] gap=10 arrange=[120,130,variable] field2>
                     <weight=55 margin=[15,5,10,5] gap=10 field3>
                     <weight=70 margin=[15,5,20,5] gap=10 field4>)");
            fplace.field("field1") << cb_cl << cb_le << cb_ge << lname;
            fplace.field("field2") << fname << mname << phone;
            fplace.field("field3") << addrs << email;
            fplace.field("field4") << btn1 << btn2 << btn3;

            fplace.collocate();
            modality();
            show();
        }

        //эти данные надо записать в бд
        void setPupilData(pupil_view& pppl) {

            cb_cl.option(pppl.sclass);
            cb_le.option(pppl.letter);
            cb_ge.option(pppl.gender);

            fname.reset(pppl.fname);
            mname.reset(pppl.mname);
            lname.reset(pppl.lname);

            phone.reset(pppl.phone);
            addrs.reset(pppl.addrs);
            email.reset(pppl.email);

        }

        //вместо этих данных надо вставить то, что в бд
        void getPupilData(pupil_view& pppl) {
            pppl.fname=fname.text();  // 4-- имя
            pppl.mname=mname.text();  // 5-- отч
            pppl.lname=lname.text();  // 6-- фам

            pppl.phone=phone.text();  // 7-- тел
            pppl.addrs=addrs.text();  // 8-- адр
            pppl.email=email.text();  // 9-- eml

            pppl.sclass=cb_cl.option(); // 0-- класс 1..11
            pppl.letter=cb_le.option(); // 1-- буква 0..9
            pppl.gender=cb_ge.option();

            pppl.cl_le=sformat("%d-%s", pppl.sclass, litera[pppl.letter].c_str());
        }

        bool checkPupilData(pupil_view& pppl) {
            return (pppl.sclass!=0&&pppl.letter!=0&&pppl.gender!=0 &&
                    pppl.fname!=""&&pppl.mname!=""&&pppl.lname!="");
        }

        void savePupilData(GymnasiumDatabase& gdb, pupil_view& pppl) {
            if (pppl.id==0) gdb.insertPupil(pppl);
            else gdb.updatePupil(pppl);
        }

    };


    class tab_page_parallel	: public panel<false>
	{
		place       tplace{*this};
	public:
		listbox lstbox {*this};
		vector<stats_parallel> vec;
		tab_page_parallel(window wd) : panel<false>(wd)
		{
		    tplace.div("<list>");
            // the list field --------------------------------------
            //lstbox.enable_single(true, false);// выбор только одной строки
            lstbox.append_header( ("Параллель"),    70); // ID=1
            lstbox.append_header( ("Классов"),     60); // ID=4
            lstbox.append_header( ("Учеников"),70); // ID=5
            lstbox.append_header( ("Мальчиков"), 70); // ID=6
            lstbox.append_header( ("Девочек"),  60); // ID=7
            lstbox.append_header( ("Классы"),   80); // ID=8

			tplace.field("list")<<lstbox;

			updateListbox();
		}
		void updateListbox()
		{
		    gdb.paral_pupils(vec);
            lstbox.erase();
            for(auto &pv : vec)
            {
                lstbox.at(0).append(pv);
            }
		}
	};

	class tab_page_classes : public panel<false>
	{
		place       tplace{*this};
		vector<stats_classes> vec;
	public:
		listbox lstbox {*this};
		tab_page_classes(window wd)	: panel<false>(wd)
		{
		    tplace.div("<list>");
            // the list field --------------------------------------
            //lstbox.enable_single(true, false);// выбор только одной строки
            lstbox.append_header( ("Класс"),    170); // ID=1
            lstbox.append_header( ("Всего учеников"),100); // ID=5
            lstbox.append_header( ("Мальчиков"), 80); // ID=6
            lstbox.append_header( ("Девочек"),  60); // ID=7

			tplace.field("list")<<lstbox;

			updateListbox();
		}
		void updateListbox()
		{
		    gdb.class_pupils(vec);
            lstbox.erase();
            for(auto &pv : vec)
            {
                lstbox.at(0).append(pv);
            }
		}
	};

	class tab_page_incomplete	: public panel<false>
	{
		place       tplace{*this};
		vector<stats_incomplete> vec;
	public:
		listbox lstbox {*this};
		tab_page_incomplete(window wd)	: panel<false>(wd)
		{
		    tplace.div("<list>");
            // the list field --------------------------------------
            //lstbox.enable_single(true, false);// выбор только одной строки
            lstbox.append_header( ("Класс"),    40); // ID=1
            lstbox.append_header( ("ФИО"),     220); // ID=4
            lstbox.append_header( ("Описание"),  155); // ID=8

			tplace.field("list")<<lstbox;

			updateListbox();
		}
		void updateListbox()
		{
		    gdb.incpl_pupils(vec);
            lstbox.erase();
            for(auto &pv : vec)
            {
                lstbox.at(0).append(pv);
            }
		}
	};

	class about_form : public form
    {
        place   fplace;

        paint::image img;
        label cpt{*this},lib{*this},nanalib{*this},sqllib{*this},sql3pplib{*this},ver{*this},
              madeby{*this},fullname{*this},email{*this},github{*this};

        picture gymnasium {*this};
    public:
        //PupilDataDialog(window wd, pupil_view& pppl) : form(wd, {450, 250}, appear::optional<true>{} )
        about_form(window wd) : form(wd, {450, 250}, appear::decorate<appear::minimize>())
        {
            API::window_icon(*this, nana::paint::image("gymnasium.ico"));

            fplace.bind(*this);
            img.open (arr_search_png, sizeof(arr_search_png));

            cpt.format(true);
            lib.format(true);
            nanalib.format(true);
            sqllib.format(true);
            sql3pplib.format(true);
            ver.format(true);
            madeby.format(true);
            fullname.format(true);
            email.format(true);
            github.format(true);


            cpt.caption("GYMNASIUM. Учащиеся");
            lib.caption("Использованные библиотеки:");
            nanalib.caption("nana (<bold color=0x0000ff url=\"https://github.com/cnjinhao/nana\">ссылка</>)");
            sqllib.caption("sqlite3 (<bold color=0x0000ff url=\"https://sqlite.org\">ссылка</>)");
            sql3pplib.caption("sqlite3pp (<bold color=0x0000ff url=\"https://github.com/iwongu/sqlite3pp\">ссылка</> на github)");
            ver.caption("v1.0.0");
            madeby.caption("Разработано:");
            fullname.caption("======================");
            email.caption("notfed426@gmail.com");
            github.caption("<bold color=0x0000ff url=\"https://github.com/fedorio426\">ссылка</>");

            fplace.div(R"(horizontal <image> <vert<l1 weight=45> <l2 weight=15> <l3 weight=15>
                               <l4 weight=15> <l5 weight=45> <l6 weight=45>
                               <l7 weight=15> <l8 weight=15> <l9 weight=15>
                               <l10 weight=15>>)");

            fplace.field("l1") << cpt;
            fplace.field("l2") << lib;
            fplace.field("l3") << nanalib;
            fplace.field("l4") << sqllib;
            fplace.field("l5") << sql3pplib;
            fplace.field("l6") << ver;
            fplace.field("l7") << madeby;
            fplace.field("l8") << fullname;
            fplace.field("l9") << email;
            fplace.field("l10") << github;

            gymnasium.load( paint::image("gymnasium.ico") );
            fplace["image"] << gymnasium;

            fplace.collocate();
            show();
            modality();
        }
    }; // class about_form

    class stats_form : public form
    {
        place   fplace;
        label common{*this, true};
        tabbar<string> tab_bar{*this};

        tab_page_parallel   tp_p{*this};
        tab_page_classes    tp_c{*this};
        tab_page_incomplete tp_i{*this};

        string lable_templ {"<bold blue>\tВсего учащихся: %d\tМальчиков: %d\tДевочек: %d</>"};

        stats_pupil pv;

    public:
        stats_form(window wd) : form(wd, {450, 250}, appear::decorate<appear::minimize>())
        {
            API::window_icon(*this, nana::paint::image("gymnasium.ico"));
            common.format(true);

            caption("Статистика");

            fplace.bind(*this);

            make_tabs(*this);

            gdb.total_pupils(pv);


            fplace.div(R"(vert <common weight=30 margin=[10,5,5,0]> <tabs weight=32 margin=5> <tab_frame margin=5>)");
            fplace.field("tabs") << tab_bar;
            fplace.field("common") << common;

            common.caption(sformat(lable_templ, pv.total, pv.boys, pv.girls));

            fplace.collocate();
            this->show();
            this->modality();
        }




    private:
        void make_tabs(window wd) {
			fplace.field("tab_frame").fasten(tp_p)	//Fasten the tab pages
                                     .fasten(tp_c)
                                     .fasten(tp_i);

			tab_bar.append ("Параллели",            tp_p)
				   .append ("Классы",               tp_c)
				   .append ("Недостающие данные",   tp_i);

            tab_bar.activated(0);
        }
    }; // class stats_form

    // ===============================================================

    /////////////////////////////////////////////
    // поиск учеников / выборка списка учеников
    //

    class SearchDialog : public form
    {
        place   fplace;
        textbox fname, phone;
        button  btn1  {*this, true},
                btn2  {*this, true},
                btn3  {*this, true};
        combox  cb_cl {*this, ("Класс")} ,
                cb_le {*this, ("Литера")} ,
                cb_ge {*this, ("Пол")} ;

        paint::image img;

        pupil_view p_view;

    public:

        SearchDialog(window wd, search_data& sch) : form(wd, {400, 200}, appear::optional<true>{} )
        {
            fplace.bind(*this);
            caption("Поиск учащихся");
            img.open (arr_search_png, sizeof(arr_search_png));

            cb_cl.push_back( (" ") );
            for(int c=1; c<=11; c++)
            cb_cl.push_back( (to_string(c)));
            cb_cl.events().selected ( [this](const nana::arg_combox& acmb) mutable
            {
                cout << ("item Pclass ") << acmb.widget.option() << endl;
                p_view.sclass = acmb.widget.option();
            });

            cb_le.push_back( (" ") );
            for(int c=1; c<=10; c++)
            cb_le.push_back( (litera[c]));
            cb_le.events().selected ( [this](const nana::arg_combox& acmb) mutable
            {
                cout << ("item litera ") << acmb.widget.option() << endl;
                p_view.letter = acmb.widget.option();
            });

            cb_ge.push_back( (" ") );
            for(int c=1; c<=2; c++)
            cb_ge.push_back( (gender[c]));
            cb_ge.events().selected ( [this](const nana::arg_combox& acmb) mutable
            {
                cout << ("item gender ") << acmb.widget.option() << endl;
                p_view.gender = acmb.widget.option();
            });

            setTextbox(*this, fname);
            setTextbox(*this, phone);

            btn1.icon(img);
            btn1.caption("искать");
            btn1.events().click([this, &sch](const nana::arg_click & ei) {
                sch.mode=1;
                sch.sclass=p_view.sclass;
                sch.letter=p_view.letter;
                sch.gender=p_view.gender;
                API::close_window(*this);
            });

            btn2.icon(img);
            btn2.caption("искать");
            btn2.events().click([this, &sch](const nana::arg_click & ei) {
                sch.mode=2;
                sch.word=fname.text();
                API::close_window(*this);
            });

            btn3.icon(img);
            btn3.caption("искать");
            btn3.events().click([this, &sch](const nana::arg_click & ei) {
                sch.mode=2;
                sch.word=phone.text();
                API::close_window(*this);
            });

            fplace.div(R"(vert margin=[12,5,12,5]
                     <weight=55 margin=[15,5,10,5] gap=10 arrange=[85,85,85,variable] field1>
                     <weight=55 margin=[15,5,10,5] gap=10 arrange=[275,variable] field2>
                     <weight=55 margin=[15,5,10,5] gap=10 arrange=[275,variable] field3>)");
            fplace.field("field1") << cb_cl << cb_le << cb_ge << btn1;
            fplace.field("field2") << fname << btn2;
            fplace.field("field3") << phone << btn3;

            fplace.collocate();
            this->show();
            this->modality();
        }
    };






    /////////////////////////////////////////////
    class gymnasium_form : public form
    {
        place   fplace;
        menubar mnubar {*this};
        label   stslbl {*this, true};
        textbox txtbox {*this, true};
        listbox lstbox {*this, true};
        tabbar<string> tab_bar{*this};

        string title_templ {"GYMNASIUM. УЧЕНИКИ - [база: %s]"};
        string lable_templ {"<bold blue>\t%s %s</>"};
        string lbred_templ {"<bold red>\t%s %s</>"};

        struct pupil_item_info { unsigned int list_cat, list_pos, vect_ind; };
        pupil_item_info     pupil_info;
        pupil_view          pupil_item;

        vector<pupil_view>  vecPupil;

        vector<unique_ptr<tab_page_listbox>> vecTab;

    public:

        gymnasium_form() : form(API::make_center(800, 500), appear::decorate<appear::minimize>())
        {
            API::window_icon(*this, nana::paint::image("gymnasium.ico"));

            caption(sformat(title_templ, gdb.getDbname().c_str()));

            fplace.bind(*this);
            stslbl.format(true);

            // the menu field --------------------------------------
            make_menus();
            make_tabs(*this);

            fplace.div(R"(vert <mnubar weight=26>
                               <tabbar weight=32 margin=5> <tab_frame margin=5>
                               <status weight=20>)");
            fplace.field("mnubar") << mnubar;
            fplace.field("status") << stslbl;
            fplace.field("tabbar") << tab_bar;

            // загружаем данные из бд
            updateListbox();

            //статусная строка
            stslbl.format(true);
            stslbl.caption(sformat(lable_templ,"база данных загружена",""));

            // прослушка  ------------------------------------------

            // подтверждение выхода из программы
            this->events().unload([this](const arg_unload& ei) {
                msgbox mb(this->handle(), "Подтвердите", msgbox::yes_no);
                mb.icon(mb.icon_question);
                mb << ("Вы действительно хотите выйти из программы?");
                ei.cancel = (mb.pick_no == mb());
            });

            // сохраняем данные выделенного абонента в pupil_view
            // методы item_proxy:
            // index_pair pos() - Возвращает абсолютную позицию элемента в контейнере
            // index_pair to_display() - Возвращает позицию отображения, на которую влияет сортировка

            // обработчик single click в listbox ---------------------
            lstbox.events().selected([&](const nana::arg_listbox &arg) {
                // регистрируем лямбда-объект в качестве обработчика события "selected" в списке
                // event генерируется когда элемент выбран или отменен
                if (!arg.item.selected()) return; // только если выбран
                auto catI = arg.item.pos().cat;
                auto posI = arg.item.pos().item;
                lstbox.at(catI).at(posI).resolve_to(pupil_item); //item to pupil_view
                    cout<< "selected: catI=" << catI << " posI=" << posI <<endl;
                    cout<< "selected: " << pupil_item <<endl;
                string p = pupil_item.cl_le + ", " + pupil_item.lname + " " + pupil_item.fname;
                stslbl.caption(sformat(lable_templ,"ученик: ",p.c_str()));
            });

            // обработчик double click в listbox ---------------------
            lstbox.events().dbl_click([&](const nana::arg_mouse &arg)
            {
                // определяет какой элемент находится под курсором
                auto disp_index_pair { lstbox.cast(arg.pos) };

                // если пара индексов идентифицирует элемент - участника
                if(!disp_index_pair.empty() && !disp_index_pair.is_category())
                {
                    // преобразуем отображаемую позицию элемента в абсолютную позицию
                    auto abs_index_pair {lstbox.index_cast(disp_index_pair, true)};
                    // abs_index_pair {npos,cat,item}

                    // абсолютная позиция - индекс в массиве участников
                    pupil_item = vecPupil[abs_index_pair.item];

                    PupilDataDialog pdd(*this, pupil_item);

                    // пропускаем, если выход без сохранения
                    vecPupil[abs_index_pair.item] = pupil_item;
                    lstbox.at(abs_index_pair).resolve_from(pupil_item); //обновили строку
                    cout<< "dblclick: " << pupil_item <<endl<<endl;
                }
            });

            fplace.collocate();

        }; // ctor gymnasium_form()



    private:
        // перезагружаем и отображаем данные -------------------------
        void updateListbox(search_data sch = {}) {
            gdb.getPupilList(vecPupil, sch);
            string last=vecPupil[0].cl_le;
            int lastc=vecPupil[0].sclass;
            for(int i=0; i<11; i++)
            {
                vecTab[i]->lstbox.erase();
            }
            auto cat=vecTab[0]->lstbox.append(last);
            unsigned i=0;
            for(auto &pv : vecPupil)
            {
                if (last!=pv.cl_le)
                {
                    cat=vecTab[pv.sclass-1]->lstbox.append(pv.cl_le);
                }
                last=pv.cl_le;
                cat.append(pv).value(i++);
            }
            stslbl.caption(sformat(lable_templ,"загружена база данных: ", gdb.getDbname().c_str()));
            }

        // Выбор файлов базы данных: *.db;*.db3;*.sqlite -------------
        string pick_file(bool is_open, bool sqlite=true) const {
            filebox fb(*this, is_open);
            if (sqlite) fb.add_filter( ("Базы данных"),  ("*.db;*.db3;*.sqlite"));
            else fb.add_filter( ("Файлы CSV"),  ("*.csv"));
            auto files = fb();
            return !files.empty() ? files.front().string() : "";
        }

        // обработчик single click в listbox ---------------------
        /*
            lstbox.events().selected([&](const nana::arg_listbox &arg) {
                // регистрируем лямбда-объект в качестве обработчика события "selected" в списке
                // event генерируется когда элемент выбран или отменен
                if (!arg.item.selected()) return; // только если выбран
                auto catI = arg.item.pos().cat;
                auto posI = arg.item.pos().item;
                lstbox.at(catI).at(posI).resolve_to(pupil_item); //item to pupil_view
                    cout<< "selected: catI=" << catI << " posI=" << posI <<endl;
                    cout<< "selected: " << pupil_item <<endl;
                string p = pupil_item.cl_le + ", " + pupil_item.lname + " " + pupil_item.fname;
                stslbl.caption(sformat(lable_templ,"ученик: ",p.c_str()));
            }); */
        void make_tabs(window wd) {
            for(int i=0; i<11; i++)
            {
                vecTab.push_back(make_unique<tab_page_listbox>(wd));
                fplace.field("tab_frame").fasten(*vecTab.at(i));
                tab_bar.append (sformat("%d классы", i+1),*vecTab.at(i));

                listbox& lst = vecTab[i]->lstbox;

                vecTab[i]->lstbox.events().selected([&](const nana::arg_listbox &arg) {
                // регистрируем лямбда-объект в качестве обработчика события "selected" в списке
                // event генерируется когда элемент выбран или отменен
                cout << "make_tabs(): selected " << endl;
                if (!arg.item.selected()) return; // только если выбран
                auto catI = arg.item.pos().cat;
                auto posI = arg.item.pos().item;
                unsigned indx{arg.item.value<unsigned>()};
                pupil_item=vecPupil[indx];
                pupil_info={catI,posI,indx};

                //lst.at(catI).at(posI).resolve_to(pupil_item); //item to pupil_view
                cout<< "selected: catI=" << catI << " posI=" << posI <<endl;
                cout<< "selected: " << pupil_item <<endl;

                string p = pupil_item.cl_le + ", " + pupil_item.lname + " " + pupil_item.fname;
                stslbl.caption(sformat(lable_templ,"ученик: ",p.c_str()));;
                });

                vecTab[i]->lstbox.events().dbl_click([&](const nana::arg_mouse &arg)
                {
                    // определяет какой элемент находится под курсором
                    auto disp_index_pair { lst.cast(arg.pos) };

                    // если пара индексов идентифицирует элемент - участника
                    if(!disp_index_pair.is_category())
                    {
                        // преобразуем отображаемую позицию элемента в абсолютную позицию
                        auto abs_index_pair {lst.index_cast(disp_index_pair, true)};
                        // abs_index_pair {npos,cat,item}

                        // абсолютная позиция - индекс в массиве участников
                        pupil_item = vecPupil[pupil_info.vect_ind];

                        PupilDataDialog pdd(*this, pupil_item);

                        // пропускаем, если выход без сохранения
                        vecPupil[pupil_info.vect_ind] = pupil_item;
                        lst.at(abs_index_pair).resolve_from(pupil_item); //обновили строку
                        cout<< "--dblclick: " << pupil_item <<endl<<endl;
                    }
                });
            }
        }

        //Новая база данных — закрывает доступ к используемому в данный момент хранилищу, если таковое было открыто, очищает окно программы, запрашивает наименование и расположение нового хранилища
        //Открыть базу данных — вызывает диалог открытия файла хранилища для загрузки данных в программу
        //Сохранить базу данных — открывает окно выбора файла для указания наименования и расположения нового хранилища, в котором нужно сохранить текущие данные
        //Закрыть базу данных — закрывает доступ к текущему хранилищу, очищает окно программы
        //Создать тестовую базу — закрывает доступ к текущему хранилищу, очищает окно программы, запрашивает наименование и расположение нового хранилища
        //Выход из программы — закрывает программу

        // Формирование меню
        void make_menus() {
            /// ------------------------
            mnubar.push_back("Файл");
            mnubar.at(0).append("Новая база данных", [this](menu::item_proxy& ip) {
                // выбрать место и имя файла бд
                auto fs = pick_file(false);
                if (!fs.empty()) {
                    // принять новое имя файла бд
                    gdb.getDbname() = fs;
                    // создать таблицы в бд
                    gdb.createTables();
                    // прочитать пустую бд
                    updateListbox();
                    caption(sformat(title_templ, gdb.getDbname().c_str()));
                    stslbl.caption(sformat(lable_templ,"новая база создана",""));
                }
            });
            mnubar.at(0).append("Открыть базу данных", [this](menu::item_proxy& ip){
                                // выбрать место и имя файла бд
                                auto fs = pick_file(true);
                                if (!fs.empty()) {
                                    // принять новое имя файла бд
                                    gdb.getDbname() = fs;
                                    // прочитать пустую бд
                                    updateListbox();
                                    caption(sformat(title_templ, gdb.getDbname().c_str()));
                                    stslbl.caption("<bold=true, color=0x0000ff>база данных открыта</>");
                                }
                                });
            mnubar.at(0).append("Сохранить базу данных", [this](menu::item_proxy&){
                                auto fs = pick_file(false);
                                if (!fs.empty()) {
                                    // принять новое имя файла бд
                                    gdb.getDbname() = fs;
                                    gdb.savePupils(vecPupil);
                                    caption(sformat(title_templ, gdb.getDbname().c_str()));
                                    stslbl.caption("<bold=true, color=0x0000ff>база данных сохранена</>");
                                }
                                });
            mnubar.at(0).append("Закрыть базу данных", [this](menu::item_proxy& ip){
                                gdb.getDbname() = "";
                                lstbox.erase();
                                stslbl.caption("<bold=true, color=0x0000ff>база данных закрыта</>");
                                });
            mnubar.at(0).append("Импорт данных CSV", [this](menu::item_proxy& ip){
                                auto fs = pick_file(true, false);
                                if (!fs.empty()) {
                                    // принять новое имя файла бд
                                    gdb.getCsv() = fs;
                                    vecPupil.clear();
                                    gdb.createTables();
                                    gdb.importCsv(vecPupil);
                                    lstbox.erase();
                                    for(auto &pv : vecPupil)
                                        lstbox.at(0).append(pv);
                                    stslbl.caption("<bold=true, color=0x0000ff>база данных загружена из csv</>");
                                }
                                });
            mnubar.at(0).append("Добавить данные CSV", [this](menu::item_proxy& ip){
                                //GymnasiumDatabase gdb;
                                // выбрать место и имя файла бд
                                auto fs = pick_file(true, false);
                                //GymnasiumDatabase gdb;
                                if (!fs.empty()) {
                                    // принять новое имя файла бд
                                    gdb.getCsv() = fs;
                                    gdb.importCsv(vecPupil);
                                    lstbox.erase();
                                    for(auto &pv : vecPupil)
                                        lstbox.at(0).append(pv);
                                    stslbl.caption("<bold=true, color=0x0000ff>данные из csv добавлены/>");
                                }
                                });
            mnubar.at(0).append("Экспорт данных CSV", [this](menu::item_proxy& ip){
                                auto fs = pick_file(false, false);
                                if (!fs.empty()) {
                                    // принять новое имя файла бд
                                    gdb.getCsv() = fs;
                                    gdb.exportCsv(vecPupil);
                                    stslbl.caption("<bold=true, color=0x0000ff>база данных экспотрирована в csv</>");
                                }
                                });
            mnubar.at(0).append("Создать тестовую базу", [this](menu::item_proxy& ip){
                //GymnasiumDatabase gdb;
                gdb.makeTestDatabase();
                updateListbox();
//                gdb.createTables();
                caption(sformat(title_templ, gdb.getDbname().c_str()));
                stslbl.caption("<bold=true, color=0x0000ff>тестовая база данных создана</>");
                });
            mnubar.at(0).append("Выход из программы", [this](menu::item_proxy& ip){
                API::exit_all();
                });
            /// ------------------------
            mnubar.push_back("Данные");
            mnubar.at(1).append("Добавить ученика", [this](menu::item_proxy& ip){
                pupil_view pv={};
                PupilDataDialog pdd(*this, pv);
                updateListbox();
                stslbl.caption("<bold=true, color=0x0000ff>новый ученик создан</>");
                });
            mnubar.at(1).append("Изменить данные", [this](menu::item_proxy& ip){
                PupilDataDialog pdd(*this, pupil_item);
                lstbox.at(pupil_info.list_cat).at(pupil_info.list_pos).resolve_from(pupil_item);
                vecPupil[pupil_info.vect_ind] = pupil_item;
                stslbl.caption("<bold=true, color=0x0000ff>данные ученика изменены</>");
                });
            mnubar.at(1).append("Удалить ученика", [this](menu::item_proxy& ip){
                msgbox mb(this->handle(), "Подтвердите", msgbox::yes_no);
                mb.icon(mb.icon_question);
                mb << (sformat("Вы действительно хотите удалить запись?\nКласс: %s\n%s %s %s",
                               pupil_item.cl_le.c_str(),
                               pupil_item.fname.c_str(),
                               pupil_item.lname.c_str(),
                               pupil_item.mname.c_str()));
                if (mb.pick_yes == mb()) {
                    gdb.deletePupil(pupil_item.id);
                    updateListbox();
                    stslbl.caption("<bold=true, color=0x0000ff>запись ученика удалена</>");
                }
                });
            mnubar.at(1).check_style(0, menu::checks::highlight);
            /// ------------------------
            mnubar.push_back("Справка");
            mnubar.at(2).append("Статистика",  [this](menu::item_proxy& ip){
                //- количество классов: 1(5), 2(4), ... 11(7)
                //- общее количество учащихся: 899
                //- количество учащихся по годам и классам: 1-А(22), ... 11-Е(15)
                //- количество мальчив и девочек всего и по классам: 450/449; 1-А(11/11), ...
                //- список учащихся с неполными данными
                stats_form s(*this);
                });
            /// ------------------------
            mnubar.at(2).append("Поиск по базе", [this](menu::item_proxy& ip){
                search_data sch={};
                SearchDialog sd(*this, sch);
                updateListbox(sch);
                });
            /// ------------------------
            mnubar.at(2).append("О программе", [this](menu::item_proxy& ip){
                about_form s(*this);
                });

        }
    }; // class gymnasium_form

} // Gymnasium



// ===================================================================
//
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Gymnasium::gymnasium_form npform;
    npform.show();
    Gymnasium::exec();
//    nana::form fm;
//    pupil_view pv;
//    Gymnasium::PupilDataDialog pdd(fm, pv);
//    pdd.setPupilData(pppl);
//    GymnasiumDatabase gsd;
//    gsd.makeTestDatabase();
//    vector<pupil_view> vec;
//    gsd.getPupilList(vec);
//    for(auto p : vec)
//        cout << p << endl;

    return 0;
}
