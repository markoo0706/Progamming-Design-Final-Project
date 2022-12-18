#include <iostream>
#include <cmath>
using namespace std;

/////////////////Function Titles///////////////////
/////////////////指標函式///////////////////
double K(double** sheet, double date); //找當日k值（應界在0到100之間）
double D(double** sheet, double date); //找當日d值（應界在0到100之間）
double MACD(double** sheet, double date); //找當日MACD值
bool inBoolingerBand(double** sheet, double date, int days, int times); //當日股票價格是否落在布林通道, 使用者可輸入days來決定要用幾天的平均線、輸入times來決定要用幾倍標準差。
double boolingerUpper(double** sheet, double date, int days, int times); //當日布林通道上界
double boolingerLower(double** sheet, double date, int days, int times); //當日布林通道下界
double MoM(double** sheet, double date); //找當天往前推30天對30到60天的營收成長（應屆在-1到1之間）
double MA5(double** sheet, double date); //找當日5MA線
double MA10(double** sheet, double date); //找當日10MA線
double MA20(double** sheet, double date); //找當日20MA線
double RSI(double** sheet, double date); //找6日RSI值（應界在0到100之間）
///////////////指標函式結束//////////////////

///////////////KD值函式///////////////////
double RSV(double** sheet, int dateIndex); //找當日rsv值
double findHighest(double** sheet, int dateIndex); //找當日往回算9天內最高價
double findLowest(double** sheet, int dateIndex); //找當日往回算9天內最低價
double kRecursive(double** sheet, int dateIndex);
double dRecursive(double** sheet, int dateIndex);
//////////////KD值函式結束//////////////////

//////////////MACD值函式///////////////////
double demandIndex(double** sheet, int dateIndex); //找當日demand index
double EMA12(double** sheet, int dateIndex); //找當日EMA12
double EMA26(double** sheet, int dateIndex); //找當日EMA26
double DIF(double** sheet, int dateIndex); //找當日DIF
double MACDRecursive(double** sheet, int dateIndex); 
/////////////MACD值函式結束/////////////////

////////////布林通道函式////////////////////
double findStandardDeviation(double** sheet, double date, int days); //找MA線標準差
double findAverage(double** sheet, double date, int days); //找days日平均
////////////布林通道函式結束////////////////

//////////////通用函式////////////////////
int findDateIndex(double** sheet, double date); //找符合日期的index
double findClose(double** sheet, int dateIndex); //找當日收盤價
double findHigh(double** sheet, int dateIndex); //找當日最高價
double findLow(double** sheet, int dateIndex); //找當日最低價
///////////////通用函式結束//////////////////
/////////////////End of Function Titles///////////////////////////

////////////////////Functions////////////////////
///////////////////指標函式//////////////////////

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

double MACD(double** sheet, double date){
    int i = findDateIndex(sheet, date);
    double MACD = MACDRecursive(sheet, i);
    return MACD;
}

bool inBoolingerBand(double** sheet, double date, int days, int times){
    int dateIndex = findDateIndex(sheet, date);
    double priceToday = findClose(sheet, dateIndex);
    if(priceToday < boolingerUpper(sheet, date, days, times) and priceToday > boolingerLower(sheet, date, days, times)){
        return true;
    }
    else{
        return false;
    }
}

double boolingerUpper(double** sheet, double date, int days, int times){
    double standardDeviation = findStandardDeviation(sheet, date, days);
    double average = findAverage(sheet, date, days);
    double upperBound = average + times * standardDeviation;
    return upperBound;
}

double boolingerLower(double** sheet, double date, int days, int times){
    double standardDeviation = findStandardDeviation(sheet, date, days);
    double average = findAverage(sheet, date, days);
    double lowerBound = average - times * standardDeviation;
    return lowerBound;
}

double MoM(double** sheet, double date){
    double returnThisMonth = 0;
    double returnLastMonth = 0;

    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 30; i++){
        returnThisMonth += sheet[dateIndex + i][4] / 30;
    }
    for(int i = 30; i < 60; i++){
        returnLastMonth += sheet[dateIndex + i][4] / 30;
    }

    double MoM = static_cast<double>(static_cast<int>(((returnThisMonth - returnLastMonth) / returnLastMonth) * 100)) / 100;
    return MoM;
}

