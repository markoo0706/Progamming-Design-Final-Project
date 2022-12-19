#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
#include <SFML/Graphics.hpp>
void loadPrice(const string& fileName, string** priceInfo, const int colNum);
void loadEco();
double** toDouble(string** , int, int);
double** dataToCsv(double** doubleArr, const int arrRow, const int arrCol);

#include <SFML/Graphics.hpp>


int main()
{

    //    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }


    string fileName = R"(C:\Users\Andy Chen\Documents\GitHub\Progamming-Design-Final-Project\SP500_date.csv)";
    const int rowNum = 7900;
    const int colNum = 5;
    string** priceInfo = new string*[rowNum]; // 7900為最大列數
    for(int i = 0; i < rowNum; i++){
        priceInfo[i] = new string[colNum];
    }

    return 0;
}

void loadPrice(const string& fileName, string** priceInfo, const int colNum){ // 需要dateNum
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
    int rowCount = 0;
    //遍历文件中的每一行数据
    for (auto it = item.begin(); it != item.end(); it++)
    {
        string str;
        //其作用是把字符串分解为单词(在此处就是把一行数据分为单个数据)
        istringstream istr(*it);
        // 一列一列輸入，第0個item是日期
        for (int i = 0; i < colNum; i++){
            getline(istr, str, ',');
            priceInfo[rowCount][i] = str;
//            if(rowCount == 0) cout << str;
            if (i < colNum - 1) cout << priceInfo[rowCount++][i] << '\t';
            else cout << priceInfo[rowCount++][i] << endl;
        }
        rowCount++;
    }
}
void loadEco(char* fileName, double** ecoInfo, int ecoNum){
    ifstream myFile;
    myFile.open(fileName);
    myFile.close();
}

double** toDouble(string ** stringArray, int colNum,int rowNum){
    // 此函數需輸入 一個 二維的String陣列、行數以及列數，並吐出一個大小相同的 double二維陣列。
    double** outputArray = new double* [rowNum]; // 新增一個2維double Array，一格一格更新，最後return回去

    for (int i = 0; i < rowNum; i ++) // 每一列
    {
        outputArray[i] = new double [colNum];
        for (int j = 0; j < colNum; j ++) // 每一行
        {
            string aString = stringArray[i][j];
            if (j == 0) //  第一column，代表此string為日期
            {
                string dateStr = aString.substr(0,2); 
                string monthStr = aString.substr(3,5);
                string yearStr = aString.substr(6,10);
                double date = 0;
                date = stod(yearStr) * 10000 + stod(monthStr) * 100 + stod(dateStr);
                outputArray[i][j] = date;
            }
            else // 代表此string為單純數字
            {
                double num = stod(aString); // 把數字轉為 double 
                outputArray[i][j] = num;
            }
        }
    }
    return outputArray;
}
void writeCSV(double** doubleArr, const int arrRow, const int arrCol){
    //writefile
    fstream file;
    file.open("write.csv");
    for (int i=0;i<arrRow;i++)
    {
        for(int j = 0; j < arrCol; j++){
            if (j == arrCol - 1) file << doubleArr[i][j] << endl;
            else file << doubleArr[i][j] << ",";
        }

    }
    file.close();
    return;
}