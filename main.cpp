#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void loadPrice(char* fileName, double** priceInfo);
void loadEco();

int main(){

    char fileName[30] = "SP500_date.csv";
    double** priceInfo;
    loadPrice(fileName, priceInfo);
    return 0;
}
void loadPrice(char* fileName, double** priceInfo){ // 需要dateNum
    ifstream ifs;							//创建流对象

    ifs.open(fileName, ios::in);	//打开文件

    if (!ifs.is_open())						//判断文件是否打开
    {
        cout << "openWrong";
        return;
    }

    vector<string> item;		//用于存放文件中的一行数据

    string temp;				//临时存储文件中的一行数据

    while (getline(ifs, temp))  //利用 getline（）读取文件中的每一行，并放入到 item 中
    {
        item.push_back(temp);
    }

    //遍历文件中的每一行数据
    for (auto it = item.begin(); it != item.end(); it++)
    {
        string str;

        //其作用是把字符串分解为单词(在此处就是把一行数据分为单个数据)
        istringstream istr(*it);

        //将字符串流 istr 中的字符读入到 str 字符串中，读取方式是以逗号为分隔符
        getline(istr, str, ',');
        cout << str << "\t";            // str 对应第一列数据
        //atoi(str.c_str())该函数将字符转化为 int 数据

        getline(istr, str, ',');
        cout << str << "\t";            // str 对应第二列数据

        getline(istr, str, ',');
        cout << str << "\t";            // str 对应第三列数据

        getline(istr, str, ',');
        cout << str << "\t";            // str 对应第四列数据

        getline(istr, str, ',');
        cout << str << "\t";            // str 对应第五列数据

        getline(istr, str, ',');
        cout << str << endl;            // str 对应第六列数据
    }

}
void loadEco(char* fileName, double** ecoInfo, int ecoNum){
    ifstream myFile;
    myFile.open(fileName);
    myFile.close();
}