double MA5(double** sheet, double date){
    double MA = 0;
    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 5; i++){
        MA += sheet[dateIndex + i][4] / 5;
    }
    MA = static_cast<double>(static_cast<int>((MA) * 100)) / 100;
    return MA;
}

double MA10(double** sheet, double date){
    double MA = 0;
    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 10; i++){
        MA += sheet[dateIndex + i][4] / 10;
    }
    MA = static_cast<double>(static_cast<int>((MA) * 100)) / 100;
    return MA;
}

double MA20(double** sheet, double date){
    double MA = 0;
    int dateIndex = findDateIndex(sheet, date);

    for(int i = 0; i < 20; i++){
        MA += sheet[dateIndex + i][4] / 20;
    }
    MA = static_cast<double>(static_cast<int>((MA) * 100)) / 100;
    return MA;
}

double RSI(double** sheet, double date){
    int dateIndex = findDateIndex(sheet, date);
    double increase = 0; 
    double decrease = 0;
    for(int i = 0; i < 6; i++){
        if(sheet[dateIndex + i][4] - sheet[dateIndex + i + 1][4] >= 0){
            increase += sheet[dateIndex + i][4] - sheet[dateIndex + i + 1][4];
        }
        else{
            decrease += sheet[dateIndex + i + 1][4] - sheet[dateIndex + i][4];
        }
    }
    double rsi = static_cast<double>(static_cast<int>((increase / (increase + decrease)) * 10000)) / 100;
    return rsi;
}
//////////////////指標函式結束//////////////////

///////////////////KD值函式////////////////////
double RSV(double** sheet, int dateIndex){
    double close = findClose(sheet, dateIndex);
    double high = findHighest(sheet, dateIndex);
    double low = findLowest(sheet, dateIndex);
    double rsv = static_cast<double>(static_cast<int>(((close - low) / (high - low)) * 10000)) / 100;
    return rsv;
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
///////////////////KD值函式結束//////////////////

///////////////////MACD值函式////////////////////
double demandIndex(double** sheet, int dateIndex){
    double DI = static_cast<double>(static_cast<int>(((findHigh(sheet, dateIndex) + findLow(sheet, dateIndex) + 2 * findClose(sheet, dateIndex)) / 4) * 10000)) / 100;
    return DI;
}

double EMA12(double** sheet, int dateIndex){
    if(dateIndex == 7885){
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
    if(dateIndex == 7871){
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

double MACDRecursive(double** sheet, int dateIndex){
    double MACDYesterday = MACD(sheet, dateIndex + 1);
    double DIFToday = DIF(sheet, dateIndex);
    double MACDToday = static_cast<double>(static_cast<int>((MACDYesterday * (8 / 10) + DIFToday * (2 / 10)) * 100)) / 100;
    return MACDToday;
}
//////////////////MACD值函式結束///////////////////

/////////////////布林通道函式//////////////////////
double findAverage(double** sheet, double date, int days){
    int dateIndex = findDateIndex(sheet, date);
    double average = 0;
    for(int i = 0; i < days; i++){
        average += sheet[dateIndex + i][4] / days;
    }

}
double findStandardDeviation(double** sheet, double date, int days){
    //this function finds standard deviation by the equation E(pow(x, 2)) - pow((E(x)), 2).
    int dateIndex = findDateIndex(sheet, date);

    //E(pow(x, 2))
    double e2 = 0; 
    for(int i = 0; i < days; i++){
        e2 += pow(sheet[dateIndex + i][4], 2);
    }
    e2 = e2 / days;
    //pow(E(x), 2)
    double e = findAverage(sheet, date, days);
    e = pow(e, 2);

    double variation = e2 - e;
    double standardDeviation = pow(variation, 1 / 2);
    standardDeviation = static_cast<double>(static_cast<int>((standardDeviation) * 100)) / 100;
    return standardDeviation;
}
//////////////////布林通道函式結束//////////////////

//////////////////通用函式////////////////////////
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

double findHigh(double** sheet, int dateIndex){
    double high = sheet[dateIndex][2];
    return high;
}

double findLow(double** sheet, int dateIndex){
    double low = sheet[dateIndex][3];
    return low;
}
///////////////////通用函式結束/////////////////////
/////////////////////End of Functions////////////////////////////