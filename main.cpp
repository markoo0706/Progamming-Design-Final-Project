#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

void loadPrice(const string& fileName, string** priceInfo, const int colNum);
void loadEco();

int main(){

    string fileName = R"(C:\Users\Andy Chen\Documents\GitHub\Progamming-Design-Final-Project\SP500_date.csv)";
    const int rowNum = 7900;
    const int colNum = 5;
    string** priceInfo = new string*[rowNum]; // 7900為最大列數
    for(int i = 0; i < rowNum; i++){
        priceInfo[i] = new string[colNum];
    }
    loadPrice(fileName, priceInfo, colNum);
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