#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <exception>
#include <algorithm>
#include <iterator>



using namespace std;
using namespace std::filesystem;



class Exception;                                                                          //Класс исключений
void Handler(string& path_1, string& format);                                             //Обработчик команды.
bool syntax(string &command, string &path, string &format);                               //Проверка синтаксиса команды.
vector<string> GetDirectoryFiles(path& dir, vector<string>& type_of_file);               //Получение vector<string> директории.



class Exception final : public exception 
{
 private:
 string name_error_information;
 int error;
 public:
 Exception(string name_error_information, int error) : exception()
 {
 	this->name_error_information = name_error_information;
 	this->error = error;
 }
 Exception(const Exception& ex)
 {
 	this->name_error_information = name_error_information;
 	this->error = error;
 }
 friend ostream& operator << (ostream& out, const Exception& ex)
 {
 	out << "LINK: " << ex.error << ". Message: " <<ex.name_error_information << "." << endl; 
 	return out;
 }
};


bool syntax(string& command, string& path, string& format)                             //dirwalk [] []
{
 string command_example = "dirwalk [";
 char t = ']';
 int t_i = 0;
 if(command.length() < 10)       
 {
    throw Exception("Неверный тип команды", 2006);
 }
 for(int i =0; i <9; ++i)
 {
 	if(command[i] != command_example[i])
 	{
 	  return false;
 	}
}

for(int i = 9; command[i] != ']'; ++i)
{
   path += command[i];
   t_i += 1;
}
 t_i +=12;
 if(command[t_i] == ']'){}
 else
 {
 	for(t_i; command[t_i] != ']'; ++t_i)                                                  //добавить проверку для конечного ]
 	{   
 		format += command[t_i];
 	}
 }
 for(int i = 0; i < format.length(); ++i)
 {
   if(format[i]=='s' || format[i]=='l' || format[i] == 'd' || format[i]=='f'){}
   else 
   {
          throw Exception("Ошибка ввода формата для вывода файлов согласно типу данных!", 2000);
   }
 }
 return true;
}


void Handler(string& path_1, string& format)
{

        vector<string> type_of_file;
	path dir {path_1};
	if(format.length() == 0)
	{
                vector<string> PathCommandDirectory = GetDirectoryFiles(dir, type_of_file);          
	}
	else if(format.length() >1)
	{
		throw Exception("Перебор опций", 2022);
	}
	else if(format.length() == 1 && format == "d")
	{
		type_of_file.push_back("d");
		vector<string> PathCommandDirectory = GetDirectoryFiles(dir, type_of_file);
	}
	else if(format.length() == 1 && format == "f")
	{
		type_of_file.push_back("f");
		vector<string> PathCommandDirectrory = GetDirectoryFiles(dir, type_of_file);
	}
	else if(format.length() == 1 && format == "l")
	{
		type_of_file.push_back("l");
		vector<string> PathCommandDirectrory = GetDirectoryFiles(dir, type_of_file);
	}
	else if(format.length() == 1 && format == "s")
	{
		type_of_file.push_back("s");
		vector<string> PathCommandDirectrory = GetDirectoryFiles(dir, type_of_file);
	}
	else 
	{
		throw Exception("Формат команды не определен!", 2010);
	}
	
}


vector<string> GetDirectoryFiles(path& dir, vector<string>& type_of_file)                         //Using '' in Linux and filesystem
{
      path current_place = current_path();
      vector<string> files;
      for(auto &p : recursive_directory_iterator(dir))                       //Можно использовать directory_iterator(string -> путь)
      {
      		if(type_of_file.size() == 0) 
      		{
              		cout << p.path().string() << endl;
              	}
              	else if(type_of_file.size() == 1 && type_of_file[0] == "d")
              	{
              		if(is_directory(p))
              		{
              			cout << p.path().string() << endl;
              		}
              	}
              	else if(type_of_file.size() == 1 && type_of_file[0] == "f")
              	{
              		if(!is_directory(p))
              		{
              			cout << p.path().string() << endl;
              		}
              	}
              	else if(type_of_file.size() == 1 && type_of_file[0] == "l")
              	{
              		if(is_symlink(p))
              		{
              			cout << p.path().string() << endl; 
              		}
              	}
              	else if(type_of_file.size() == 1 && type_of_file[0] == "s")
              	{
              		if(!exists(dir.string()) )                                    
              		{
              			cout << "Данной директории не существует!" << endl;
              		}
              		files.push_back(p.path().string());
              	}
      }
      if(type_of_file[0] == "s")
      {
      		sort(files.begin(), files.end());
      		for(int i =0; i < files.size(); ++ i)
      		{
      			cout << files[i] << endl;
      		}
      }
      return files;
}
int main()
{
	cout << "Добро пожаловать в программу dirwalk!" << endl;
	cout << "Введите комманду по примеру: dirwalk [dir] [options]" << endl; 
	cout << "dir -> начальный каталог. Если опущен, то текущий (./)" << endl;
	cout << "Options -> опции: " << endl;
	cout << "-l -- только символические ссылки. " << endl; 
	cout << "-d -- только каталоги ( -type |). " << endl;                                                     //+Каталог - путь к папке(Dir) 
	cout << "-f -- только файлы (type f). " << endl;                                                          //+Проверка
	cout << "-s -- сортировать выход в соответствии LC_COLLATE." << endl;                                     //Сделать сортировку - рассширение
	cout << "Если опции ldf опущены, выводятся файлы, ссылки, как у find без параметров!" << endl;
	

	string command;
	string path = "";
	string format = "";
	
	
	do 
	{
	try
	{
	cout << "Введите команду: ";
	getline(cin, command);
	if(syntax(command, path, format))
	{
	cout << "Путь вывода файлов -> [" << path << "]" << endl;
	cout << "Формат вывода файлов -> [" << format <<"]"<<endl; 
		if(exists(path))
		{
			Handler(path, format); 
			break;
		}
		else
		{
		   	cout << "Данного пути не существует, попробуйте снова" << endl;
		}
	
	break;
	}
	else
	{
		throw Exception("Ошибка ввода команды", 2005);
	}
	}
	catch(Exception& ex)
	{
	   cout << "Произошла ошибка ввода!" << endl; 
	   cout << ex << endl; 
	}
	catch(filesystem_error&)                                                   //Ошибка по вводу директории!!!
	{
		cout << "Ошибка файловой системы OC IPE" << endl;
	}
	catch(bad_alloc&)
	{
		cout << ">>>Ошибка работы с памятью OPI OC<<<" << endl;
	}
	catch (exception&)
	{
		cout << "Ошибка ввода исполнения команды, перепроверьте правильность написания деклорации для написания исполнительного файла" << endl;	
	}
	}while(true);
	cout << "Программа выполнена успешно!" << endl;
}
