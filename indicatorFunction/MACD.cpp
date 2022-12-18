#include <iostream>
using namespace std;

//使用以下網站公式
//http://nengfang.blogspot.com/2014/09/macd-excel.html

double MACD(double** sheet, double date); //找當日MACD值

double demandIndex(double** sheet, int dateIndex); //找當日demand index
double EMA12(double** sheet, int dateIndex); //找當日EMA12
double EMA26(double** sheet, int dateIndex); //找當日EMA26
double DIF(double** sheet, int dateIndex); //找當日DIF
double MACDRecursive(double** sheet, int dateIndex); 
int findDateIndex(double** sheet, double date); //找符合日期的index
double findClose(double** sheet, int dateIndex); //找當日收盤價
double findHigh(double** sheet, int dateIndex); //找當日最高價
double findLow(double** sheet, int dateIndex); //找當日最低價

int findDateIndex(double** sheet, double date){
    for(int i = 0; i < 7888; i++){
        if(sheet[i][0] == date){
            return i;
        }
    }
}

double findClose(double** sheet, int dateIndex){
    double close = sheet[dateIndex][4];
    return close;
}

double findHighest(double** sheet, int dateIndex){
    double high = sheet[dateIndex][2];
    return high;
}

double findLowest(double** sheet, int dateIndex){
    double low = sheet[dateIndex][3];
    return low;
}

double demandIndex(double** sheet, int dateIndex){
    double DI = static_cast<double>(static_cast<int>(((findHigh(sheet, dateIndex) + findLow(sheet, dateIndex) + 2 * findClose(sheet, dateIndex)) / 4) * 10000)) / 100;
    return DI;
}

double EMA12(double** sheet, int dateIndex){
    if(dateIndex = 7885){
        double DI12 = 0;
        for(int i = 0; i < 12; i++){
            DI12 += static_cast<double>(static_cast<int>((demandIndex(sheet, dateIndex + i) / 12) * 100)) / 100;
        }
        return DI12;
    }
    else{
        double EMA12Yesterday = EMA12(sheet, dateIndex + 1);
        double DIToday = demandIndex(sheet, dateIndex);
        double EMA12Today = static_cast<double>(static_cast<int>((EMA12Yesterday * (11 / 13) + DIToday * (2 / 13)) * 100)) / 100;
        return EMA12Today;
    }
}

double EMA26(double** sheet, int dateIndex){
    if(dateIndex = 7871){
        double DI26 = 0;
        for(int i = 0; i < 26; i++){
            DI26 += static_cast<double>(static_cast<int>((demandIndex(sheet, dateIndex + i) / 26) * 100)) / 100;
        }
        return DI26;
    }
    else{
        double EMA26Yesterday = EMA26(sheet, dateIndex + 1);
        double DIToday = demandIndex(sheet, dateIndex);
        double EMA26Today = static_cast<double>(static_cast<int>((EMA26Yesterday * (25 / 27) + DIToday * (2 / 27)) * 100)) / 100;
        return EMA26Today;
    }
}

double DIF(double** sheet, int dateIndex){
    double ema12 = EMA12(sheet, dateIndex);
    double ema26 = EMA26(sheet, dateIndex);
    double DIF = ema12 - ema26;
    return DIF;
}

double MACD(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double MACD = MACDRecursive(sheet, i);
    return MACD;
}
double MACDRecursive(double** sheet, int dateIndex){
    double MACDYesterday = MACD(sheet, dateIndex + 1);
    double DIFToday = DIF(sheet, dateIndex);
    double MACDToday = static_cast<double>(static_cast<int>((MACDYesterday * (8 / 10) + DIFToday * (2 / 10)) * 100)) / 100;
    return MACDToday;
}
