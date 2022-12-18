#include <iostream>
using namespace std;

//使用以下網站公式
//https://www.cmoney.tw/learn/course/technicals/topic/484

double K(double** sheet, double date); //找當日k值
double D(double** sheet, double date); //找當日d值

double RSV(double** sheet, int dateIndex); //找當日rsv值
int findDateIndex(double** sheet, double date); //找符合日期的index
double findClose(double** sheet, int dateIndex); //找當日收盤價
double findHighest(double** sheet, int dateIndex); //找當日往回算9天內最高價
double findLowest(double** sheet, int dateIndex); //找當日往回算9天內最低價
double kRecursive(double** sheet, int dateIndex);
double dRecursive(double** sheet, int dateIndex);

int findDateIndex(double** sheet, double date){
    int dateIndex = 0;
    for(int i = 0; i < 7888; i++){
        if(sheet[i][0] == date){
            dateIndex = i;
            break;
        }
    }
    return dateIndex;
}

double findClose(double** sheet, int dateIndex){
    double close = sheet[dateIndex][4];
    return close;
}

double findHighest(double** sheet, int dateIndex){
    double highest = 0;
    for(int j = 0; j < 9; j++){
        if(sheet[dateIndex + j][2] > highest){
            highest = sheet[dateIndex + j][2];
        }
    }
    return highest;
}

double findLowest(double** sheet, int dateIndex){
    double lowest = 100000;
    for(int j = 0; j < 9; j++){
        if(sheet[dateIndex + j][3] < lowest){
            lowest = sheet[dateIndex + j][3];
        }
    }
    return lowest;
}

double RSV(double** sheet, int dateIndex){
    double close = findClose(sheet, dateIndex);
    double high = findHighest(sheet, dateIndex);
    double low = findLowest(sheet, dateIndex);
    double rsv = static_cast<double>(static_cast<int>(((close - low) / (high - low)) * 10000)) / 100;
    return rsv;
}

double K(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double k = kRecursive(sheet, i);
    return k;
}

double D(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double d = dRecursive(sheet, i);
    return d;
}

double kRecursive(double** sheet, int dateIndex){
    if(dateIndex == 7888){
        return 50;
    }
    else{
        double kYesterday = kRecursive(sheet, dateIndex + 1);
        double rsvToday = RSV(sheet, dateIndex);
        double kToday = static_cast<double>(static_cast<int>((kYesterday * (2 / 3) + rsvToday / 3) * 100)) / 100;
        return kToday;
    }
}

double dRecursive(double** sheet, int dateIndex){
    if(dateIndex == 7888){
        return 50;
    }
    else{
        double dYesterday = dRecursive(sheet, dateIndex + 1);
        double kToday = kRecursive(sheet, dateIndex);
        double dToday = static_cast<double>(static_cast<int>((dYesterday * (2 / 3) + kToday / 3) * 100)) / 100;
        return dToday;
    }
}


